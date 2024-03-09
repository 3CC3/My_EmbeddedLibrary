//
// Created by 86159 on 2024-01-26.
//

#ifndef MY_FLYBACK_SERVO_H
#define MY_FLYBACK_SERVO_H

#include "main.h"
#include "tim.h"

typedef struct
{
    char *Name;
    TIM_HandleTypeDef *TIM;
    uint32_t Channel;
    uint8_t Angle;  //角度（0-180）
}SERVOHandle;

void Servo_Init(SERVOHandle *Servo,TIM_HandleTypeDef *htim,uint32_t Channel);
void ServoSetAngle(SERVOHandle *Servo,uint8_t Angle);


#endif //MY_FLYBACK_SERVO_H
