/*
 * HAL_Dma.h
 *
 *  Created on: Jul 30, 2023
 *      Author: Seif pc
 */


#ifndef HAL_HAL_DMA_H_
#define HAL_HAL_DMA_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
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

#define D_EN					0U
#define DMA_EN				(1<<D_EN)
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
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
/*----------Defined Data types END-------------------*/

/*-----Multiple Channel At the Same Time using DMA----*/

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/



/*-----------Software Interfaces End--------------*/


#endif /* HAL_HAL_DMA_H_ */
