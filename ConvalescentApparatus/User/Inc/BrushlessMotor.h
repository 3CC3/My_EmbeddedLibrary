//
// Created by 86159 on 2023-11-18.
//

#ifndef MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H
#define MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H

#define  BRUSHLESSMotorNum          3 //一共有3个无刷电机

#define  BRUSHLESSMotorARR                    20000 //重装载值为20000
#define  BRUSHLESSMotorFREQ                   50    //无刷电机的工作频率
#define  BRUSHLESSMotorMAXDUTY           11    //无刷电机的最大占空比(%)
#define  BRUSHLESSMotorMINDUTY           4     //无刷电机的最小占空比(%)
#define  MotorMaxPERIOD          2200  //无刷电机的最大高电平时间us (正向最快)
#define  MotorMinPERIOD          800   //无刷电机的最小高电平时间（反向最快）
#define  MotorMidPERIOD          1500  //无刷电机的中间高电平时间（停止）
#define  MotorMidPERIOD2         1480  //双向模式初始化时，若用上面那个没成功，就试试这个

#include "main.h"
#include "MPU6050.h"

typedef enum
{
    Right = 0,
    Left,
    Below,
}MOTORSelect;
typedef struct
{
    uint8_t *Name;      //名称（便于Debug）
    uint32_t Channel;  //无刷电机所使用的通道
    uint16_t Duty;     //无刷电机当前占空比
    uint16_t Period;    //当前高电平时间(us)
}BrushLessSingleMotor;
typedef struct BRUSHLESSMotor
{
    BrushLessSingleMotor AllBLMotorArry[BRUSHLESSMotorNum]; //该数组包含了所有无刷电机
    TIM_HandleTypeDef *htim; //无刷电机所使用的定时器
    uint8_t Freq;      //无刷电机的工作频率
    uint16_t MaxPeriod;//无刷电机的最大高电平时间
    uint16_t MinPeriod;//无刷电机的最小高电平时间
    uint16_t MidPeriod;//无刷电机的中间高电平时间
    void (*MotorSetFun)(struct BRUSHLESSMotor*,MOTORSelect,uint16_t Period);
    void (*MotorMoveFun)(struct BRUSHLESSMotor,MOTORSelect);
    void (*MotorAllStopFun)(struct BRUSHLESSMotor *BrushLessMotorHandle);
    void (*MotorAllMoveFun)(struct BRUSHLESSMotor BrushLessMotorHandle);
}BRUSHLESS;

void BrushLessMotor_Init(BRUSHLESS *BrushLessMotorHandle, TIM_HandleTypeDef *htim);
void BrushLessMotor_Set(BRUSHLESS *BrushLessMotorHandle,MOTORSelect MotorSite,uint16_t Period);
void BrushLessMotor_Move(BRUSHLESS BrushLessMotorHandle,MOTORSelect MotorSite);
__attribute__((weak))void BrushLessMotor_Start(BRUSHLESS BrushLessMotorHandle, MOTORSelect MotorSite);
__attribute__((weak))void AllBrushlessMotor_Start(BRUSHLESS *BrushLessMotor);
void BrushLessMotor_AllStop(BRUSHLESS *BrushLessMotorHandle);
void BrushLessMotor_AllMove(BRUSHLESS BrushLessMotorHandle);
void Posture_Adjust(BRUSHLESS *BrushLessMotorHandle,MPUHandle Mpu);
float MotorLink2PID(float roll);


#endif //MY_FREERTOS_TEMPLATE_BRUSHLESSMOTOR_H
