//
// Created by 86159 on 2024-01-30.
//
/***
 * @note 1.控制灯带使用的是单极性归零码的协议(本程序使用的是8分频，也就是9Mbps)
 *       2.码0:高电平0.3us低电平0.6us  码1:高电平0.6us低电平0.3us
 *       3.控制一个灯需要24个码，格式为GRB（高位在前）
 *       4.由于控制精度要求十分变态，达到了零点几us的级别，所以不可以用定时器+IO口手动电平翻转（速度太慢）。所以只能用SPI+DMA（最高可到18Mbps）速度直接拉满
 * ***/




#include "LED_Ribbon.h"
#include "Delay.h"
#include "spi.h"
#include "retarget.h"
#include "RGBColor.h"
#include "string.h"

LEDRibHandle hLedRib;
uint8_t ResultBuff[28] = {0}; //这里一定要是28个，多出来的一个一定要是0，再发送完毕后拉低电平，防止打乱后面的时序（因为都是高电平开头的）

//该函数为内部调用
//返回把Color转化为单极性归零码对应的第NowCount个字节
uint8_t SpecificByte(uint8_t Color,uint8_t NowCount)
{
    uint8_t temp = 0x80;
    uint8_t RetVal = 0x00;
    //第一个
    if(NowCount == 0)
    {
        //1码
        if( Color & (temp >> NowCount) )
        {
            RetVal |= (ONECODE>>1);
        }
        //0码
        else
        {
            RetVal |= (ZEROCODE>>1);
        }
    }
    //最后一个
    else if(NowCount == 8)
    {
        //1码
        if( Color & ( temp >> (NowCount-1) ) )
        {
            RetVal |= (ONECODE&0xFF);
        }
        //0码
        else
        {
            RetVal |= (ZEROCODE&0xFF);
        }
    }
    //中间的数
    else
    {
        uint8_t FrontNum = Color & ( temp >> (NowCount-1) );
        uint8_t Num = Color & ( temp >> NowCount );
        //1.Colord的前面一位bit和当前位都为1
        if(FrontNum && Num)
        {
            RetVal |= ONECODE<<(8-NowCount);
            RetVal |= ONECODE>>(NowCount+1);
        }
        //2.Colord的前面一位bit为1，当前位为0
        else if(FrontNum && !Num)
        {
            RetVal |= ONECODE<<(8-NowCount);
            RetVal |= ZEROCODE>>(NowCount+1);
        }
        //3.Colord的前面一位bit为0，当前位为1
        else if(!FrontNum && Num)
        {
            RetVal |= ZEROCODE<<(8-NowCount);
            RetVal |= ONECODE>>(NowCount+1);
        }
        //4.Colord的前面一位bit和当前位都为0
        else if(!FrontNum && !Num)
        {
            RetVal |= ZEROCODE<<(8-NowCount);
            RetVal |= ZEROCODE>>(NowCount+1);
        }
    }
    return RetVal;
}

/**
 * @brief 设置彩带单个LED的颜色
 * @param Red   红色的HEX
 * @param Green 绿色的HEX
 * @param Blue  蓝色的HEX
 * @note  1.注意发送给灯带的格式为GRB
 */
void LED_RibSetSingleLEDColor(uint8_t Red, uint8_t Green, uint8_t Blue)
{
    //uint8_t ResultBuff[28] = {0}; //不能把ResultBuff放在这个函数的内部，否则为局部变量，出了函数之后，局部变量会被销毁，DMA传输会出错
    uint8_t count = 0;
    for(count = 0; count < 9; count++)
    {
        ResultBuff[count] = SpecificByte(Green,count);
    }
    for(count = 0; count < 9; count++)
    {
        ResultBuff[count+9] = SpecificByte(Red,count);
    }
    for(count = 0; count < 9; count++)
    {
        ResultBuff[count+18] = SpecificByte(Blue,count);
    }
    /*等待SPI准备好*/
    while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
    while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

    HAL_SPI_Transmit_DMA(&hspi1,ResultBuff,28);
}


/**
 * @brief 复位彩带（也相当于是停止位）
 */
void LED_RibReset(void)
{
    uint8_t LowBuff[1] = {0x00};
    uint8_t HighBuff[1] = {0xFF};

    /*等待SPI发送完成*/
    while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
    /*等待SPI准备好*/
    while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

    /*产生RESET位*/
    HAL_SPI_Transmit_DMA(&hspi1,LowBuff,1);
    Delay_us(80);
    HAL_SPI_Transmit_DMA(&hspi1,HighBuff,1);

    /*产生完RESET位后再把引脚拉低防止影响下一次的时序*/
    while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
    while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
    HAL_SPI_Transmit_DMA(&hspi1,LowBuff,1);
}

