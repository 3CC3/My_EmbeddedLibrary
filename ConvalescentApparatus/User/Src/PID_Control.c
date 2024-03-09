//
// Created by 86159 on 2024-03-03.
//
#include "PID_Control.h"

/**
 * 位置式PID
 * @param PID     PID句柄
 * @param CurVal  要控制的变量的当前值、
 * @retval 返回位置式PID的输出值
 */
float PositionPID(PID_Handle *PID,float CurVal)
{
    float Error = CurVal - PID->TarVal; //误差值
    float OutVal = 0; //PID控制器输出的值

    OutVal = PID->kp*Error+PID->ki*PID->IntegralVal+PID->kd*(CurVal-PID->LastVal);
    OutVal = OutVal<PID->MaxOutVal ? OutVal:PID->MaxOutVal;

    if(PID->IntegralVal < PID->MaxIntegralVal && PID->ki != 0) PID->IntegralVal += Error; //当积分项小于最大值 且 开启时积分器时才+
    PID->LastVal = CurVal;
    PID->OutValue = OutVal;
    return OutVal;
}

/**
 * 增量式PID
 * @param PID       PID句柄
 * @param CurVal    要控制的变量的当前值、
 * @return          返回位置式PID的增加值
 */
float IncrementalPID(PID_Handle *PID,float CurVal)
{
    float Error = PID->TarVal - CurVal;
    float OutVal = 0; //PID控制器输出的值

    OutVal = PID->kp*(Error-PID->LastVal)+PID->ki*Error+PID->kd*(Error-2*PID->LastVal+PID->PreVal);
    OutVal = OutVal<PID->MaxOutVal ? OutVal:PID->MaxOutVal;

    PID->PreVal = PID->LastVal;
    PID->LastVal = Error;
    return OutVal;
}