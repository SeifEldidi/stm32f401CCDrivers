/*
 * std_macros.h
 *
 *  Created on: Jul 24, 2023
 *      Author: Seif pc
 */

#ifndef STD_MACROS_H_
#define STD_MACROS_H_

/*-------------Includes Section Start---------------*/
#include <stdint.h>
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/
#define BIT_MASK (uint32_t)0x00000001U
#define __IO volatile
#define EN  0x01
#define DIS 0x00
#ifndef NULL
#define NULL ((void *)0)
#endif
/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/
#define SET_BIT(REG,BIT_POS)     (REG|=(1<<BIT_POS))
#define READ_BIT(REG,BIT_POS)    (((REG>>BIT_POS)&BIT_MASK))
#define TOGGLE_BIT(REG,BIT_POS)  (REG^=(1<<BIT_POS))
#define CLEAR_BIT(REG,BIT_POS)	 ((REG&=~(1<<BIT_POS)))
/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/
typedef float float32_t;

typedef enum
{
	HAL_OK,
	HAL_ERR,
}ERR;
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/

/*-----------Software Interfaces End--------------*/


#endif /* STD_MACROS_H_ */
