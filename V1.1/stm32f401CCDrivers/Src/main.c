/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Seif Eldidi
 * @brief          : Main program body
 ******************************************************************************
*/

#include <stdint.h>
#include "snake.h"
#include "HAL/Hal_RCC.h"
#include "core/CortexM4_core_Systick.h"
#include "HAL/Hal_GPIO.h"
#include "core/CortexM4_core_NVIC.h"
#include "HAL/Hal_ADC.h"
#include "HAL/Hal_Usart.h"
#include "HAL/hal_Timer.h"
#include "../Inc/Mcal/Mcal_I2C.h"
#include "HAL/Hal_BME280.h"

uint32_t Led_On =0;
uint32_t Led_2_on =0;
uint32_t CNT =0;
uint8_t  data[5];

void ICP_1(void)
{
	Led_On++;
	CNT = TIM2->CNT;
}

void ICP2(void)
{
	Led_2_on++;
}

void Main_Init()
{
	RCC_Config_t RCC_Config = { .MSTR_CLK_SRC = SW_CLK_PLLO, .PLL_SRC = PLL_HSE,
			.PLL_M = 25, .PLL_N = 168, .PLL_P = PLLP_2, .PLL_Q = PLLQ_4_S,
			.AHB_PRE = AHBDIV_1, .APB1_PRE = APB_AHB_2, .APB2_PRE = APB_AHB_1, };
	/* SystemClock Init */
	HAL_RCC_Init(&RCC_Config);
	/*---------Init USart-----*/
	USART_Config Uart = { .BaudRate = 115200, .Data_Size = USART_DATA_SIZE_8,
			.Instance = USART1, .No_StopBits = USART_STOPBITS_1, .Mode =
					USART_TX_RX_MODE, .Parity = USART_NPARITY,
			.TX_RX_Interrupts = INT_DIS, };

	TIMX_t Timer2 = { .Inst = TIM3, .Mode = TIMX_TIMER, .TimeBaseSet.Base_Mode =
			BASE_1MS_TICK, .TimeBaseSet.Mode = TIMX_UPCOUNTER, };

	GPIO_InitStruct GPIO = { .Pin = GPIO_PIN_4, .Speed = GPIO_Speed_50MHz,
			.mode = GPIO_MODE_OUTPUT_PP, .pull = GPIO_NOPULL, };
	I2C_cfg_t I2C ={
			.SM_FM_SEL = I2C_SM_MODE,
			.Instance  = I2C1,
			.SL_MSTR_MODE = I2C_MSTR_MODE,
	};
	xMCAL_I2C_Init(&I2C);
	HAL_GPIO_Init(GPIOA, &GPIO);
	xHAL_TIMX_Init(&Timer2);
}

int main(void)
{
	/* RCC configuration Structure */
	uint8_t RxData;
	uint8_t RXDATA;
	int32_t Temp;
	Main_Init();
	HAL_BME280Init();
	while(1)
	{
		Temp = HAL_BME280_TEMP_Read();
		vHAL_TIMXDelay(TIM3,500);

	}
	return 0;
}



