/*
 * Mcal_Flash_Driver.c
 *
 *  Created on: Aug 8, 2023
 *      Author: Seif pc
 */

#include "../../Inc/Mcal/Mcal_Flash_driver.h"

/*----------------Static Functions Declarations--------------*/
static FLASH_ERRORS Flash_SER(uint8_t Voltage_Level,uint8_t Sector,uint8_t NoSectors);
static FLASH_ERRORS FLASH_GetError();
static FLASH_ERRORS FLASH_WAITBUSY();
static void FLASH_WRITE_BYTE(uint32_t Add ,uint8_t Data);
static void FLASH_WRITE_HALF_WORD(uint32_t Add ,uint16_t Data);
static void FLASH_WRITE_WORD(uint32_t Add ,uint32_t Data);
static void FLASH_WRITE_DWORD(uint32_t Add ,uint64_t Data);
static FLASH_ERRORS Flash_MER(uint8_t Voltage_Level);
static FLASH_ERRORS Flash_SER(uint8_t Voltage_Level,uint8_t Sector,uint8_t NoSectors);

__attribute__((always_inline)) static inline void __ISB(void)
{
  __asm volatile ("isb 0xF":::"memory");
}

/*----------------Static Functions Definition----------*/
static FLASH_ERRORS FLASH_GetError()
{
	FLASH_ERRORS  ERR=FLASH_SUCCESS;
	if(READ_BIT(FLASH->SR,RDERR))
	{
		ERR = FLASH_READ_PROTECTION_ERR;
	}else if(READ_BIT(FLASH->SR,PGSERR))
	{
		ERR = FLASH_PROG_SEQ_ERR;
	}else if(READ_BIT(FLASH->SR,PGPERR))
	{
		ERR = FLASH_PROG_PARELLISM_ERR;
	}else if(READ_BIT(FLASH->SR,PGAERR))
	{
		ERR = FLASH_PROG_ALLIGNMENT_ERR;
	}else if(READ_BIT(FLASH->SR,WRPERR))
	{
		ERR = FLASH_WRITE_PROTECTION_ERR;
	}
	return ERR;
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

static FLASH_ERRORS FLASH_WAITBUSY()
{
	FLASH_ERRORS Err = FLASH_SUCCESS;
	while(FLASH->SR & FLASH_SR_BSY_READ)
	{
		Err = FLASH_GetError();
		if(Err != FLASH_SUCCESS)
			break;
	}
	return Err;
}

static FLASH_ERRORS Flash_MER(uint8_t Voltage_Level)
{
	FLASH_ERRORS ERR;
	/*-----Wait for Busy Flag------*/
	ERR = FLASH_WAITBUSY();
	/*------Clear PSize---*/
	FLASH->CR &= ~FLASH_CR_8BYTES;
	/*------Set PSize----*/
	if (Voltage_Level == VOLTAGE_MAX_2_1V) {
		FLASH->CR |= FLASH_CR_1BYTE;
	} else if (Voltage_Level == VOLTAGE_MAX_2_4V
			|| VOLTAGE_MAX_2_7V == Voltage_Level) {
		FLASH->CR |= FLASH_CR_2BYTES;
	} else if (Voltage_Level == VOLTAGE_MAX_3_6V) {
		FLASH->CR |= FLASH_CR_4BYTES;
	} else if (Voltage_Level == VOLTAGE_MAX_3_6V_VPP) {
		FLASH->CR |= FLASH_CR_8BYTES;
	}
	/*------Set Mass Erase----*/
	FLASH->CR |= FLASH_CR_MER;
	/*------Start Mass Erase---*/
	FLASH->CR |= FALSH_CR_STRT;
	/*------Wait Untill Mass Erase is Complete---*/
	ERR = FLASH_WAITBUSY();

	CLEAR_BIT(FLASH->CR, MER);
	return ERR;
}

static FLASH_ERRORS Flash_SER(uint8_t Voltage_Level,uint8_t Sector,uint8_t NoSectors)
{
	FLASH_ERRORS Err_State = FLASH_SUCCESS;
	if(Sector <=FLASH__S7 && Sector>= FLASH__S0)
	{
		if(Sector <=FLASH__S7 -(NoSectors-1) )
		{
			uint8_t SectorsErased = 0;
			/*-----Wait for Busy Flag------*/
			Err_State = FLASH_WAITBUSY();
			if(Err_State == FLASH_SUCCESS)
			{
				/*----Erase Sectors----*/
				for(SectorsErased = Sector;SectorsErased<=Sector+NoSectors-1;SectorsErased++)
				{
					CLEAR_MSK(FLASH->CR,FLASH_CR_8BYTES);
					/*------Set PSize----*/
					if (Voltage_Level == VOLTAGE_MAX_2_1V) {
						FLASH->CR |= FLASH_CR_1BYTE;
					} else if (Voltage_Level == VOLTAGE_MAX_2_4V
							|| VOLTAGE_MAX_2_7V == Voltage_Level) {
						FLASH->CR |= FLASH_CR_2BYTES;
					} else if (Voltage_Level == VOLTAGE_MAX_3_6V) {
						FLASH->CR |= FLASH_CR_4BYTES;
					} else if (Voltage_Level == VOLTAGE_MAX_3_6V_VPP) {
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
					if(Err_State != FLASH_SUCCESS)
						return Err_State;
					/*-----Clear Mask-----*/
					CLEAR_MSK(FLASH->CR ,FLASH_CR_SER|FLASH_CR_S7);
				}
			}
		}else{
			Err_State = FLASH_ERR;
		}
	}else{
		Err_State = FLASH_ERR;
	}
	return Err_State;
}

/*-----------------------------Global Functions Declarations--------------*/

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
	ERR Err_State = HAL_OK;
	/*------Lock Flash Memory OPTCT-----*/
	FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
	return Err_State;
}


FLASH_ERRORS MCAL_FLASH_Program(FLASH_Program *Flash_Write)
{
	ERR Err_State = HAL_OK;
	FLASH_ERRORS Err = FLASH_SUCCESS;
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
				Err = FLASH_WAITBUSY();
				if(Err == FLASH_SUCCESS)
				{
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
			}
		}else{
			Err = FLASH_ERR;
		}
	}else{
		Err = FLASH_POINTER_ERR;
	}
	return Err;
}

