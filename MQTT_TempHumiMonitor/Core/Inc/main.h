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
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOA
#define I2C_SCL_Pin GPIO_PIN_0
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_1
#define I2C_SDA_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_10
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_11
#define OLED_SCL_GPIO_Port GPIOB
#define Motor1_A_Pin GPIO_PIN_14
#define Motor1_A_GPIO_Port GPIOB
#define Motor1_B_Pin GPIO_PIN_15
#define Motor1_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
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
    UART_HandleTypeDef *huart; //使用的串???
    TIM_HandleTypeDef *htim; //串口接收数据???用到的定时器
    uint16_t MAX_RxLength;//??????大字节数
    uint16_t Cur_RxLength; //当前串口接收数组中的字节??????
    uint8_t *Rx_Package; //字符接收数组
    uint8_t *Tx_Package; //字符发???数??????
    uint8_t TempData; //用于暂时存储接收到的??????个字??????
    volatile uint8_t RxStartFalg; //串口??????始接收字符标志位   因为标志位会在中断里面随时变化，???以是’易变的‘需要加上???volatile’防止被编译器优???
    volatile uint8_t RxEndFalg; //串口结束接收字符标志??????
}UartRx_Package;

extern MOTOR Motor1;

extern UartRx_Package Uart2_Package;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
