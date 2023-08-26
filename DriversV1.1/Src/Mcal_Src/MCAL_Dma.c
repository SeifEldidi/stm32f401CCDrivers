/*
 * MCAL_Dma.c
 *
 *  Created on: Aug 26, 2023
 *      Author: Seif pc
 */
#include "../../Inc/Mcal/MCAL_Dma.h"

/*--------------Static Functions Definition---------*/
static DMA_Errors DMA_CLK_EN(__IO DMA_TypeDef * Inst);
static DMA_Errors DMA_STREAM_DIS(__IO DMA_Stream_TypeDef * Inst, __IO DMA_TypeDef *DMA );
static DMA_Errors DMA_STREAM_DIRECTMODE_Config(DMA_StreamX_t *DmaSX);
static DMA_Errors DMA_STREAM_DoubleBuffer_Config(DMA_StreamX_t *DmaSX);
static NVIC_IRQN  DMA_ChannelIntSource(__IO DMA_Stream_TypeDef * Inst, __IO DMA_TypeDef *DMA,uint8_t *Pos);

/*--------------Static Variables Definition---------*/
Callback DMA1_STREAMX_INT[DAM_STREAM_NO] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
Callback DMA2_STREAMX_INT[DAM_STREAM_NO] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

/*------------Static Functions Defintions-------*/
static DMA_Errors DMA_CLK_EN(__IO DMA_TypeDef * Inst)
{
	DMA_Errors Err_State = DMA_E_OK ;
	if(Inst != NULL)
	{
		if (Inst == DMA1) {
			HAL_RCC_DMA1_EN();
		} else if (Inst == DMA2) {
			HAL_RCC_DMA2_EN();
		} else
			Err_State = DMA_INCORRECT_REF;
	}else{
		Err_State = DMA_NULLPTR;
	}
	return Err_State;
}

static DMA_Errors DMA_STREAM_DIS(__IO DMA_Stream_TypeDef * Inst, __IO DMA_TypeDef *DMA )
{
	DMA_Errors Err_State = DMA_E_OK ;
	if (Inst != NULL) {
		__IO DMA_TypeDef *DMA_t = ((DMA==DMA1)?DMA1:(DMA == DMA2)?DMA2:NULL);
		/*--------Disable DMA Stream-------*/
		Inst->CR &= DMA_DIS;
		while(Inst->CR & DMA_EN);
		/*--------Clear All Status Bits-----*/
		if(DMA_t != NULL)
		{
			if(Inst == DMA1_Stream0 || Inst == DMA2_Stream0 )
			{
				SET_MSK(DMA_t->LIFCR ,(1<<CFEIF0|1<<CDMEIF0|1<<CTEIF0|1<<CHTIF0|1<<CTCIF0));
			}else if(Inst == DMA1_Stream1 || Inst == DMA2_Stream1)
			{
				SET_MSK(DMA_t->LIFCR ,(1<<CFEIF1|1<<CDMEIF1|1<<CTEIF1|1<<CHTIF1|1<<CTCIF1));
			}else if(Inst == DMA1_Stream2 || Inst == DMA2_Stream2)
			{
				SET_MSK(DMA_t->LIFCR ,(1<<CFEIF2|1<<CDMEIF2|1<<CTEIF2|1<<CHTIF2|1<<CTCIF2));
			}else if(Inst == DMA1_Stream3 || Inst == DMA2_Stream3)
			{
				SET_MSK(DMA_t->LIFCR ,(1<<CFEIF3|1<<CDMEIF3|1<<CTEIF3|1<<CHTIF3|1<<CTCIF3));
			}else if(Inst == DMA1_Stream4 || Inst == DMA2_Stream4)
			{
				SET_MSK(DMA_t->HIFCR ,(1<<CFEIF4|1<<CDMEIF4|1<<CTEIF4|1<<CHTIF4|1<<CTCIF4));
			}else if(Inst == DMA1_Stream5 || Inst == DMA2_Stream5)
			{
				SET_MSK(DMA_t->HIFCR ,(1<<CFEIF5|1<<CDMEIF5|1<<CTEIF5|1<<CHTIF5|1<<CTCIF5));
			}else if(Inst == DMA1_Stream6 || Inst == DMA2_Stream6)
			{
				SET_MSK(DMA_t->HIFCR ,(1<<CFEIF6|1<<CDMEIF6|1<<CTEIF6|1<<CHTIF6|1<<CTCIF6));
			}else if(Inst == DMA1_Stream7 || Inst == DMA2_Stream7)
			{
				SET_MSK(DMA_t->HIFCR ,(1<<CFEIF7|1<<CDMEIF7|1<<CTEIF7|1<<CHTIF7|1<<CTCIF7));
			}else{
				Err_State = DMA_INCORRECT_REF;
			}
		}else{
			Err_State = DMA_INCORRECT_REF;
		}
	} else {
		Err_State = DMA_NULLPTR;
	}
	return Err_State;
}

