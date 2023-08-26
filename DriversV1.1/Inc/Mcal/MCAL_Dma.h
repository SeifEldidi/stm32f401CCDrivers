/*
 * HAL_Dma.h
 *
 *  Created on: Jul 30, 2023
 *      Author: Seif pc
 */


#ifndef HAL_MCAL_DMA_H_
#define HAL_MCAL_DMA_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
#include "../HAL/Hal_RCC.h"
#include "MCAL_Dma_cfg.h"
#include "../Core/CortexM4_core_NVIC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define AHB1PERIPH_BASE       (0x40000000UL + 0x00020000UL)
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000UL)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010UL)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028UL)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040UL)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058UL)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070UL)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088UL)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0UL)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400UL)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)

#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define DMA2_Stream0        ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1        ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2        ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3        ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4        ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5        ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6        ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7        ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)

#define DAM_STREAM_NO				8U

#define DMA_DOUBLEBUFF_MODE			2U
#define DMA_DIRECT_NCIRC_MODE		1U
#define DMA_DIRECT_CIRC_MODE		0U

#define DMA_TRANSFER_TYPE_SINGLE	0U
#define DMA_TRANSFER_TYPE_BURST		1U

#define DMA_STREAMX_INT_TCIE_ENABLED	0x01
#define DMA_STREAMX_INT_HCIE_ENABLED	0x02
#define DMA_STREAMX_INT_DISABLED		0x00

#define DMA_STREAMX_PRIORITY_LOW	0U
#define DMA_STREAMX_PRIORITY_MED	1U
#define DMA_STREAMX_PRIORITY_HIGH	2U
#define DMA_STREAMX_PRIORITY_VHIGH	3U

#define DMA_PSIZE_INC_1			0U
#define DMA_PSIZE_INC_2			1U
#define DMA_PSIZE_INC_4			2U

#define MEMORY_INC_ENABLED		1U
#define PERIPH_INC_ENABLED		1U

#define DMA_STREAMX_P2M			0U
#define DMA_STREAMX_M2P			1U
#define DMA_STREAMX_M2M			2U


#define DMA_STREAMX_CHN0		0U
#define DMA_STREAMX_CHN1		1U
#define DMA_STREAMX_CHN2		2U
#define DMA_STREAMX_CHN3		3U
#define DMA_STREAMX_CHN4		4U
#define DMA_STREAMX_CHN5		5U
#define DMA_STREAMX_CHN6		6U
#define DMA_STREAMX_CHN7		7U

#define DMA_BURST_4X			0U
#define DMA_BURST_8X			1U
#define DMA_BURST_16X			2U

/*------------Dma_cr------------*/
#define DMA_X_CHANNEL		25U
#define DMA_CHN_CLR			(~(7<<DMA_X_CHANNEL))
#define DMA_CHN_0			(0<<DMA_X_CHANNEL)
#define DMA_CHN_1			(1<<DMA_X_CHANNEL)
#define DMA_CHN_2			(2<<DMA_X_CHANNEL)
#define DMA_CHN_3			(3<<DMA_X_CHANNEL)
#define DMA_CHN_4			(4<<DMA_X_CHANNEL)
#define DMA_CHN_5			(5<<DMA_X_CHANNEL)
#define DMA_CHN_6			(6<<DMA_X_CHANNEL)
#define DMA_CHN_7			(7<<DMA_X_CHANNEL)

#define DMA_CT_				19
#define DMA_MEMORY_0		(~(DMA_MEMORY_1))
#define DMA_MEMORY_1 		(1<<DMA_CT_)

#define DMA_DBM				18U
#define DMA_DBM_EN			(1<<DMA_DBM)

#define DMA_PRIORTITY_L		16U
#define DMA_LOW_PRIORITY	(0<<DMA_PRIORTITY_L)
#define DMA_MED_PRIORITY	(1<<DMA_PRIORTITY_L)
#define DMA_HIGH_PRIORITY	(2<<DMA_PRIORTITY_L)
#define DMA_VHIGH_PRIORITY	(3<<DMA_PRIORTITY_L)

#define DMA_MSIZE			13U
#define DMA_MSIZE_8BITS		(0<<DMA_MSIZE)
#define DMA_MSIZE_16BITS	(1<<DMA_MSIZE)
#define DMA_MSIZE_32BITS	(2<<DMA_MSIZE)

#define DMA_PSIZE			11U
#define DMA_PSIZE_8BITS		(0<<DMA_PSIZE)
#define DMA_PSIZE_16BITS	(1<<DMA_PSIZE)
#define DMA_PSIZE_32BITS	(2<<DMA_PSIZE)

#define DMA_MINC			10U
#define DMA_MINC_EN			(1<<DMA_MINC)

