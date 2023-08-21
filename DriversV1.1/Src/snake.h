/*
 * snake.h
 *
 *  Created on: Aug 7, 2023
 *      Author: Seif pc
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#define MAX_SNAKE_LEN	 15U
#define SNAKE_DIR_RIGHT  5U
#define SNAKE_DIR_LEFT	 6U
#define SNAKE_DIR_UP     7U
#define SNAKE_DIR_DOWN   8U

#define GAME_Restart    9U

#define LCD_COLS		16U
#define LCD_ROWS		2U

#define GAME_OVER       1U
#define SNAKE_ATE_FOOD	1U

typedef struct
{
	int8_t x;
	int8_t y;
}SnakePoint;

SnakePoint Snake[MAX_SNAKE_LEN]= {'-'};

#endif /* SNAKE_H_ */
