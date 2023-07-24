/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Seif Eldidi
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "HAL/Hal_RCC.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	ERR Status = HAL_OK;
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
    /* SystemClock Init */
	Status = HAL_RCC_Init(&RCC_Config);
	while(1)
	{

	}
}
