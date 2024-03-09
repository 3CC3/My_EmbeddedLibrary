//
// Created by 86159 on 2023/6/3.
//

#ifndef CLION2MX_1_MY_I2C_H
#define CLION2MX_1_MY_I2C_H

typedef enum{
    Low = 0,
    High,
}I2CBitStatus;
void I2C_W_Byte(uint8_t Byte);
uint8_t I2C_R_Byte(void);
void I2C_SendAck(I2CBitStatus AckVal);
I2CBitStatus I2C_ReceiveAck(void);
void I2C_Start(void);
void I2C_Stop(void);
#endif //CLION2MX_1_MY_I2C_H
