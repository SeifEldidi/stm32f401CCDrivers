/*
 * Hal_Usart.h
 *
 *  Created on: Aug 1, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_USART_H_
#define HAL_HAL_USART_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "../std_macros.h"
#include "Hal_RCC.h"
#include "Hal_GPIO.h"
#include "HAL_Dma.h"
#include "Hal_Usart_cfg.h"
#include "../core/CortexM4_core_NVIC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define APB2_PERIPHBASE	0x40010000UL
#define USART1_OFFSET	0x00001000UL
#define USART6_OFFSET   0x00001400UL

#define APB1_PERIPHBASE	0x40000000UL
#define USART2_OFFSET	0x00004400UL

#define USART1		   ((__IO USART_Typedef *)(APB2_PERIPHBASE+USART1_OFFSET))
#define USART2		   ((__IO USART_Typedef *)(APB1_PERIPHBASE+USART2_OFFSET))
#define USART6		   ((__IO USART_Typedef *)(APB2_PERIPHBASE+USART6_OFFSET))

#define USART_TX_MODE	 0U
#define USART_RX_MODE	 1U
#define USART_TX_RX_MODE 2U

#define USART_DATA_SIZE_8	0U
#define USART_DATA_SIZE_9	1U

#define USART_STOPBITS_1	 0U
#define USART_STOPBITS_0_5	 1U
#define USART_STOPBITS_2	 2U

#define USART_PARITY_EVEN	 0U
#define USART_PARITY_ODD	 1U
#define USART_NPARITY	     2U

#define INT_TX				0U
#define INT_RX				1U
#define INT_TX_RX			2U
#define INT_DIS				3U

#define DMA_DIS				0U
#define DMA_EN_TX			1U
#define DMA_EN_RX			2U
#define DMA_EN_TX_RX		7U


#define AF_USART_1 	7U
#define AF_USART_2  7U
#define AF_USART_6  8U
/*---------Usart_SR-----------*/
#define CTS			  9U
#define CTS_READ	  (1<<CTS)

#define LBD			  8U
#define LBD_READ	  (1<<LBD)

#define TXE			  7U
#define TXE_READ	  (1<<TXE)

#define TC			  6U
#define TC_READ		  (1<<TC)

#define RXNE		  5U
#define RXNE_READ	  (1<<RXNE)

#define IDLE		  4U
#define IDLE_READ	  (1<<IDLE)

#define ORE           3U
#define ORE_READ      (1<<ORE)

#define NF			  2U
#define NF_READ		  (1<<NF)

#define FE			  1U
#define FE_READ		  (1<<FE)

#define PE			  0U
#define PE_READ		  (1<<PE)
/*---------USART_CR1-----------*/
#define OVER8		  15U
#define OVERSMP_8	  (1<<OVER8)
#define OVERSMP_16	  (~OVERSMP_8)

#define UE			  13U
#define UE_EN	      (1<<UE)

#define M			  12U
#define M_8BITS	  	  (~M_9BITS)
#define M_9BITS		  (1<<M)

#define WAKE		  11U
#define WAKE_IDLE	  (1<<WAKE)

#define PCE		 	  10U
#define PCE_EN	  	  (1<<PCE)
#define PCE_DIS	  	  (~PCE_EN)

#define PS		  	  9U
#define PS_EVEN	  	  (~PS_ODD)
#define PS_ODD  	  (1<<PS)

#define PEIE          8U
#define PEIE_EN       (1<<PEIE)

#define TXEIE         7U
#define TXEIE_EN      (1<<TXEIE)

#define TCIE          6U
#define TCIE_EN       (1<<TCIE)

#define RXNEIE        5U
#define RXNEIE_EN     (1<<RXNEIE)

#define IDLEIE        4U
#define IDLEIE_EN     (1<<IDLEIE)

#define TE            3U
#define TE_EN         (1<<TE)

#define RE        	  2U
#define RE_EN     	  (1<<RE)
/*-------USART_CR2----------*/
#define LINEN		  14U
#define LINEN_EN      (1<<LINEN)
#define LINEN_DIS	  (~LINEN_EN)

#define STOP		  12U
#define STOP_1BIT	  (0<<STOP)
#define STOP_0_5BIT	  (1<<STOP)
#define STOP_2BIT	  (2<<STOP)

#define CLKEN		  11U
#define CLKEN_EN	  (1<<CLKEN)
#define CLKEN_DIS	  (~CLKEN_EN)

#define CPOL		  10U
#define CPOL_0	      (~CPOL_1)
#define CPOL_1	      (1<<CPOL)

#define CPHA		  8U
#define CPHA_0	  	  (~CPHA_1)
#define CPHA_1	  	  (1<<CPHA)
/*-------USART_CR3----------*/
#define DMA_TX		 7U
#define DMA_TX_EN	 (1<<DMA_TX)

#define DMA_RX		 6U
#define DMA_RX_EN	 (1<<DMA_RX)
/*------------Macro Declarations End-----------*/
/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef struct
{
	__IO uint32_t SR;
	__IO uint32_t DR;
	__IO uint32_t BRR;
	__IO uint32_t CR1;
	__IO uint32_t CR2;
	__IO uint32_t CR3;
	__IO uint32_t GTPR;
}USART_Typedef;

typedef struct
{
	uint16_t NTDR_TX;
	uint16_t NTDR_RX;
	uint32_t DMA_MAR_TX;
	uint32_t DMA_MAR_RX;
	uint8_t DMA_EN_Dis;
}DMA_Config_t;

#if ConfigUSART_ASYNC_Mode == EN
typedef struct
{
	USART_Typedef * Instance;
	uint8_t Mode;
	uint8_t No_StopBits;
	uint8_t Parity;
	uint32_t BaudRate;
#if ConfigUSART_INT_EN
	uint8_t TX_RX_Interrupts;
#if ConfigUSART_INT_EN_TX == EN
		void (*Tx_Callback) (void);
#endif
#if ConfigUSART_INT_EN_TX == EN
		void (*Rx_Callback) (void);
#endif
#endif
#if ConfigUSART_DMAA_EN_
	DMA_Config_t D_config;
#endif
	uint8_t Data_Size;
}USART_Config;
#endif

typedef void (*CallBack)(void);
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
#if ConfigUSART_ASYNC_Mode == EN
ERR xHAL_UsartInit(USART_Config * Usart);
ERR xHAL_UsartSendBYTEP(USART_Typedef *Usart , uint8_t data);
ERR xHAL_UsartLogInfo(USART_Typedef *Usart , uint8_t *Data ,...);
ERR xHAL_UsartSendNBYTESP(USART_Typedef * Usart , uint8_t *Buffer ,uint8_t Len ,uint8_t TermChar);
ERR xHAL_UsartSendNBYTESAsync(USART_Typedef * Usart , uint8_t *Buffer ,uint8_t Len ,uint8_t TermChar);
ERR xHAL_UsartRX1BYTESP(USART_Typedef * Usart , uint8_t *Buffer);
ERR xHAL_UsartRXNBYTEP(USART_Typedef * Usart,uint8_t *Buffer,uint8_t Term_Char);
#endif

/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_USART_H_ */
