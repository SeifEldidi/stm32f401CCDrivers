/*
 * Hal_RCC.c
 *
 *  Created on: Jul 24, 2023
 *      Author: Seif pc
 */

#include "../Inc/HAL/Hal_RCC.h"

uint32_t SYSC_CLK;

static ERR HAL_RCC_HSI_Init(RCC_Config_t *RCC_C);
static ERR HAL_RCC_PLL_Init(RCC_Config_t *RCC_C);
static ERR HAL_RCC_HSE_Init(RCC_Config_t *RCC_C);
static ERR HAL_RCC_PLL_P_Init(RCC_Config_t *RCC_C);
static ERR HAL_RCC_AHB_Init(uint8_t Prescaler);
static ERR HAL_RCC_APB1_Init(uint8_t APB1_P);
static ERR HAL_RCC_APB2_Init(uint8_t APB2_P);
static void FLASH_MEM_SET();

ERR HAL_RCC_Init(RCC_Config_t *RCC_C)
{
	ERR ERR_Status = HAL_OK;
	if(RCC_C != NULL)
	{
		HAL_RCC_PWR_EN();
		RCC->CR |= CSSON_EN;
		switch(RCC_C ->MSTR_CLK_SRC)
		{
			case SW_CLK_HSI:
				HAL_RCC_HSI_Init(RCC_C);
				break;
			case SW_CLK_HSE:
				HAL_RCC_HSE_Init(RCC_C);
				break;
			case SW_CLK_PLLO:
				HAL_RCC_PLL_Init(RCC_C);
				break;
			default:
				ERR_Status = HAL_ERR;
				break;
		}
		FLASH_MEM_SET();
	}else{
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static void FLASH_MEM_SET()
{
	uint32_t Temp = 0;
#if VOLTAGE_RANGE_2_7_3_3 ==EN
	FLASH_ACR |= (1<<8);
	if(SYSC_CLK >=0 && SYSC_CLK <=30000000UL )
	{
		FLASH_ACR |= 0b0000;
		Temp = FLASH_ACR;
	}
	else if(SYSC_CLK >30000000UL && SYSC_CLK <=60000000UL )
	{
		FLASH_ACR |= 0b0001;
		Temp = FLASH_ACR;
	}
	else if(SYSC_CLK >60000000UL && SYSC_CLK <=84000000UL)
	{
		FLASH_ACR |= 0b0010;
		Temp = FLASH_ACR;
	}
#endif
}

static ERR HAL_RCC_HSI_Init(RCC_Config_t *RCC_C)
{
	ERR ERR_Status = HAL_OK;
	if (RCC_C != NULL) {
		SYSC_CLK = 16000000UL;
		/*Enable HSI CLOCK and wait untill CLK is ready*/
		RCC->CR |= HSION_EN;
		/*Wait untill CLOCK is ready*/
		while(!(RCC->CR & HSIRDY_READ));
		/*CLear fLAG*/
		RCC->CIR |= (1<<HSIRDYC);
		/*Turn Off HSE Reduce Power Consumption*/
		RCC->CR &= HSEON_DIS;
		/*Set AHB prescaler*/
		ERR_Status = HAL_RCC_AHB_Init(RCC_C->AHB_PRE);
		if (ERR_Status == HAL_OK)
		{
		/*Set APB1 and APB2 prescaler*/
			ERR_Status = HAL_RCC_APB1_Init(RCC_C->APB1_PRE);
			ERR_Status |= HAL_RCC_APB2_Init(RCC_C->APB2_PRE);
			/*Select HSI as Main CLK SRC*/
			if (ERR_Status == HAL_OK)	/*Select HSI as Main CLK SRC*/
				RCC->CFGR |= SW_HSI;
		}

	} else {
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_PLL_Init(RCC_Config_t *RCC_C)
{
	ERR ERR_Status = HAL_OK;
	if (RCC_C != NULL) {
		switch( RCC_C->PLL_SRC )
		{
			case PLL_HSI:
				SYSC_CLK = 16000000UL;
				RCC->CR |= HSION_EN;
				/*Wait untill CLOCK is ready*/
				while (!(RCC->CR & HSIRDY_READ));
				/*CLear fLAG*/
				RCC->CIR |= (1 << HSIRDYC);
				/*Turn Off HSE Reduce Power Consumption*/
				RCC->CR &= HSEON_DIS;
				/*Select HSI to be SRC*/
				RCC->PLLCFGR &= PLLSRC_HSI;
				break;
			case PLL_HSE:
				SYSC_CLK = 25000000UL;
				/*Enable HSe CLOCK and wait untill CLK is ready*/
				RCC->CR |= HSEON_EN;
				/*Wait untill CLOCK is ready*/
				while (!(RCC->CR & HSERDY_READ));
				/*CLear fLAG*/
				RCC->CIR |= (1 << HSERDYC);
				/*Turn Off HSi Reduce Power Consumption*/
				RCC->CR &= HSION_DIS;
				/*Select PLL SRC to be HSE*/
				RCC->PLLCFGR |= PLLSRC_HSE;
				break;
		}
		/*CFG PLL*/
		ERR_Status  = HAL_RCC_PLL_P_Init(RCC_C);
		ERR_Status |= HAL_RCC_AHB_Init(RCC_C->AHB_PRE);
		if (ERR_Status == HAL_OK)
		{
			/*Set APB1 and APB2 prescaler*/
			ERR_Status = HAL_RCC_APB1_Init(RCC_C->APB1_PRE);
			ERR_Status |= HAL_RCC_APB2_Init(RCC_C->APB2_PRE);
			if (ERR_Status == HAL_OK)
			{
				/*Enable PLL */
				RCC->CR |= PLLON_EN;
				/*wait Untill phase locked loop locks*/
				while(!(RCC->CR & PLLRDY_READ));
			}
		}
	} else {
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_HSE_Init(RCC_Config_t *RCC_C)
{
	ERR ERR_Status = HAL_OK;
	if (RCC_C != NULL) {
		SYSC_CLK = 25000000UL;
		/*Enable HSe CLOCK and wait untill CLK is ready*/
		RCC->CR |= HSEON_EN;
		/*Wait untill CLOCK is ready*/
		while (!(RCC->CR & HSERDY_READ));
		/*CLear fLAG*/
		RCC->CIR |= (1<<HSERDYC);
		/*Turn Off HSi Reduce Power Consumption*/
		RCC->CR &= HSION_DIS;
		/*Set AHB prescaler*/
		ERR_Status=HAL_RCC_AHB_Init(RCC_C->AHB_PRE);
		if (ERR_Status == HAL_ERR)
		{
			/*Set APB1 and APB2 prescaler*/
			ERR_Status =HAL_RCC_APB1_Init(RCC_C->APB1_PRE);
			ERR_Status|=HAL_RCC_APB2_Init(RCC_C->APB2_PRE);
			/*Select HSI as Main CLK SRC*/
			if (ERR_Status == HAL_ERR)
				RCC->CFGR |= SW_HSE;
		}

	} else {
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_PLL_P_Init(RCC_Config_t *RCC_C)
{
	ERR ERR_Status = HAL_OK;
	float32_t Prescaler = 1;
	if (RCC_C != NULL) {
		if(RCC_C->PLL_N > PLLN_MAX || RCC_C->PLL_N <PLLN_MIN)
		{
			ERR_Status = HAL_ERR;
		}else{
			RCC->PLLCFGR |= (RCC_C->PLL_N<<PLLN);
			Prescaler *= RCC_C->PLL_N;
			if(RCC_C->PLL_M > PLLM_MAX || RCC_C->PLL_M <PLLM_MIN)
			{
				ERR_Status = HAL_ERR;
			}
			else{
				RCC->PLLCFGR |= (RCC_C->PLL_M<<PLLM);
				Prescaler /= RCC_C->PLL_M;
				switch( RCC_C ->PLL_P )
				{
					case PLLP_2:
						RCC->PLLCFGR |= PPLP_2;
						Prescaler /=2;
						break;
					case PLLP_4:
						RCC->PLLCFGR |= PPLP_4;
						Prescaler /=4;
						break;
					case PLLP_6:
						RCC->PLLCFGR |= PPLP_6;
						Prescaler /=6;
						break;
					case PLLP_8:
						RCC->PLLCFGR |= PPLP_8;
						Prescaler /=8;
						break;
					default:
						ERR_Status = HAL_ERR;
						break;
				}
				if(ERR_Status == HAL_OK)
				{
					switch(RCC_C->PLL_Q)
					{
						case PLLQ_2_S:
							RCC->PLLCFGR |= PLLQ_2;
							break;
						case PLLQ_3_S:
							RCC->PLLCFGR |= PLLQ_3;
							break;
						case PLLQ_4_S:
							RCC->PLLCFGR |= PLLQ_4;
							break;
						case PLLQ_5_S:
							RCC->PLLCFGR |= PLLQ_5;
							break;
						case PLLQ_6_S:
							RCC->PLLCFGR |= PLLQ_6;
							break;
						case PLLQ_7_S:
							RCC->PLLCFGR |= PLLQ_7;
							break;
						case PLLQ_8_S:
							RCC->PLLCFGR |= PLLQ_8;
							break;
						case PLLQ_9_S:
							RCC->PLLCFGR |= PLLQ_9;
							break;
						case PLLQ_10_S:
							RCC->PLLCFGR |= PLLQ_10;
							break;
						case PLLQ_11_S:
							RCC->PLLCFGR |= PLLQ_11;
							break;
						case PLLQ_12_S:
							RCC->PLLCFGR |= PLLQ_12;
							break;
						case PLLQ_13_S:
							RCC->PLLCFGR |= PLLQ_13;
							break;
						case PLLQ_14_S:
							RCC->PLLCFGR |= PLLQ_14;
							break;
						case PLLQ_15_S:
							RCC->PLLCFGR |= PLLQ_15;
							break;
						default:
							ERR_Status = HAL_ERR;
						break;
					}
				}else{
				}
				SYSC_CLK *= Prescaler;
			}
		}
	} else {
		ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_AHB_Init(uint8_t Prescaler)
{
	ERR ERR_Status = HAL_OK;
	switch( Prescaler )
	{
		case AHBDIV_1:
			RCC->CFGR |= AHB_1;
			break;
		case AHBDIV_2:
			RCC->CFGR |= AHB_2;
			break;
		case AHBDIV_4:
			RCC->CFGR |= AHB_4;
			break;
		case AHBDIV_8:
			RCC->CFGR |= AHB_8;
			break;
		case AHBDIV_16:
			RCC->CFGR |= AHB_16;
			break;
		case AHBDIV_64:
			RCC->CFGR |= AHB_64;
			break;
		case AHBDIV_128:
			RCC->CFGR |= AHB_128;
			break;
		case AHBDIV_256:
			RCC->CFGR |= AHB_256;
			break;
		case AHBDIV_512:
			RCC->CFGR |= AHB_512;
			break;
		default:
			ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_APB1_Init(uint8_t APB1_P)
{
	ERR ERR_Status = HAL_OK;
	switch (APB1_P) {
		case APB_AHB_1:
			RCC->CFGR |= APB1_AHB_1;
			break;
		case APB_AHB_2:
			RCC->CFGR |= APB1_AHB_2;
			break;
		case APB_AHB_4:
			RCC->CFGR |= APB1_AHB_4;
			break;
		case APB_AHB_8:
			RCC->CFGR |= APB1_AHB_8;
			break;
		case APB_AHB_16:
			RCC->CFGR |= APB1_AHB_16;
			break;
		default:
			ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

static ERR HAL_RCC_APB2_Init(uint8_t APB2_P)
{
	ERR ERR_Status = HAL_OK;
	switch (APB2_P) {
		case APB_AHB_1:
			RCC->CFGR |= APB2_AHB_1;
			break;
		case APB_AHB_2:
			RCC->CFGR |= APB2_AHB_2;
			break;
		case APB_AHB_4:
			RCC->CFGR |= APB2_AHB_4;
			break;
		case APB_AHB_8:
			RCC->CFGR |= APB2_AHB_8;
			break;
		case APB_AHB_16:
			RCC->CFGR |= APB2_AHB_16;
			break;
		default:
			ERR_Status = HAL_ERR;
	}
	return ERR_Status;
}

uint32_t HAL_RCC_GET_AHBFREQ()
{

}

uint32_t HAL_RCC_GET_APB1FREQ()
{

}

uint32_t HAL_RCC_GET_APB2FREQ()
{

}

uint32_t HAL_RCC_GET_SYSCLKFREQ()
{

}

uint32_t HAL_RCC_GET_APB1TIMERFREQ()
{

}

uint32_t HAL_RCC_GET_APB2TIMERFREQ()
{

}
