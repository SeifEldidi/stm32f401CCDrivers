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
#include "../Inc/Mcal/Mcal_Flash_driver.h"

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
			.TX_RX_Interrupts = INT_TX,
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
	uint32_t Buffer_HEX[]={0xDEADDEAD,0xDEADDEAD,0xDEADDEAD,0xDEADDEAD};
	FLASH_Buffer Write ={
			.Address = 0x08010000,
			.Data = Buffer_HEX,
			.Length = sizeof(Buffer_HEX)/sizeof(*Buffer_HEX),
			.No_Sectors = 1,
			.SectorNumber = FLASH__S4,
	};
	Main_Init();
	MCAL_FLASH_Hex(&Write);
	while(1)
	{
		xHAL_UsartSendBYTEAsync(USART1,'A');
		vHAL_TIMXDelay(TIM3,500);
	}
	return 0;
}



