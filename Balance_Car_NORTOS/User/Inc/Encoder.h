//
// Created by 86159 on 2023-11-04.
//

#ifndef MY_FREERTOS_TEMPLATE_ENCODER_H
#define MY_FREERTOS_TEMPLATE_ENCODER_H
#include "main.h"


typedef struct
{
    TIM_HandleTypeDef htim; //编码器所用的定时器
    int16_t EncoderSpeed;   //编码器的速度
}ENCODER;
extern int16_t EncoderSpeed;

void Encoder_Init(ENCODER *Encoder,TIM_HandleTypeDef *htim);
int16_t Encoder_Get(ENCODER *Encoder);

#endif //MY_FREERTOS_TEMPLATE_ENCODER_H
