//
// Created by 86159 on 2023-11-18.
//
#include "BrushlessMotor.h"

#define MyDelay osDelay
/**
  * 无刷电机初始化函数，默认占空比为0
  * @param BrushLessMotor 无刷电机句柄
  * @param htim           无刷电机所用的定时器
  * @param Channel TIM Channels to be configured.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
 */
void BrushLessMotor_Init(BRUSHLESSMOTOR *BrushLessMotor, TIM_HandleTypeDef *htim, uint32_t Channel)
{
    BrushLessMotor->htim = htim;
    BrushLessMotor->Channel = Channel;
    BrushLessMotor->Duty = 0;
    BrushLessMotor->Freq = BRUSHLESSMOTORFREQ; //无刷电机的工作频率（50HZ）
    BrushLessMotor->MaxPeriod = BRUSHLESSMOTORMAXPERIOD; //最大周期
    BrushLessMotor->MinPeriod = BRUSHLESSMOTORMINPERIOD;//最小周期
    BrushLessMotor->MidPeriod = BRUSHLESSMOTORMIDPERIOD;//中间周期
}
//把以us为单位的高电平时间转为占空比(0-1000)
uint16_t Period2Duty(uint16_t Period)
{
    return (uint16_t)((Period/1000.0)/(1.0/BRUSHLESSMOTORFREQ));
}
/**
 * 无刷电机的占空比配置函数（注意：仅配置，但还没改变占空比）
 * @param BrushLessMotor 无刷电机句柄
 * @param Period         高电平时间（us）
 */
inline void BrushLessMotor_Set(BRUSHLESSMOTOR *BrushLessMotor,uint16_t Period)
{
    BrushLessMotor->Period = Period;
    BrushLessMotor->Duty = Period2Duty(Period);
}

/**
 * 无刷电机的运动控制函数，控制无刷电机运动到相应的占空比
 * @param BrushLessMotor 无刷电机句柄
 */
inline void BrushLessMotor_Move(BRUSHLESSMOTOR BrushLessMotor)
{
    __HAL_TIM_SET_COMPARE(BrushLessMotor.htim,BrushLessMotor.Channel,BrushLessMotor.Duty*BRUSHLESSMOTORARR/1000);
}
/**
 * 无刷电机解除保护机制函数(弱定义（拿来玩玩）)
 * @param BrushLessMotor
 */
__attribute__((weak))void BrushLessMotor_Start(BRUSHLESSMOTOR BrushLessMotor)
{
    HAL_TIM_PWM_Start(BrushLessMotor.htim,BrushLessMotor.Channel);
    __HAL_TIM_SET_COMPARE(BrushLessMotor.htim,BrushLessMotor.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));
    MyDelay(3000);
    __HAL_TIM_SET_COMPARE(BrushLessMotor.htim,BrushLessMotor.Channel,Period2Duty(BRUSHLESSMOTORMIDPERIOD)*(BRUSHLESSMOTORARR/1000));
    MyDelay(3000);
}
/**
 * 同时启动三个无刷电机
 * @param BrushLessMotor1 无刷电机句柄一
 * @param BrushLessMotor2 无刷电机句柄二
 * @param BrushLessMotor3 无刷电机句柄三
 */
__attribute__((weak))void AllBrushlessMotor_Start(BRUSHLESSMOTOR BrushLessMotor1,BRUSHLESSMOTOR BrushLessMotor2,BRUSHLESSMOTOR BrushLessMotor3)
{
    HAL_TIM_PWM_Start(BrushLessMotor1.htim,BrushLessMotor1.Channel);
    HAL_TIM_PWM_Start(BrushLessMotor2.htim,BrushLessMotor2.Channel);
    HAL_TIM_PWM_Start(BrushLessMotor3.htim,BrushLessMotor3.Channel);

    __HAL_TIM_SET_COMPARE(BrushLessMotor1.htim,BrushLessMotor1.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));
    __HAL_TIM_SET_COMPARE(BrushLessMotor2.htim,BrushLessMotor2.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));
    __HAL_TIM_SET_COMPARE(BrushLessMotor3.htim,BrushLessMotor3.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));

    MyDelay(3000);
    __HAL_TIM_SET_COMPARE(BrushLessMotor1.htim,BrushLessMotor1.Channel,Period2Duty(BRUSHLESSMOTORMIDPERIOD)*(BRUSHLESSMOTORARR/1000));
    __HAL_TIM_SET_COMPARE(BrushLessMotor2.htim,BrushLessMotor2.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));
    __HAL_TIM_SET_COMPARE(BrushLessMotor3.htim,BrushLessMotor3.Channel,Period2Duty(BRUSHLESSMOTORMAXPERIOD)*(BRUSHLESSMOTORARR/1000));

    MyDelay(3000);
}
