/*
 * CortexM4_core_NVIC.c
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */

#include "../../Inc/core/CortexM4_core_NVIC.h"

/*
 * author 	 : Seif Eldidi
 * arguments : none
 * brief  	 : An NVIC library API to enable all maskable interrupts
 */
__attribute__((naked)) void __enable_irq(void)
{
	/*Clear Primask Register */
	asm volatile ("CPSIE i");
	asm volatile ("BX LR");
}
/*
 * author 	 : Seif Eldidi
 * arguments : none
 * brief  	 : An NVIC library API to disable all maskable interrupts
 */
__attribute__((naked)) void __disable_irq(void)
{
	/*set Primask Register*/
	asm volatile ("CPSID i");
	asm volatile ("BX LR");
}
/*
 * author 	 : Seif Eldidi
 * arguments : none
 * brief  	 : An NVIC library API to enable all interrupts
 */
__attribute__((naked)) void __enable_fault_irq(void)
{
	asm volatile ("CPSIE f");
	asm volatile ("BX LR");
}
/*
 * author 	 : Seif Eldidi
 * arguments : none
 * brief  	 : An NVIC library API to disable all interrupts except NMI
 */
__attribute__((naked)) void __disable_fault_irq(void)
{
	asm volatile ("CPSID f");
	asm volatile ("BX LR");
}

/*
 * author 	 : Seif Eldidi
 * arguments : none
 * brief  	 : An NVIC library API to enable a ceratin IRQ on NVIC controller
 */
void NVIC_EnableIRQ(NVIC_IRQN IRQN)
{
	if((uint32_t)IRQN >= 0)
	{
		NVIC->ISER[(((uint32_t)IRQN) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQN) & 0x1FUL));
	}else{

	}
}

void NVIC_DisableIRQ(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		NVIC->ICER[(((uint32_t)IRQN) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQN) & 0x1FUL));
	} else {

	}
}

void NVIC_SetPendingIRQ(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		NVIC->ISPR[(((uint32_t)IRQN) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQN) & 0x1FUL));
	} else {

	}
}

void NVIC_ClearPendingIRQ(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		NVIC->ICPR[(((uint32_t)IRQN) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQN) & 0x1FUL));
	} else {

	}
}

uint32_t NVIC_GetPendingIRQ(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		return READ_BIT(NVIC->ISPR[(((uint32_t)IRQN)>>5)], (((uint32_t)IRQN)&(0x1FUL)));
	} else {

	}
}

void NVIC_SetPriority(NVIC_IRQN IRQN, uint32_t priority)
{
	if (((uint32_t) IRQN )>= 0) {
		NVIC->IPR[(uint32_t)IRQN]=(uint8_t)((priority<<(8U-NVIC_PRIORITYBITS))&(uint32_t)0xFF);
	} else {

	}
}

uint32_t NVIC_GetPriority(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		 return(((uint32_t)NVIC->IPR[((uint32_t)IRQN)]  >> (8U - NVIC_PRIORITYBITS)));
	} else {

	}
}

uint32_t NVIC_GetActive(NVIC_IRQN IRQN)
{
	if ((uint32_t) IRQN >= 0) {
		return READ_BIT(NVIC->IABR[(((uint32_t)IRQN)>>5)], (((uint32_t)IRQN)&(0x1FUL)));
	} else {

	}
}
