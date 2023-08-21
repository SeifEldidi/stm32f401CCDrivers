/*
 * Ecual_Lcd.h
 *
 *  Created on: Aug 6, 2023
 *      Author: Seif pc
 */

#ifndef ECUAL_ECUAL_LCD_H_
#define ECUAL_ECUAL_LCD_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
#include "../std_macros.h"
#include "../HAL/Hal_GPIO.h"
#include "../HAL/Hal_Timer.h"
#include "Ecual_lcd_cfg.h"
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define ECUAL_LCD_DATA					0x00U
#define ECUAL_LCD_INSTR					0x01U

#define LCD_CLEAR_DISPLAY 					0x01U
#define LCD_RETURN_HOME 					0x02U
#define LCD_INCREMENT_SHIFT_LEFT  			0x07U
#define LCD_DECREMENT_SHIFT_RIGHT 			0x05U
#define LCD_INCREMENT_SHIFT_OFF   			0x06U
#define LCD_DECREMENT_SHIFT_OFF   			0x04U
#define LCD_DISPLAY_OFF                    	0x08U
#define LCD_DISPLAY_ON_CURSOR_OFF          	0x0CU
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_OFF 	0x0EU
#define LCD_DISPLAY_ON_CURSOR_ON_BLINK_ON  	0x0FU
#define LCD_CURSOR_MOVE_RIGHT    			0x14U
#define LCD_CURSOR_MOVE_LEFT     			0x10U
#define LCD_SHIFT_LEFT            			0x18U
#define LCD_SHIFT_RIGHT           			0x1CU
#define LCD_4BIT_ONE_LINE         			0x20U
#define LCD_4BIT_TWO_LINE         			0x28U
#define LCD_8BIT_ONE_LINE         			0x30U
#define LCD_8BIT_TWO_LINE        			0x38U
#define LCD_SET_CGRAM_ADD         			0x40U
#define LCD_SET_DDRAM_ADD         			0x80U
#define LCD_4BIT_PINS 						4U
#define LCD_8BIT_PINS 						8U

#define ROW1_ADDRESS 						0x00U
#define ROW2_ADDRESS 						0x40U
#define ROW3_ADDRESS 						0x14U
#define ROW4_ADDRESS 						0x54U
/*------------Macro Declarations End-----------*/
/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
#if LCD_4BIT_MODE == EN
typedef struct
{
	GPIO_Typedef * Instance;
	uint16_t R_W_Pin;
	uint16_t E_Pin;
	uint16_t R_S_Pin;
	uint16_t Data_Pins[4];
}LCD_4Bit_t;
#endif
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
#if LCD_4BIT_MODE == EN
ERR xEcualLcd_Init(LCD_4Bit_t * LCD);
void vEcualLCD_Send(LCD_4Bit_t * LCD ,uint8_t Data ,uint8_t flag);
void vEcualLCD_SendStr(LCD_4Bit_t * LCD ,uint8_t *Data );
void vEcualLCD_SendNum(LCD_4Bit_t * LCD ,uint32_t Data );
void vEcualLCD_SendSpecialChar(LCD_4Bit_t * LCD ,uint8_t *Data );
void vEcualLcd_SetCursor(LCD_4Bit_t * LCD , uint8_t x ,uint8_t y);
#endif
/*-----------Software Interfaces End--------------*/


#endif /* ECUAL_ECUAL_LCD_H_ */
