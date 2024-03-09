//
// Created by 86159 on 2023/6/10.
//

#include "Motor.h"

//Motor: 选择是电机1还是电机2
// Dir:设置电机的转向，1：左转 2：右转
void Set_MotorDirc(MotorSelect Motor,MotorDirection Dir)
{
    if(Motor == 1)
    {
        if(Dir == 1){
            HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_RESET);
        }
        else if(Dir == 2){
            HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_SET);
        }
    }
    else if(Motor == 2)
    {
        if(Dir == 1){
            HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_RESET);
        }
        else if(Dir == 2){
            HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_SET);
        }
    }
}
//Motor:选择电机1还是电机2  CCR:设置的输出比较的值(0-100)
void Set_MotorCCR(MotorSelect Motor,uint16_t CCR)
{
    if(Motor == 1){  //选择电机1
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, CCR);
    }
    else if(Motor == 2)
    {
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, CCR);
    }
}
