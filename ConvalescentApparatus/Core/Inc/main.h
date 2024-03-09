/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_it.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOA
#define BLE_TX_Pin GPIO_PIN_2
#define BLE_TX_GPIO_Port GPIOA
#define BLE_RX_Pin GPIO_PIN_3
#define BLE_RX_GPIO_Port GPIOA
#define MPU_SCL_Pin GPIO_PIN_10
#define MPU_SCL_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_11
#define MPU_SDA_GPIO_Port GPIOB
#define Motor1_A_Pin GPIO_PIN_14
#define Motor1_A_GPIO_Port GPIOB
#define Motor1_B_Pin GPIO_PIN_15
#define Motor1_B_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef struct
{
    volatile uint8_t AdjustON_OFF : 1; //开启或停止调整姿态（0：停止 1：开始）
    uint8_t reserve : 7; //保留位
}SystemStatusBit;


typedef struct
{
    char *Name;
    TIM_HandleTypeDef TIM;
    uint32_t Channel;
    int16_t Speed;
    uint8_t Direction;

}MOTOR;

typedef struct
{
    UART_HandleTypeDef *huart; //使用的串�?
    TIM_HandleTypeDef *htim; //串口接收数据用到的定时器
    uint16_t MAX_RxLength;//�?????大字节数
    volatile uint16_t Cur_RxLength; //当前串口接收数组中的字节数目
    uint8_t *Rx_Package; //字符接收数据的数据包
    uint8_t *Tx_Package; //字符发�?�数据的数据�?
    uint8_t TempData; //用于暂时存储接收到的�?个字节数�?
    volatile uint8_t RxStartFalg; //串口�?????始接收字符标志位   因为标志位会在中断里面随时变化，�??以是’易变的‘需要加上�?�volatile’防止被编译器优�??
    volatile uint8_t RxEndFalg; //串口结束接收字符标志�?????
}UartRx_Package;

extern MOTOR Motor1;

extern UartRx_Package Uart2_Package;
extern SystemStatusBit SysStatus;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