void LED_RibALLLight(uint8_t Red, uint8_t Green, uint8_t Blue)
{
    uint8_t count = 0;
    for(count = 0; count < NUMBER_OF_ALL_LIGHT; count++)
    {
        LED_RibSetSingleLEDColor(Red,Green,Blue);
    }
    LED_RibReset();
}

/**
 * @brief 流水灯实现
 * @param Mode 选择流水灯的模式
 */
inline void Water_FallLight(WaterFallLight_Mode Mode)
{
    static uint8_t count = 0;
    uint8_t i = 0;
    static uint8_t WaterFallReboundMode = 0;
    switch(Mode)
    {
        case BackToStart:
        {
            for(i = 0; i<count; i++)
            {
                LED_RibSetSingleLEDColor(BlackColor);
            }
            LED_RibSetSingleLEDColor(WhiteColor);
            LED_RibSetSingleLEDColor(RedColor);
            LED_RibSetSingleLEDColor(MaroonColor);
            LED_RibSetSingleLEDColor(OrangeRedColor);
            LED_RibSetSingleLEDColor(OrangeColor);
            LED_RibSetSingleLEDColor(PinkColor);
            LED_RibSetSingleLEDColor(GreenColor);
            LED_RibSetSingleLEDColor(DarkGreenColor);
            LED_RibSetSingleLEDColor(CyanColor);
            LED_RibSetSingleLEDColor(IndigoColor);
            LED_RibSetSingleLEDColor(BlueColor);
            LED_RibSetSingleLEDColor(SkyBlueColor);
            LED_RibSetSingleLEDColor(VioletColor);
            LED_RibSetSingleLEDColor(YellowColor);
            LED_RibSetSingleLEDColor(LightBlueColor);
            LED_RibSetSingleLEDColor(WhiteColor);
            LED_RibSetSingleLEDColor(RedColor);
            LED_RibSetSingleLEDColor(MaroonColor);
            LED_RibSetSingleLEDColor(OrangeRedColor);
            LED_RibSetSingleLEDColor(OrangeColor);
            LED_RibSetSingleLEDColor(PinkColor);
            LED_RibSetSingleLEDColor(GreenColor);
            LED_RibSetSingleLEDColor(DarkGreenColor);
            LED_RibSetSingleLEDColor(CyanColor);
            LED_RibSetSingleLEDColor(IndigoColor);
            LED_RibSetSingleLEDColor(BlueColor);

            for(i = 0; i < (uint8_t)((NUMBER_OF_ALL_LIGHT-NUMBER_OF_WATERFALL_LIGHT)-count); i++)
            {
                LED_RibSetSingleLEDColor(BlackColor);
            }
            count++;
            if(count > (NUMBER_OF_ALL_LIGHT-NUMBER_OF_WATERFALL_LIGHT))count = 0;
        }break;
        case Rebound:
        {
            //1.灯带回到起始点
            if(count == 0)
            {
                WaterFallReboundMode = 0; //正向流水模式
            }
            //2。灯带达到终点
            else if(count == (NUMBER_OF_ALL_LIGHT-NUMBER_OF_WATERFALL_LIGHT))
            {
                WaterFallReboundMode = 1; //反向流水模式
            }
            //正向流水
            if(WaterFallReboundMode == 0)
            {
                for(i = 0; i<count; i++)
                {
                    LED_RibSetSingleLEDColor(BlackColor);
                }
                LED_RibSetSingleLEDColor(WhiteColor);
                LED_RibSetSingleLEDColor(RedColor);
                LED_RibSetSingleLEDColor(MaroonColor);
                LED_RibSetSingleLEDColor(OrangeRedColor);
                LED_RibSetSingleLEDColor(OrangeColor);
                LED_RibSetSingleLEDColor(PinkColor);
                LED_RibSetSingleLEDColor(GreenColor);
                LED_RibSetSingleLEDColor(DarkGreenColor);
                LED_RibSetSingleLEDColor(CyanColor);
                LED_RibSetSingleLEDColor(IndigoColor);
                LED_RibSetSingleLEDColor(BlueColor);
                LED_RibSetSingleLEDColor(SkyBlueColor);
                LED_RibSetSingleLEDColor(VioletColor);
                LED_RibSetSingleLEDColor(YellowColor);
                LED_RibSetSingleLEDColor(LightBlueColor);
                LED_RibSetSingleLEDColor(WhiteColor);
                LED_RibSetSingleLEDColor(RedColor);
                LED_RibSetSingleLEDColor(MaroonColor);
                LED_RibSetSingleLEDColor(OrangeRedColor);
                LED_RibSetSingleLEDColor(OrangeColor);
                LED_RibSetSingleLEDColor(PinkColor);
                LED_RibSetSingleLEDColor(GreenColor);
                LED_RibSetSingleLEDColor(DarkGreenColor);
                LED_RibSetSingleLEDColor(CyanColor);
                LED_RibSetSingleLEDColor(IndigoColor);
                LED_RibSetSingleLEDColor(BlueColor);
                LED_RibSetSingleLEDColor(SkyBlueColor);
                LED_RibSetSingleLEDColor(VioletColor);
                LED_RibSetSingleLEDColor(YellowColor);
                LED_RibSetSingleLEDColor(LightBlueColor);

                for(i = 0; i < (uint8_t)((NUMBER_OF_ALL_LIGHT-NUMBER_OF_WATERFALL_LIGHT)-count); i++)
                {
                    LED_RibSetSingleLEDColor(BlackColor);
                }
                count++;
            }
            //反向流水
            else if(WaterFallReboundMode == 1)
            {
                for(i = count; i>0; i--)
                {
                    LED_RibSetSingleLEDColor(BlackColor);
                }
                LED_RibSetSingleLEDColor(WhiteColor);
                LED_RibSetSingleLEDColor(RedColor);
                LED_RibSetSingleLEDColor(MaroonColor);
                LED_RibSetSingleLEDColor(OrangeRedColor);
                LED_RibSetSingleLEDColor(OrangeColor);
                LED_RibSetSingleLEDColor(PinkColor);
                LED_RibSetSingleLEDColor(GreenColor);
                LED_RibSetSingleLEDColor(DarkGreenColor);
                LED_RibSetSingleLEDColor(CyanColor);
                LED_RibSetSingleLEDColor(IndigoColor);
                LED_RibSetSingleLEDColor(BlueColor);
                LED_RibSetSingleLEDColor(SkyBlueColor);
                LED_RibSetSingleLEDColor(VioletColor);
                LED_RibSetSingleLEDColor(YellowColor);
                LED_RibSetSingleLEDColor(LightBlueColor);
                LED_RibSetSingleLEDColor(WhiteColor);
                LED_RibSetSingleLEDColor(RedColor);
                LED_RibSetSingleLEDColor(MaroonColor);
                LED_RibSetSingleLEDColor(OrangeRedColor);
                LED_RibSetSingleLEDColor(OrangeColor);
                LED_RibSetSingleLEDColor(PinkColor);
                LED_RibSetSingleLEDColor(GreenColor);
                LED_RibSetSingleLEDColor(DarkGreenColor);
                LED_RibSetSingleLEDColor(CyanColor);
                LED_RibSetSingleLEDColor(IndigoColor);
                LED_RibSetSingleLEDColor(BlueColor);
                LED_RibSetSingleLEDColor(SkyBlueColor);
                LED_RibSetSingleLEDColor(VioletColor);
                LED_RibSetSingleLEDColor(YellowColor);
                LED_RibSetSingleLEDColor(LightBlueColor);

                for(i = 0; i < ((uint8_t)(NUMBER_OF_ALL_LIGHT-NUMBER_OF_WATERFALL_LIGHT)-count); i++)
                {
                    LED_RibSetSingleLEDColor(BlackColor);
                }
                count--;
            }
        }break;
        default:printf("Water-Fall-Mode Not Found!\r\n");
    }
    LED_RibReset();
    Delay_ms(10);
}