#define DMA_PINC			9U
#define DMA_PINC_EN			(1<<DMA_PINC)

#define DMA_CIRC			8U
#define DMA_CIRC_EN			(1<<DMA_CIRC)

#define DMA_DIR				6U
#define DMA_P2M				(0<<DMA_DIR)
#define DMA_M2P				(1<<DMA_DIR)
#define DMA_M2M				(2<<DMA_DIR)

#define DMA_TCIE			4U
#define DMA_TCIE_EN			(1<<DMA_TCIE)

#define DMA_HCIE			3U
#define DMA_HCIE_EN			(1<<DMA_HCIE)

#define D_EN					0U
#define DMA_EN				(1<<D_EN)
#define DMA_DIS				(~(1<<D_EN))
/*------------DMA_LIFCR------------*/
#define CFEIF0				0U
#define CFEIF1				6U
#define CFEIF2				16U
#define CFEIF3				22U

#define CDMEIF0				2U
#define CDMEIF1				8U
#define CDMEIF2				18U
#define CDMEIF3				24U

#define CTEIF0				3U
#define CTEIF1				9U
#define CTEIF2				19U
#define CTEIF3				25U

#define CHTIF0				4U
#define CHTIF1				10U
#define CHTIF2				20U
#define CHTIF3				26U

#define CTCIF0				5U
#define CTCIF1				11U
#define CTCIF2				21U
#define CTCIF3				27U
/*------------DMA_HICR------------*/
#define CFEIF4				0U
#define CFEIF5				6U
#define CFEIF6				16U
#define CFEIF7				22U

#define CDMEIF4				2U
#define CDMEIF5				8U
#define CDMEIF6				18U
#define CDMEIF7				24U

#define CTEIF4				3U
#define CTEIF5				9U
#define CTEIF6				19U
#define CTEIF7				25U

#define CHTIF4				4U
#define CHTIF5				10U
#define CHTIF6				20U
#define CHTIF7				26U

#define CTCIF4				5U
#define CTCIF5				11U
#define CTCIF6				21U
#define CTCIF7				27U
/*--------Interrupts------*/
/*------------DMA_LISR------------*/
#define FEIF0				0U
#define FEIF1				6U
#define FEIF2				16U
#define FEIF3				22U

#define DMEIF0				2U
#define DMEIF1				8U
#define DMEIF2				18U
#define DMEIF3				24U

#define TEIF0				3U
#define TEIF1				9U
#define TEIF2				19U
#define TEIF3				25U

#define HTIF0				4U
#define HTIF1				10U
#define HTIF2				20U
#define HTIF3				26U

#define TCIF0				5U
#define TCIF1				11U
#define TCIF2				21U
#define TCIF3				27U
/*------------DMA_HISR------------*/
#define FEIF4				0U
#define FEIF5				6U
#define FEIF6				16U
#define FEIF7				22U

#define DMEIF4				2U
#define DMEIF5				8U
#define DMEIF6				18U
#define DMEIF7				24U

#define TEIF4				3U
#define TEIF5				9U
#define TEIF6				19U
#define TEIF7				25U

#define HTIF4				4U
#define HTIF5				10U
#define HTIF6				20U
#define HTIF7				26U

#define TCIF4				5U
#define TCIF5				11U
#define TCIF6				21U
#define TCIF7				27U
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef void (*Callback)(void);

typedef struct
{
  __IO uint32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

typedef struct
{
  __IO uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
  __IO uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
  __IO uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  __IO uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_TypeDef;

typedef enum
{
	DMA_INCORRECT_REF,
	DMA_NULLPTR,
	DMA_CONFIG_ERR,
	DMA_E_OK,
}DMA_Errors;

typedef struct
{
	__IO DMA_TypeDef *DMAinst;
	__IO DMA_Stream_TypeDef *Instance;
	uint8_t Channel;
	uint8_t StreamXIntEnabled;
	uint8_t DMA_StreamXMode;
	uint8_t MemoryIncrementSize;
	uint8_t PeripheralIncrementSize;
	uint8_t StreamXPriority;
	uint8_t MemoryIncEnable;
	uint8_t PeripheralIncEnable;
	Callback Func;
	uint8_t DataDir;
	uint8_t Mburst;
	uint8_t Pburst;
	uint16_t NDTRG;
	uint32_t MAR;
	uint32_t PAR;
}DMA_StreamX_t;
/*----------Defined Data types END-------------------*/

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
DMA_Errors MCAL_DmaStreamX_Init(DMA_StreamX_t *DmaSX);
void       MCAL_DmaCallBackSet(__IO DMA_TypeDef *DMAinst,__IO DMA_Stream_TypeDef *Instance,Callback Func);
/*-----------Software Interfaces End--------------*/


#endif /* HAL_MCAL_DMA_H_ */
