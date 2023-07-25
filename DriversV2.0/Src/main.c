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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t APB1_CLK = 0x00000000U;
	ERR Status = HAL_OK;
	/* RCC configuration Structure */
	RCC_Config_t RCC_Config={
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
	/*Systick Configuration Structure */
	SYSTICK_CFG SYS_Config ={
			.CLCK_DIV = SYS_CLK_8,
	};
    /* SystemClock Init */
	Status   = HAL_RCC_Init(&RCC_Config);
	/* APB1 Speed API */
	APB1_CLK = HAL_RCC_GET_APB1FREQ();
	/* Systick Init */
	Status   = HAL_SYSTICK_Init(&SYS_Config);
	while(1)
	{
		Status = Hal_SYSTICK_DelayMs(1000);
	}
}
