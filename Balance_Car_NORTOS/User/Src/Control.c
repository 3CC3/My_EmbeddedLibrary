//
// Created by 86159 on 2023-11-05.
//
#include "Control.h"
#include "Encoder.h"
void PID_Init(PID_PARAMETER *PID,float Expected_Value,float Kp,float Ki,float Kd)
{
    PID->ExpecteedValue = Expected_Value;
    PID->Kp = Kp;
    PID->ki = Ki;
    PID->kd = Kd;
}
/**以下两个PID均为位置式PID**/
//PID直立环
float PID_VerticalControl(PID_PARAMETER PID,float True_Pitch,float GYROy)
{
    float PWM_Out = 0,Error = PID.ExpecteedValue - True_Pitch;
    PWM_Out = PID.Kp*Error+PID.kd*(GYROy);
    return PWM_Out;
}
/**速度环PID
 * 速度环加入低通滤波目的是为了减小速度环对速度的影响，毕竟直立才是最重要的
 * **/
float PID_SpeedControl(PID_PARAMETER PID,float True_Speed1,float True_Speed2)
    {
        static float Encoder_Integral,PWM_Out,Encoder_Error,Encoder_LowOut,Encoder_LastLowOut;
        float a = (float)0.7;
        Encoder_Error = PID.ExpecteedValue - (True_Speed1+True_Speed2)/2;
        Encoder_LowOut = a*Encoder_Error+(1-a)*Encoder_LastLowOut; //低通滤波
        Encoder_Integral+=Encoder_LowOut;
        //积分限幅
        if(Encoder_Integral>10000)
        {
            Encoder_Integral = 10000;
        }
        else if(Encoder_Integral<-10000)
        {
            Encoder_Integral = -10000;
        }
    //Encoder_Integral = Encoder_Integral>10000?10000:(Encoder_Integral<(-10000)?-10000:Encoder_Integral);
    PWM_Out = PID.Kp*Encoder_LowOut+PID.ki*Encoder_Integral;
    Encoder_LastLowOut = Encoder_LowOut;
    return PWM_Out;
}

/** 转向环PID
*
*/
float PID_TurnControl(PID_PARAMETER PID,float Yaw)
{
    float turn = 0;
    turn = (PID.ExpecteedValue-Yaw)*PID.kd;
    return turn;
}