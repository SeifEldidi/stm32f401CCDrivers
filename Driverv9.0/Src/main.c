/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Seif Eldidi
 * @brief          : Main program body
 ******************************************************************************
*/

#include <stdint.h>
#include "HAL/Hal_RCC.h"
#include "core/CortexM4_core_Systick.h"
#include "HAL/Hal_GPIO.h"
#include "core/CortexM4_core_NVIC.h"
#include "HAL/Hal_ADC.h"
#include "HAL/Hal_Usart.h"
#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/system_stm32f4xx.h"
#include "../FreeRTOS/semphr.h"
#include "../FreeRTOS/event_groups.h"

#define TASK1 (1<<0)
#define TASK2 (1<<0)


TaskHandle_t vTask1_Handle = NULL;
TaskHandle_t vTask2_Handle = NULL;
SemaphoreHandle_t xUsartSemaphore;
EventGroupHandle_t xUsartEventGroup;
uint8_t Buffer[] = "vTASK1 Sending !\n\r";
uint8_t Buffer2[] = "vTASK2 Sending !\n\r";

void vTask1(void *pvParameters)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	EventBits_t uxBits;
	while(1)
	{
		/*----------Wait On Usart To be Free--------*/
		uxBits = xEventGroupWaitBits(xUsartEventGroup,TASK1,pdTRUE,pdTRUE,xDelay);
		if(uxBits & TASK1)
		{
			xHAL_UsartSendNBYTESP(USART1,Buffer,30,'\0');
			vTaskDelay(xDelay);
			xEventGroupSetBits(xUsartEventGroup,TASK2);
		}
	}
}


void vTask3(void *pvParameters)
{
	EventBits_t uxBits;
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	while(1)
	{
		/*----------Wait On Usart To be Free--------*/
		uxBits = xEventGroupWaitBits(xUsartEventGroup, TASK1, pdTRUE, pdTRUE,xDelay);
		if (uxBits & TASK2) {
			xHAL_UsartSendNBYTESP(USART1, Buffer2, 30, '\0');
			vTaskDelay(xDelay);
			xEventGroupSetBits(xUsartEventGroup, TASK1);
		}
	}
}

void ISR_UsartRX (void)
{

}

void InitTask(void *pvParameters)
{
	BaseType_t Ret ;
	USART_Config Usart={
			.Data_Size = USART_DATA_SIZE_8,
			.BaudRate = 115200,
			.TX_RX_Interrupts = INT_DIS,
			.Tx_Callback = NULL,
			.Rx_Callback = ISR_UsartRX ,
			.Instance = USART1,
			.Mode = USART_TX_RX_MODE,
			.No_StopBits = USART_STOPBITS_1,
			.Parity = USART_NPARITY,
	};
	ERR State = xHAL_UsartInit(&Usart);
	/*----------Init Task Handles-------*/
	vTask1_Handle = (TaskHandle_t *)(&vTask1);
	vTask2_Handle = (TaskHandle_t *)(&vTask3);
	/*----------Create Semaphore-----*/
	//xUsartSemaphore = xSemaphoreCreateBinary();
	//xSemaphoreGive(xUsartSemaphore);/*Allow the use of the Usart*/
	/*----------Create EventGroup-----*/
	xUsartEventGroup = xEventGroupCreate();
	xEventGroupSetBits(xUsartEventGroup,TASK1);/*Allow the use of the Usart by Single Task*/
	/*----------Create Tasks-------*/
	Ret = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 4, &vTask1_Handle);
	if(Ret == pdPASS)
	{
		Ret = xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 4, &vTask2_Handle);
	}
	else{

	}
	/*-------Delete Init Task(Run to completion Task)------*/
	vTaskDelete(NULL);
}

int main(void)
{
	/* RCC configuration Structure */
	RCC_Config_t RCC_Config = {
			.MSTR_CLK_SRC = SW_CLK_PLLO,
			.PLL_SRC = PLL_HSE,
			.PLL_M = 25,
			.PLL_N = 168,
			.PLL_P = PLLP_2,
			.PLL_Q = PLLQ_4_S,
			.AHB_PRE = AHBDIV_1,
			.APB1_PRE = APB_AHB_2,
			.APB2_PRE = APB_AHB_1,
	};
	/* SystemClock Init */
	HAL_RCC_Init(&RCC_Config);
	SystemCoreClockUpdate();
	/*-----Init System using Init Task---*/
	BaseType_t Ret = xTaskCreate(InitTask,"Def_Task",configMINIMAL_STACK_SIZE,NULL,7,NULL);
	/*-----Start Scheduler--------*/
	if(Ret == pdPASS)
		vTaskStartScheduler();
	else{

	}
	while(1)
	{

	}
	return 0;
}



