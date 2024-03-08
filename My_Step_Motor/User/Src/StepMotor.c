//
// Created by 86159 on 2023-11-01.
//
/**本工程的时钟频率为72MHZ,ARR为100,默认CCR为50
 * 目前测试的步进电机最多能够5圈/s
 * */
/**接线注意:1.步进电机驱动模块的MS2接高电平，MS1与MS3可以不接(默认低电平)----表细分(步进值)为1/4
 *        2.ENABLE管脚置0开始工作，置1停止工作,故可以不接
 *        3.Sleep管脚电平置0，进入睡眠模式，驱动器输出待机模式；Sleep管脚置1，驱动器处于正常工作状态
 *        4.复位模式下容易消耗能量，产生的冲击电流较大。直接RESET管脚置1，在不影响系统工作时RESET管脚置0复位。
 *           一旦驱动芯片复位，系统将回归到原始A4988 I/O端口控制状态；
 *         Step_Dir为步进方向控制脚
 * */
/****************************************************************************************************
 *******                                    MS3 MS2 MS1 步进值
 *******                                     0   0   0    1
 *******                                     0   0   1   1/2
 *******                                     0   1   0   1/4   ----本工程的细分值
 *******                                     1   1   0   1/8
 *******                                     1   1   1   1/16
 ***************************************************************************************************/
#include "StepMotor.h"
#include "tim.h"
#include "OLED.h"
#include "cmsis_os.h"
#define CLOCK_FREQ  72000000  //72MHZ
#define CLOCK_ARR   100
void Step_Motor_Init(StepMotor *SpM)
{
    SpM->htim = &htim2;
    SpM->Dir_GPIOPort = Step_Dir_GPIO_Port;
    SpM->Dir_GPIO_Pin = Step_Dir_Pin;
    SpM->Subdivision = 1.0/4; //细分值(1 1/2 1/4 1/8 1/16)一共五个细分值，若改了细分值，那么也要修改步进电机驱动器的细分为相应的高低电平
    SpM->PSC = (CLOCK_FREQ*1.0/CLOCK_ARR)/(1.0/SpM->Subdivision*200);
    SpM->CCR = 50;
    SpM->htim->Channel = TIM_CHANNEL_1;
}
/**
 * 函数功能: 关闭步进电机
 * @param SpM 步进电机句柄
 */
void Step_Motor_Stop(StepMotor *SpM)
{
    HAL_TIM_PWM_Stop(SpM->htim,SpM->htim->Channel); //关掉PWM输出,注意：该函数关闭了定时器，所以计数器也被关闭了，需要再手动打开计数器
    //__HAL_TIM_ENABLE(SpM->htim);
    SpM->htim->Instance->CR1 |= 0X01;  //使能TIM2的计数器，使得定时中断能够正常进行(注意：)
}
/**
 * 函数功能: 关闭步进电机
 * @param SpM 步进电机句柄
 */
void Step_Motor_Start(StepMotor *SpM)
{
    HAL_TIM_PWM_Start(SpM->htim,SpM->htim->Channel); //开启PWM输出
    //HAL_TIM_Base_Start_IT(SpM->htim);  //关掉定时器中断
}
/************************************************************************************************************************
 * 函数功能: 控制步进电机运动的速度
 * @param SpM           StepMotor的句柄
 * @param Speed         速度(0-5)代表1s转多少圈
 ************************************************************************************************************************/
void Step_Motor_SetSpeed(StepMotor *SpM,float Speed)
{
    double Pulse = 0;
    if(Speed != 0)
    {
        Step_Motor_Start(SpM);
        Pulse = (1.0/SpM->Subdivision)*200; //一圈的脉冲数
        SpM->PSC =  ((CLOCK_FREQ*1.0/CLOCK_ARR)*1.0/(Pulse*Speed));
    }
    else Step_Motor_Stop(SpM); //电机停转
}

/************************************************************************************************************************
 * 函数功能: 设置步进电机的速度
 * @param SpM           StepMotor的句柄
 * @param Dirction      方向(1:正转 0:反转)
 * @param CircleNum     步进电机运动的圈数
 *************************************************************************************************************************/
void Step_Motor_SetDistance(StepMotor *SpM,uint8_t Dirction,uint32_t CircleNum)
{
    double Pulse = (1.0/SpM->Subdivision)*200; //一圈的脉冲数
    //SpM->PSC =  ((CLOCK_FREQ*1.0/CLOCK_ARR)*1.0/(Pulse*Speed));
    double Freq = (CLOCK_FREQ*1.0/CLOCK_ARR)/SpM->PSC*1.0;  //时钟频率
    double StepTime = CircleNum*Pulse / Freq; //步进电机运动的时间
   // Step_Motor_Start(SpM);

        if(Dirction != 0)
        {
            HAL_GPIO_WritePin(SpM->Dir_GPIOPort,SpM->Dir_GPIO_Pin,1);
            __HAL_TIM_SET_PRESCALER(SpM->htim,SpM->PSC);
        }
        else
        {
            HAL_GPIO_WritePin(SpM->Dir_GPIOPort,SpM->Dir_GPIO_Pin,0);
            __HAL_TIM_SET_PRESCALER(SpM->htim,SpM->PSC);
        }
        osDelay(StepTime*1000);
        Step_Motor_Stop(SpM);
}