static NVIC_IRQN  DMA_ChannelIntSource(__IO DMA_Stream_TypeDef * Inst, __IO DMA_TypeDef *DMA,uint8_t *Pos)
{
	NVIC_IRQN Nvic_IRQN;
	__IO DMA_TypeDef *DMA_t = ((DMA==DMA1)?DMA1:(DMA == DMA2)?DMA2:NULL);
	if (DMA_t != NULL) {
		if (Inst == DMA1_Stream0 || Inst == DMA2_Stream0) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream0_IRQ:(DMA == DMA2)?DMA2_Stream0_IRQ:0);
			*Pos = 0;
		} else if (Inst == DMA1_Stream1 || Inst == DMA2_Stream1) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream1_IRQ:(DMA == DMA2)?DMA2_Stream1_IRQ:0);
			*Pos = 1;
		} else if (Inst == DMA1_Stream2 || Inst == DMA2_Stream2) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream2_IRQ:(DMA == DMA2)?DMA2_Stream2_IRQ:0);
			*Pos = 2;
		} else if (Inst == DMA1_Stream3 || Inst == DMA2_Stream3) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream3_IRQ:(DMA == DMA2)?DMA2_Stream3_IRQ:0);
			*Pos = 3;
		} else if (Inst == DMA1_Stream4 || Inst == DMA2_Stream4) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream4_IRQ:(DMA == DMA2)?DMA2_Stream4_IRQ:0);
			*Pos = 4;
		} else if (Inst == DMA1_Stream5 || Inst == DMA2_Stream5) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream5_IRQ:(DMA == DMA2)?DMA2_Stream5_IRQ:0);
			*Pos = 5;
		} else if (Inst == DMA1_Stream6 || Inst == DMA2_Stream6) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream6_IRQ:(DMA == DMA2)?DMA2_Stream6_IRQ:0);
			*Pos = 6;
		} else if (Inst == DMA1_Stream7 || Inst == DMA2_Stream7) {
			Nvic_IRQN = ((DMA==DMA1)?DMA1_Stream7_IRQ:(DMA == DMA2)?DMA2_Stream7_IRQ:0);
			*Pos = 7;
		}
	} else {
	}
	return Nvic_IRQN;
}

