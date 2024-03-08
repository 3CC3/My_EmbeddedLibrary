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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "retarget.h"
#include "Motor.h"
#include "MFRC522.h"
#include "Uart_RxPackage.h"
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
MOTOR Motor1;
uint8_t k = 0;
uint16_t i = 0,j = 0;
UartRx_Package Uart2_Package;
uint8_t Last_Package[200];
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
    RetargetInit(&huart2);
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

    Uart_RxPackage_Init(&Uart2_Package,200,&huart2); //��ʼ�����ڽ�������
    uint8_t Pure_Package[200];
    HAL_Delay(250);
    Motor_Init(&Motor1); //�����ʼ��


    //__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,50);
//    for(k = 0;k<40;k++)
//    {
//        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,k);
//        HAL_Delay(125);
//    }
//    for(k = 40;k<=100;k++)
//    {
//        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,k);
//        HAL_Delay(100-k);
//    }

    HAL_UART_Receive_DMA(&huart2,&Uart2_Package.TempData,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      if(LED_Flag == 0) HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
      if(Uart2_Package.RxEndFalg == 1) //˵�������жϽ��ս���
        {
            Uart_UpPack(Uart2_Package.Rx_Package,Uart2_Package.Cur_RxLength,Pure_Package);//�������ݵ���ȷ�Լ�ȥ��֡ͷ֡β
            if(strcmp(Pure_Package,Last_Package) != 0)//��ζ��������ݺ���һ�εĲ�һ��
            {
                HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
                HAL_UART_Transmit(&huart2,Pure_Package,Uart2_Package.Cur_RxLength-12,999);
            }
            memset(Last_Package,0,200);
            for(j = 0;j<Uart2_Package.Cur_RxLength-11;j++) //�洢��һ�ε�����
            {
                *(Last_Package+j) = *(Pure_Package+j);
            }
            //printf("Status:%d",Uart_UpPack(Uart2_Package.Rx_Package,Uart2_Package.Cur_RxLength));
            Uart2_Package.RxEndFalg = 0;
            Uart2_Package.Cur_RxLength = 0;
            memset(Uart2_Package.Rx_Package,2,sizeof(uint8_t)*Uart2_Package.Cur_RxLength); //��ʼ����������
            memset(Pure_Package,0,sizeof(Pure_Package));
        }
        else if(Uart2_Package.RxEndFalg == 0)  //�������
        {
            if(k<40)
            {
                __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,k);
                k++;
                HAL_Delay(125);
            }
            else if(k >= 40 && k<=100)
            {
                __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,k);
                HAL_Delay(100-k);
                k++;
            }
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
