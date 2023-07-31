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
uint16_t ADC_BUFFER[5];
float Temp;

TaskHandle_t vTask1_Handle = NULL;
TaskHandle_t vTask2_Handle = NULL;

typedef struct
{
	unsigned int ID;
	unsigned int ID_VAL;
}PACK;

PACK PACK_1 ={
		.ID = 200,
		.ID_VAL = 1500,
};

void vTask1(void *pvParameters)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
	PACK * Ptr =(PACK *)(pvParameters);
	unsigned int ID = Ptr->ID;
	unsigned int ID_Val = Ptr->ID_VAL;
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_7);
		ID_Val++;
		ID++;
		vTaskDelay(xDelay);
	}
}

void vTask3(void *pvParameters)
{
	/*--------Use Suitable Delay-----*/
	const TickType_t xDelay = 1500 / portTICK_PERIOD_MS;
	while(1)
	{
		Temp = 25+(((((ADC_BUFFER[4]*3.3)/4095)-0.76)*1000)/2.5);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5 | GPIO_PIN_6);
		vTaskDelay(xDelay);
	}
}


void InitTask(void *pvParameters)
{
	/*----------GPIO_INIT----------*/
	GPIO_InitStruct GPIO = { .Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
			.Speed = GPIO_Speed_2MHz, .mode = GPIO_MODE_OUTPUT_PP, .pull =
					GPIO_NOPULL, };
	HAL_GPIO_Init(GPIOA, &GPIO);

	/*----------ADC_INIT----------*/
//   ADC_CConfig_t ADC1_C = {
//		.APB_P = ADC_APBP_8,
//		.Allign = RIGHT_ALLIGNMENT,
//		.Channel = ADC_IN0 | ADC_IN1 | ADC_IN2 | ADC_IN3,
//		.Cont_SNG =ADC_CONTINOUS_MODE,
//		.RES = ADC_RES_12,
//    };
//	//HAL_ADC_GroupInitDMA(&ADC1_C,&ADC_BUFFER[0]);
	/*----------Create_Tasks-------*/
	vTask1_Handle = (TaskHandle_t *)(&vTask1);
	vTask2_Handle = (TaskHandle_t *)(&vTask3);
	xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, (void *) (&PACK_1), 2, &vTask1_Handle);
	xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 4, &vTask2_Handle);
	/*-------Delete Init Task(Run to completion Task)------*/
	vTaskDelete(NULL);
}

int main(void)
{

	uint16_t *Buffer;
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
    ADC_CConfig_t ADC1_C = {
			.APB_P = ADC_APBP_8,
			.Allign = RIGHT_ALLIGNMENT,
			.Channel = ADC_IN0 ,
			.Cont_SNG =ADC_CONTINOUS_MODE,
			.RES = ADC_RES_12,
			.SMPT = ADC_SMP_3CYC,
	 };
    GPIO_InitStruct GPIO = {
    		.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
    		.Speed = GPIO_Speed_2MHz,
			.mode  = GPIO_MODE_OUTPUT_PP,
			.pull  = GPIO_NOPULL,
    };
//    HAL_GPIO_Init(GPIOA, &GPIO);
    xHAL_ADC_Init(&ADC1_C,ADC_SINGLE_INT_DIS);
	/*-----Init System using Init Task---*/
//	BaseType_t Ret = xTaskCreate(InitTask,"Def_Task",configMINIMAL_STACK_SIZE,NULL,7,NULL);
//	/*-----Start Scheduler--------*/
//	if(Ret == pdPASS)
//		vTaskStartScheduler();
//	else{
//
//	}
	while(1)
	{
		ADC_BUFFER[0] = sHAL_ADC_ResultBlockingCont(RIGHT_ALLIGNMENT);
//		if( ADC_BUFFER[0] > 2023)
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
//		else
//			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
	}
	return 0;
}



