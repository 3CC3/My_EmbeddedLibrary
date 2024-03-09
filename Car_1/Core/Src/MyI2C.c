#include "main.h"                  // Device header

#define MyI2C_SCL GPIO_Pin_4
#define MyI2C_SDA GPIO_Pin_5

void MyI2C_W_SCL(uint8_t BitValue)
{
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, BitValue);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin,BitValue);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitVal;
	BitVal = HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
	return BitVal;
}

/*
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = MyI2C_SCL | MyI2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, MyI2C_SCL | MyI2C_SDA);
}
*/
//void MPU_SDA_OUT(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = MyI2C_SDA;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}

//void MPU_SDA_IN(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = MyI2C_SDA;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
/*void MyI2C_SendAck(uint8_t Ack)
{
    MyI2C_W_SDA(Ack);
    MyI2C_W_SCL(1); //让从机读数据
    MyI2C_W_SCL(0);
}
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t Ack;
    MyI2C_W_SDA(1); //释放SDA
    MyI2C_W_SCL(0); //让从机放数据
    MyI2C_W_SCL(1);
    Ack = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return Ack;
}*/
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);  //最好先释放SDA
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);  //先拉低SDA
	MyI2C_W_SCL(0);  //再拉低SCL
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);  //先拉低SDA
	MyI2C_W_SCL(1);

	MyI2C_W_SDA(1);  //释放SDA
}

//注意发送字节要高位先行
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i<8;i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i)); //0x80 = 1000 0000 即可取出最高位(非0即1)
		MyI2C_W_SCL(1);  //释放SCL从机立刻会把放在SDA线上的数据读走
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i;
	uint8_t Byte = 0x00;
	MyI2C_W_SDA(1);  //主机释放SDA
	for(i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);  //SCL高电平（读取状态）
		if(MyI2C_R_SDA() == 1){Byte |= (0x80>>i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

//发送应答
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit); //0x80 = 1000 0000 即可取出最高位(非0即1)
	MyI2C_W_SCL(1);  //释放SCL从机立刻会把放在SDA线上的数据读走
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	
	uint8_t ucErrTime=0;
	MyI2C_W_SDA(1);  //主机释放SDA
	MyI2C_W_SCL(1);  //SCL高电平（读取状态）
	while(MyI2C_R_SDA()){
		ucErrTime++;
		if(ucErrTime>250){
			MyI2C_Stop();
			return 1;
		}
	}
	MyI2C_W_SCL(0);
	
	return 0;
}
