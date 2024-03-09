//
// Created by 86159 on 2023-11-18.
//
#include "BrushlessMotor.h"
#include "cmsis_os.h"
#include "PID_Control.h"

#define MyDelay osDelay

extern PID_Handle BrushLessPID;

//把以us为单位的高电平时间转为占空比(0-1000)
static uint16_t Period2Duty(uint16_t Period)
{
    return (uint16_t)((Period/1000.0)/(1.0/BRUSHLESSMotorFREQ));
}

#define Period2CCR(x) Period2Duty(x)*(BRUSHLESSMotorARR/1000)

/**
  * 无刷电机初始化函数，默认占空比为0
  * @param BrushLessMotor 无刷电机句柄
  * @param htim           无刷电机所用的定时器
  * @param Channel TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
 */
void BrushLessMotor_Init(BRUSHLESS *BrushLessMotorHandle, TIM_HandleTypeDef *htim)
{
    BrushLessMotorHandle->htim = htim;
    BrushLessMotorHandle->Freq = BRUSHLESSMotorFREQ; //无刷电机的工作频率（50HZ）
    BrushLessMotorHandle->MaxPeriod = MotorMaxPERIOD; //最大周期
    BrushLessMotorHandle->MinPeriod = MotorMinPERIOD;//最小周期
    BrushLessMotorHandle->MidPeriod = MotorMidPERIOD;//中间周期
    BrushLessMotorHandle->MotorSetFun = BrushLessMotor_Set;
    BrushLessMotorHandle->MotorMoveFun = BrushLessMotor_Move;
    BrushLessMotorHandle->MotorAllMoveFun = BrushLessMotor_AllMove;
    BrushLessMotorHandle->MotorAllStopFun = BrushLessMotor_AllStop;

    /*所有无刷电机逐个初始化*/
    BrushLessMotorHandle->AllBLMotorArry[Right].Name = (uint8_t*)"Right";
    BrushLessMotorHandle->AllBLMotorArry[Right].Channel = TIM_CHANNEL_1;
    BrushLessMotorHandle->AllBLMotorArry[Right].Duty = 0;
    BrushLessMotorHandle->AllBLMotorArry[Right].Period = 0;

    BrushLessMotorHandle->AllBLMotorArry[Left].Name = (uint8_t*)"Left";
    BrushLessMotorHandle->AllBLMotorArry[Left].Channel = TIM_CHANNEL_2;
    BrushLessMotorHandle->AllBLMotorArry[Left].Duty = 0;
    BrushLessMotorHandle->AllBLMotorArry[Left].Period = 0;

    BrushLessMotorHandle->AllBLMotorArry[Below].Name = (uint8_t*)"Below";
    BrushLessMotorHandle->AllBLMotorArry[Below].Channel = TIM_CHANNEL_3;
    BrushLessMotorHandle->AllBLMotorArry[Below].Duty = 0;
    BrushLessMotorHandle->AllBLMotorArry[Below].Period = 0;

}

/**
 * 无刷电机的占空比配置函数（注意：仅配置，但还没改变占空比）
 * @param BrushLessMotor 无刷电机句柄
 * @param Period         高电平时间（us）
 */
void BrushLessMotor_Set(BRUSHLESS *BrushLessMotorHandle,MOTORSelect MotorSite,uint16_t Period)
{
    BrushLessMotorHandle->AllBLMotorArry[MotorSite].Period = Period;
    BrushLessMotorHandle->AllBLMotorArry[MotorSite].Duty = Period2Duty(Period);
}

/**
 * 无刷电机的运动控制函数，控制无刷电机运动到相应的占空比
 * @param BrushLessMotor 无刷电机句柄
 */
inline void BrushLessMotor_Move(BRUSHLESS BrushLessMotorHandle,MOTORSelect MotorSite)
{
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle.htim, BrushLessMotorHandle.AllBLMotorArry[MotorSite].Channel, Period2CCR(BrushLessMotorHandle.AllBLMotorArry[MotorSite].Period));
}
/**
 * 所有电机按指定值转动
 * @param BrushLessMotorHandle
 */
void BrushLessMotor_AllMove(BRUSHLESS BrushLessMotorHandle)
{
    BrushLessMotor_Move(BrushLessMotorHandle,Right);
    BrushLessMotor_Move(BrushLessMotorHandle,Left);
    BrushLessMotor_Move(BrushLessMotorHandle,Below);
}
/**
 * 使所有电机停下
 * @param BrushLessMotorHandle
 */
