//
// Created by 86159 on 2023-11-01.
//

#ifndef MY_FREERTOS_TEMPLATE_STEPMOTOR_H
#define MY_FREERTOS_TEMPLATE_STEPMOTOR_H
#include "main.h"

typedef struct
{
    TIM_HandleTypeDef *htim; //步进电机所用于产生PWM波的定时器句柄
    double PSC;//定时器的预分频值
    uint16_t CCR;//定时器的自动重装值
    GPIO_TypeDef *Dir_GPIOPort; //步进方向控制端口
    uint16_t Dir_GPIO_Pin;
    double Subdivision; //步进电机的细分值
}StepMotor;
void Step_Motor_SetSpeed(StepMotor *SpM,float Speed);
void Step_Motor_SetDistance(StepMotor *SpM,uint8_t Dirction,uint32_t CircleNum);
void Step_Motor_Init(StepMotor *SpM);

#endif //MY_FREERTOS_TEMPLATE_STEPMOTOR_H
