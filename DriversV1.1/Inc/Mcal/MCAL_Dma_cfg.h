/*
 * MCAL_Dma_cfg.h
 *
 *  Created on: Aug 26, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_DMA_CFG_H_
#define MCAL_MCAL_DMA_CFG_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
#include "../HAL/Hal_RCC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define DMA1_STREAM0_INT_EN			DIS
#define DMA1_STREAM1_INT_EN			DIS
#define DMA1_STREAM2_INT_EN			DIS
#define DMA1_STREAM3_INT_EN			DIS
#define DMA1_STREAM4_INT_EN			DIS
#define DMA1_STREAM5_INT_EN			DIS
#define DMA1_STREAM6_INT_EN			DIS
#define DMA1_STREAM7_INT_EN			DIS

#define DMA2_STREAM0_INT_EN			DIS
#define DMA2_STREAM1_INT_EN			DIS
#define DMA2_STREAM2_INT_EN			DIS
#define DMA2_STREAM3_INT_EN			DIS
#define DMA2_STREAM4_INT_EN			DIS
#define DMA2_STREAM5_INT_EN			EN
#define DMA2_STREAM6_INT_EN			DIS
#define DMA2_STREAM7_INT_EN			EN
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/

/*----------Defined Data types END-------------------*/

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/

/*-----------Software Interfaces End--------------*/

#endif /* MCAL_MCAL_DMA_CFG_H_ */