static DMA_Errors DMA_STREAM_DIRECTMODE_Config(DMA_StreamX_t *DmaSX)
{
	DMA_Errors Err_State = DMA_E_OK ;
	if(DmaSX != NULL)
	{
		uint32_t Reg_CPY = 0x00000000;
		uint8_t  Callback_Pos = 0 ;
		Callback (*Callback_Array)[8] = ((DmaSX->DMAinst==DMA1)?&DMA1_STREAMX_INT:(DmaSX->DMAinst == DMA2)?&DMA2_STREAMX_INT:NULL);
		NVIC_IRQN NVIC_ENUM = DMA_ChannelIntSource(DmaSX->Instance,DmaSX->DMAinst,&Callback_Pos);
		/*-------Configure Channel-----*/
		CLEAR_MSK(Reg_CPY,DMA_CHN_7);
		Reg_CPY |= (DmaSX->Channel << DMA_X_CHANNEL);
		/*-------Configure PSIZE/MSIZE--------*/
		if(DmaSX->MemoryIncEnable == MEMORY_INC_ENABLED)
		{
			CLEAR_MSK(Reg_CPY,DMA_MINC_EN);
			SET_MSK(Reg_CPY,(DmaSX->MemoryIncrementSize << DMA_MSIZE)|DMA_MINC_EN);
		}else{
			SET_MSK(Reg_CPY,(DmaSX->MemoryIncrementSize << DMA_MSIZE));
		}
		if(DmaSX->PeripheralIncEnable == PERIPH_INC_ENABLED)
		{
			CLEAR_MSK(Reg_CPY,DMA_PINC_EN);
			SET_MSK(Reg_CPY,(DmaSX->PeripheralIncrementSize << DMA_PSIZE)|DMA_PINC_EN);
		}else{
			SET_MSK(Reg_CPY,(DmaSX->PeripheralIncrementSize << DMA_PSIZE));
		}
		/*-----Set PriorityLevel------*/
		uint32_t Priority_level = (DmaSX->StreamXPriority == DMA_STREAMX_PRIORITY_LOW)?DMA_LOW_PRIORITY:
				(DmaSX->StreamXPriority == DMA_STREAMX_PRIORITY_MED)?DMA_MED_PRIORITY:
				(DmaSX->StreamXPriority == DMA_STREAMX_PRIORITY_HIGH)?DMA_HIGH_PRIORITY:
				(DmaSX->StreamXPriority == DMA_STREAMX_PRIORITY_VHIGH)?DMA_VHIGH_PRIORITY:DMA_LOW_PRIORITY;
		CLEAR_MSK(Reg_CPY,DMA_VHIGH_PRIORITY);
		SET_MSK(Reg_CPY,Priority_level);
		/*------Circular Mode Set-----*/
		if(DmaSX->DMA_StreamXMode == DMA_DIRECT_NCIRC_MODE)
			CLEAR_MSK(Reg_CPY,DMA_CIRC_EN);
		else if(DmaSX->DMA_StreamXMode == DMA_DIRECT_CIRC_MODE)
		{
			CLEAR_MSK(Reg_CPY, DMA_CIRC_EN);
			SET_MSK(Reg_CPY, DMA_CIRC_EN);
		}
		/*----Set Data Dir-----*/
		switch(DmaSX->DataDir)
		{
			case DMA_STREAMX_P2M:
				SET_MSK(Reg_CPY,DMA_P2M);
				break;
			case DMA_STREAMX_M2P:
				SET_MSK(Reg_CPY,DMA_M2P);
				break;
			case DMA_STREAMX_M2M:
				SET_MSK(Reg_CPY,DMA_M2M);
				break;
			default:
				Err_State = DMA_CONFIG_ERR;
		}
		if(Err_State == DMA_E_OK)
		{
			/*------Set PAR/MAR/NDTR-----*/
			DmaSX->Instance->PAR = DmaSX->PAR;

			DmaSX->Instance->M0AR = DmaSX->MAR;

			DmaSX->Instance->NDTR = DmaSX->NDTRG;
			/*------Check Interrupts----*/
			if(DmaSX->StreamXIntEnabled == DMA_STREAMX_INT_TCIE_ENABLED)
			{
				Reg_CPY |= DMA_TCIE_EN;
				(*Callback_Array)[Callback_Pos] = DmaSX->Func ;
				NVIC_EnableIRQ(NVIC_ENUM);
				DmaSX->Instance->CR = Reg_CPY;
			}else if(DmaSX->StreamXIntEnabled == DMA_STREAMX_INT_TCIE_ENABLED)
			{
				Reg_CPY |= DMA_HCIE_EN;
				(*Callback_Array)[Callback_Pos] = DmaSX->Func ;
				NVIC_EnableIRQ(NVIC_ENUM);
				DmaSX->Instance->CR = Reg_CPY;
			}else {
				DmaSX->Instance->CR = Reg_CPY;
			}
		}
	}else{
		Err_State = DMA_NULLPTR;
	}
	return Err_State;
}


static DMA_Errors DMA_STREAM_DoubleBuffer_Config(DMA_StreamX_t *DmaSX)
{

}


/*--------------Global Functions Definition---------*/
void MCAL_DmaCallBackSet(__IO DMA_TypeDef *DMAinst,__IO DMA_Stream_TypeDef *Instance , Callback Func)
{
	uint8_t  Callback_Pos = 0 ;
	DMA_ChannelIntSource(Instance,DMAinst,&Callback_Pos);
	Callback (*Callback_Array)[8] = ((DMAinst==DMA1)?&DMA1_STREAMX_INT:(DMAinst == DMA2)?&DMA2_STREAMX_INT:NULL);
	(*Callback_Array)[Callback_Pos] = Func ;
}

