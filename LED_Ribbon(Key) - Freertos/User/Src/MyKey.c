//
// Created by 86159 on 2024-02-08.
//

#include "MyKey.h"
//#include "cmsis_os.h"
#include "LED_Ribbon.h"

KeyHandle hKey;


/**
 * @brief 读按键值，并且处理标志位
 * @note 该函数应该要在程序(中断)中每20ms轮询一次
 */
void KeyRead(void)
{
    hKey.KeyVal = HAL_GPIO_ReadPin(hKey.KeyPort,hKey.Key_Pin); //读按键的值
    //按键松开
    if(hKey.KeyPressFlag == 1 && hKey.KeyVal == 1)
    {
        hKey.KeyPressFlag = 0;
        hKey.KeyLooseFlag = 1;
        hKey.KeyPressNum++;
    }
    //按键按下
    if(hKey.KeyLooseFlag == 0 && hKey.KeyVal == 0)
    {
        hKey.KeyPressFlag = 1;
    }
    //osDelay(10);
}

uint8_t GetKeyVal(void)
{
    return hKey.KeyVal;
}

/**
 * @brief 按下按键后要做的任务
 */
void PressKey2DoTask(void)
{
    if(hKey.KeyLooseFlag == 1)
    {   /*What to do when you press the key*/
        switch(hKey.KeyPressNum)
        {
            case 1:
            {
                hLedRib.RuningMode = RandomLightMode;
            }break;
            case 2:
            {
                hLedRib.RuningMode = WaterFallBack2Start;
            }break;
            case 3:
            {
                hLedRib.RuningMode = WaterFallRebound;

            }break;
            default:
            {
                hKey.KeyPressNum = 0;
                hLedRib.RuningMode = DefaultMode;
            }
        }
        /*----------------------------------*/
        hKey.KeyLooseFlag = 0;
    }
}
/**
 * @brief 按键初始化
 */
void KeyInit(void)
{
    hKey.KeyPort = KEY1_GPIO_Port;
    hKey.Key_Pin = KEY1_Pin;
    hKey.KeyVal = 1;
    hKey.KeyPressNum = 0;

    hKey.KeyLooseFlag = 0;
    hKey.KeyPressFlag = 0;
    hKey.KeyReadFun = KeyRead;
}