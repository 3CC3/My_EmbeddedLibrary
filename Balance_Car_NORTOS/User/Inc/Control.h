//
// Created by 86159 on 2023-11-05.
//

#ifndef BALANCE_CAR_NORTOS_CONTROL_H
#define BALANCE_CAR_NORTOS_CONTROL_H
#include "main.h"
typedef struct
{
    float Kp;
    float ki;
    float kd;
    float ExpecteedValue;
}PID_PARAMETER;
void PID_Init(PID_PARAMETER *PID,float Expected_Value,float Kp,float Ki,float Kd);
float PID_VerticalControl(PID_PARAMETER PID,float True_Pitch,float GYROy);
float PID_SpeedControl(PID_PARAMETER PID,float True_Speed1,float True_Speed2);
float PID_TurnControl(PID_PARAMETER PID,float Yaw);

#endif //BALANCE_CAR_NORTOS_CONTROL_H
