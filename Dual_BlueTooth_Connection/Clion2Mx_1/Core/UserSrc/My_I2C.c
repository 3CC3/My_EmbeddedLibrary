//
// Created by 86159 on 2023/6/3.
//
#include "main.h"
#include "My_I2C.h"
#include "stm32f103xb.h"
//#define I2C_H_SCL   GPIO_BSRR_BS14   //Set Pin14
//#define I2C_H_SCL   GPIO_BSRR_BR14   //Reset Pin14

//写SCL
void I2C_W_SCL(uint8_t BitStatus)
{
    HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,BitStatus);
}

//写SDA
void I2C_W_SDA(uint8_t BitStatus)
{
    HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,BitStatus);
}

//读SDA
I2CBitStatus I2C_R_SDA(void)
{
    return HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port,OLED_SDA_Pin);
}
//起始位  在SCL高电平期间拉低SDA产生起始位
void I2C_Start(void)
{
    I2C_W_SDA(1);
    I2C_W_SCL(1);
    I2C_W_SDA(0);
    I2C_W_SCL(0);
}
//停止位 在SCL高电平期间拉高SDA产生停止位
void I2C_Stop(void)
{
    I2C_W_SDA(0);
    I2C_W_SCL(1);
    I2C_W_SDA(1);
}
//写一个字节 高位在前
void I2C_W_Byte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++){
        //I2C_W_SDA(Byte & (0x80>>i));
        if(Byte & (0x80>>i)) I2C_W_SDA(1);
        else I2C_W_SDA(0);
        I2C_W_SCL(1);  //让从机读取数据
        I2C_W_SCL(0);  //拉低SCK，准备下一次写入数据
    }
}
//读取一个字节 高位在前
uint8_t I2C_R_Byte(void)
{
    uint8_t i;
    uint8_t Data = 0x00;
    I2C_W_SDA(1); //主机释放数据线
    for(i =0;i<8;i++){
        I2C_W_SCL(1); //主机读数据
        if(I2C_R_SDA()) Data |= (0x80>>i);
        I2C_W_SCL(0); //从机写数据
    }
    return Data;
}
//发送应答位
void I2C_SendAck(I2CBitStatus AckVal)
{
    I2C_W_SDA(AckVal);
    I2C_W_SCL(1); //从机读应答位
    I2C_W_SCL(0); //默认给个低电平
}
//接收应答位
I2CBitStatus I2C_ReceiveAck(void)
{
    I2CBitStatus BitAck;
    I2C_W_SDA(1); //释放数据线
    I2C_W_SCL(1); //主机读数据
    BitAck = I2C_R_SDA();
    I2C_W_SCL(0);
    return BitAck;
}