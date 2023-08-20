/*
 * snake_game.c
 *
 *  Created on: Aug 19, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "snake_game.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"

void GameSnake_Start(void){
	uint8_t headX = 0;
	uint8_t headY = 0;

	uint8_t tailX = 0;
	uint8_t tailY = 0;

	uint8_t pathX = 0;
	uint8_t pathY = 0;

	uint8_t directionFlag = GAMESNAKE_TURNRIGHT;


	for(uint8_t snakeLength = 0; snakeLength < 4; snakeLength++){
		GameSnake_UpdatePositionHead(&directionFlag, &headX, &headY);
		GameSnake_SavePathTraversal(&headX, &headY, &pathX, &pathY);
	}

	while(1){
		if(!GameSnake_UpdatePositionHead(&directionFlag, &headX, &headY)) break;

		GameSnake_SavePathTraversal(&headX, &headY, &pathX, &pathY);
		GameSnake_UpdatePositionTail(&tailX, &tailY);

		if(flagBtnAction0) {
			directionFlag = GAMESNAKE_TURNLEFT;
			EXTI_ClearIRQFlag(&flagBtnAction0);
		}
		else if(flagBtnAction1) {
			directionFlag = GAMESNAKE_TURNRIGHT;
			EXTI_ClearIRQFlag(&flagBtnAction1);
		}
		else if(flagBtnAction2) {
			directionFlag = GAMESNAKE_MOVEUP;
			EXTI_ClearIRQFlag(&flagBtnAction2);
		}
		else if(flagBtnAction3) {
			directionFlag = GAMESNAKE_MOVEDOWN;
			EXTI_ClearIRQFlag(&flagBtnAction3);
		}

		Timer2_Delay(500);
	}

	LCD_ClearScreen();
	LCD_SetPosition(0, 0);
	LCD_WriteData(0xFF);
	LCD_WriteData(0xFF);

	LCD_WriteData(0x33);
	LCD_WriteData(0x33);

	while(1);
}

uint8_t GameSnake_UpdatePositionHead(uint8_t *dirFlag, uint8_t *Xpos, uint8_t *Ypos){
	if(*dirFlag == GAMESNAKE_TURNLEFT) (*Xpos)--;
	else if(*dirFlag == GAMESNAKE_TURNRIGHT) (*Xpos)++;
	else if(*dirFlag == GAMESNAKE_MOVEUP) (*Ypos)--;
	else if(*dirFlag == GAMESNAKE_MOVEDOWN) (*Ypos)++;

	// If snake head touch it's body then end game
	if((bufferLCD[*Ypos][*Xpos] & 0x1) == 1) return 0;

	LCD_DrawPixel(*Xpos, *Ypos);
	return 1;
}

void GameSnake_UpdatePositionTail(uint8_t *Xidx, uint8_t *Yidx){
	if(*Xidx > LCD_MAX_X_PIXEL) {
		(*Yidx)++;
		*Xidx = 0;
	}
	if(*Yidx > LCD_MAX_Y_BANK){
		*Yidx = 0;
		*Xidx = 0;
	}

	LCD_ErasePixel(bufferLCD[(*Yidx)][(*Xidx)] >> 1, bufferLCD[(*Yidx)][(*Xidx) + 1] >> 1);
	// Clear saved path
	bufferLCD[(*Yidx)][(*Xidx)] &= ~(0xFE);
	bufferLCD[(*Yidx)][(*Xidx) + 1] &= ~(0xFE);
	// Update index position for the next cycle
	*Xidx += 2;
}

void GameSnake_SavePathTraversal(uint8_t *Xpos, uint8_t *Ypos, uint8_t *Xidx, uint8_t *Yidx){
	if(*Xidx > LCD_MAX_X_PIXEL) {
		(*Yidx)++;
		*Xidx = 0;
	}
	if(*Yidx > LCD_MAX_Y_BANK){
		*Yidx = 0;
		*Xidx = 0;
	}
	// Set Xpos and Ypos to buffer[Y][X] and buffer[Y][X + 1] respectively
	// Shift left 1 to keep LCD_DrawPixel data
	bufferLCD[(*Yidx)][(*Xidx)++] |= (*Xpos) << 1;
	bufferLCD[(*Yidx)][(*Xidx)++] |= (*Ypos) << 1;
}

