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
#include "HAL/hal_Timer.h"
#include "../Inc/Mcal/Mcal_I2C.h"
#include "HAL/Hal_BME280.h"

void SysClockInit()
{
	/* RCC configuration Structure */
	RCC_Config_t RCC_Config = {
			.MSTR_CLK_SRC = SW_CLK_PLLO,
			.PLL_SRC = PLL_HSE,
			.PLL_M = 25,
			.PLL_N = 336,
			.PLL_P = PLLP_4,
			.PLL_Q = PLLQ_7_S,
			.AHB_PRE = AHBDIV_1,
			.APB1_PRE = APB_AHB_2,
			.APB2_PRE = APB_AHB_1,
	};
	/* SystemClock Init */
	HAL_RCC_Init(&RCC_Config);
}

void PeriphInit()
{
	TIMX_t Timer2 = {
			.Inst = TIM3,
			.Mode = TIMX_TIMER,
			.TimeBaseSet.Base_Mode =BASE_1MS_TICK,
			.TimeBaseSet.Mode = TIMX_UPCOUNTER,
	};
	I2C_cfg_t I2C ={
			.SM_FM_SEL = I2C_SM_MODE,
			.Instance  = I2C1,
			.SL_MSTR_MODE = I2C_MSTR_MODE,
	};
	USART_Config Usart = {
			.Instance = USART1,
			.BaudRate = 115200,
			.Data_Size = USART_DATA_SIZE_8,
			.Mode = USART_TX_RX_MODE,
			.No_StopBits = USART_STOPBITS_1,
			.Parity = USART_NPARITY,
			.TX_RX_Interrupts = INT_DIS,
	};
	xHAL_UsartInit(&Usart);
	xMCAL_I2C_Init(&I2C);
	xHAL_TIMX_Init(&Timer2);
}

void Main_Init()
{
	/*---------Init Clock----------*/
	SysClockInit();
	/*---------Init Peripherals-----*/
	PeriphInit();
}

int main(void)
{
	Main_Init();
	BME_Init_t BME = {
			.Humidity_Smp = OVERSAMPLING_1,
			.Pressure_Smp = OVERSAMPLING_1,
			.Temp_Smp  = OVERSAMPLING_1,
	};
	HAL_BME280Init(&BME);
	BME_Mes BME_Read;
	while(1)
	{
		HAL_BME_Read(&BME_Read);
		xHAL_UsartLogInfo(USART1,"Temp Val = %u \n\rPressure Val = %u \n\r",BME_Read.Temp);
		xHAL_UsartLogInfo(USART1,"Pressure Val = %u \n\r",BME_Read.Press);
		xHAL_UsartLogInfo(USART1,"Humidity Val = %u \n\r",BME_Read.Hum);
		vHAL_TIMXDelay(TIM3,250);
	}
	return 0;
}