void BrushLessMotor_AllStop(BRUSHLESS *BrushLessMotorHandle)
{
    BrushLessMotor_Set(BrushLessMotorHandle,Right,MotorMidPERIOD);
    BrushLessMotor_Set(BrushLessMotorHandle,Left,MotorMidPERIOD);
    BrushLessMotor_Set(BrushLessMotorHandle,Below,MotorMidPERIOD);

    BrushLessMotor_AllMove(*BrushLessMotorHandle);
}
/**
 * 无刷电机解除保护机制函数(弱定义（拿来玩玩）)
 * @param BrushLessMotor
 */
__attribute__((weak))void BrushLessMotor_Start(BRUSHLESS BrushLessMotorHandle, MOTORSelect MotorSite)
{
    HAL_TIM_PWM_Start(BrushLessMotorHandle.htim,BrushLessMotorHandle.AllBLMotorArry[MotorSite].Channel);
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle.htim,BrushLessMotorHandle.AllBLMotorArry[MotorSite].Channel,Period2CCR(MotorMaxPERIOD));
    MyDelay(4000);
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle.htim,BrushLessMotorHandle.AllBLMotorArry[MotorSite].Channel,Period2CCR(MotorMinPERIOD));
    MyDelay(4000);
}
/**
 * 同时启动三个无刷电机
 * @param BrushLessMotor1 无刷电机句柄一
 * @param BrushLessMotor2 无刷电机句柄二
 * @param BrushLessMotor3 无刷电机句柄三
 */
__attribute__((weak))void AllBrushlessMotor_Start(BRUSHLESS *BrushLessMotorHandle)
{
    HAL_TIM_PWM_Start(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Right].Channel);
    HAL_TIM_PWM_Start(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Left].Channel );
    HAL_TIM_PWM_Start(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Below].Channel);

    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Right].Channel, Period2CCR(MotorMaxPERIOD));
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Left].Channel , Period2CCR(MotorMaxPERIOD));
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Below].Channel, Period2CCR(MotorMaxPERIOD));

    MyDelay(4000);
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Right].Channel, Period2CCR(MotorMidPERIOD));
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Left].Channel , Period2CCR(MotorMidPERIOD));
    __HAL_TIM_SET_COMPARE(BrushLessMotorHandle->htim, BrushLessMotorHandle->AllBLMotorArry[Below].Channel, Period2CCR(MotorMidPERIOD));

    MyDelay(4000);

    /*默认所有无刷电机的周期为MotorMidPERIOD，即是使得电机停止的周期*/
    BrushLessMotorHandle->MotorSetFun(BrushLessMotorHandle,Right,MotorMidPERIOD);
    BrushLessMotorHandle->MotorSetFun(BrushLessMotorHandle,Left,MotorMidPERIOD);
    BrushLessMotorHandle->MotorSetFun(BrushLessMotorHandle,Below,MotorMidPERIOD);

}

/**
 * 接收到开始命令时的开始调整函数
 * @param Roll             横滚角，监测患者是否
 * @param BrushLessMotor1
 * @param BrushLessMotor2
 * @param BrushLessMotor3
 */
void Posture_Adjust(BRUSHLESS *BrushLessMotorHandle,MPUHandle Mpu)
{

//    if(Mpu.roll >= 165 || Mpu.roll <= -160) //垂直状态，电机不工作
//    {
//        BrushLessMotor_AllStop(BrushLessMotorHandle);
//    }
//    else if(0 <= Mpu.roll && Mpu.roll< 165) //下肢偏右
//    {
//        BrushLessMotor_Set(BrushLessMotorHandle,Right,); //把水外右推，给一个向左的回复力
//
//        BrushLessMotor_AllMove(*BrushLessMotorHandle);
//    }
//    else if(0 >= Mpu.roll && Mpu.roll > -160) //下肢偏左
//    {
//        BrushLessMotor_Set(BrushLessMotorHandle,Left,); //把水外左推，给一个向右的回复力
//        BrushLessMotor_AllMove(*BrushLessMotorHandle);
//    }
    if(BrushLessPID.OutValue > 0) //下肢右偏
    {
        BrushLessMotor_Set(BrushLessMotorHandle,Right,(uint16_t)BrushLessPID.OutValue*7+MotorMidPERIOD); //把水外右推，给一个向左的回复力
    }
    else if(BrushLessPID.OutValue < 0) //下肢左偏
     {
         BrushLessMotor_Set(BrushLessMotorHandle,Left,(uint16_t)BrushLessPID.OutValue*7+MotorMidPERIOD); //把水外左推，给一个向右的回复力
     }
    BrushLessMotor_AllMove(*BrushLessMotorHandle);
}

float MotorLink2PID(float roll)
{
    float OutVal = 0;
    if(roll >= 165 || roll <= 160) OutVal = 0;
    else if(0 <= roll && roll< 165) //下肢偏右
    {
        OutVal = 180 - roll;
    }
    else if(0 >= roll && roll > -160) //下肢偏左
    {
        OutVal = -(180 + roll);
    }
    return roll;
}