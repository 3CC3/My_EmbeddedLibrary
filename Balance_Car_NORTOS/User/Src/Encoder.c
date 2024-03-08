//
// Created by 86159 on 2023-11-04.
//
#include "Encoder.h"

void Encoder_Init(ENCODER *Encoder,TIM_HandleTypeDef *htim)
{
    Encoder->htim = *htim;
    Encoder->EncoderSpeed = 0;
    HAL_TIM_Encoder_Start(htim,TIM_CHANNEL_ALL);
}
/**
 *
 * @param EncoderSelect 1:编码器一 2:编码器二
 * @return 返回编码器测得的速度
 */
int16_t Encoder_Get(ENCODER *Encoder)
{
    return -Encoder->EncoderSpeed;
}