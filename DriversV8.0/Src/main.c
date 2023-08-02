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

uint16_t Res;
uint32_t Num;
uint16_t GsAdcResult;
TaskHandle_t vTask1_Handle = NULL;
TaskHandle_t vTask2_Handle = NULL;
SemaphoreHandle_t xSemaphore;

void vTask1(void *pvParameters)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 20 / portTICK_PERIOD_MS;
	while(1)
	{
		xSemaphoreTake(xSemaphore,0);
		if( GsAdcResult > 2023 )
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
		xSemaphoreGive(xSemaphore);
		vTaskDelay(xDelay);
	}
}

void vTask3(void *pvParameters)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 50 / portTICK_PERIOD_MS;
	while(1)
	{
		xSemaphoreTake(xSemaphore,0);
		GsAdcResult = sHAL_ADC_ResultBlockingCont(RIGHT_ALLIGNMENT);
		vTaskDelay(xDelay);
		xSemaphoreGive(xSemaphore);
	}
}


void InitTask(void *pvParameters)
{
	BaseType_t Ret ;
	/*----------GPIO_INIT----------*/
	GPIO_InitStruct GPIO = {
			.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
			.Speed = GPIO_Speed_2MHz,
			.mode = GPIO_MODE_OUTPUT_PP,
			.pull =GPIO_NOPULL,
	};
	HAL_GPIO_Init(GPIOA, &GPIO);
	/*----------ADC_INIT---------*/
	ADC_CConfig_t ADC1_C = {
			.APB_P = ADC_APBP_8,
			.Allign = RIGHT_ALLIGNMENT,
			.Channel = ADC_IN0,
			.Cont_SNG = ADC_CONTINOUS_MODE,
			.RES =ADC_RES_12,
			.SMPT = ADC_SMP_3CYC,
	};
	xHAL_ADC_Init(&ADC1_C, ADC_SINGLE_INT_DIS);
	/*----------Init Task Handles-------*/
	vTask1_Handle = (TaskHandle_t *)(&vTask1);
	vTask2_Handle = (TaskHandle_t *)(&vTask3);
	/*----------Create Semaphore-----*/
	xSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(xSemaphore);
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

uint8_t Buffer[10] = "Sasa Mix";
int8_t Password[10];

void ISR_UsartRX()
{
	static int32_t Counter;
	uint8_t USART_VAL = USART1->DR;
	if(USART_VAL == 127)
	{
		Counter--;
		Password[Counter] = 0;
		if(Counter == -1)
			Counter=0;
	}
	else{
		if(Counter < 9)
		{
			Password[Counter] = USART_VAL;
			Counter++;
		}else if(Counter == 9)
			Password[Counter] = '\0';
	}
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
	USART_Config Usart={
			.Data_Size = USART_DATA_SIZE_8,
			.BaudRate = 115200,
			.TX_RX_Interrupts = INT_RX,
			.Tx_Callback = NULL,
			.Rx_Callback = ISR_UsartRX ,
			.Instance = USART1,
			.Mode = USART_TX_RX_MODE,
			.No_StopBits = USART_STOPBITS_1,
			.Parity = USART_NPARITY,
	};
	ERR State = xHAL_UsartInit(&Usart);
	SystemCoreClockUpdate();
	/*-----Init System using Init Task---*/
//	BaseType_t Ret = xTaskCreate(InitTask,"Def_Task",configMINIMAL_STACK_SIZE,NULL,7,NULL);
//	/*-----Start Scheduler--------*/
//	if(Ret == pdPASS)
//		vTaskStartScheduler();
//	else{
//
//	}
	uint32_t Del = 0;
	while(1)
	{

	}
	return 0;
}



