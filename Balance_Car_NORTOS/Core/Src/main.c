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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include "usart.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "MPU6050.h"
#include "Control.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MECHANICALMEDIAN (-1)   //平衡车的机械中�?�为4
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
MOTOR Motor1,Motor2;
ENCODER Encoder1,Encoder2;
PID_PARAMETER PID_Vertical,PID_Speed,PID_Turn;
float Mpu_Pitch,Mpu_Roll,Mpu_Yaw;
int16_t i = 0;
short gyrox,gyroy,gyroz;	//�??螺仪--角�?�度
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint16_t j = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    RetargetInit(&huart1);
    OLED_Init();
    HAL_TIM_Base_Start_IT(&htim2);
    Motor_Init(&Motor1,"BalanceMoror1",MOTORA,&htim2,TIM_CHANNEL_1);
    Motor_Init(&Motor2,"BalanceMoror2",MOTORB,&htim2,TIM_CHANNEL_2);
    Encoder_Init(&Encoder1,&htim3);
    Encoder_Init(&Encoder2,&htim4);
    PID_Init(&PID_Vertical,MECHANICALMEDIAN,(float)1.5,(float)0,(float)-0.0075); //直立�??
    PID_Init(&PID_Speed,0,(float)0.25,(float)0.00125,(float)0);
    PID_Init(&PID_Turn,0,(float)0,(float)0,(float)0);
    mpu_dmp_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      //printf("ssss\r\n");
      while(mpu_dmp_get_data(&Mpu_Pitch,&Mpu_Roll,&Mpu_Yaw)!=0);
      while(MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz)!=0);
      Mpu_Pitch = -Mpu_Pitch;
      OLED_ShowString(3,1,"Pitch:");
      OLED_ShowString(1,1,"Speed1:");
      OLED_Float(1,10,Encoder1.EncoderSpeed,1);
      OLED_Float(3,10,Mpu_Pitch,1);
      OLED_ShowString(2,1,"YAW:");
      OLED_Float(2,6,Mpu_Yaw,1);
      OLED_ShowString(4,1,"Gyroy:");
      if(j++>=7)
      {
          OLED_Float(4,7,gyroy/20,1);
          j = 0;
      }
      //Set_MotorSpeed(&Motor1,50);
      //HAL_Delay(1000);
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
//0.01s进一次中�??????
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t i = 0;
    if(htim->Instance == TIM2)
    {
        if(i++>=5)
        {
            Encoder1.EncoderSpeed = __HAL_TIM_GET_COUNTER(&Encoder1.htim);
            Encoder2.EncoderSpeed = __HAL_TIM_GET_COUNTER(&Encoder2.htim);
            Encoder2.EncoderSpeed = -Encoder2.EncoderSpeed; //这里加个'-'号就是为了让电机二在正转时编码器读到的是正数
            __HAL_TIM_SET_COUNTER(&Encoder1.htim,0);
            __HAL_TIM_SET_COUNTER(&Encoder2.htim,0);

//            Encoder1.EncoderSpeed = __HAL_TIM_GET_COUNTER(&Encoder1.htim);
//            Encoder2.EncoderSpeed = __HAL_TIM_GET_COUNTER(&Encoder2.htim);
//            __HAL_TIM_SET_COUNTER(&Encoder1.htim,0);
//            __HAL_TIM_SET_COUNTER(&Encoder2.htim,0);
//            Motor1.Speed = PID_VerticalControl(PID_Vertical,Mpu_Pitch,gyroy/20.0);
//            Motor2.Speed = Motor1.Speed;
//            Limit_MotorSpeed(&Motor1);
//            Limit_MotorSpeed(&Motor2);
            //Set_MotorSpeed(&Motor1,Motor1.Speed);
            //Set_MotorSpeed(&Motor2,Motor2.Speed);
            //HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
            i = 0;
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
    static float Balance_PWM = 0,Velocity_PWM = 0,Turn_PWM = 0,PWM_Out;
    if(GPIO_Pin == MPU6050_INT_Pin) //一级判定
    {
        if(HAL_GPIO_ReadPin(MPU6050_INT_GPIO_Port,MPU6050_INT_Pin) == 0)  //二级判定
        {

            Balance_PWM = PID_VerticalControl(PID_Vertical,Mpu_Pitch,gyroy); //平衡环PID
            Velocity_PWM = PID_SpeedControl(PID_Speed,Encoder1.EncoderSpeed,Encoder2.EncoderSpeed);//速度环PID
            //Turn_PWM = PID_TurnControl(PID_Turn,Mpu_Yaw); //转向环PID

            PWM_Out = Balance_PWM+PID_Vertical.Kp*Velocity_PWM;
            Motor1.Speed = PWM_Out+Turn_PWM;
            Motor2.Speed = PWM_Out-Turn_PWM;
            Limit_MotorSpeed(&Motor1);
            Limit_MotorSpeed(&Motor2);
            Set_MotorSpeed(&Motor1,Motor1.Speed);
            Set_MotorSpeed(&Motor2,Motor2.Speed);
            HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
        }
    }
}
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
