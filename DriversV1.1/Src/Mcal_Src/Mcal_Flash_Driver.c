/*
 * Mcal_Flash_Driver.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Seif pc
 */

#include "../../Inc/Mcal/Mcal_Flash_driver.h"

static ERR Flash_SER(uint8_t Type,uint8_t Sector,uint8_t NoSectors);

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
		*(__IO uint32_t *)(Add) = Data;
	}
}

static void FLASH_WRITE_HALF_WORD(uint32_t Add ,uint16_t Data)
{
	/*------Check if Current Address is in the Flash Memory----*/
	if (IS_FLASH_ADD(Add)) {
		/*--------CLear Flash PSIZE-----*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSIZE to 2BYTE----*/
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
		/*------Set PSIZE to 4BYTE----*/
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
		/*------Set PSIZE to 8BYTE----*/
		FLASH->CR |= FLASH_CR_8BYTES;
		/*-------Enable Flash Programming----*/
		FLASH->CR |= FLASH_CR_PG;
		/*--------Write Data to Flash-----*/
		*(__IO uint32_t*) Add = Data;
		__ISB();
		*(__IO uint32_t*) (Add + 4) = Data;
	}
}

ERR MCAL_FLASH_Program(FLASH_Program *Flash_Write)
{
	ERR Err_State = HAL_OK;
	if(Flash_Write != NULL)
	{
	/*-------Check if a valid TypeProgram is Sent----*/
		if( FLASH_WRITE_SIZE(Flash_Write->TypeProgram) == 1)
		{
			/*----Unlock Flash Memory----*/
			Err_State = FLASH_UNLOCK();
			if(Err_State == HAL_OK)
			{
				/*-----Wait for Busy Flag------*/
				Err_State = FLASH_WAITBUSY();
				/*------Write Data to Memory---*/
				if( Flash_Write->TypeProgram == FLASH_BYTE )
				{
					FLASH_WRITE_BYTE(Flash_Write->Address,(uint8_t)Flash_Write->Data);
				}else if(Flash_Write->TypeProgram == FLASH_HALF_WORD)
				{
					FLASH_WRITE_HALF_WORD(Flash_Write->Address,(uint16_t)Flash_Write->Data);
				}else if(Flash_Write->TypeProgram == FLASH_WORD)
				{
					FLASH_WRITE_WORD(Flash_Write->Address,(uint32_t)Flash_Write->Data);
				}else if(Flash_Write->TypeProgram == FLASH_DWORD)
				{
					FLASH_WRITE_DWORD(Flash_Write->Address,Flash_Write->Data);
				}
				/*------Wait For busy Flag----*/
				Err_State = FLASH_WAITBUSY();
				/*--------Disable Programming---*/
				FLASH->CR &= (~FLASH_CR_PG);
			}
		}else{
			Err_State = HAL_ERR;
		}
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

ERR MCAL_FLASH_Hex(FLASH_Buffer  *Flash_Write_Buff)
{
	ERR Err_State = HAL_OK;
	if (Flash_Write_Buff != NULL) {
		uint8_t ByteX = 0;
		/*----Unlock Flash Memory----*/
		Err_State = FLASH_UNLOCK();
		/*----Erase Sector to be Written To---*/
		Flash_SER(FLASH_WORD,Flash_Write_Buff->SectorNumber,Flash_Write_Buff->No_Sectors);
		/*------Write Data to Memory---*/
		for(ByteX =0 ; ByteX<=Flash_Write_Buff->Length-1;ByteX++)
		{
			FLASH_WRITE_WORD(Flash_Write_Buff->Address +4*ByteX ,*(Flash_Write_Buff->Data + ByteX));
			/*------Wait For busy Flag----*/
			Err_State = FLASH_WAITBUSY();
			/*--------Disable Programming---*/
			FLASH->CR &= (~FLASH_CR_PG);
		}
	} else {
		Err_State = HAL_ERR;
	}
	return Err_State;
}

static ERR Flash_MER(uint8_t Type)
{
	ERR Err_State = HAL_OK;
	/*----Unlock Flash Memory----*/
	if(Err_State == HAL_OK)
	{
		/*-----Wait for Busy Flag------*/
		Err_State = FLASH_WAITBUSY();
		/*------Clear PSize---*/
		FLASH->CR &= ~FLASH_CR_8BYTES;
		/*------Set PSize----*/
		if (Type == FLASH_BYTE) {
			FLASH->CR |= FLASH_CR_1BYTE;
		} else if (Type == FLASH_HALF_WORD) {
			FLASH->CR |= FLASH_CR_2BYTES;
		} else if (Type == FLASH_WORD) {
			FLASH->CR |= FLASH_CR_4BYTES;
		} else if (Type == FLASH_DWORD) {
			FLASH->CR |= FLASH_CR_8BYTES;
		}
		/*------Set Mass Erase----*/
		FLASH->CR |= FLASH_CR_MER;
		/*------Start Mass Erase---*/
		FLASH->CR |= FALSH_CR_STRT;
		/*------Wait Untill Mass Erase is Complete---*/
		Err_State = FLASH_WAITBUSY();

		CLEAR_BIT(FLASH->CR ,MER);
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

static ERR Flash_SER(uint8_t Type,uint8_t Sector,uint8_t NoSectors)
{
	ERR Err_State = HAL_OK;
	if(Sector <=FLASH__S7 && Sector>= FLASH__S0)
	{
		if(Sector <=FLASH__S7 -(NoSectors-1) )
		{
			uint8_t SectorsErased = 0;
			/*-----Wait for Busy Flag------*/
			Err_State = FLASH_WAITBUSY();
			/*----Erase Sectors----*/
			for(SectorsErased = Sector;SectorsErased<=Sector+NoSectors-1;SectorsErased++)
			{
				CLEAR_MSK(FLASH->CR,FLASH_CR_8BYTES);
				/*------Set PSize----*/
				if (Type == FLASH_BYTE) {
					FLASH->CR |= FLASH_CR_1BYTE;
				} else if (Type == FLASH_HALF_WORD) {
					FLASH->CR |= FLASH_CR_2BYTES;
				} else if (Type == FLASH_WORD) {
					FLASH->CR |= FLASH_CR_4BYTES;
				} else if (Type == FLASH_DWORD) {
					FLASH->CR |= FLASH_CR_8BYTES;
				}
				/*------Set Sector Erase----*/
				FLASH->CR |= FLASH_CR_SER;
				/*-----Erase A certain Sector---*/
				FLASH->CR |= (SectorsErased << SNB);
				/*------Start Sector Erase---*/
				FLASH->CR |= FALSH_CR_STRT;
				/*------Wait Untill Sector Erase is Complete---*/
				Err_State = FLASH_WAITBUSY();
				/*-----Clear Mask-----*/
				CLEAR_MSK(FLASH->CR ,FLASH_CR_SER|FLASH_CR_S7);
			}
		}else{
			Err_State = HAL_ERR;
		}
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}

ERR MCAL_FLASH_Erase(FLASH_Erase  *Flash_Erase)
{
	ERR Err_State = HAL_OK;
	if(Flash_Erase != NULL)
	{
		if ( FLASH_WRITE_SIZE(Flash_Erase->TypeProgram) == 1) {
			/*----Unlock Flash Memory----*/
			Err_State = FLASH_UNLOCK();
			switch(Flash_Erase->Mass_Sector_Erase)
			{
				case FLASH_MER:
					Flash_MER(Flash_Erase->TypeProgram);
					break;
				case FLASH_SER:
					Flash_SER(Flash_Erase->TypeProgram,Flash_Erase->SectorNumber,Flash_Erase->No_Sectors);
					break;
				default:
					Err_State = HAL_ERR;
			}
		} else {
			Err_State = HAL_ERR;
		}
	}else{
		Err_State = HAL_ERR;
	}
	return Err_State;
}
