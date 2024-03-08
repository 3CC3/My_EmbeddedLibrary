//
// Created by 86159 on 2023-11-18.
//

#ifndef MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H
#define MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H

#define  BRUSHLESSMOTORARR                    20000 //重装载值为20000
#define  BRUSHLESSMOTORFREQ                   50    //无刷电机的工作频率
#define  BRUSHLESSMOTORMAXDUTY           11    //无刷电机的最大占空比(%)
#define  BRUSHLESSMOTORMINDUTY           4     //无刷电机的最小占空比(%)
#define BRUSHLESSMOTORMAXPERIOD          2200  //无刷电机的最大高电平时间 (us)
#define BRUSHLESSMOTORMINPERIOD          800   //无刷电机的最小高电平时间
#define BRUSHLESSMOTORMIDPERIOD          1500  //无刷电机的中间高电平时间

#include "main.h"
#include "cmsis_os.h"
typedef struct
{
    TIM_HandleTypeDef *htim; //无刷电机所使用的定时器
    uint32_t Channel;  //无刷电机所使用的通道
    uint16_t Duty;     //无刷电机当前占空比
    uint16_t Period;    //当前高电平时间(us)
    uint8_t Freq;      //无刷电机的工作频率
    uint16_t MaxPeriod;//无刷电机的最大高电平时间
    uint16_t MinPeriod;//无刷电机的最小高电平时间
    uint16_t MidPeriod;//无刷电机的中间高电平时间
}BRUSHLESSMOTOR;

void BrushLessMotor_Init(BRUSHLESSMOTOR *BrushLessMotor, TIM_HandleTypeDef *htim, uint32_t Channel);
void BrushLessMotor_Set(BRUSHLESSMOTOR *BrushLessMotor,uint16_t Period);
void BrushLessMotor_Move(BRUSHLESSMOTOR BrushLessMotor);
void BrushLessMotor_Start(BRUSHLESSMOTOR BrushLessMotor);
__attribute__((weak))void AllBrushlessMotor_Start(BRUSHLESSMOTOR BrushLessMotor1,BRUSHLESSMOTOR BrushLessMotor2,BRUSHLESSMOTOR BrushLessMotor3);


#endif //MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H
