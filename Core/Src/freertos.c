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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip.h"
#include "lwip/api.h"
#include "mqtt_task.h"
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
extern struct netif gnetif;

osThreadId mqttClientSubTaskHandle;
osThreadId mqttClientPubTaskHandle;

osThreadId defaultTaskHandle;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void StartDefaultTask(void const *argument);

/* USER CODE END FunctionPrototypes */

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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void MX_FREERTOS_Init()
{
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
}

void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	MX_LWIP_Init();

	while(1)
	{
		//waiting for valid ip address
		if (gnetif.ip_addr.addr == 0 || gnetif.netmask.addr == 0 || gnetif.gw.addr == 0) //system has no valid ip address
		{
			osDelay(1000);
			continue;
		}
		else
		{
//			printf("DHCP/Static IP O.K.\n");
			break;
		}
	}
	/*
	 * !!! Important
	 * I don't know but mqttClientPubTask failed without compiler optimization option
	 */
	osThreadDef(mqttClientSubTask, MqttClientSubTask, osPriorityNormal, 0, 1024); //subscribe task
	mqttClientSubTaskHandle = osThreadCreate(osThread(mqttClientSubTask), NULL);
	osThreadDef(mqttClientPubTask, MqttClientPubTask, osPriorityNormal, 0, 1024); //publish task
	mqttClientPubTaskHandle = osThreadCreate(osThread(mqttClientPubTask), NULL);

	/* Infinite loop */
	for(;;)
	{
//		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin); //toggle running led
		osDelay(500);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE END Application */
