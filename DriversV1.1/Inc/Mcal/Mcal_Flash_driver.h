/*
 * Mcal_Flash_driver.h
 *
 *  Created on: Aug 8, 2023
 *      Author: Seif pc
 */

#ifndef MCAL_MCAL_FLASH_DRIVER_H_
#define MCAL_MCAL_FLASH_DRIVER_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define FLASH_OFFSET		0x00003C00
#define FLASH				((__IO FLASH_TypeDef *)(AHB1_PERIPHBASE+FLASH_OFFSET))

#define FLASH_BASE            0x08000000UL /*!< FLASH(up to 1 MB) base address in the alias region*/
#define FLASH_END             0x0803FFFFUL /*!< FLASH end address     */
#define FLASH_OTP_BASE        0x1FFF7800UL /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area                */
#define FLASH_OTP_END         0x1FFF7A0FUL /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area                 */
#define FLASH_INT_EN		  DIS

#define FLASH_BYTE			 0x00U
#define FLASH_HALF_WORD		 0x01U
#define FLASH_WORD			 0x02U
#define FLASH_DWORD			 0x03U

#define VOLTAGE_MAX_2_1V		0U
#define VOLTAGE_MAX_2_4V		1U
#define VOLTAGE_MAX_2_7V		2U
#define VOLTAGE_MAX_3_6V		3U
#define VOLTAGE_MAX_3_6V_VPP	4U

#define FLASH_SER			0x00
#define FLASH_MER			0x01

#define FLASH_CR_KEY1		0x45670123
#define FLASH_CR_KEY2		0xCDEF89AB

#define FLASH_OPTR_KEY1		0x08192A3B
#define FLASH_OPTR_KEY2		0x4C5D6E7F

#define FLASH__S0			(0)
#define FLASH__S1			(1)
#define FLASH__S2			(2)
#define FLASH__S3			(3)
#define FLASH__S4			(4)
#define FLASH__S5			(5)
#define FLASH__S6			(6)
#define FLASH__S7			(7)
/*--------FLASH_ACR----------*/
#define DCRST				12U
#define FLASH_ACR_DCRST		(1<<DCRST)

#define ICRST				11U
#define FLASH_ACR_ISCRST	(1<<ICRST)

#define DCEN				10U
#define FLASH_ACR_DCEN		(1<<DCEN)

#define ICEN				9U
#define FLASH_ACR_ICEN		(1<<ICEN)

#define PRFTEN				8U
#define FLASH_ACR_PREFREN	(1<<PREFTEN)

#define LATENCY				0U
#define FLASH_ACR_LAT0WS	(0b0000<<LATENCY)
#define FLASH_ACR_LAT1WS	(0b0001<<LATENCY)
#define FLASH_ACR_LAT2WS	(0b0010<<LATENCY)
#define FLASH_ACR_LAT3WS	(0b0011<<LATENCY)
#define FLASH_ACR_LAT4WS	(0b0100<<LATENCY)
#define FLASH_ACR_LAT5WS	(0b0101<<LATENCY)
#define FLASH_ACR_LAT6WS	(0b0110<<LATENCY)
#define FLASH_ACR_LAT7WS	(0b0111<<LATENCY)
#define FLASH_ACR_LAT8WS	(0b1000<<LATENCY)
#define FLASH_ACR_LAT9WS	(0b1001<<LATENCY)
#define FLASH_ACR_LAT10WS	(0b1010<<LATENCY)
#define FLASH_ACR_LAT11WS	(0b1011<<LATENCY)
#define FLASH_ACR_LAT12WS	(0b1100<<LATENCY)
#define FLASH_ACR_LAT13WS	(0b1101<<LATENCY)
#define FLASH_ACR_LAT14WS	(0b1110<<LATENCY)
#define FLASH_ACR_LAT15WS	(0b1111<<LATENCY)
/*------FLASH_SR--------*/
#define BSY					16U
#define FLASH_SR_BSY_READ	(1<<BSY)

#define RDERR				8U
#define FLASH_SR_RDERR		(1<<RDERR)

#define PGSERR				7U
#define FLASH_SR_PGSERR		(1<<PGSERR)

#define PGPERR				6U
#define FLASH_SR_PGPERR		(1<<PGPERR)

#define PGAERR				5U
#define FLASH_SR_PGAERR		(1<<PGAERR)

#define WRPERR				4U
#define FLASH_SR_WRPERR		(1<<WRPERR)

#define OPERR				1U
#define FLASH_SR_OPERR		(1<<OPERR)

#define EOP					0U
#define FLASH_SR_EOP		(1<<EOP)
/*-----------FLASH_CR------------*/
#define LOCK				31U
#define FLASH_CR_LOCK		(1<<LOCK)

#define ERRIE				25U
#define FLASH_CR_ERRIEN		(1<<ERRIE)

#define EOPIE				24U
#define FLASH_CR_EOPIEN		(1<<EOPIE)

#define STRT				16U
#define FALSH_CR_STRT		(1<<STRT)

#define PSIZE				8U
#define FLASH_CR_8BYTES		(0b11<<PSIZE)
#define FLASH_CR_4BYTES		(0b10<<PSIZE)
#define FLASH_CR_2BYTES		(0b01<<PSIZE)
#define FLASH_CR_1BYTE		(0b00<<PSIZE)