DMA_Errors MCAL_DmaStreamX_Init(DMA_StreamX_t *DmaSX)
{
	DMA_Errors Err_State = DMA_E_OK ;
	if(DmaSX != NULL)
	{
		/*--------Enable Clock To Dma Controller----*/
		Err_State = DMA_CLK_EN(DmaSX->DMAinst);
		/*--------Configure DMA Stream-----------*/
		Err_State = DMA_STREAM_DIS(DmaSX->Instance , DmaSX->DMAinst);
		/*--------Configure Mode of Stream-----*/
		switch(DmaSX->DMA_StreamXMode)
		{
			case DMA_DIRECT_CIRC_MODE:
			case DMA_DIRECT_NCIRC_MODE:
				Err_State = DMA_STREAM_DIRECTMODE_Config(DmaSX);
				break;
			case DMA_DOUBLEBUFF_MODE:
				Err_State = DMA_STREAM_DoubleBuffer_Config(DmaSX);
				break;
			default:
				Err_State = DMA_CONFIG_ERR;
		}
	}else{
		Err_State = DMA_NULLPTR;
	}
	return Err_State;
}

#if DMA1_STREAM0_INT_EN == EN
void DMA1_Stream0_IRQHandler()
{
	if(DMA1->LISR & (1<<TEIF0) || (DMA1->LISR & (1<<HTIF0)))
	{
		SET_MSK(DMA1->LIFCR ,(1<<CHTIF0|1<<CTCIF0));
		if(DMA1_STREAMX_INT[0])
		 DMA1_STREAMX_INT[0]();
	}
}
#endif

#if DMA1_STREAM1_INT_EN == EN
void DMA1_Stream1_IRQHandler()
{
	if(DMA1->LISR & (1<<TEIF1) || (DMA1->LISR & (1<<HTIF1)))
	{
		SET_MSK(DMA1->LIFCR ,(1<<CHTIF1|1<<CTCIF1));
		if(DMA1_STREAMX_INT[1])
			 DMA1_STREAMX_INT[1]();
	}
}
#endif

#if DMA1_STREAM2_INT_EN == EN
void DMA1_Stream2_IRQHandler()
{
	SET_MSK(DMA1->LIFCR ,(1<<CHTIF2|1<<CTCIF2));
	if (DMA1->LISR & (1 << TEIF2) || (DMA1->LISR & (1 << HTIF2))) {
		if (DMA1_STREAMX_INT[2])
			DMA1_STREAMX_INT[2]();
	}
}
#endif

#if DMA1_STREAM3_INT_EN == EN
void DMA1_Stream3_IRQHandler()
 {
	SET_MSK(DMA1->LIFCR ,(1<<CHTIF3|1<<CTCIF3));
	if (DMA1->LISR & (1 << TEIF3) || (DMA1->LISR & (1 << HTIF3))) {
		if (DMA1_STREAMX_INT[3])
			DMA1_STREAMX_INT[3]();
	}

}
#endif

#if DMA1_STREAM4_INT_EN == EN
void DMA1_Stream4_IRQHandler()
{
	SET_MSK(DMA1->HIFCR ,(1<<CHTIF4|1<<CTCIF4));
	if (DMA1->HISR & (1 << TEIF4) || (DMA1->HISR & (1 << HTIF4))) {
		if (DMA1_STREAMX_INT[4])
			DMA1_STREAMX_INT[4]();
	}
}
#endif

#if DMA1_STREAM5_INT_EN == EN
void DMA1_Stream5_IRQHandler()
{
	SET_MSK(DMA1->HIFCR ,(1<<CHTIF5|1<<CTCIF5));
	if (DMA1->HISR & (1 << TEIF5) || (DMA1->HISR & (1 << HTIF5))) {
		if (DMA1_STREAMX_INT[5])
			DMA1_STREAMX_INT[5]();
	}
}
#endif

#if DMA1_STREAM6_INT_EN == EN
void DMA1_Stream6_IRQHandler()
{
	SET_MSK(DMA1->HIFCR ,(1<<CHTIF6|1<<CTCIF6));
	if (DMA1->HISR & (1 << TEIF6) || (DMA1->HISR & (1 << HTIF6))) {
		if (DMA1_STREAMX_INT[6])
			DMA1_STREAMX_INT[6]();
	}
}
#endif

