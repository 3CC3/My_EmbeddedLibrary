#ifndef __MPU6050_H_
#define __MPU6050_H_

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_Init(void);

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
				int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t MPU6050_GetID(void);

uint8_t MPU6050_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t MPU6050_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
#endif