#define SNB					3U
#define FLASH_CR_S0			(0<<SNB)
#define FLASH_CR_S1			(1<<SNB)
#define FLASH_CR_S2			(2<<SNB)
#define FLASH_CR_S3			(3<<SNB)
#define FLASH_CR_S4			(4<<SNB)
#define FLASH_CR_S5			(5<<SNB)
#define FLASH_CR_S6			(6<<SNB)
#define FLASH_CR_S7			(7<<SNB)

#define MER					2U
#define FLASH_CR_MER		(1<<MER)

#define SER					1U
#define FLASH_CR_SER		(1<<SER)

#define PG_FLASH			0U
#define FLASH_CR_PG			(1<<PG_FLASH)
/*-----------FLASH_OPTCR------------*/
#define SPRMOD				  31U
#define FLASH_OPTCR_PCROP_EN  (1<<SPRMOD)

#define NWRP				  	 16U
#define FLASH_OPTCR_WRDIS_S0  	 (0b000001<<NWRP)
#define FLASH_OPTCR_WRDIS_S1  	 (0b000010<<NWRP)
#define FLASH_OPTCR_WRDIS_S2  	 (0b000100<<NWRP)
#define FLASH_OPTCR_WRDIS_S3  	 (0b001000<<NWRP)
#define FLASH_OPTCR_WRDIS_S4  	 (0b010000<<NWRP)
#define FLASH_OPTCR_WRDIS_S5  	 (0b100000<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S0  (0b000001<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S1  (0b000010<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S2  (0b000100<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S3  (0b001000<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S4  (0b010000<<NWRP)
#define FLASH_OPTCR_PCR_WREN_S5  (0b100000<<NWRP)

#define RDP						 8U
#define FALSH_OPTCR_RDP_LEVEL_0	 (0xAA<<RDP)
#define FALSH_OPTCR_RDP_LEVEL_2	 (0xCC<<RDP)
#define FALSH_OPTCR_RDP_LEVEL_1	 (0xBB<<RDP)

#define USER					 5U
#define FLASH_OPTCR_STDBY		 (0b100<<USER)
#define FLASH_OPTCR_STOP		 (0b010<<USER)
#define FLASH_OPTCR_SW			 (0b001<<USER)

#define BOR_LEV					  2U
#define FLASH_OPTCR_BOR_LEV_OFF	  (0b10<<BOR_LEV)
#define FLASH_OPTCR_BOR_LEV_1	  (0b10<<BOR_LEV)
#define FLASH_OPTCR_BOR_LEV_2	  (0b01<<BOR_LEV)
#define FLASH_OPTCR_BOR_LEV_3	  (0b00<<BOR_LEV)

#define OPTSRT					1U
#define FLASH_OPTCR_OPSTRT		(1<<OPTSRT)

#define OPTLOCK					0U
#define FLASH_OPTCR_OPTLOCK		(1<<OPTLOCK)

/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
#define IS_FLASH_ADD(ADD)	  ((ADD>=FLASH_BASE &&ADD<=FLASH_END)||(ADD>=FLASH_OTP_BASE && ADD<=FLASH_OTP_END))
#define FLASH_WRITE_SIZE(TYPE) (TYPE == FLASH_BYTE || TYPE == FLASH_HALF_WORD || TYPE == FLASH_WORD || TYPE == FLASH_DWORD)
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef struct
{
  __IO uint32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
  __IO uint32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
  __IO uint32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
  __IO uint32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
  __IO uint32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
  __IO uint32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
  __IO uint32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
} FLASH_TypeDef;

typedef struct
{
	uint32_t TypeProgram;
	uint32_t Address;
	uint64_t Data;
}FLASH_Program;

typedef struct
{
	uint32_t Address;
	uint32_t  *Data;
	uint32_t Length;
	uint8_t  SectorNumber;
	uint8_t  No_Sectors;
}FLASH_Buffer;

typedef struct
{
	uint8_t  Voltage_Level;
	uint16_t SectorNumber;
	uint16_t Mass_Sector_Erase;
	uint8_t  No_Sectors;
}FLASH_Erase;

typedef enum
{
	FLASH_POINTER_ERR,
	FLASH_READ_PROTECTION_ERR,
	FLASH_PROG_SEQ_ERR,
	FLASH_PROG_PARELLISM_ERR,
	FLASH_PROG_ALLIGNMENT_ERR,
	FLASH_WRITE_PROTECTION_ERR,
	FLASH_SUCCESS,
	FLASH_ERR,
}FLASH_ERRORS;
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
ERR FLASH_UNLOCK();
ERR FLASH_LOCK();
ERR FLASH_OB_UNLOCK();
ERR FLASH_OB_LOCK();
FLASH_ERRORS MCAL_FLASH_Program(FLASH_Program *Flash_Write);
FLASH_ERRORS MCAL_FLASH_Hex(FLASH_Buffer  *Flash_Write_Buff);
FLASH_ERRORS MCAL_FLASH_Erase(FLASH_Erase  *Flash_Erase);
/*-----------Software Interfaces End--------------*/

#endif /* MCAL_MCAL_FLASH_DRIVER_H_ */
