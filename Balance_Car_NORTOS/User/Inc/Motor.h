//
// Created by 86159 on 2023-11-04.
//

#ifndef MY_FREERTOS_TEMPLATE_MOTOR_H
#define MY_FREERTOS_TEMPLATE_MOTOR_H

#include "main.h"
#include "tim.h"


#define MAXSPEED        100  //定义电机最大转速为100（最大反向转速为-100）
#define MOTOR_PSC       7200
#define MOTOR_ARR       100
#define FORWARD_ROTATION 1   //正转
#define Reverse_ROTATION 0   //反转
#define MOTORA 1
#define MOTORB 2
typedef struct
{
    char* MotorName;        //电机名
    uint8_t MotorNumber;    //电机编号，用于区分不同的电机
    TIM_HandleTypeDef htim; //电机所用的定时器
    uint8_t Direction;      //电机转动的方向
    int16_t Speed;         //电机转动的速度(0-100)
    uint32_t Channel;       //电机所用通道
}MOTOR;
typedef enum{
    Left = 1,
    Right,
}MotorDirection;
void Motor_Init(MOTOR *Motor,char* name,uint8_t MotorNumber,TIM_HandleTypeDef *htim,uint32_t Channel);
void Set_MotorSpeed(MOTOR *Motor,int16_t Speed);
void Limit_MotorSpeed(MOTOR *Motor);
void Set_MotorDirc(uint8_t Motor,uint8_t Dir);



#endif //MY_FREERTOS_TEMPLATE_MOTOR_H
