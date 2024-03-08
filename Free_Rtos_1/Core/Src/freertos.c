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
QueueHandle_t Queue_Test;   //定义�?个队列句�?
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
struct Pills_Message {
    char ucMessageID;
    uint8_t ucData[3];
} xPillsMessage;

struct Pills_Message *SubBoxPills;

uint16_t i = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osMutexId Mutex_Test;  //定义一个互斥锁句柄

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTaskLED1Handle;
osThreadId myTaskLED2Handle;
osThreadId myTaskLED3Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osMutexDef(Mutex_Test);  //定义互斥�?

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTaskLED1(void const * argument);
void StartTaskLED2(void const * argument);
void StartTaskLED3(void const * argument);

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
    Mutex_Test = osMutexCreate(osMutex(Mutex_Test)); //创建互斥�?
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  Queue_Test = xQueueCreate(10,sizeof(struct Pills_Message *));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128*3);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTaskLED1 */
  osThreadDef(myTaskLED1, StartTaskLED1, osPriorityAboveNormal, 0, 128*2);
  myTaskLED1Handle = osThreadCreate(osThread(myTaskLED1), NULL);

  /* definition and creation of myTaskLED2 */
  osThreadDef(myTaskLED2, StartTaskLED2, osPriorityNormal, 0, 128*8);
  myTaskLED2Handle = osThreadCreate(osThread(myTaskLED2), NULL);

  /* definition and creation of myTaskLED3 */
  osThreadDef(myTaskLED3, StartTaskLED3, osPriorityBelowNormal, 0, 128*4);
  myTaskLED3Handle = osThreadCreate(osThread(myTaskLED3), NULL);

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
  /* Infinite loop */
    for(;;)
  {

     // printf("Free_Rtos OK!\r\n");

      //osThreadSuspend(defaultTaskHandle);
/*      if(i<5) osThreadSuspend(defaultTaskHandle);
      else {
          osThreadResume(defaultTaskHandle);
          i = 0;
      }
      printf("defaulttask is running!\r\n");*/
    osDelay(10);
    //osThreadYield(); //让出控制权给下一个就绪的线程（相当于我后面的代码都不执行了，我直接让出来！）

     // printf("I got it!\r\n");
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskLED1 */
/**
* @brief Function implementing the myTaskLED1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED1 */
void StartTaskLED1(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED1 */
  /* Infinite loop */
  for(;;)
  {
      HAL_GPIO_TogglePin(LED_1_GPIO_Port,LED_1_Pin);
      printf("task1 ok!\r\n");
     // osThreadResumeAll();
/*      if(i++<2)osThreadSuspend(defaultTaskHandle); //挂起默认任务
      else {
          osThreadResume(defaultTaskHandle);
          i = 0;
      }*/
    osDelay(1000);

  }
  /* USER CODE END StartTaskLED1 */
}

/* USER CODE BEGIN Header_StartTaskLED2 */
/**
* @brief Function implementing the myTaskLED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED2 */
void StartTaskLED2(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED2 */
  /* Infinite loop */
  struct Pills_Message *pxreadpointer;
  for(;;)
  {
      //printf("debug2.\r\n");
      if(Queue_Test!=NULL)
      {
          if(xQueueReceive(Queue_Test,&pxreadpointer,0) == pdPASS)
          {
              printf("%c\r\n",pxreadpointer->ucMessageID);
              printf("%d\r\n",pxreadpointer->ucData[0]);
              printf("%d\r\n",pxreadpointer->ucData[1]);
              printf("%d\r\n",pxreadpointer->ucData[2]);

          }
      }
     // i++;
     HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);
    osDelay(500);
  }
  /* USER CODE END StartTaskLED2 */
}

/* USER CODE BEGIN Header_StartTaskLED3 */
/**
* @brief Function implementing the myTaskLED3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED3 */
void StartTaskLED3(void const * argument)
{
  /* USER CODE BEGIN StartTaskLED3 */
  static struct Pills_Message myPill;
  /* Infinite loop */
  for(;;)
  {
      if(osMutexWait(Mutex_Test,0) == osOK) //获取互斥锁
      {
          if (Queue_Test != NULL) {
              SubBoxPills = &myPill;
              SubBoxPills->ucMessageID = 'B';
              SubBoxPills->ucData[0] = 1;
              SubBoxPills->ucData[1] = 2;
              SubBoxPills->ucData[2] = 3;
              xQueueSend(Queue_Test, &SubBoxPills, 0);//如果队列已满，不阻塞
              printf("Queue Send.\r\n");
          }


          printf("Access Mutex!\r\n");
          osMutexRelease(Mutex_Test);//释放互斥锁

          HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
          //if(osThreadGetId() == myTaskLED2Handle) printf("I make it!");
          // printf("Priority = %d\r\n",osThreadGetPriority(myTaskLED2Handle));
          osDelay(100);
      }
  }
  /* USER CODE END StartTaskLED3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

