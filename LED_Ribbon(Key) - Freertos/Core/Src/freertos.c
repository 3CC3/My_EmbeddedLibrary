/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "retarget.h"
#include "Motor.h"
#include "Uart_RxPackage.h"
#include "OLED.h"
#include "Delay.h"
#include "LED_Ribbon.h"
#include "RGBColor.h"
#include "usart.h"
#include "MyKey.h"
#include "ESP32_MQTT.h"
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
/* USER CODE BEGIN Variables */
uint8_t ESPSubscribeFlag = 0;
char  *JSONData = NULL;
int JSONLength = 0;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);
    Uart_RxPackage_Init(&Uart2_Package,&htim2,300,&huart2); //初始化串口接收数�??
    Uart_PackageStart(&Uart2_Package);
    OLED_Init();

    LED_RibInit();
    KeyInit();

    HAL_Delay(1000);
    ESPMQTT_Start();
    HAL_Delay(1000);

    if(ESP32Subscribe(Uart2_Package) == ESPSuccess)ESPSubscribeFlag = 1;
    OLED_ShowString(4,1,"MQTTSubscribeSus");
    /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128*4);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    OLED_ShowString(1,1,"   LEDRibbon");
    OLED_ShowString(2,1,"Mode:");
    //OLED_ShowString(4,1,"TouchNum:");
    /* Infinite loop */
  for(;;)
  {
      //OLED_ShowNum(3,1,temp,5);
      switch(hLedRib.RuningMode)
      {
          //0.默认全黑
          case DefaultMode:
          {
              hLedRib.LightAllLEDFun(0x00,0x00,0x00); //黑色
              OLED_ShowString(2,6,"Default    ");
          }break;
          //1.随机亮灯模式
          case RandomLightMode:
          {
              hLedRib.RandomLED.RandomLightFun();
              OLED_ShowString(2,6,"Random     ");
          }break;

          //2.回到起始点的流水灯
          case WaterFallBack2Start:
          {
              hLedRib.WaterFallLED.WaterFallLightFun(BackToStart);
              OLED_ShowString(2,6,"WFB2S      ");
          }break;

          //3.反弹的流水灯
          case WaterFallRebound:
          {
              hLedRib.WaterFallLED.WaterFallLightFun(Rebound);
              OLED_ShowString(2,6,"WFRebound  ");
          }break;

          //默认模式
          default:hLedRib.LightAllLEDFun(0x00,0x00,0x00); //黑色
      }


      if(Uart2_Package.RxEndFalg == 1 && ESPSubscribeFlag == 1) //说明串口中断接收结束
      {
          /**阿里云平台下发数据到ESP再到STM32，阿里云平台无法确定数据是否正确发放，所以需要下位机把接收到的数据发布回给阿里云平台。
           阿里云平台接收到发布回来的数据后，会回一串数据和万恶的“OK”**/
          printf("ReceiveOK!\r\n");
          ESP32ReceiveMsgJSONParse(&Uart2_Package,&JSONData,&JSONLength);
         // JSONALLTask(JSONData,JSONLength);
         // JSONBreathLEDTask(JSONData,JSONLength);
          JSONLEDRibTask(JSONData,JSONLength);
          Uart_DoTask(&Uart2_Package);
      }
      osDelay(10);

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
    OLED_ShowString(3,1,"KeyNum:");
    /* Infinite loop */
  for(;;)
  {
      //hKey.KeyReadFun();
      PressKey2DoTask();
      OLED_ShowNum(3,8,hKey.KeyPressNum,1);
      osDelay(20);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