FLASH_ERRORS MCAL_FLASH_Hex(FLASH_Buffer  *Flash_Write_Buff)
{
	ERR Err_State = HAL_OK;
	FLASH_ERRORS  Err = FLASH_SUCCESS;
	if (Flash_Write_Buff != NULL) {
		uint8_t ByteX = 0;
		/*----Unlock Flash Memory----*/
		Err_State = FLASH_UNLOCK();
		if(Err_State == HAL_OK)
		{
			/*----Erase Sector to be Written To---*/
			Err = Flash_SER(FLASH_WORD,Flash_Write_Buff->SectorNumber,Flash_Write_Buff->No_Sectors);
			if(Err == FLASH_SUCCESS)
			{
				/*------Write Data to Memory---*/
				for(ByteX =0 ; ByteX<=Flash_Write_Buff->Length-1;ByteX++)
				{
					FLASH_WRITE_WORD(Flash_Write_Buff->Address +4*ByteX ,*(Flash_Write_Buff->Data + ByteX));
					/*------Wait For busy Flag----*/
					Err = FLASH_WAITBUSY();
					if(Err != FLASH_SUCCESS)
						break;
					/*--------Disable Programming---*/
					FLASH->CR &= (~FLASH_CR_PG);
				}
			}
		}else{
			Err = FLASH_ERR;
		}
	} else {
		Err = FLASH_POINTER_ERR;
	}
	return Err;
}

FLASH_ERRORS MCAL_FLASH_Erase(FLASH_Erase  *Flash_Erase)
{
	ERR Err_State = HAL_OK;
	FLASH_ERRORS  Err = FLASH_SUCCESS;
	if(Flash_Erase != NULL)
	{
		/*----Unlock Flash Memory----*/
		Err_State = FLASH_UNLOCK();
		if (Err_State == HAL_OK) {
			switch (Flash_Erase->Mass_Sector_Erase) {
				case FLASH_MER:
					Err = Flash_MER(Flash_Erase->Voltage_Level);
					break;
				case FLASH_SER:
					Err = Flash_SER(Flash_Erase->Voltage_Level,
							Flash_Erase->SectorNumber, Flash_Erase->No_Sectors);
					break;
				default:
					Err = FLASH_ERR;
			}
		} else {
			Err = FLASH_ERR;
		}
	}else{
		Err = FLASH_POINTER_ERR;
	}
	return Err;
}

#if FLASH_INT_EN == EN
void FLASH_IRQHandler()
{

}
#endif
