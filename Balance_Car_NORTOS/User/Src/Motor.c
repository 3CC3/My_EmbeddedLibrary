//
// Created by 86159 on 2023-11-04.
//

/*** 使用到的引脚和端口，结合CubeMx使用时只需把对应端口的命名对应好，就不用开下面的define了
 *  #define MotorA_1_Pin GPIO_PIN_11
 *  #define MotorA_1_GPIO_Port GPIOA
 *  #define MotorA_2_Pin GPIO_PIN_12
 *  #define MotorA_2_GPIO_Port GPIOA
 */



#include "Motor.h"

/***
 * 函数功能: 初始化电机
 * @param Motor   电机句柄
 * @param name    电机名
 * @param htim    电机PWM波所用的定时器
 * @param Channel 电机PWM波的通道
 */

void Motor_Init(MOTOR *Motor,char* name,uint8_t MotorNumber,TIM_HandleTypeDef *htim,uint32_t Channel)
{
    Motor->MotorName = name;
    Motor->MotorNumber = MotorNumber;
    Motor->htim = *htim;
    Motor->Channel = Channel;
    Motor->Direction = FORWARD_ROTATION; //正转
    Motor->Speed = 0;
    HAL_TIM_PWM_Start(&Motor->htim,Motor->Channel);
}
/***
 * 函数功能: 设置电机转速
 * @param Motor 电机句柄
 * @param Speed 电机转速(-MAXSPEED --- MAXSPEED)
 */
void Set_MotorSpeed(MOTOR *Motor,int16_t Speed)
{
    uint8_t Dir = 1;
    if(Speed < 0)
    {
        Speed = -Speed; //把Speed转为正的
        Dir = 2; //反转
    }
    Set_MotorDirc(Motor->MotorNumber,Dir); //控制电机转向
    __HAL_TIM_SET_COMPARE(&Motor->htim,Motor->Channel,Speed*1.0/MAXSPEED*MOTOR_ARR);
}
void Set_MotorDirc(uint8_t Motor,uint8_t Dir)
{
    if(Motor == 1)
    {
        if(Dir == 1){
            HAL_GPIO_WritePin(MotorA_1_GPIO_Port,MotorA_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(MotorA_2_GPIO_Port,MotorA_2_Pin,GPIO_PIN_RESET);
        }
        else if(Dir == 2){
            HAL_GPIO_WritePin(MotorA_1_GPIO_Port,MotorA_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MotorA_2_GPIO_Port,MotorA_2_Pin,GPIO_PIN_SET);
        }
    }
    else if(Motor == 2)
    {
        if(Dir == 1){
            HAL_GPIO_WritePin(MotorB_1_GPIO_Port,MotorB_1_Pin,GPIO_PIN_SET);
            HAL_GPIO_WritePin(MotorB_2_GPIO_Port,MotorB_2_Pin,GPIO_PIN_RESET);
        }
        else if(Dir == 2){
            HAL_GPIO_WritePin(MotorB_1_GPIO_Port,MotorB_1_Pin,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MotorB_2_GPIO_Port,MotorB_2_Pin,GPIO_PIN_SET);
        }
    }
}

/**
 * 限制最大PWM输出
 * @param Motor 电机句柄
 */
void Limit_MotorSpeed(MOTOR *Motor)
{
    if(Motor->Speed>MAXSPEED)
    {
        Motor->Speed = MAXSPEED;
    }
    else if(Motor->Speed<-MAXSPEED)
    {
        Motor->Speed = -MAXSPEED;
    }
}