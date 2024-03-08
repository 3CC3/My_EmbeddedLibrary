//
// Created by 86159 on 2023-10-20.
//

#ifndef MY_FREERTOS_TEMPLATE_OLED_H
#define MY_FREERTOS_TEMPLATE_OLED_H

#include "main.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFNum(uint8_t Line, uint8_t Column, float Number, uint8_t InLength, uint8_t DeLength);
uint8_t OLED_ReadStatus(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_Float(unsigned char Y,unsigned char X,double real,unsigned char N);

#endif //MY_FREERTOS_TEMPLATE_OLED_H
