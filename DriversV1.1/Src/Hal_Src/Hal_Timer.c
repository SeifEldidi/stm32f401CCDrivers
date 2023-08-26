/*
 * Hal_Timer.c
 *
 *  Created on: Aug 4, 2023
 *      Author: Seif pc
 */

/*-------------Includes Section Start---------------*/
#include "../../Inc/HAL/Hal_Timer.h"
/*-------------Includes Section End----------------*/


/*-------------Static Functions Declaration----------------*/
static void vHAL_TIMX_CLK_EN(__IO TIM_TypeDef *Inst);
static uint8_t HAL_TIMX_SIZE(__IO TIM_TypeDef *Inst);
static uint32_t Get_TIM_Freq(__IO TIM_TypeDef *Inst);

#if configHALTIMER_DELAY_EN == EN
static ERR xHAL_TIMXTB_Init(__IO TIM_TypeDef *Inst ,TIMX_TimeBase_t *TIMX_init);
static void vHal_TIMXTimeBaseInt_EN(__IO TIM_TypeDef *Inst,uint8_t Flag);
static void vHal_TIMXTimeBaseInt1_EN(__IO TIM_TypeDef *Inst,uint8_t Flag,void(*TIMX_TIMEBASE)(void));
static ERR vHal_TIMXCNTEn(__IO TIM_TypeDef *Inst ,uint8_t FLAG);
static void vHAL_Base1us(__IO TIM_TypeDef *Inst);
static void vHAL_Base1ms(__IO TIM_TypeDef *Inst);
static void vHal_CNT_Pin_Init(__IO TIM_TypeDef *Inst);
#endif

#if configHALTIMX_ICP_EN == EN
static ERR vHALICP_Init(__IO TIM_TypeDef *Inst ,TIMX_ICP_t *TIMX_init);
static void vHALICP_CallBack_Init(__IO TIM_TypeDef *Inst ,TIMX_ICP_t *TIMX_init);
static void vHALICP_Pin_Init(__IO TIM_TypeDef *Inst ,uint8_t CHN_No);
static void vHALICP_CHN_Init(__IO TIM_TypeDef *Inst,uint8_t Prescaler,uint8_t CHN_No,uint8_t DigitalFilter,uint8_t EdgeSel);
static void vHalIcp_PreSet(__IO uint32_t *REG,uint8_t Prescaler ,uint8_t CHN_no);
static void vHALICP_EdgeSet(__IO TIM_TypeDef *Inst,uint8_t Edge,uint8_t CCP,uint8_t CCNP);
static void vHALICP_FilterSet(__IO uint32_t *REG,uint8_t DigitalFilter,uint8_t Shift);
#endif

#if configHALTIMX_PWM_EN
static ERR vHALPWMOut_Init(__IO TIM_TypeDef *Inst ,TIMX_PWM_Config_t *TIMX_init);
#endif

static ERR vHal_SetPrescaler(__IO TIM_TypeDef *Inst ,uint8_t Flag);
/*-------------Static Variable Declaration----------------*/
#if configHALTIM1_INT_EN == EN
P2Func 	TIM1_TIMEBASE = NULL;
P2Func 	TIM1CH1_ICP = NULL;
P2Func	TIM1CH2_ICP = NULL;
P2Func	TIM1CH3_ICP = NULL;
P2Func	TIM1CH4_ICP = NULL;
#endif
#if configHALTIM2_INT_EN == EN
P2Func	TIM2_TIMEBASE = NULL;
P2Func	TIM2CH1_ICP = NULL;
P2Func	TIM2CH2_ICP = NULL;
P2Func	TIM2CH3_ICP = NULL;
P2Func	TIM2CH4_ICP = NULL;
#endif
#if configHALTIM3_INT_EN == EN
P2Func	TIM3_TIMEBASE = NULL;
P2Func	TIM3CH1_ICP = NULL;
P2Func	TIM3CH2_ICP = NULL;
P2Func	TIM3CH3_ICP = NULL;
P2Func	TIM3CH4_ICP = NULL;
#endif
#if configHALTIM4_INT_EN == EN
P2Func	TIM4_TIMEBASE = NULL;
P2Func	TIM4CH1_ICP = NULL;
P2Func	TIM4CH2_ICP = NULL;
P2Func	TIM4CH3_ICP = NULL;
P2Func	TIM4CH4_ICP = NULL;
#endif
#if configHALTIM5_INT_EN == EN
P2Func	TIM5_TIMEBASE = NULL;
P2Func	TIM5CH1_ICP = NULL;
P2Func	TIM5CH2_ICP = NULL;
P2Func	TIM5CH3_ICP = NULL;
P2Func	TIM5CH4_ICP = NULL;
#endif
/*-------------Global Variable Definitions----------*/



/*-------------Software Interfaces Defintions----------------*/
ERR xHAL_TIMX_Init(TIMX_t *Config )
{
	ERR Err_Status = HAL_OK;
	if(Config != NULL)
	{
		switch(Config->Mode)
		{
			case TIMX_TIMER:
				Err_Status = xHAL_TIMXTB_Init(Config->Inst,&Config->TimeBaseSet);
				break;
			case TIMX_COUNTER_RSEDG:
			case TIMX_COUNTER_FEDG:
			case TIMX_COUNTER_RFEDG:
				Err_Status = vHal_TIMXCNTEn(Config->Inst,Config->Mode);
				break;
			case TIMX_ICP:
				Err_Status = vHALICP_Init(Config->Inst,&Config->TimeXICPSet);
				break;
			case TIMX_PWM_OUTPUT:
				Err_Status = vHALPWMOut_Init(Config->Inst,&Config->TimxPwmSet);
				break;
			case TIMX_FORCED_OUTPUT:
				break;
			case TIMX_PWM_INPUT:
				break;
			case TIMX_OnePulse:
				break;
			case TIMX_Encoder:
				break;
			case TIMX_OCP:
				break;
		}
	}else{
		Err_Status = HAL_ERR;
	}
	return Err_Status;
}

