/*
 * Mcal_Flash_Driver.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Seif pc
 */

#include "../../Inc/Mcal/Mcal_Flash_driver.h"

__attribute__((always_inline)) static inline void __ISB(void)
{
  __asm volatile ("isb 0xF":::"memory");
}

static ERR FLASH_WAITBUSY()
{
	while(FLASH->SR & FLASH_SR_BSY_READ);

	return HAL_OK;
}

ERR FLASH_UNLOCK()
{
	ERR Err_State = HAL_OK;
	if((READ_BIT(FLASH->CR,LOCK)))
	{
		/*---------Unlocking Sequence-------*/
		FLASH->KEYR = FLASH_CR_KEY1;
		FLASH->KEYR = FLASH_CR_KEY2;
		/*-------Check if flash is now unlocked ---*/
		if((READ_BIT(FLASH->CR,LOCK)))
		{
			Err_State = HAL_ERR;
		}
	}
	return Err_State;
}

ERR FLASH_LOCK()
{
	/*------Lock Flash Memory-----*/
	FLASH->CR |= FLASH_CR_LOCK;

	return HAL_OK;
}

ERR FLASH_OB_UNLOCK()
{
	ERR Err_State = HAL_OK;
	if ((READ_BIT(FLASH->OPTCR, OPTLOCK))) {
		/*---------Unlocking Sequence-------*/
		FLASH->OPTKEYR = FLASH_OPTR_KEY1;
		FLASH->OPTKEYR = FLASH_OPTR_KEY2;
		/*-------Check if flash is now unlocked ---*/
		if ((READ_BIT(FLASH->OPTCR, OPTLOCK)))
		{
			Err_State = HAL_ERR;
		}
	}
	return Err_State;
}

ERR FLASH_OB_LOCK()
{
	/*------Lock Flash Memory OPTCT-----*/
	FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
}

static void FLASH_WRITE_BYTE(uint32_t Add ,uint8_t Data)
{
	/*------Check if Current Address is in the Flash Memory----*/
	if(IS_FLASH_ADD(Add))
	{
		/*--------CLear Flash PSIZE-----*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSIZE to 1BYTE----*/
		FLASH->CR |= FLASH_CR_1BYTE;
		/*-------Enable Flash Programming----*/
		FLASH->CR |= FLASH_CR_PG;
		/*--------Write Data to Flash-----*/
		*(__IO uint32_t *)Add = Data;
	}
}

static void FLASH_WRITE_HALF_WORD(uint32_t Add ,uint16_t Data)
{
	/*------Check if Current Address is in the Flash Memory----*/
	if (IS_FLASH_ADD(Add)) {
		/*--------CLear Flash PSIZE-----*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSIZE to 1BYTE----*/
		FLASH->CR |= FLASH_CR_2BYTES;
		/*-------Enable Flash Programming----*/
		FLASH->CR |= FLASH_CR_PG;
		/*--------Write Data to Flash-----*/
		*(__IO uint32_t*) Add = Data;
	}
}

static void FLASH_WRITE_WORD(uint32_t Add ,uint32_t Data)
{
	/*------Check if Current Address is in the Flash Memory----*/
	if (IS_FLASH_ADD(Add)) {
		/*--------CLear Flash PSIZE-----*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSIZE to 1BYTE----*/
		FLASH->CR |= FLASH_CR_4BYTES;
		/*-------Enable Flash Programming----*/
		FLASH->CR |= FLASH_CR_PG;
		/*--------Write Data to Flash-----*/
		*(__IO uint32_t*) Add = Data;
	}
}

static void FLASH_WRITE_DWORD(uint32_t Add ,uint64_t Data)
{

	/*------Check if Current Address is in the Flash Memory----*/
	if (IS_FLASH_ADD(Add)) {
		/*--------CLear Flash PSIZE-----*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSIZE to 1BYTE----*/
		FLASH->CR |= FLASH_CR_8BYTES;
		/*-------Enable Flash Programming----*/
		FLASH->CR |= FLASH_CR_PG;
		/*--------Write Data to Flash-----*/
		*(__IO uint32_t*) Add = Data;
		__ISB();
		*(__IO uint32_t*) (Add + 4) = Data;
	}
}

ERR MCAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
	ERR Err_State = HAL_OK;
	/*-------Check if a valid TypeProgram is Sent----*/
	if( FLASH_WRITE_SIZE(TypeProgram) == 1)
	{
		/*-----Wait for Busy Flag------*/
		Err_State = FLASH_WAITBUSY();
		/*------Write Data to Memory---*/
		if( TypeProgram == FLASH_BYTE )
		{
			FLASH_WRITE_BYTE(Address,(uint8_t)Data);
		}else if(TypeProgram == FLASH_HALF_WORD)
		{
			FLASH_WRITE_HALF_WORD(Address,(uint16_t)Data);
		}else if(TypeProgram == FLASH_WORD)
		{
			FLASH_WRITE_WORD(Address,(uint32_t)Data);
		}else if(TypeProgram == FLASH_DWORD)
		{
			FLASH_WRITE_DWORD(Address,Data);
		}
		/*------Wait For busy Flag----*/
		Err_State = FLASH_WAITBUSY();
		/*--------Disable Programming---*/
		FLASH->CR &= (~FLASH_CR_PG);
	}else{
		Err_State = HAL_ERR;
	}
	return HAL_ERR;
}
