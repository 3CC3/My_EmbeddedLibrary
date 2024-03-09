#include "main.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_Address 0x68

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte((MPU6050_Address<<1)|0); //写模式
	MyI2C_ReceiveAck();  //应答位
	MyI2C_SendByte(RegAddress); //寄存器地址，会存在当前地址指针里
	MyI2C_ReceiveAck();  //应答位
	MyI2C_SendByte(Data); 
	MyI2C_ReceiveAck();  //应答位
	MyI2C_Stop(); 
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU6050_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i; 
    MyI2C_Start(); 
	MyI2C_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(MyI2C_ReceiveAck())	//等待应答
	{
		MyI2C_Stop();		 
		return 1;		
	}
    MyI2C_SendByte(reg);	//写寄存器地址
    MyI2C_ReceiveAck();		//等待应答
	for(i=0;i<len;i++)
	{
		MyI2C_SendByte(buf[i]);	//发送数据
		if(MyI2C_ReceiveAck())		//等待ACK
		{
			MyI2C_Stop();	 
			return 1;		 
		}		
	}    
    MyI2C_Stop();	 
	return 0;	
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte((MPU6050_Address<<1)|0);
	MyI2C_ReceiveAck();  //应答位
	MyI2C_SendByte(RegAddress); //寄存器地址，会存在当前地址指针里
	MyI2C_ReceiveAck();  //应答位
	
	MyI2C_Start();
	MyI2C_SendByte((MPU6050_Address<<1)|0x01);
	MyI2C_ReceiveAck();  //应答位
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1); // 主机给从机发送应答位
	MyI2C_Stop();
	return Data;
}
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU6050_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	MyI2C_Start(); 
	MyI2C_SendByte((addr<<1)|0x00);//发送器件地址+写命令	
	if(MyI2C_ReceiveAck())	//等待应答
	{
		MyI2C_Stop();		 
		return 1;		
	}
    MyI2C_SendByte(reg);	//写寄存器地址
    MyI2C_ReceiveAck();		//等待应答
    MyI2C_Start();
	MyI2C_SendByte((addr<<1)|0x01);//发送器件地址+读命令	
    MyI2C_ReceiveAck();		//等待应答 
	while(len)
	{
		if(len==1){
			*buf=MyI2C_ReceiveByte();//读数据,发送nACK 
			MyI2C_SendAck(1);
		}
		else {
			*buf=MyI2C_ReceiveByte();		//读数据,发送ACK  
			MyI2C_SendAck(0);
		}
		len--;
		buf++; 
	}    
    MyI2C_Stop();	//产生一个停止条件 
	return 0;	
}
void MPU6050_Init(void)
{
	MyI2C_Init();
		
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);  //解除睡眠，并选择陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);  //六个轴均不待机
	
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);  //十分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);      //滤波参数给最大
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);  //配置陀螺仪
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);  //配置加速度传感器
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
				int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