#if configHALTIMER_DELAY_EN == EN
uint32_t uHAL_TIMX_GetTicks(__IO TIM_TypeDef *Inst)
{
	return Inst->CNT;
}

static void vHAL_Base1ms(__IO TIM_TypeDef *Inst)
{
	uint8_t TIMX_Size = HAL_TIMX_SIZE(Inst);
	uint32_t F_CPU = Get_TIM_Freq(Inst);
	uint32_t ARR = (F_CPU / (TIME_MS));
	if(TIMX_Size == TIMER_16BIT )
	{
		if( ARR > TIMER_16BIT_MAX)
		{
			F_CPU = F_CPU/2;
			ARR = (F_CPU / (TIME_MS));
			Inst->PSC = 1;
		}
	}
	Inst->ARR = ARR;
}

static void vHAL_Base1us(__IO TIM_TypeDef *Inst)
{
	uint32_t F_CPU = Get_TIM_Freq(Inst);
	uint32_t ARR   = (F_CPU/(TIME_US));
	Inst->ARR  = ARR;
}

void  vHAL_TIMXDelay(__IO TIM_TypeDef *Inst ,uint32_t Delay)
{
	uint32_t delay_Cnt ;
	for(delay_Cnt = 0; delay_Cnt<=Delay-1 ;delay_Cnt++)
	{
		while(!(Inst->SR & UIE_READ));
		Inst->SR &= (~UIE_READ);
	}
}

void  vHAL_TIMXDelayCALL(__IO TIM_TypeDef *Inst ,uint32_t Delay)
{
	uint32_t delay_Cnt;
	for (delay_Cnt = 0; delay_Cnt <= Delay - 1; delay_Cnt++) {
		while (!(Inst->SR & UIE_READ));
		Inst->SR &= (~UIE_READ);
		while (!(Inst->SR & UIE_READ));
		Inst->SR &= (~UIE_READ);
	}
}

void vHAL_TIMXsetDutyChn(__IO TIM_TypeDef *Inst ,uint8_t Duty,uint8_t CHN)
{
	switch (CHN) {
		case TIMX_CHN_1:
			Inst->CCR1 = ((Inst->ARR * Duty) / 100);
			break;
		case TIMX_CHN_2:
			Inst->CCR2 = ((Inst->ARR * Duty) / 100);
			break;
		case TIMX_CHN_3:
			Inst->CCR3 = ((Inst->ARR * Duty) / 100);
			break;
		case TIMX_CHN_4:
			Inst->CCR4 = ((Inst->ARR * Duty) / 100);
			break;
	}
	Inst->EGR |= UG_EN;
}

#endif
/*-------------Static Functions Defintions----------------*/
#if configHALTIMX_PWM_EN == EN
static void vHALPWMFreqSet(__IO TIM_TypeDef *Inst,uint8_t TIMX_Size,uint32_t CoreFreq,uint32_t PWM_Freq)
{
	uint32_t ARR = (CoreFreq/PWM_Freq);
	uint16_t Prescaler = 0;
	if( TIMX_Size == TIMER_16BIT)
	{
		while(ARR > TIMER_16BIT_MAX)
		{
			Prescaler +=1;
			ARR = (CoreFreq/(Prescaler*PWM_Freq));
		}
		Inst->ARR = ARR;
		Inst->PSC = Prescaler;
	}else{
		Inst->ARR = ARR;
	}
}


static void vHALPWMChn_init(__IO TIM_TypeDef *Inst,uint8_t CHN,uint8_t DutyCycle,uint8_t PWM_DIR)
{
	uint8_t MSK = (PWM_DIR == PWM_NORMAL)?6:7;
	switch(CHN)
	{
		case TIMX_CHN_1:
			Inst->CCMR1 &= ~(OC1M_PWM2);
			Inst->CCMR1 |= (MSK << OC1M);
			Inst->CCMR1 |= OC1PE_PREEN;
			Inst->CCER  |= CC1E_EN;
			Inst->CCR1  = ((Inst->ARR * DutyCycle)/100);
			break;
		case TIMX_CHN_2:
			Inst->CCMR1 &= ~(OC2M_PWM2);
			Inst->CCMR1 |= (MSK << OC2M);
			Inst->CCMR1 |= OC2PE_PREEN;
			Inst->CCER  |= CC2E_EN;
			Inst->CCR2  = ((Inst->ARR * DutyCycle)/100);
			break;
		case TIMX_CHN_3:
			Inst->CCMR2 &= ~(OC3M_PWM2);
			Inst->CCMR1 |= (MSK << OC3M);
			Inst->CCMR1 |= OC3PE_PREEN;
			Inst->CCER  |= CC3E_EN;
			Inst->CCR3  = ((Inst->ARR * DutyCycle)/100);
			break;
		case TIMX_CHN_4:
			Inst->CCMR2 &= ~(OC4M_PWM2);
			Inst->CCMR1 |= (MSK << OC4M);
			Inst->CCMR1 |= OC4PE_PREEN;
			Inst->CCER  |= CC4E_EN;
			Inst->CCR4  = ((Inst->ARR * DutyCycle)/100);
			break;
	}
}

