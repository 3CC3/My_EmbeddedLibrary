//
// Created by 86159 on 2023/6/10.
//

#ifndef CAR_1_MOTOR_H
#define CAR_1_MOTOR_H
#include "main.h"
#include "tim.h"
typedef enum{
    Left = 1,
    Right,
}MotorDirection;
typedef enum{
    Motor1 = 1,
    Motor2,
}MotorSelect;
void Set_MotorDirc(MotorSelect Motor,MotorDirection Dir);
void Set_MotorCCR(MotorSelect Motor,uint16_t CCR);
#endif //CAR_1_MOTOR_H
