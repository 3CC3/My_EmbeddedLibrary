/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "retarget.h"
#include "Motor.h"
#include "Uart_RxPackage.h"
#include "OLED.h"
#include "inv_mpu.h"
#include "JSON_Test.h"
#include "ESP32_MQTT.h"
#include "Servo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t i = 0;
UartRx_Package Uart2_Package;
SERVOHandle MQTTSERVO;
uint8_t ESPSubscribeFlag = 0;
float Mpu_Pitch,Mpu_Roll,Mpu_Yaw;
char  *JSONData = NULL;
int JSONLength = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

    RetargetInit(&huart1); //重定向串口一
    //HAL_DMA_Start(&hdma1,);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    Uart_RxPackage_Init(&Uart2_Package,&htim2,300,&huart2); //初始化串口接收数组
    Uart_PackageStart(&Uart2_Package);

    OLED_Init();
    //mpu_dmp_init(); //初始化MPU6050
    HAL_Delay(1000);
    ESPMQTT_Start();
    HAL_Delay(1000);

    //ESP32Publish(Uart2_Package.huart,77.7,1,80);
    if(ESP32Subscribe(Uart2_Package) == ESPSuccess)ESPSubscribeFlag = 1;
    OLED_ShowString(1,1,"MQTTSubscribeSus");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//      while(mpu_dmp_get_data(&Mpu_Pitch,&Mpu_Roll,&Mpu_Yaw)!=0);
//      OLED_ShowString(1,1,"Pitch:");
//      OLED_Float(1,7,Mpu_Pitch,1);
//      OLED_ShowString(2,1,"Roll:");
//      OLED_Float(2,7,Mpu_Roll,1);
//      OLED_ShowString(3,1,"Yaw:");
//      OLED_Float(3,7,Mpu_Yaw,1);
//      OLED_ShowNum(4,1,sizeof("H"),1); //末尾有个‘\0’
    if(Uart2_Package.RxEndFalg == 1 && ESPSubscribeFlag == 1) //说明串口中断接收结束
    {
        /**阿里云平台下发数据到ESP再到STM32，阿里云平台无法确定数据是否正确发放，所以需要下位机把接收到的数据发布回给阿里云平台。
         阿里云平台接收到发布回来的数据后，会回一串数据和万恶的“OK”**/

        ESP32ReceiveMsgJSONParse(&Uart2_Package,&JSONData,&JSONLength);
        JSONALLTask(JSONData,JSONLength);
        //JSONBreathLEDTask(JSONData,JSONLength);
        Uart_DoTask(&Uart2_Package);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