static ERR vHALPWMOut_Init(__IO TIM_TypeDef *Inst ,TIMX_PWM_Config_t *TIMX_init)
{
	ERR TIMX_Status = HAL_OK;
	if(Inst != NULL && TIMX_init !=NULL)
	{
		uint8_t PCnt = 0;
		uint8_t Timx_CHN =1 ;
		uint8_t TIMx_Size  = HAL_TIMX_SIZE(Inst);
		uint32_t TIMX_freq = Get_TIM_Freq(Inst);
		vHAL_TIMX_CLK_EN(Inst);
		vHALPWMFreqSet(Inst,TIMx_Size,TIMX_freq,TIMX_init->Freq);
		for(PCnt = 0; PCnt<= NO_CHNS-1 ; PCnt++)
		{
			if (READ_BIT(TIMX_init->Channel_Active, PCnt))
			{
				uint8_t PWM_DIR = ((TIMX_init->PWM_MODE &(1<<PCnt))>>PCnt);
				vHALICP_Pin_Init(Inst,Timx_CHN);
				vHALPWMChn_init(Inst,Timx_CHN,TIMX_init->Duty_C[PCnt],PWM_DIR);
			} else {

			}
			Timx_CHN *=2;
		}
		Inst->EGR |= UG_EN;
		Inst->CR1 |= CNT_EN;
	}else{
		TIMX_Status = HAL_ERR;
	}
	return TIMX_Status;
}
#endif

#if configHALTIMER_DELAY_EN == EN
static ERR xHAL_TIMXTB_Init(__IO TIM_TypeDef *Inst ,TIMX_TimeBase_t *TIMX_init)
{
	ERR TIMX_Status = HAL_OK;
	if(Inst != NULL && TIMX_init !=NULL)
	{
		/*---------Check Timer Size------*/
		uint8_t TIMx_Size = HAL_TIMX_SIZE(Inst);
		/*------ Timer Clock Init------*/
		vHAL_TIMX_CLK_EN(Inst);
		/*------ Timer Mode Select ----*/
		switch (TIMX_init->Mode)
		{
			case TIMX_UPCOUNTER:
				Inst->CR1 &= CNT_UP;
				break;
			case TIMX_DCOUNTER:
				Inst->CR1 |= CNT_D;
				break;
			case TIMX_UPDCOUNTER_1:
				Inst->CR1 |= CMS_C_UP;
				break;
			case TIMX_UPDCOUNTER_2:
				Inst->CR1 |= CMS_C_D;
				break;
			case TIMX_UPDCOUNTER_3:
				Inst->CR1 |= CMS_C_U_D;
				break;
			default:
				TIMX_Status = HAL_ERR;
		}
		if(TIMX_Status == HAL_OK)
		{
			/*----------Int Enable-------*/
			vHal_TIMXTimeBaseInt1_EN(Inst,TIMX_init->Mode,TIMX_init->TIMX_Callback);
			/*----------Set prescaler---------*/
			if(TIMX_init->Mode != TIMX_EXT_Counter)
			{
				if(TIMX_init->Base_Mode == BASE_USER_DEFINED)
				{
					if(TIMx_Size == TIMER_16BIT)
					{
						Inst->PSC = TIMX_init->PSC & 0x0000FFFF;
						Inst->ARR = TIMX_init->ARR & 0x0000FFFF;
					}else{
						Inst->PSC = TIMX_init->PSC ;
						Inst->ARR = TIMX_init->ARR ;
					}
				}else{
					if(TIMX_init->Base_Mode == BASE_1MS_TICK)
						vHAL_Base1ms(Inst);
					else if (TIMX_init->Base_Mode == BASE_1US_TICK)
						vHAL_Base1us(Inst);
					else
						TIMX_Status = HAL_ERR;
				}
				if(TIMX_Status == HAL_OK)
				{
					Inst->CR1 |= CNT_EN;
					/*-------Enable Prescaler-----*/
					TIMX_Status = vHal_SetPrescaler(Inst,TIMX_init->Mode);
				}
			}
		}
	}else{
		TIMX_Status = HAL_ERR;
	}
	return TIMX_Status;
}
#endif

static uint32_t Get_TIM_Freq(__IO TIM_TypeDef *Inst)
{
	uint32_t Freq = 0 ;
	if(Inst == TIM2 || Inst == TIM3 ||Inst == TIM4 || Inst == TIM5 )
			Freq = HAL_RCC_GET_APB1FREQ();
	else if(Inst == TIM1 || Inst == TIM9 ||Inst == TIM10 || Inst == TIM11)
			Freq = HAL_RCC_GET_APB2FREQ();
	return Freq;
}

static uint8_t HAL_TIMX_SIZE(__IO TIM_TypeDef *Inst)
{
	uint8_t TIMX_Size = TIMER_16BIT ;
	if(Inst == TIM1 || Inst == TIM3 ||Inst == TIM4 || Inst == TIM9 || Inst == TIM10 || Inst == TIM9)
		TIMX_Size = TIMER_16BIT;
	else if(Inst == TIM2 || Inst == TIM5)
		TIMX_Size = TIMER_32BIT;
	return TIMX_Size;
}

static void vHAL_TIMX_CLK_EN(__IO TIM_TypeDef *Inst)
{
	if(Inst == TIM1)
	{
		HAL_RCC_TIM1_EN();
	}else if(Inst == TIM2)
	{
		HAL_RCC_TIM2_EN();
	}else if(Inst == TIM3)
	{
		HAL_RCC_TIM3_EN();
	}else if(Inst == TIM4)
	{
		HAL_RCC_TIM4_EN();
	}else if(Inst == TIM5)
	{
		HAL_RCC_TIM5_EN();
	}else if(Inst == TIM9)
	{
		HAL_RCC_TIM9_EN();
	}else if(Inst == TIM10)
	{
		HAL_RCC_TIM10_EN();
	}else if(Inst == TIM11)
	{
		HAL_RCC_TIM11_EN();
	}
}

