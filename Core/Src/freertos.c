/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
osThreadId BlueTooth1Handle;  //创建两个蓝牙句柄
osThreadId BlueTooth2Handle;

uint8_t Uart2Flag = 0;
uint8_t Uart3Flag = 0;

uint8_t RxBuff[1];
uint8_t TxBuff[20]={10,11,12,13};
uint8_t cnt[1];
struct Uart_print Uart_Print1 ,Uart_Print2;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128*5);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    osThreadDef(Blue1Task, StartBlueTooth1Task,osPriorityNormal,0,128*2);
    osThreadDef(Blue2Task, StartBlueTooth2Task,osPriorityNormal,0,128*2);
    BlueTooth1Handle = osThreadCreate(osThread(Blue1Task),NULL);
    BlueTooth2Handle = osThreadCreate(osThread(Blue2Task),NULL);
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
  taskENTER_CRITICAL(); //进入临界区（确保临界区内的函数在freertos的调度器�?????启前能够全部执行，防止未初始化完成就进入时间片，导致出错�?????

    HAL_UART_Receive_DMA(&huart2,Uart_Print1.Rx_temp,1);
    HAL_UART_Receive_DMA(&huart3,RxBuff,1);
    Uart_creat(&Uart_Print1,"BlueTooth1");
    Uart_creat(&Uart_Print2,"BlueTooth2");
    printf("%s creat success!\r\n",Uart_Print1.name);
    printf("%s creat success!\r\n",Uart_Print2.name);
    //uart2_printf("hello!\r\n");
  taskEXIT_CRITICAL();   //退出临界区
  /* Infinite loop */
  for(;;)
  {
      Uart_Print1.tick_new = HAL_GetTick();
      if(Uart_Print1.tick_new - Uart_Print1.tick_old >=100 && Uart_Print1.Rx_statrtFlag == 1) //�?????5ms的时间给他发数据
      {
          //串口二接收数据结�?????
          Uart2print(&huart2,&Uart_Print1);
      }
      if(Uart_Print2.tick_new - Uart_Print2.tick_old >=5 && Uart_Print2.Rx_statrtFlag == 1)
      {
          //串口三接收数据结�?????
          Uart2print(&huart3,&Uart_Print2);
      }
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartBlueTooth1Task(void const * argument)
{
    //static uint8_t Uart2Flag = 0;
    for(;;)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        osDelay(300);
    }
}

void StartBlueTooth2Task(void const * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
        osDelay(300);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(huart->Instance == USART2)
    {
       // xTaskNotifyFromISR( xTaskhandle2W5500, 0x01, eSetBits, &xHigherPriorityTaskWoken );
       if(Uart_Print1.Rx_cnt == 0)
       {
           Uart_Print1.tick_old = HAL_GetTick();
           Uart_Print1.Rx_statrtFlag = 1;
       }
        Uart_Print1.Rx_package[Uart_Print1.Rx_cnt] = Uart_Print1.Rx_temp[0];
        Uart_Print1.Rx_cnt++;

        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        HAL_UART_Receive_DMA(&huart2,Uart_Print1.Rx_temp,1);
    }
    //HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    else if(huart->Instance == USART3)
    {
        Uart_Print2.tick_old = HAL_GetTick();
        if(Uart_Print2.Rx_cnt == 0)
        {
            Uart_Print2.Rx_statrtFlag = 1;
        }
        Uart_Print2.Rx_package[Uart_Print2.Rx_cnt] = Uart_Print2.Rx_temp[0];
        Uart_Print2.Rx_cnt++;
        HAL_UART_Receive_IT(&huart3,Uart_Print2.Rx_temp,1);
    }
}


/* USER CODE END Application */

