//
// Created by 86159 on 2024-03-03.
//

#ifndef MY_FLYBACK_PID_CONTROL_H
#define MY_FLYBACK_PID_CONTROL_H

typedef struct
{
    float kp; //比例系数
    float ki; //积分系数
    float kd; //微分系数

    float LastVal;          //上一次进入函数时变量的值
    float PreVal;           //上上次进入函数时变量的值
    float TarVal;           //目标值
    float IntegralVal;      //积分项
    float OutValue;
    float MaxIntegralVal;   //积分项的最大值（用于积分限幅）
    float MaxOutVal;        //变量的最大输出值
}PID_Handle;

float PositionPID(PID_Handle *PID,float CurVal);

#endif //MY_FLYBACK_PID_CONTROL_H