static void vHal_TIMXTimeBaseInt1_EN(__IO TIM_TypeDef *Inst,uint8_t Flag,void(*TIMX_TIMEBASE)(void))
{
#if configHALTIM1_INT_EN == EN
	if (Inst == TIM1)
	{
		vHal_TIMXTimeBaseInt_EN(TIM1, Flag);
		TIM1_TIMEBASE = TIMX_TIMEBASE;
	}
#endif
#if configHALTIM2_INT_EN == EN
	if (Inst == TIM2)
	{
		vHal_TIMXTimeBaseInt_EN(TIM2, Flag);
		NVIC_EnableIRQ(TIM2_IRQ);
		TIM2_TIMEBASE = TIMX_TIMEBASE;
	}
#endif
#if configHALTIM3_INT_EN == EN
	if (Inst == TIM3)
	{
		vHal_TIMXTimeBaseInt_EN(TIM3, Flag);
		NVIC_EnableIRQ(TIM3_IRQ);
		TIM3_TIMEBASE = TIMX_TIMEBASE;
	}
#endif
#if configHALTIM4_INT_EN == EN
	if (Inst == TIM4)
	{
		vHal_TIMXTimeBaseInt_EN(TIM4, Flag);
		NVIC_EnableIRQ(TIM4_IRQ);
		TIM4_TIMEBASE = TIMX_TIMEBASE;
	}
#endif
#if configHALTIM5_INT_EN == EN
	if (Inst == TIM5)
	{
		vHal_TIMXTimeBaseInt_EN(TIM5, Flag);
		NVIC_EnableIRQ(TIM5_IRQ);
		TIM5_TIMEBASE = TIMX_TIMEBASE;
	}
#endif
}

static void vHal_TIMXTimeBaseInt_EN(__IO TIM_TypeDef *Inst,uint8_t Flag)
{
	switch (Flag) {
		case TIMX_UPCOUNTER:
		case TIMX_DCOUNTER:
		case TIMX_UPDCOUNTER_1:
		case TIMX_UPDCOUNTER_2:
		case TIMX_UPDCOUNTER_3:
			Inst->DIER |= UIE_EN;
			break;
		case TIMX_EXT_Counter:
			Inst->DIER |= TIE_EN;
			break;
		default:
			break;
	}
}

static ERR vHal_SetPrescaler(__IO TIM_TypeDef *Inst ,uint8_t Flag)
{
	ERR State = HAL_OK;
	switch (Flag) {
		case TIMX_UPCOUNTER:
		case TIMX_DCOUNTER:
		case TIMX_UPDCOUNTER_1:
		case TIMX_UPDCOUNTER_2:
		case TIMX_UPDCOUNTER_3:
			Inst->CR1 |= URS_EN;
			Inst->EGR |= UG_EN;
			break;
		default:
			break;
	}
	if(State == HAL_OK )
	{
		Inst->CR1 &= URS_DIS;
		Inst->EGR &= UG_DIS;
	}
	return State;
}

static void vHal_CNT_Pin_Init(__IO TIM_TypeDef *Inst)
{
	__IO GPIO_Typedef * GPIO_PORT = GPIOA;
	GPIO_InitStruct GPIO_TI2;
	__IO uint32_t * GPIO_AFR = NULL;
	uint32_t GPIO_AFR_MSK	 = 0x00;
	uint32_t GPIO_CLEAR_MSK	 = 0x0F;
	GPIO_TI2.pull = GPIO_NOPULL;
	GPIO_TI2.mode = GPIO_MODE_AF_PP;
	if (Inst == TIM1) {
		GPIO_PORT = GPIOA;
		GPIO_TI2.Pin = GPIO_PIN_9;
		GPIO_AFR = &GPIO_PORT->AFRH;
		GPIO_AFR_MSK = 1<<4;
		GPIO_CLEAR_MSK <<=4;
	} else if (Inst == TIM2) {
		GPIO_PORT = GPIOA;
		GPIO_TI2.Pin = GPIO_PIN_1;
		GPIO_AFR = &GPIO_PORT->AFRL;
		GPIO_AFR_MSK = 1<<4;
		GPIO_CLEAR_MSK <<=4;
	} else if (Inst == TIM3) {
		GPIO_PORT = GPIOB;
		GPIO_TI2.Pin = GPIO_PIN_5;
		GPIO_AFR = &GPIO_PORT->AFRL;
		GPIO_AFR_MSK = 2<<20;
		GPIO_CLEAR_MSK <<=20;
	} else if (Inst == TIM4) {
		GPIO_PORT = GPIOB;
		GPIO_TI2.Pin = GPIO_PIN_7;
		GPIO_AFR = &GPIO_PORT->AFRL;
		GPIO_AFR_MSK = 2<<28;
		GPIO_CLEAR_MSK <<=28;
	} else if (Inst == TIM5) {
		GPIO_PORT = GPIOA;
		GPIO_TI2.Pin = GPIO_PIN_1;
		GPIO_AFR = &GPIO_PORT->AFRL;
		GPIO_AFR_MSK = 2<<4;
		GPIO_CLEAR_MSK <<=4;
	} else if (Inst == TIM9) {
		GPIO_PORT = GPIOA;
		GPIO_TI2.Pin = GPIO_PIN_3;
		GPIO_AFR = &GPIO_PORT->AFRL;
		GPIO_AFR_MSK = 3<<12;
		GPIO_CLEAR_MSK <<=12;
	}
	HAL_GPIO_Init(GPIO_PORT,&GPIO_TI2);
	/*------GPIO AFR-----*/
	*GPIO_AFR &= (~GPIO_CLEAR_MSK);
	*GPIO_AFR |= GPIO_AFR_MSK;
}

