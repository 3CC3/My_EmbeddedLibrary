//
// Created by 86159 on 2023-09-19.
//
#include "Servo.h"
#include "retarget.h"
//Angle取值在0-180之间
/**
 * 舵机初始化
 * @param Servo   舵机句柄
 * @param htim    PWM输出控制舵机所用到的TIM
 * @param Channel TIM Channels to be configured.
 *          This parameter can be one of the following values:
 *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
 *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
 *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
 *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
 */
void Servo_Init(SERVOHandle *Servo,TIM_HandleTypeDef *htim,uint32_t Channel)
{
    Servo->Name = "MQTTServo";
    Servo->Angle = 0;
    Servo->TIM = htim;
    Servo->Channel = Channel;
    HAL_TIM_PWM_Start(Servo->TIM,Servo->Channel);
}

void ServoSetAngle(SERVOHandle *Servo,uint8_t Angle)
{
    uint8_t tempnum = (Servo->TIM->Instance->ARR+1)/20; //得到1ms相当于tempnum大小的ARR（舵机固定工作在20ms）
    uint16_t ServoCCR = Angle*tempnum/180+tempnum;
    printf("ServoCCR = %d\r\n",ServoCCR);
    __HAL_TIM_SET_COMPARE(Servo->TIM,Servo->Channel,ServoCCR);
    Servo->Angle = Angle;
}