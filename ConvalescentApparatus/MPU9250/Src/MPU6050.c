#include "main.h"                  // Device header
#include "My_I2C.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h"

#define MPU6050_Address 0x68

#define HARDWARE_I2C   //SOFTWARE_I2C HARDWARE_I2C


#ifdef HARDWARE_I2C
#include "i2c.h"
#define I2CHandle hi2c2
#endif


//IIC连续写
//addr:器件地址
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU6050_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
#ifdef HARDWARE_I2C
    uint8_t j = 0;
    uint8_t TempBuff[len+1];
    TempBuff[0] = reg; //要写的寄存器地址
    for(j = 0; j < len; j++)
    {
        *(TempBuff+j+1) = *(buf+j);
    }
    HAL_I2C_Master_Transmit(&I2CHandle,addr<<1,TempBuff,len+1,999);
#endif
#ifdef SOFTWARE_I2C
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
#endif
    return 0;
}
//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data)
{
#ifdef HARDWARE_I2C

    uint8_t TempBuff[2];
    TempBuff[0] = reg;
    TempBuff[1] = data;
    HAL_I2C_Master_Transmit(&I2CHandle,(MPU_ADDR<<1)|0,TempBuff,2,999);
#endif
#ifdef SOFTWARE_I2C
    MyI2C_Start();
    MyI2C_SendByte((MPU_ADDR<<1)|0);//发送器件地址+写命令
    if(MyI2C_ReceiveAck())	//等待应答
    {
        MyI2C_Stop();
        return 1;
    }
    MyI2C_SendByte(reg);	//写寄存器地址
    MyI2C_ReceiveAck();		//等待应答
    MyI2C_SendByte(data);//发送数据
    if(MyI2C_ReceiveAck())	//等待ACK
    {
        MyI2C_Stop();
        return 1;
    }
    MyI2C_Stop();
#endif
    return 0;
}
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;
#ifdef HARDWARE_I2C
    HAL_I2C_Master_Transmit(&I2CHandle,(MPU6050_Address<<1)|0x00,&RegAddress,1,999);//写模式发送寄存器地址
    HAL_I2C_Master_Receive(&I2CHandle,(MPU6050_Address<<1)|0x01,&Data,1,999); //读模式读取数据
#endif

#ifdef SOFTWARE_I2C

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
#endif
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
#ifdef HARDWARE_I2C

    HAL_I2C_Master_Transmit(&I2CHandle,(addr<<1)|0x00,&reg,1,999); //写模式发送寄存器地址
    HAL_I2C_Master_Receive(&I2CHandle,(addr<<1)|0x01,buf,len,999); //读模式读取数据
#endif

#ifdef SOFTWARE_I2C

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
#endif
    return 0;
}
void MPU6050_Init(void)
{
#ifdef SOFTWARE_I2C
    MyI2C_Init();
#endif
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
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_LPF(u16 lpf)
{
    u8 data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
u8 MPU_Set_Rate(u16 rate)
{
    u8 data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
    return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
    float temp;
    MPU6050_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((u16)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;
    res=MPU6050_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((u16)buf[0]<<8)|buf[1];
        *gy=((u16)buf[2]<<8)|buf[3];
        *gz=((u16)buf[4]<<8)|buf[5];
    }
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;
    res=MPU6050_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((u16)buf[0]<<8)|buf[1];
        *ay=((u16)buf[2]<<8)|buf[3];
        *az=((u16)buf[4]<<8)|buf[5];
    }
    return res;;
}