static ERR vHal_TIMXCNTEn(__IO TIM_TypeDef *Inst ,uint8_t FLAG)
{
	ERR TIMX_Status = HAL_OK;
	if (Inst != NULL) {
		/*------ Timer Clock Init------*/
		vHAL_TIMX_CLK_EN(Inst);
		/*------ Set GPIO Input and set AFIOR---*/
		vHal_CNT_Pin_Init(Inst);
		/*------Configure Pin on Rising Edge---*/
		switch (FLAG)
		{
			case TIMX_COUNTER_RSEDG:
				Inst->CCMR1 |= CC2S_IN_T1;
				Inst->SMCR  |= EXT_CLOCK_MODE1;
				Inst->SMCR  |= EXT_TI2;
				break;
			case TIMX_COUNTER_FEDG:
				Inst->CCMR1 |= CC2S_IN_T1;
				Inst->CCER  |= CC2P_FEDG;
				Inst->SMCR  |= EXT_CLOCK_MODE1;
				Inst->SMCR  |= EXT_TI2;
				break;
			case TIMX_COUNTER_RFEDG:
				Inst->CCMR1 |= CC2S_IN_T1;
				Inst->CCER  |= CC2P_FEDG;
				Inst->CCER  |= CC2NP_EN;
				Inst->SMCR  |= EXT_CLOCK_MODE1;
				Inst->SMCR  |= EXT_TI2;
				break;
			default:
				TIMX_Status = HAL_ERR;
		}
		if( TIMX_Status == HAL_OK)
		{
			/*------Enable Disable Interrupt----*/
			vHal_TIMXTimeBaseInt_EN(Inst,TIMX_EXT_Counter);
			/*------Enable Cnt------*/
			Inst->CR1 |= CNT_EN;
		}
	} else {
		TIMX_Status = HAL_ERR;
	}
	return TIMX_Status;
}


static void vHALICP_Pin_Init(__IO TIM_TypeDef *Inst,uint8_t CHN_No)
{
	__IO GPIO_Typedef *GPIO_PORT = GPIOA;
	GPIO_InitStruct GPIO_TI2;
	__IO uint32_t *GPIO_AFR = NULL;
	uint32_t GPIO_AFR_MSK = 0x00;
	uint32_t GPIO_CLEAR_MSK = 0x0F;

	GPIO_TI2.pull = GPIO_NOPULL;
	GPIO_TI2.mode = GPIO_MODE_AF_PP;

	if (Inst == TIM1) {
		GPIO_PORT = GPIOA;
		GPIO_AFR =  &GPIO_PORT->AFRH;
		switch(CHN_No)
		{
			case TIMX_CHN_1:
				GPIO_TI2.Pin = GPIO_PIN_8;
				GPIO_AFR_MSK = 1<<0;
				GPIO_CLEAR_MSK<<=0;
				break;
			case TIMX_CHN_2:
				GPIO_TI2.Pin = GPIO_PIN_9;
				GPIO_AFR_MSK = 1<<4;
				GPIO_CLEAR_MSK<<=4;
				break;
			case TIMX_CHN_3:
				GPIO_TI2.Pin = GPIO_PIN_10;
				GPIO_AFR_MSK = 1<<8;
				GPIO_CLEAR_MSK<<=8;
				break;
			case TIMX_CHN_4:
				GPIO_TI2.Pin = GPIO_PIN_11;
				GPIO_AFR_MSK = 1<<12;
				GPIO_CLEAR_MSK<<=12;
				break;
			default:
				break;
		}
	} else if (Inst == TIM2) {
		GPIO_PORT = GPIOA;
		GPIO_AFR =  &GPIO_PORT->AFRL;
		switch (CHN_No) {
			case TIMX_CHN_1:
				GPIO_TI2.Pin = GPIO_PIN_0;
				GPIO_AFR_MSK = 1<<0;
				GPIO_CLEAR_MSK<<=0;
				break;
			case TIMX_CHN_2:
				GPIO_TI2.Pin = GPIO_PIN_1;
				GPIO_AFR_MSK = 1<<4;
				GPIO_CLEAR_MSK<<=4;
				break;
			case TIMX_CHN_3:
				GPIO_TI2.Pin = GPIO_PIN_2;
				GPIO_AFR_MSK = 1<<8;
				GPIO_CLEAR_MSK<<=8;
				break;
			case TIMX_CHN_4:
				GPIO_TI2.Pin = GPIO_PIN_3;
				GPIO_AFR_MSK = 1<<12;
				GPIO_CLEAR_MSK<<=12;
				break;
			default:
				break;
		}
	} else if (Inst == TIM3) {
		GPIO_PORT = GPIOA;
		switch (CHN_No) {
			case TIMX_CHN_1:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_TI2.Pin = GPIO_PIN_6;
				GPIO_AFR_MSK = 2<<24;
				GPIO_CLEAR_MSK<<=24;
				break;
			case TIMX_CHN_2:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_TI2.Pin = GPIO_PIN_7;
				GPIO_AFR_MSK = 2<<28;
				GPIO_CLEAR_MSK<<=28;
				break;
			case TIMX_CHN_3:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_PORT = GPIOB;
				GPIO_TI2.Pin = GPIO_PIN_0;
				GPIO_AFR_MSK = 2 << 0;
				GPIO_CLEAR_MSK <<= 0;
				break;
			case TIMX_CHN_4:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_PORT = GPIOB;
				GPIO_TI2.Pin = GPIO_PIN_1;
				GPIO_AFR_MSK = 2 << 4;
				GPIO_CLEAR_MSK <<= 4;
				break;
			default:
				break;
		}
	} else if (Inst == TIM4) {
		GPIO_PORT = GPIOB;
		switch (CHN_No) {
			case TIMX_CHN_1:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_TI2.Pin = GPIO_PIN_6;
				GPIO_AFR_MSK = 2 << 24;
				GPIO_CLEAR_MSK <<= 23;
				break;
			case TIMX_CHN_2:
				GPIO_AFR =  &GPIO_PORT->AFRL;
				GPIO_TI2.Pin = GPIO_PIN_7;
				GPIO_AFR_MSK = 2 << 28;
				GPIO_CLEAR_MSK <<= 28;
				break;
			case TIMX_CHN_3:
				GPIO_AFR =  &GPIO_PORT->AFRH;
				GPIO_TI2.Pin = GPIO_PIN_8;
				GPIO_AFR_MSK = 2 << 0;
				GPIO_CLEAR_MSK <<= 0;
				break;
			case TIMX_CHN_4:
				GPIO_AFR =  &GPIO_PORT->AFRH;
				GPIO_TI2.Pin = GPIO_PIN_9;
				GPIO_AFR_MSK = 2 << 4;
				GPIO_CLEAR_MSK <<= 4;
				break;
			default:
				break;
		}
	} else if (Inst == TIM5) {
		GPIO_PORT = GPIOA;
		GPIO_AFR =  &GPIO_PORT->AFRL;
		switch (CHN_No) {
			case TIMX_CHN_1:
				GPIO_TI2.Pin = GPIO_PIN_0;
				GPIO_AFR_MSK = 2 << 0;
				GPIO_CLEAR_MSK <<= 0;
				break;
			case TIMX_CHN_2:
				GPIO_TI2.Pin = GPIO_PIN_1;
				GPIO_AFR_MSK = 2 << 4;
				GPIO_CLEAR_MSK <<= 4;
				break;
			case TIMX_CHN_3:
				GPIO_TI2.Pin = GPIO_PIN_2;
				GPIO_AFR_MSK = 2 << 8;
				GPIO_CLEAR_MSK <<= 8;
				break;
			case TIMX_CHN_4:
				GPIO_TI2.Pin = GPIO_PIN_3;
				GPIO_AFR_MSK = 2 << 12;
				GPIO_CLEAR_MSK <<= 12;
				break;
			default:
				break;
		}
	} else if (Inst == TIM9) {
		GPIO_PORT = GPIOA;
		GPIO_AFR =  &GPIO_PORT->AFRL;
		switch (CHN_No) {
			case TIMX_CHN_1:
				GPIO_TI2.Pin = GPIO_PIN_2;
				GPIO_AFR_MSK = 3 << 8;
				GPIO_CLEAR_MSK <<= 8;
				break;
			case TIMX_CHN_2:
				GPIO_TI2.Pin = GPIO_PIN_3;
				GPIO_AFR_MSK = 3 << 12;
				GPIO_CLEAR_MSK <<= 12;
				break;
			default:
				break;
		}
	}
	HAL_GPIO_Init(GPIO_PORT, &GPIO_TI2);
	/*------GPIO AFR-----*/
	*GPIO_AFR &= (~GPIO_CLEAR_MSK);
	*GPIO_AFR |= GPIO_AFR_MSK;
}

