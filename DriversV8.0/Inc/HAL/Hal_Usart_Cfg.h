/*
 * Hal_Usart_Cfg.h
 *
 *  Created on: Aug 1, 2023
 *      Author: Seif pc
 */

#ifndef HAL_HAL_USART_CFG_H_
#define HAL_HAL_USART_CFG_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define ConfigUSART_ASYNC_Mode    EN
#define ConfigUSART_INT_EN_TX	  EN
#define ConfigUSART_INT_EN_RX	  EN
#define ConfigUSART_DMA_EN	      DIS
#define ConfigUSARTDMA_INT_EN	  DIS


#define ConfigUSART_INT_EN	  (ConfigUSART_INT_EN_TX == EN || ConfigUSART_INT_EN_RX == EN )&& (ConfigUSART_DMA_EN    == DIS)
#define ConfigUSART_DMAA_EN_	  ConfigUSART_DMA_EN    == EN

/*------------Macro Declarations End-----------*/


#endif /* HAL_HAL_USART_CFG_H_ */
