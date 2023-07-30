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
#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/system_stm32f4xx.h"

uint16_t Res;
uint32_t Num;

void vTask1(void *Pargument)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
		vTaskDelay(xDelay);
	}
}

void vTask3(void *Pargument)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 1500 / portTICK_PERIOD_MS;
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1 | GPIO_PIN_2);
		vTaskDelay(xDelay);
	}
}

void vTask2(void *Pargument)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
	/*----TypeCast Parmeter to ADC config-------*/
	ADC_SConfig_t *Adc = (ADC_SConfig_t *)(Pargument);
	while(1)
	{
		Res=HAL_ADC_ResultBlocking(Adc->Allign,ADC_IN4);
		vTaskDelay(xDelay);
	}
}

void InitTask(void *Pargument)
{
	/*----------GPIO_INIT----------*/
	GPIO_InitStruct GPIO = { .Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			.Speed = GPIO_Speed_2MHz, .mode = GPIO_MODE_OUTPUT_PP, .pull =
					GPIO_NOPULL, };
	HAL_GPIO_Init(GPIOA, &GPIO);

	/*----------ADC_INIT----------*/
	ADC_SConfig_t Adc = { .APB_P = ADC_APBP_8, .Allign = LEFT_ALLIGNMENT,
			.Channel = ADC_IN4, .RES = ADC_RES_12, };
	HAL_ADC_Init(&Adc);
	/*----------Create_Tasks-------*/
	xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, (void *)(&Adc), 2, NULL);
	xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
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
	xTaskCreate(InitTask,"Def_Task",configMINIMAL_STACK_SIZE,NULL,8,NULL);
	/*-----Start Scheduler--------*/
	vTaskStartScheduler();
	while(1)
	{

	}
}


