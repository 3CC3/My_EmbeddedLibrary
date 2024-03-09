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
#include "retarget.h"
#include "usart.h"
#include "BrushlessMotor.h"
#include "tim.h"
#include "OLED.h"
#include "Uart_RxPackage.h"
#include "inv_mpu.h"
#include "MPU6050.h"
#include "PID_Control.h"
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
uint16_t i = 0;
float pitch = 0,row = 0,yaw = 0;
BRUSHLESS BLMotorHandle;
MPUHandle MyMpu = {.MPUIntPIN = GPIO_PIN_1,
                   .pitch = 0,.roll = 0,.yaw = 0};
PID_Handle BrushLessPID = {.kp = (float)0.00,.ki = 0,.kd = 0,
                           .TarVal = (float)0,.MaxOutVal = 100,.MaxIntegralVal = 2200,
                           .IntegralVal = 0,.LastVal = 0,.PreVal = 0};
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
    RetargetInit(&huart1); //定向串口�?????????为printf调试�?????????

    Uart_RxPackage_Init(&Uart2_Package,&htim2,100,&huart2); //初始化蓝牙收发串�?????????
    Uart_PackageStart(&Uart2_Package);      //�?????????启蓝牙串口收�?????????

    mpu_dmp_init(); //初始化MPU9250

    BrushLessMotor_Init(&BLMotorHandle,&htim3); //初始化三个无刷电机
    OLED_Init(); //初始化OLED
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
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

  /*此函数会阻塞8s左右*/
    AllBrushlessMotor_Start(&BLMotorHandle);

    /* Infinite loop */
  for(;;)
  {
      //开始调整
      if(SysStatus.AdjustON_OFF == 1)
      {
          //BrushLessMotor_Set(&BLMotorHandle,Right,1400);
          BrushLessMotor_Set(&BLMotorHandle,Left,1800);
          BrushLessMotor_Set(&BLMotorHandle,Below,1400);

          BrushLessMotor_AllMove(BLMotorHandle);

          Posture_Adjust(&BLMotorHandle,MyMpu);
      }
      //停止调整
      else if(SysStatus.AdjustON_OFF == 0)
      {
          BrushLessMotor_Set(&BLMotorHandle,Right,MotorMidPERIOD);
          BrushLessMotor_Set(&BLMotorHandle,Left,MotorMidPERIOD);
          BrushLessMotor_Set(&BLMotorHandle,Below,MotorMidPERIOD);

          BrushLessMotor_AllMove(BLMotorHandle);
      }
      HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    osDelay(100);
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
    OLED_ShowString(1,1,"Pitch:");
    OLED_ShowString(2,1,"Roll:  ");
    OLED_ShowString(3,1,"Yaw:  ");

    /* Infinite loop */
  for(;;)
  {
      /*临界区还是不太�?�合放这里，因为这个MPU的读取函数太大了*/
      //taskENTER_CRITICAL();
//      while(mpu_dmp_get_data(&pitch,&row,&yaw) != 0); //获取角度的代码放到了外部中断里面
      //taskEXIT_CRITICAL();

      OLED_ShowFNum(1,7,MyMpu.pitch,3,1);
      OLED_ShowFNum(2,7,MyMpu.roll,3,1);
      OLED_ShowFNum(3,7,MyMpu.yaw,3,1);
      if(Uart2_Package.RxEndFalg == 1) //说明串口中断接收结束
      {
          Uart_DoTask(&Uart2_Package);
      }
      osDelay(50);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