static void vHalIcp_PreSet(__IO uint32_t *REG,uint8_t Prescaler ,uint8_t CHN_no)
{
	switch(Prescaler)
	{
		case Prescaler1_CH1:
			*REG |= (0<<CHN_no);
			break;
		case Prescaler2_CH1:
			*REG |= (1<<CHN_no);
			break;
		case Prescaler4_CH1:
			*REG |= (2<<CHN_no);
			break;
		case Prescaler8_CH1:
			*REG |= (3<<CHN_no);
			break;
	}
}

static void vHALICP_EdgeSet(__IO TIM_TypeDef *Inst,uint8_t Edge,uint8_t CCP,uint8_t CCNP)
{
	switch(Edge)
	{
		case RISING_EDGE_CH1:
			Inst->CCER &= (~(1<<CCP));
			Inst->CCER &= (~(1<<CCNP));
			break;
		case FALLING_EDGE_CH1:
			Inst->CCER |= (1<<CCP);
			break;
		case RISING_FALLING_EDGE_CH1:
			Inst->CCER |= (1<<CCP);
			Inst->CCER |= (1<<CCNP);
			break;
		default:
			break;
	}
}

static void vHALICP_FilterSet(__IO uint32_t *REG,uint8_t DigitalFilter,uint8_t Shift)
{
	*REG |= (DigitalFilter <<Shift);
}


