#ifndef __MPU6050_H_
#define __MPU6050_H_

#define u8  uint8_t
#define u16 uint16_t


typedef struct
{
    uint16_t MPUIntPIN;//使用外部中断的引脚
    float pitch;       //俯仰角(-90.0° <---> +90.0°)
    float roll;         //横滚角(-180.0°<---> +180.0°)
    float yaw;         //偏航角(-180.0°<---> +180.0°)
}MPUHandle;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_Init(void);

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
				int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t MPU6050_GetID(void);

uint8_t MPU6050_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU6050_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);


u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
short MPU_Get_Temperature(void);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);

#endif