#if DMA1_STREAM7_INT_EN == EN
void DMA1_Stream7_IRQHandler()
{
	SET_MSK(DMA1->HIFCR ,(1<<CHTIF7|1<<CTCIF7));
	if (DMA1->HISR & (1 << TEIF7) || (DMA1->HISR & (1 << HTIF7))) {
		if (DMA1_STREAMX_INT[7])
			DMA1_STREAMX_INT[7]();
	}
}
#endif

#if DMA2_STREAM0_INT_EN == EN
void DMA2_Stream0_IRQHandler()
{
	if(DMA2->LISR & (1<<TEIF0) || (DMA2->LISR & (1<<HTIF0)))
	{
		SET_MSK(DMA2->LIFCR ,(1<<CHTIF0|1<<CTCIF0));
		if(DMA2_STREAMX_INT[0])
			 DMA2_STREAMX_INT[0]();
	}
}
#endif

#if DMA2_STREAM1_INT_EN == EN
void DMA2_Stream1_IRQHandler()
{
	if(DMA2->LISR & (1<<TEIF1) || (DMA2->LISR & (1<<HTIF1)))
	{
		SET_MSK(DMA2->LIFCR ,(1<<CHTIF1|1<<CTCIF1));
		if(DMA2_STREAMX_INT[1])
			 DMA2_STREAMX_INT[1]();
	}
}
#endif

#if DMA2_STREAM2_INT_EN == EN
void DMA2_Stream2_IRQHandler()
{
	if(DMA2->LISR & (1<<TEIF2) || (DMA2->LISR & (1<<HTIF2)))
	{
		SET_MSK(DMA2->LIFCR ,(1<<CHTIF2|1<<CTCIF2));
		if(DMA2_STREAMX_INT[2])
			DMA2_STREAMX_INT[2]();
	}
}
#endif

#if DMA2_STREAM3_INT_EN == EN
void DMA2_Stream3_IRQHandler()
{
	if(DMA2->LISR & (1<<TEIF3) || (DMA2->LISR & (1<<HTIF3)))
	{
		SET_MSK(DMA2->LIFCR ,(1<<CHTIF3|1<<CTCIF3));
		if(DMA2_STREAMX_INT[3])
			DMA2_STREAMX_INT[3]();
	}
}
#endif

#if DMA2_STREAM4_INT_EN == EN
void DMA2_Stream4_IRQHandler()
{
	if(DMA2->HISR & (1<<TEIF4) || (DMA2->HISR & (1<<HTIF4)))
	{
		SET_MSK(DMA2->HIFCR ,(1<<CHTIF4|1<<CTCIF4));
		if(DMA2_STREAMX_INT[4])
			DMA2_STREAMX_INT[4]();
	}
}
#endif

#if DMA2_STREAM5_INT_EN == EN
void DMA2_Stream5_IRQHandler()
{
	if(DMA2->HISR & (1<<TEIF5) || (DMA2->HISR & (1<<HTIF5)))
	{
		SET_MSK(DMA2->HIFCR ,(1<<CHTIF5|1<<CTCIF5));
		if(DMA2_STREAMX_INT[5])
			DMA2_STREAMX_INT[5]();
	}
}
#endif

#if DMA2_STREAM6_INT_EN == EN
void DMA2_Stream6_IRQHandler()
{
	if(DMA2->HISR & (1<<TEIF06) || (DMA2->HISR  & (1<<HTIF6)))
	{
		SET_MSK(DMA2->HIFCR ,(1<<CHTIF6|1<<CTCIF6));
		if(DMA2_STREAMX_INT[6])
			DMA2_STREAMX_INT[6]();
	}
}
#endif

#if DMA2_STREAM7_INT_EN == EN
void DMA2_Stream7_IRQHandler()
{
	if (DMA2->HISR & (1 << TEIF7) || (DMA2->HISR & (1 << HTIF7)))
	{
		SET_MSK(DMA2->HIFCR ,(1<<CHTIF7|1<<CTCIF7));
		if (DMA2_STREAMX_INT[7] != NULL)
			DMA2_STREAMX_INT[7]();
	}
}
#endif