static void vHALICP_CHN_Init(__IO TIM_TypeDef *Inst,uint8_t Prescaler,uint8_t CHN_No,uint8_t DigitalFilter,uint8_t EdgeSel)
{
	switch (CHN_No) {
		case TIMX_CHN_1:
			Inst->CCMR1 |=CC1S_IN_T1;
			vHalIcp_PreSet(&Inst->CCMR1,Prescaler,IC1PSC);
			vHALICP_EdgeSet(Inst,EdgeSel,CC1P,CC1NP);
			vHALICP_FilterSet(&Inst->CCMR1,DigitalFilter,IC1F);
			Inst->CCER |=CC1E_EN;
			break;
		case TIMX_CHN_2:
			Inst->CCMR1 |=CC2S_IN_T1;
			vHalIcp_PreSet(&Inst->CCMR1,Prescaler,IC2PSC);
			vHALICP_EdgeSet(Inst,EdgeSel,CC2P,CC2NP);
			vHALICP_FilterSet(&Inst->CCMR1,DigitalFilter,IC2F);
			Inst->CCER |=CC2E_EN;
			break;
		case TIMX_CHN_3:
			Inst->CCMR2 |=CC1S_IN_T1;
			vHalIcp_PreSet(&Inst->CCMR2,Prescaler,IC1PSC);
			vHALICP_EdgeSet(Inst,EdgeSel,CC3P,CC3NP);
			vHALICP_FilterSet(&Inst->CCMR2,DigitalFilter,IC1F);
			Inst->CCER |=CC3E_EN;
			break;
		case TIMX_CHN_4:
			Inst->CCMR2 |=CC2S_IN_T1;
			vHalIcp_PreSet(&Inst->CCMR2,Prescaler,IC2PSC);
			vHALICP_EdgeSet(Inst,EdgeSel,CC4P,CC4NP);
			vHALICP_FilterSet(&Inst->CCMR2,DigitalFilter,IC2F);
			Inst->CCER |=CC4E_EN;
			break;
		default:
			break;
	}
}

static void vHALICP_CallBack_Init(__IO TIM_TypeDef *Inst ,TIMX_ICP_t *TIMX_init)
{
	P2PFunc CllBack1 = NULL;
	P2PFunc CllBack2 = NULL;
	P2PFunc CllBack3 = NULL;
	P2PFunc CllBack4 = NULL;

	if (Inst == TIM1) {
#if configHALTIM1_INT_EN == EN
		CllBack1 = &TIM1CH1_ICP;
		CllBack2 = &TIM1CH2_ICP;
		CllBack3 = &TIM1CH3_ICP;
		CllBack4 = &TIM1CH4_ICP;
#endif
	} else if (Inst == TIM2) {
#if configHALTIM2_INT_EN == EN
		Inst->DIER |= (TIMX_init->TIMX_Callback1 != NULL) ? (CC1IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback2 != NULL) ? (CC2IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback3 != NULL) ? (CC3IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback4 != NULL) ? (CC4IF) : 0;
		NVIC_EnableIRQ(TIM2_IRQ);
		CllBack1 = &TIM2CH1_ICP;
		CllBack2 = &TIM2CH2_ICP;
		CllBack3 = &TIM2CH3_ICP;
		CllBack4 = &TIM2CH4_ICP;
#endif
	} else if (Inst == TIM3) {
#if configHALTIM3_INT_EN == EN
		Inst->DIER |= (TIMX_init->TIMX_Callback1 != NULL) ? (CC1IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback2 != NULL) ? (CC2IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback3 != NULL) ? (CC3IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback4 != NULL) ? (CC4IF) : 0;
		NVIC_EnableIRQ(TIM3_IRQ);
		CllBack1 = &TIM3CH1_ICP;
		CllBack2 = &TIM3CH2_ICP;
		CllBack3 = &TIM3CH3_ICP;
		CllBack4 = &TIM3CH4_ICP;
#endif
	} else if (Inst == TIM4) {
#if configHALTIM4_INT_EN == EN
		Inst->DIER |= (TIMX_init->TIMX_Callback1 != NULL) ? (CC1IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback2 != NULL) ? (CC2IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback3 != NULL) ? (CC3IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback4 != NULL) ? (CC4IF) : 0;
		NVIC_EnableIRQ(TIM4_IRQ);
		CllBack1 = &TIM4CH1_ICP;
		CllBack2 = &TIM4CH2_ICP;
		CllBack3 = &TIM4CH3_ICP;
		CllBack4 = &TIM4CH4_ICP;
#endif
	} else if (Inst == TIM5) {
#if configHALTIM5_INT_EN == EN
		Inst->DIER |= (TIMX_init->TIMX_Callback1 != NULL) ? (CC1IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback2 != NULL) ? (CC2IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback3 != NULL) ? (CC3IF) : 0;
		Inst->DIER |= (TIMX_init->TIMX_Callback4 != NULL) ? (CC4IF) : 0;
		NVIC_EnableIRQ(TIM5_IRQ);
		CllBack1 = &TIM5CH1_ICP;
		CllBack2 = &TIM5CH2_ICP;
		CllBack3 = &TIM5CH3_ICP;
		CllBack4 = &TIM5CH4_ICP;
#endif
	}
	*CllBack1 = TIMX_init->TIMX_Callback1;
	*CllBack2 = TIMX_init->TIMX_Callback2;
	*CllBack3 = TIMX_init->TIMX_Callback3;
	*CllBack4 = TIMX_init->TIMX_Callback4;
}

