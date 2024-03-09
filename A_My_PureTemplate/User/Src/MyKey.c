//
// Created by 86159 on 2024-02-08.
//

#include "MyKey.h"

KeyHandle hKey;
KeyHandle hKey2;
KeyHandle hKey3;

/**
 * @brief 读按键值，并且处理标志位
 * @note 该函数应该要在程序(中断)中每20ms轮询一次
 */
void KeyRead(void)
{
    /** KEY **/
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


    /** KEY2 **/
    hKey2.KeyVal = HAL_GPIO_ReadPin(hKey2.KeyPort,hKey2.Key_Pin); //读按键的值
    //按键松开
    if(hKey2.KeyPressFlag == 1 && hKey2.KeyVal == 1)
    {
        hKey2.KeyPressFlag = 0;
        hKey2.KeyLooseFlag = 1;
        hKey2.KeyPressNum++;
    }
    //按键按下
    if(hKey2.KeyLooseFlag == 0 && hKey2.KeyVal == 0)
    {
        hKey2.KeyPressFlag = 1;
    }

    /** KEY3 **/
    hKey3.KeyVal = HAL_GPIO_ReadPin(hKey3.KeyPort,hKey3.Key_Pin); //读按键的值
    //按键松开
    if(hKey3.KeyPressFlag == 1 && hKey3.KeyVal == 1)
    {
        hKey3.KeyPressFlag = 0;
        hKey3.KeyLooseFlag = 1;
        hKey3.KeyPressNum++;
    }
    //按键按下
    if(hKey3.KeyLooseFlag == 0 && hKey3.KeyVal == 0)
    {
        hKey3.KeyPressFlag = 1;
    }

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
    if(hKey.KeyLooseFlag == 1 || hKey2.KeyLooseFlag == 1 || hKey3.KeyLooseFlag == 1)
    {   /*What to do when you press the key*/


        /*----------------------------------*/
        hKey.KeyLooseFlag = 0;
        hKey2.KeyLooseFlag = 0;
        hKey3.KeyLooseFlag = 0;
    }
}
/**
 * @brief 按键初始化
 */
void KeyInit(void)
{
    /** KEY **/
/*
    hKey.KeyPort = KEY1_GPIO_Port;
    hKey.Key_Pin = KEY1_Pin;
    hKey.KeyVal = 1;        //引脚默认上拉
    hKey.KeyPressNum = 0;

    hKey.KeyLooseFlag = 0;
    hKey.KeyPressFlag = 0;
    hKey.KeyReadFun = KeyRead;
*/


    /** KEY2 **/
/*
    hKey2.KeyPort = KEY2_GPIO_Port;
    hKey2.Key_Pin = KEY2_Pin;
    hKey2.KeyVal = 1;
    hKey2.KeyPressNum = 0;

    hKey2.KeyLooseFlag = 0;
    hKey2.KeyPressFlag = 0;
    hKey2.KeyReadFun = KeyRead;
*/

    /** KEY3 **/
/*
    hKey3.KeyPort = KEY3_GPIO_Port;
    hKey3.Key_Pin = KEY3_Pin;
    hKey3.KeyVal = 1;
    hKey3.KeyPressNum = 0;

    hKey3.KeyLooseFlag = 0;
    hKey3.KeyPressFlag = 0;
    hKey3.KeyReadFun = KeyRead;
*/

}