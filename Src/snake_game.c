/*
 * snake_game.c
 *
 *  Created on: Aug 19, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include <stdlib.h>
#include "snake_game.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"
#include "gpio.h"

void GameSnake_Start(void){
	EXTI_ClearAllIRQFlag();
	uint8_t again = 1;

	while(again){
		uint8_t headX = 4;
		uint8_t headY = 4;

		uint8_t tailX = 0;
		uint8_t tailY = 0;

		uint8_t pathX = 0;
		uint8_t pathY = 0;

		uint16_t appleXY = 0;

		uint8_t stage = 0;
		uint8_t speed = 200;
		uint8_t increaseSnakelength = 0;

		uint8_t directionFlag = GAMESNAKE_TURNRIGHT;
		uint8_t appleFlag = 0;

		LCD_ClearScreen();

		Timer2_Delay(100);

		GameSnake_GUI();

		for(uint8_t snakeLength = 0; snakeLength < 5; snakeLength++){
			GameSnake_UpdatePositionHead(&directionFlag, &appleFlag, &headX, &headY);
			GameSnake_SavePathTraversal(&headX, &headY, &pathX, &pathY);
		}

		while(1){
			if(!appleFlag){
				GPIOC->BSRR |= (1 << SET_PIN(13));
				appleXY = GameSnake_CreateApple(&appleFlag);
				if(++stage >= 5) {
					stage = 0;
					if(speed - 10 >= 170) speed -= 10;
				}
			}

			if(!GameSnake_UpdatePositionHead(&directionFlag, &appleFlag, &headX, &headY)) break;
			if(appleFlag) GameSnake_SavePathTraversal(&headX, &headY, &pathX, &pathY);

			if(!appleFlag) {
				GPIOC->BSRR |= (1 << RESET_PIN(13));
				GameSnake_RemoveApple(appleXY >> 8, appleXY & 0x00FF);
				LCD_DrawPixel(headX, headY);
				GameSnake_SavePathTraversal(&headX, &headY, &pathX, &pathY);
				increaseSnakelength = 3;
			}

			if(increaseSnakelength > 0) increaseSnakelength--;
			if(increaseSnakelength <= 0) GameSnake_UpdatePositionTail(&tailX, &tailY);

			if(flagBtnAction1) {
				if(directionFlag != GAMESNAKE_TURNRIGHT)
					directionFlag = GAMESNAKE_TURNLEFT;
				EXTI_ClearIRQFlag(&flagBtnAction1);
			}
			else if(flagBtnAction2) {
				if(directionFlag != GAMESNAKE_TURNLEFT)
					directionFlag = GAMESNAKE_TURNRIGHT;
				EXTI_ClearIRQFlag(&flagBtnAction2);
			}
			else if(flagBtnAction3) {
				if(directionFlag != GAMESNAKE_MOVEDOWN)
					directionFlag = GAMESNAKE_MOVEUP;
				EXTI_ClearIRQFlag(&flagBtnAction3);
			}
			else if(flagBtnAction4) {
				if(directionFlag != GAMESNAKE_MOVEUP)
					directionFlag = GAMESNAKE_MOVEDOWN;
				EXTI_ClearIRQFlag(&flagBtnAction4);
			}
			Timer2_Delay(speed);
		}

		EXTI_ClearIRQFlag(&flagBtnAction5);
		EXTI_ClearIRQFlag(&flagBtnAction0);

		LCD_ClearScreen();
		LCD_SetPosition(5, 0);
		LCD_WriteString("You lose!");
		LCD_SetPosition(5, 1);
		LCD_WriteString("Try again ?");
		LCD_SetPosition(0, 2);
		LCD_WriteString("Yes(L)/No(R)");
		while(!flagBtnAction5 && !flagBtnAction0);
		if(flagBtnAction0) {
			again = 0;
			LCD_ClearScreen();
		}

		Timer2_Delay(50);
		EXTI_ClearAllIRQFlag();
	}
}

void GameSnake_GUI(void){
	LCD_DrawSquare(1, LCD_MAX_X_PIXEL - 1, 1, LCD_MAX_Y_PIXEL);
}

uint8_t GameSnake_UpdatePositionHead(uint8_t *dirFlag, uint8_t *appleFlag, uint8_t *Xpos, uint8_t *Ypos){
	if(*dirFlag == GAMESNAKE_TURNLEFT) (*Xpos)--;
	else if(*dirFlag == GAMESNAKE_TURNRIGHT) (*Xpos)++;
	else if(*dirFlag == GAMESNAKE_MOVEUP) (*Ypos)--;
	else if(*dirFlag == GAMESNAKE_MOVEDOWN) (*Ypos)++;
	// If snake head touch it's body & not Apple then end game
	if((bufferLCD[*Ypos][*Xpos] & 0x1) == 1 && bufferLCD[*Ypos][*Xpos] != 0xFF) return 0;
	// Check if Xpos and Ypos is Apple location
	if(bufferLCD[*Ypos][*Xpos] == 0xFF) {
		*appleFlag = 0;
		return 1;
	}

	LCD_DrawPixel(*Xpos, *Ypos);
	return 1;
}

void GameSnake_UpdatePositionTail(uint8_t *Xidx, uint8_t *Yidx){
	if(*Xidx >= LCD_MAX_X_PIXEL) {
		(*Yidx)++;
		*Xidx = 0;
	}
	if(*Yidx >= LCD_MAX_Y_PIXEL){
		*Yidx = 0;
		*Xidx = 0;
	}

	LCD_ErasePixel(bufferLCD[(*Yidx)][(*Xidx)] >> 1, bufferLCD[(*Yidx)][(*Xidx) + 1] >> 1);
	// Clear saved path
	Timer2_Delay(1);
	bufferLCD[(*Yidx)][(*Xidx)] &= ~(0xFE);
	bufferLCD[(*Yidx)][(*Xidx) + 1] &= ~(0xFE);
	// Update index position for the next cycle
	*Xidx += 2;
}

void GameSnake_SavePathTraversal(uint8_t *Xpos, uint8_t *Ypos, uint8_t *Xidx, uint8_t *Yidx){
	if(*Xidx >= LCD_MAX_X_PIXEL) {
		(*Yidx)++;
		*Xidx = 0;
	}
	if(*Yidx >= LCD_MAX_Y_PIXEL){
		*Yidx = 0;
		*Xidx = 0;
	}
	Timer2_Delay(1);

	// TEST: Clear bit [1;7] first then set Xpos and Ypos below

	// Set Xpos and Ypos to buffer[Y][X] and buffer[Y][X + 1] respectively
	// Shift left 1 to keep LCD_DrawPixel data
	bufferLCD[(*Yidx)][(*Xidx)++] |= (*Xpos) << 1;
	bufferLCD[(*Yidx)][(*Xidx)++] |= (*Ypos) << 1;
}

uint16_t GameSnake_CreateApple(uint8_t *appleFlag){
	uint8_t Xpos, Ypos;
	do {
		// Generate random numb from 3 to 80 (83 - 1 - 2)
		Xpos = (rand() % (LCD_MAX_X_PIXEL - 2 - 3) + 3);
		// Generate random numb from 3 to 44 (47 - 1 - 2)
		Ypos = (rand() % (LCD_MAX_Y_PIXEL - 2 - 3) + 3);
		// Check if location have been drawn yet
	} while((bufferLCD[Ypos][Xpos] & 1) == 1 || (bufferLCD[Ypos][Xpos + 1] & 1) == 1
			|| (bufferLCD[Ypos + 1][Xpos] & 1) == 1 || (bufferLCD[Ypos + 1][Xpos + 1] & 1) == 1);

	// Draw 2x2 Apple
	LCD_DrawPixel(Xpos, Ypos);
	LCD_DrawPixel(Xpos, Ypos + 1);
	LCD_DrawPixel(Xpos + 1, Ypos);
	LCD_DrawPixel(Xpos + 1, Ypos + 1);

	Timer2_Delay(1);
	// [0;8] bits are written 0xFF
	// special condition to recognize Apple instead of Snake Body in the buffer
	bufferLCD[Ypos][Xpos] = 0xFF;
	bufferLCD[Ypos][Xpos + 1] = 0xFF;
	bufferLCD[Ypos + 1][Xpos] = 0xFF;
	bufferLCD[Ypos + 1][Xpos + 1] = 0xFF;

	// Set apple flag
	*appleFlag = 1;

	// Format data, Xpos in the MSB 8 bits, Ypos in the LSB 8 bits
	uint16_t position = Xpos;
	position = position << 8;
	position |= Ypos;

	return position;
}

void GameSnake_RemoveApple(uint8_t Xpos, uint8_t Ypos){
	LCD_ErasePixel(Xpos, Ypos);
	LCD_ErasePixel(Xpos, Ypos + 1);
	LCD_ErasePixel(Xpos + 1, Ypos);
	LCD_ErasePixel(Xpos + 1, Ypos + 1);

	Timer2_Delay(1);
	// [0;8] bits are cleared
	bufferLCD[Ypos][Xpos] &= 0x00;
	bufferLCD[Ypos][Xpos + 1] &= 0x00;
	bufferLCD[Ypos + 1][Xpos] &= 0x00;
	bufferLCD[Ypos + 1][Xpos + 1] &= 0x00;
}
