//
// Created by 86159 on 2023-11-04.
//

#ifndef MY_FREERTOS_TEMPLATE_MY_I2C_H
#define MY_FREERTOS_TEMPLATE_MY_I2C_H
#include "main.h"

void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
uint8_t MyI2C_R_SDA(void);
void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif //MY_FREERTOS_TEMPLATE_MY_I2C_H