static ERR vHALICP_Init(__IO TIM_TypeDef *Inst ,TIMX_ICP_t *TIMX_init)
{
	ERR TIMX_Status = HAL_OK;
	if (Inst != NULL) {
		uint8_t CHN_cnt =0;
		/*---------Check Timer Size------*/
		uint8_t TIMx_Size = HAL_TIMX_SIZE(Inst);
		uint8_t POW_2 = 1;
		/*------ Timer Clock Init------*/
		vHAL_TIMX_CLK_EN(Inst);
		/*------- Configure Channel----------*/
		for(CHN_cnt = 0 ;CHN_cnt<= NO_CHNS-1;CHN_cnt++)
		{
			/* ----if Channel is Enabled Init Channel---*/
			if(READ_BIT(TIMX_init->Channel , CHN_cnt))
			{
				uint8_t SHM_1 = CHN_cnt*2;
				uint8_t SHM_2 = 4*CHN_cnt;
				uint8_t Prescaler = ((TIMX_init->Prescaler & (0x03<<SHM_1))>>SHM_1);
				uint8_t DigitalFilter = ((TIMX_init->DigitalFilter & (0x0F<<SHM_2))>>SHM_2);
				uint8_t Edge = ((TIMX_init->EdgeSelector & (0x03<<SHM_1))>>SHM_1);
				/*--------Init GPIO and AFRIO-----*/
				vHALICP_Pin_Init(Inst,POW_2);
				/*--------CHN_Config------------*/
				vHALICP_CHN_Init(Inst,Prescaler,POW_2,DigitalFilter,Edge);
			}else{

			}
			POW_2 *=2;
		}
		if (TIMx_Size == TIMER_16BIT) {
			Inst->PSC = TIMX_init->PSC & 0x0000FFFF;
			Inst->ARR = TIMX_init->ARR & 0x0000FFFF;
		} else {
			Inst->PSC = TIMX_init->PSC;
			Inst->ARR = TIMX_init->ARR;
		}
		/*-------Set Up Interrupts-----*/
		vHALICP_CallBack_Init(Inst,TIMX_init);
		/*-------Enable Prescaler-----*/
		Inst->CR1 |= URS_EN;
		Inst->EGR |= UG_EN;

		Inst->CR1 |= CNT_EN;
		/*-------Enable Prescaler-----*/
		Inst->CR1 &= URS_DIS;
		Inst->EGR &= UG_DIS;
		/*-------Done----------------*/
	} else {
		TIMX_Status = HAL_ERR;
	}
	return TIMX_Status;
}



/*----------------------------ISR---------------------*/
#if configHALTIM2_INT_EN == EN
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & UIE_READ)
	{
		TIM2->SR &= (~UIE_READ);
		if(TIM2_TIMEBASE != NULL)
			TIM2_TIMEBASE();
	}
	if(TIM2->SR & CC1IF)
	{
		TIM2->SR &= (~CC1IF);
		if(TIM2CH1_ICP != NULL)
			TIM2CH1_ICP();
	}
	if(TIM2->SR & CC2IF)
	{
		TIM2->SR &= (~CC2IF);
		if(TIM2CH2_ICP != NULL)
			TIM2CH2_ICP();
	}
	if(TIM2->SR & CC3IF)
	{
		TIM2->SR &= (~CC3IF);
		if(TIM2CH3_ICP != NULL)
			TIM2CH3_ICP();
	}
	if(TIM2->SR & CC4IF)
	{
		TIM2->SR &= (~CC4IF);
		if(TIM2CH4_ICP != NULL)
			TIM2CH4_ICP();
	}
}
#endif

#if configHALTIM3_INT_EN == EN
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & UIE_READ)
	{
		TIM3->SR &= (~UIE_READ);
		if(TIM3_TIMEBASE != NULL)
			TIM3_TIMEBASE();
	}
	if(TIM3->SR & CC1IF)
	{
		TIM3->SR &= (~CC1IF);
		if(TIM3CH1_ICP != NULL)
			TIM3CH1_ICP();
	}
	if(TIM3->SR & CC2IF)
	{
		TIM3->SR &= (~CC2IF);
		if(TIM3CH2_ICP != NULL)
			TIM3CH2_ICP();
	}
	if(TIM3->SR & CC3IF)
	{
		TIM3->SR &= (~CC3IF);
		if(TIM3CH3_ICP != NULL)
			TIM3CH3_ICP();
	}
	if(TIM3->SR & CC4IF)
	{
		TIM3->SR &= (~CC4IF);
		if(TIM3CH4_ICP != NULL)
			TIM3CH4_ICP();
	}
}
#endif

#if configHALTIM4_INT_EN == EN
void TIM4_IRQHandler(void)
{
	if (TIM4->SR & UIE_READ) {
		TIM4->SR &= (~UIE_READ);
		if (TIM4_TIMEBASE != NULL)
			TIM4_TIMEBASE();
	}
	if (TIM4->SR & CC1IF) {
		TIM4->SR &= (~CC1IF);
		if (TIM4CH1_ICP != NULL)
			TIM4CH1_ICP();
	}
	if (TIM4->SR & CC2IF) {
		TIM4->SR &= (~CC2IF);
		if (TIM4CH2_ICP != NULL)
			TIM4CH2_ICP();
	}
	if (TIM4->SR & CC3IF) {
		TIM4->SR &= (~CC3IF);
		if (TIM4CH3_ICP != NULL)
			TIM4CH3_ICP();
	}
	if (TIM4->SR & CC4IF) {
		TIM4->SR &= (~CC4IF);
		if (TIM4CH4_ICP != NULL)
			TIM4CH4_ICP();
	}
}
#endif

#if configHALTIM5_INT_EN == EN
void TIM5_IRQHandler(void)
{
	if (TIM5->SR & UIE_READ) {
		TIM5->SR &= (~UIE_READ);
		if (TIM5_TIMEBASE != NULL)
			TIM5_TIMEBASE();
	}
	if (TIM5->SR & CC1IF) {
		TIM5->SR &= (~CC1IF);
		if (TIM5CH1_ICP != NULL)
			TIM5CH1_ICP();
	}
	if (TIM5->SR & CC2IF) {
		TIM5->SR &= (~CC2IF);
		if (TIM5CH2_ICP != NULL)
			TIM5CH2_ICP();
	}
	if (TIM5->SR & CC3IF) {
		TIM5->SR &= (~CC3IF);
		if (TIM5CH3_ICP != NULL)
			TIM5CH3_ICP();
	}
	if (TIM5->SR & CC4IF) {
		TIM5->SR &= (~CC4IF);
		if (TIM5CH4_ICP != NULL)
			TIM5CH4_ICP();
	}
}
#endif