/**
 * @brief 给所有LED灯随机一个颜色
 */
void LED_RibRandomLight(void)
{
    uint8_t count = 0;
    if(hLedRib.RandomLED.StartFlag == 1)
    {
        for(count = 0; count<hLedRib.NumOfAllLight; count++)
        {
            //当count为0时，采用下面的随机取样的话，RGB就都是一样的数值，所以第0个灯永远都是白色的，为了避免这种情况，就把LED0单独取样
            if(count == 0)
            {
                LED_RibSetSingleLEDColor(hLedRib.RandomLED.RandomBuff[0],hLedRib.RandomLED.RandomBuff[NUMBER_OF_ALL_LIGHT/2],hLedRib.RandomLED.RandomBuff[NUMBER_OF_ALL_LIGHT-1]);
            }
            else
            {
                LED_RibSetSingleLEDColor(hLedRib.RandomLED.RandomBuff[count],hLedRib.RandomLED.RandomBuff[count/2],hLedRib.RandomLED.RandomBuff[count/3]);
            }
        }
        hLedRib.RandomLED.StartFlag = 0;
        LED_RibReset();
        //memset(hLedRib.RandomLED.RandomBuff,0,hLedRib.NumOfAllLight); //清空数组
    }
    else
    {
        //nothing to do...
    }
}

void LED_RibInit(void)
{
    hLedRib.hspi = &hspi1;
    hLedRib.NumOfAllLight = NUMBER_OF_ALL_LIGHT;
    hLedRib.NumOfWaterFallLight = NUMBER_OF_WATERFALL_LIGHT;
    hLedRib.RandomLED.SwitchPeriod = 1000; //每1s的时间切换一次LED灯
    hLedRib.RandomLED.StartFlag = 0;
    hLedRib.RandomLED.RandomLightFun = LED_RibRandomLight;
    LED_RibALLLight(0x00,0x00,0x00); //把全部灯熄灭
}