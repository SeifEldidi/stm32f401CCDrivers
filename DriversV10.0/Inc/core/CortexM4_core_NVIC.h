/*
 * CortexM4_core_NVIC.h
 *
 *  Created on: Jul 25, 2023
 *      Author: Seif pc
 */

#ifndef CORE_CORTEXM4_CORE_NVIC_H_
#define CORE_CORTEXM4_CORE_NVIC_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define NVIC_BASEADD		0xE000E100
#define NVIC				((__IO NVIC_Typedef *)(NVIC_BASEADD))
#define NVIC_PRIORITYBITS   4U
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/

typedef struct
{
   uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        uint32_t RESERVED0[24U];
   uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        uint32_t RSERVED1[24U];
   uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        uint32_t RESERVED2[24U];
   uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        uint32_t RESERVED3[24U];
   uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        uint32_t RESERVED4[56U];
  uint8_t  IPR[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        uint32_t RESERVED5[644U];
   uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Typedef;


typedef enum
{
	/*------Non Configurable Interrupts----*/
	NMI_IRQ 		= -14,
	HardFault_IRQ 	= -13,
	MemManage_IRQ 	= -12,
	BusFault_IRQ 	= -11,
	UsageFault_IRQ  = -10,
	SVCall_IRQ 		= -5,
	PendSV_IRQ 		= -2,
	SysTick_IRQ 	= -1,
	/*------Configurable Interrupts---------*/
	WWDG_IRQ		    	= 0,
	EXTI16_PVD_IRQ			= 1,
	EXTI21_TAMP_STAMP_IRQ	= 2,
	EXTI22_RTC_WKUP_IRQ		= 3,
	FLASH_IRQ 				= 4,
	RCC_IRQ  				= 5,
	EXTI0_IRQ 				= 6,
	EXTI1_IRQ  				= 7,
	EXTI2_IRQ 				= 8,
	EXTI3_IRQ  				= 9,
	EXTI4_IRQ  				= 10,
	DMA1_Stream0_IRQ		= 11,
	DMA1_Stream1_IRQ		= 12,
	DMA1_Stream2_IRQ		= 13,
	DMA1_Stream3_IRQ		= 14,
	DMA1_Stream4_IRQ		= 15,
	DMA1_Stream5_IRQ		= 16,
	DMA1_Stream6_IRQ		= 17,
	ADC_IRQ					= 18,
	EXTI9_5_IRQ		    	= 23,
	TIM1_BRK_TIM9_IRQ		= 24,
	TIM1_UP_TIM10_IRQ	    = 25,
	TIM1_TRG_COM_TIM11_IRQ  = 26,
	TIM1_CC_IRQ 			= 27,
	TIM2_IRQ  				= 28,
	TIM3_IRQ 				= 29,
	TIM4_IRQ  				= 30,
	I2C1_EV_IRQ 			= 31,
	I2C1_ER_IRQ  			= 32,
	I2C2_EV_IRQ  			= 33,
	I2C2_ER_IRQ				= 34,
	SPI1_IRQ				= 35,
	SPI2_IRQ				= 36,
	USART1_IRQ				= 37,
	USART2_IRQ		   		= 38,
	EXTI15_10_IRQ		    = 40,
	EXTI17_RTC_Alarm_IRQ    = 41,
	EXTI18_OTG_FS_WKUP		= 42,
	DMA1_Stream7_IRQ 		= 47,
	SDIO_IRQ  				= 49,
	TIM5_IRQ  			    = 50,
	SPI3_IRQ				= 51,
	DMA2_Stream0_IRQ		= 56,
	DMA2_Stream1_IRQ		= 57,
	DMA2_Stream2_IRQ		= 58,
	DMA2_Stream3_IRQ		= 59,
	DMA2_Stream4_IRQ		= 60,
	OTG_FS_IRQ    		    = 67,
	DMA2_Stream5_IRQ		= 68,
	DMA2_Stream6_IRQ		= 69,
	DMA2_Stream7_IRQ		= 70,
	USART6_IRQ				= 71,
	I2C3_EV_IRQ				= 72,
	I2C3_ER_IRQ				= 73,
	FPU_IRQ					= 81,
	SPI4_IRQ				= 84,
}NVIC_IRQN;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
__attribute__((naked)) void __enable_irq(void);
__attribute__((naked)) void __disable_irq(void);

__attribute__((naked)) void __enable_fault_irq(void);
__attribute__((naked)) void __disable_fault_irq(void);

void NVIC_EnableIRQ(NVIC_IRQN IRQN);
void NVIC_DisableIRQ(NVIC_IRQN IRQN);
void NVIC_SetPendingIRQ(NVIC_IRQN IRQN);
void NVIC_ClearPendingIRQ(NVIC_IRQN IRQN);
uint32_t NVIC_GetPendingIRQ(NVIC_IRQN IRQN);
void NVIC_SetPriority(NVIC_IRQN IRQN, uint32_t priority);
uint32_t NVIC_GetPriority(NVIC_IRQN IRQN);
uint32_t NVIC_GetActive(NVIC_IRQN IRQN);
/*-----------Software Interfaces End--------------*/


#endif /* CORE_CORTEXM4_CORE_NVIC_H_ */
