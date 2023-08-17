/*
 * Mcal_I2C.h
 *
 *  Created on: Aug 14, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_I2C_H_
#define MCAL_MCAL_I2C_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
#include "../HAL/Hal_RCC.h"
#include "../HAL/Hal_GPIO.h"
#include "../core/CortexM4_core_NVIC.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
/*-----Register Definition------*/
#define APB1_PERIPHBASE			0x40000000U
#define I2C1_OFFSET				0x00005400U
#define I2C2_OFFSET				0x00005800U
#define I2C3_OFFSET				0x00005C00U
#define I2C1					((__IO I2C_TypeDef*)(APB1_PERIPHBASE + I2C1_OFFSET))
#define I2C2					((__IO I2C_TypeDef*)(APB1_PERIPHBASE + I2C2_OFFSET))
#define I2C3					((__IO I2C_TypeDef*)(APB1_PERIPHBASE + I2C3_OFFSET))
#define I2C_PRESCALER			1000000UL
#define I2C_STANDARD_MODE		100000UL
#define I2C_FAST_MODE			400000UL
#define TRISE_SM				1000
#define TRISE_FM				300
#define I2C_WRITE				0x00
#define I2C_READ				0x01

/*------User Selection------*/
#define I2C_MSTR_MODE		0U
#define I2C_SLAVE_MODE		1U

#define I2C_SLAVE_ADDR_7BIT	 0U
#define I2C_SLAVE_ADDR_10BIT 1U

#define I2C_SM_MODE			0U
#define I2C_FM_MODE			1U

#define I2C_FM_DUTY_2		0U
#define I2C_FM_DUTY_16_9	1U
/*------I2C_OAR1-------*/
#define ADDMODE 		15U
#define ADD_10BIT		(1<<ADDMODE)
#define ADD_7BIT		(~(ADD_10BIT))
/*-----I2C_CR1----------*/
#define SWRST			15U
#define SWRST_RST		(1<<SWRST)

#define ACK				10U
#define ACK_EN			(1<<ACK)
#define ACK_DIS			(~(ACK_EN))

#define STOP			9U
#define STOP_EN			(1<<STOP)

#define START			8U
#define START_EN		(1<<START)

#define STRETCH			7U
#define STRETCH_DIS		(1<<STRETCH)

#define I2CPE_EN		0U
#define I2C_EN			(1<<I2CPE_EN)
/*------I2C_SR1--------*/
#define OVR				11U
#define OVR_SET			(1<<OVR)

#define AF				10U
#define AF_SET			(1<<AF)

#define ARLO			9U
#define I2C_ARLO		(1<<ARLO)

#define TxE				7U
#define TxE_SET			(1<<TxE)

#define RxNE			6U
#define RxNE_SET		(1<<RxNE)

#define STOPF			4U
#define I2C_STOPF		(1<<STOPF)

#define ADD10			3U
#define ADD10_SET		(1<<ADD10)

#define BTF				2U
#define BTF_SET			(1<<BTF)

#define ADDR			1U
#define I2C_ADDR		(1<<ADDR)

#define SB				0U
#define I2C_SB			(1<<SB)
/*------I2C_SR2--------*/
#define BUSY			1U
#define I2C_BUS_BUSY	(1<<BUSY)
/*------I2C_CCR--------*/
#define F_S				15U
#define SM_MODE			(~(FM_MODE))
#define FM_MODE			(1<<F_S)

#define DUTY			14U
#define DUTY_2			(~(DUTY_16_9))
#define DUTY_16_9		(1<<DUTY)

/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef struct
{
  __IO uint32_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
  __IO uint32_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
  __IO uint32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
  __IO uint32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
  __IO uint32_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
  __IO uint32_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
  __IO uint32_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
  __IO uint32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
  __IO uint32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
  __IO uint32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} I2C_TypeDef;

typedef struct
{
	I2C_TypeDef *Instance;
	uint8_t SL_MSTR_MODE;
	uint8_t SL_SIZE;
	uint8_t SM_FM_SEL;
	uint8_t FM_DUTY_SEL;
	uint16_t I2C_SLAVE_ADD;
}I2C_cfg_t;

/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
ERR  xMCAL_I2C_Init(I2C_cfg_t * I2C);
ERR  xMCAL_I2C_Finder(I2C_TypeDef * I2C ,uint8_t *Data);
ERR  xMCAL_I2C_MSTRSend(I2C_TypeDef * I2C , uint8_t SA,uint8_t Reg_Addr,uint8_t *Data,uint8_t Data_Len);
ERR  xMCAL_I2C_MSTRRec (I2C_TypeDef * I2C , uint8_t SA,uint8_t Reg_Addr,uint8_t *Data,uint8_t Data_Len);
/*-----------Software Interfaces End--------------*/


#endif /* MCAL_MCAL_I2C_H_ */
