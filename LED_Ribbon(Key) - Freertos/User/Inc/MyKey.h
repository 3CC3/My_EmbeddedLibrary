//
// Created by 86159 on 2024-02-08.
//

#ifndef MY_FLYBACK_MYKEY_H
#define MY_FLYBACK_MYKEY_H

#include "main.h"

typedef struct
{
    GPIO_TypeDef    *KeyPort;
    uint16_t        Key_Pin;
    uint8_t         KeyVal:1;      //按键的值(只使用了一个Bit)
    uint8_t         KeyPressFlag:1; //按键按下标志位
    uint8_t         KeyLooseFlag:1;//松手标志位
    uint16_t        KeyPressNum;    //按下按键的数目（不一定是总次数，可以被用户应用程序改变）
    void(*KeyReadFun)(void);           //读取函数
}__attribute__((packed))KeyHandle;

extern KeyHandle hKey;
void KeyInit(void);
void KeyRead(void);
void PressKey2DoTask(void);

#endif //MY_FLYBACK_MYKEY_H
