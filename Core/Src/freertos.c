/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include "usart.h"
#include "adc.h"
#include "event_groups.h"
#include "keyled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// define BITMASK.
#define  BITMASK_KEY_LEFT			0x04
#define  BITMASK_KEY_DOWN		0x02
#define  BITMASK_KEY_RIGHT		0x01
#define  BITMASK_SYNC   			BITMASK_KEY_LEFT | BITMASK_KEY_DOWN | BITMASK_KEY_RIGHT
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_LED1 */
osThreadId_t Task_LED1Handle;
const osThreadAttr_t Task_LED1_attributes = {
  .name = "Task_LED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED2 */
osThreadId_t Task_LED2Handle;
const osThreadAttr_t Task_LED2_attributes = {
  .name = "Task_LED2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED3_ADC */
osThreadId_t Task_LED3_ADCHandle;
const osThreadAttr_t Task_LED3_ADC_attributes = {
  .name = "Task_LED3_ADC",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for eventGroup */
osEventFlagsId_t eventGroupHandle;
const osEventFlagsAttr_t eventGroup_attributes = {
  .name = "eventGroup"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_LED1(void *argument);
void AppTask_LED2(void *argument);
void AppTask_LED3_ADC(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of Task_LED1 */
  Task_LED1Handle = osThreadNew(AppTask_LED1, NULL, &Task_LED1_attributes);

  /* creation of Task_LED2 */
  Task_LED2Handle = osThreadNew(AppTask_LED2, NULL, &Task_LED2_attributes);

  /* creation of Task_LED3_ADC */
  Task_LED3_ADCHandle = osThreadNew(AppTask_LED3_ADC, NULL, &Task_LED3_ADC_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of eventGroup */
  eventGroupHandle = osEventFlagsNew(&eventGroup_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_LED1 */
/**
  * @brief  Function implementing the Task_LED1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_LED1 */
void AppTask_LED1(void *argument)
{
  /* USER CODE BEGIN AppTask_LED1 */
  /* Infinite loop */
	for(;;)
	{
		KEYS curKey=ScanPressedKey(50);
		if (curKey != KEY_LEFT)
		{
			vTaskDelay(pdMS_TO_TICKS(80));
			continue;
		}

	  	printf("Task_LED1 reaches sync point, LED1 flashes.\r\n");
		xEventGroupSync(eventGroupHandle, BITMASK_KEY_LEFT,
				BITMASK_SYNC, portMAX_DELAY);
		while(1)
		{
			LED1_Toggle();
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LED1 */
}

/* USER CODE BEGIN Header_AppTask_LED2 */
/**
* @brief Function implementing the Task_LED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED2 */
void AppTask_LED2(void *argument)
{
  /* USER CODE BEGIN AppTask_LED2 */
  /* Infinite loop */
	for(;;)
	{
		KEYS curKey=ScanPressedKey(50);
		if (curKey != KEY_RIGHT)
		{
			vTaskDelay(pdMS_TO_TICKS(80));
			continue;
		}

	  	printf("Task_LED2 reaches sync point, LED2 flashes.\r\n");
		xEventGroupSync(eventGroupHandle, BITMASK_KEY_RIGHT,
				BITMASK_SYNC, portMAX_DELAY);
		while(1)
		{
			LED2_Toggle();
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LED2 */
}

/* USER CODE BEGIN Header_AppTask_LED3_ADC */
/**
* @brief Function implementing the Task_LED3_ADC thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED3_ADC */
void AppTask_LED3_ADC(void *argument)
{
  /* USER CODE BEGIN AppTask_LED3_ADC */
  /* Infinite loop */
	for(;;)
	{
		KEYS curKey=ScanPressedKey(50);
		if (curKey != KEY_DOWN)  //if KeyDown is not pressed.
		{
			vTaskDelay(pdMS_TO_TICKS(80));
			continue;
		}

	  	printf("Task_LED3_ADC reaches sync point, LED3 flashes.\r\n");
		xEventGroupSync(eventGroupHandle, BITMASK_KEY_DOWN,
				BITMASK_SYNC, portMAX_DELAY);  //Synchronize point, waiting for synchronization
		while(1)
		{
			LED3_Toggle();

			HAL_ADC_Start(&hadc3);
			uint32_t val = HAL_ADC_GetValue(&hadc3);
			printf("Data in ADC3_IN6 = %ld\r\n",val);

			uint32_t Volt = 3300*val;  		//mV
			Volt = Volt >> 12;  			//Divided by 2^12 and converted to engineering value
			printf("ADC3_IN6 Engineering Value = %ld\r\n",Volt );
			HAL_ADC_Stop(&hadc3);

			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LED3_ADC */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
	return ch;
}
/* USER CODE END Application */

