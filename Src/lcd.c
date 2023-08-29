/*
 * lcd.c
 *
 *  Created on: Aug 12, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include <math.h>
#include "lcd.h"
#include "timer.h"
#include "spi.h"
#include "gpio.h"
#include "font.h"

// Global variable
// Buffer to keep track of drew pixel
// 0x01 at [Yn][Xn] means that location have been drawn
// 0x00 at [Yn][Xn] means that location have NOT been drawn
// [1;7] bits can be used for other purposes
uint8_t bufferLCD[48][84] = {{0}};

// Global variable
// LCD contrast value
uint8_t contrastLCD = 0x45;

// Offset from previously set position
// X: 0 to 83
// Y: 0 to 5 (Y bank)
uint8_t offsetX = 0;
uint8_t offsetY = 0;

// Previous set position
uint8_t prevSetX = 0;
uint8_t prevSetY = 0;

/************ DESCRIPTION *************
 * Initialize Nokia 5110 Monochrome LCD sequence
 * Only called one at program startup
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_Init(void){
	Timer2_Delay(10);
	// RESET Pin to LOW signal
	GPIOB->BSRR |= (1 << RESET_PIN(10));
	// Wait 10ms
	Timer2_Delay(10);
	// RESET Pin to HIGH signal
	GPIOB->BSRR |= (1 << SET_PIN(10));
	Timer2_Delay(1000);

	// Command for Function Set in extended instruction
	LCD_WriteCommand(LCD_COMMAND_FUNC_SET | EXTENDED_INSTRUCTION);
	// Set Voltage VOP (LCD contrast)
	LCD_WriteCommand(LCD_COMMAND_VOP | contrastLCD);
	// Set bias
	LCD_WriteCommand(LCD_COMMAND_VOL_BIAS);

	// Command for Function Set in basic instruction
	LCD_WriteCommand(LCD_COMMAND_FUNC_SET | BASIC_INSTRUCTION);
	// Command for display control
	LCD_WriteCommand(LCD_COMMAND_DISPLAY_CTRL | NORMAL_MODE);

	// Clear screen
	LCD_ClearScreen();
	// Set default position
	LCD_SetPosition(0, 0);
}

/************ DESCRIPTION *************
 * Sequence to write a Command to LCD
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_WriteCommand(const uint8_t command){
	// DC Pin to LOW signal
	GPIOB->BSRR |= (1 << RESET_PIN(0));
	// Clear SCE pin
	GPIOB->BSRR |= (1 << RESET_PIN(1));
	// Write to Tx buffer
	SPI1_WriteData(command);
	// Set SCE pin
	GPIOB->BSRR |= (1 << SET_PIN(1));
}


/************ DESCRIPTION *************
 * Sequence to write a Data to display in LCD
 * Call update buffer function, update offset
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_WriteData(const uint8_t data){
	// DC Pin to HIGH signal
	GPIOB->BSRR |= (1 << SET_PIN(0));
	// Clear SCE pin
	GPIOB->BSRR |= (1 << RESET_PIN(1));
	// Write to Tx buffer
	SPI1_WriteData(data);
	// Set SCE pin
	GPIOB->BSRR |= (1 << SET_PIN(1));
	// Update bufferLCD[Y][X]
	LCD_UpdateBuffer(data);
	// Update offset global variables
	LCD_UpdatePosOffset();
}


void LCD_GUI(void){
	Timer2_Delay(1000);

	LCD_DrawSquare(1, LCD_MAX_X_PIXEL - 1, 1, LCD_MAX_Y_PIXEL - 1);

	Timer2_Delay(5000);

	LCD_SetPosition(5, 4);
	LCD_WriteString("Hello world");
}

/************ DESCRIPTION *************
 * Clear LCD screen and bufferLCD global variable
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_ClearScreen(void){
	for(uint8_t row = 0; row <= 5; row++){
		prevSetY = row;
		offsetY = 0;
		for(uint8_t col = 0; col <= 83; col++)
		{
			prevSetX = col;
			offsetX = 0;
			LCD_WriteData(0x00);
		}
	}
}

/************ DESCRIPTION *************
 * Set X and Y positions for the next Data write
 * Clear offset global variables when using this function
 * Set previous X and Y global variables
 * X : 0 to 83
 * Ybank : 0 to 5
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_SetPosition(uint8_t Xpos, uint8_t Ybank){
	if(Xpos >= LCD_MAX_X_PIXEL) Xpos = LCD_MAX_X_PIXEL;
	if(Ybank >= LCD_MAX_Y_BANK) Ybank = LCD_MAX_Y_BANK;

	prevSetX = Xpos;
	prevSetY = Ybank;
	offsetX = 0;
	offsetY = 0;

	LCD_WriteCommand(LCD_COMMAND_FUNC_SET | BASIC_INSTRUCTION);
	LCD_WriteCommand(LCD_COMMAND_X_POS | Xpos);
	LCD_WriteCommand(LCD_COMMAND_Y_POS | Ybank);
}

/************ DESCRIPTION *************
 * Update position offset from previous set position
 * User should NOT call this function alone
 * Get called whenever user write a Data to LCD
 * Reset to position 0 when go out of bound
***************************************/

void LCD_UpdatePosOffset(void){
	offsetX++;

	if(prevSetX + offsetX > LCD_MAX_X_PIXEL){
		offsetY++;
		offsetX = 0;
	}

	if(prevSetY + offsetY > LCD_MAX_Y_BANK){
		offsetX = 0;
		offsetY = 0;
		prevSetX = 0;
		prevSetY = 0;
	}
}

/************ DESCRIPTION *************
 * Draw a pixel on LCD screen
 * Offset global variables have no effect when using this function repeatedly
 * X: 0 to 83
 * Y: 0 to 47
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_DrawPixel(uint8_t Xpos, uint8_t Ypos){
	if(Xpos >= LCD_MAX_X_PIXEL) Xpos = LCD_MAX_X_PIXEL;
	if(Ypos >= LCD_MAX_Y_PIXEL) Ypos = LCD_MAX_Y_PIXEL;

	uint8_t prevPixelData = 0x00;
	uint8_t upperLimitY = 8 * (Ypos / 8 + 1);
	uint8_t lowerLimitY = upperLimitY - 8;
	uint8_t bin = 0;

	for(uint8_t i = lowerLimitY; i < upperLimitY; i++){
		if((bufferLCD[i][Xpos] & 0x1) == 1) prevPixelData += 1 << bin;
		bin++;
	}

	uint8_t Ybank = Ypos >> 3;
	uint8_t pixelData = 0x00 | prevPixelData | (1 << (Ypos % 8));

	LCD_SetPosition(Xpos, Ybank);
	LCD_WriteData(pixelData);
}

/************ DESCRIPTION *************
 * Draw a square on LCD screen
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_DrawSquare(uint8_t startXpos, uint8_t endXpos, uint8_t startYpos, uint8_t endYpos){
	// User-defined error
	if(startXpos == endXpos || startYpos == endYpos) GPIOC_Warning();

	// Swap start and end if start > end
	if(startXpos > endXpos) {
		startXpos += endXpos;
		endXpos = startXpos - endXpos;
		startXpos = startXpos - endXpos;
	}
	if(startYpos > endYpos){
		startYpos += endYpos;
		endYpos = startYpos - endYpos;
		startYpos = startYpos - endYpos;
	}
	// Draw square
	for(uint8_t row = startYpos; row <= endYpos; row++){
		for(uint8_t col = startXpos; col <= endXpos; col++)
		{
			if(row == startYpos || row == endYpos) LCD_DrawPixel(col, row);
			else {
				LCD_DrawPixel(startXpos, row);
				LCD_DrawPixel(endXpos, row);
				break;
			}
		}
	}
}

/************ DESCRIPTION *************
 * Erase a pixel on LCD screen
 * Offset global variables have no effect when using this function repeatedly
 * X: 0 to 83
 * Y: 0 to 47
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_ErasePixel(uint8_t Xpos, uint8_t Ypos){
	if(Xpos >= LCD_MAX_X_PIXEL) Xpos = LCD_MAX_X_PIXEL;
	if(Ypos >= LCD_MAX_Y_PIXEL) Ypos = LCD_MAX_Y_PIXEL;

	uint8_t prevPixelData = 0x00;
	uint8_t upperLimitY = 8 * (Ypos / 8 + 1);
	uint8_t lowerLimitY = Ypos - (Ypos % 8);
	uint8_t bin = 0;

	for(uint8_t i = lowerLimitY; i < upperLimitY; i++){
		if((bufferLCD[i][Xpos] & 0x1) == 1) prevPixelData += 1 << bin;
		bin++;
	}

	uint8_t Ybank = Ypos >> 3;
	uint8_t pixelData = prevPixelData;
	pixelData &= ~(1 << (Ypos % 8));

	LCD_SetPosition(Xpos, Ybank);
	LCD_WriteData(pixelData);
}

/************ DESCRIPTION *************
 * Write a string to LCD screen
 * 5 * 8 font
 * Must enable SPI peripheral before using this function
***************************************/

void LCD_WriteString(char *string){
	for(uint8_t i = 0; string[i] != '\0'; i++){
		for(uint8_t j = 0; j < 5; j++)
			LCD_WriteData( ASCII[string[i] - 32][j] );
	}
}

/************ DESCRIPTION *************
 * Update bufferLCD[y][x]
 * User should NOT call this function alone
 * Get called whenever user write a Data to LCD
***************************************/

void LCD_UpdateBuffer(uint8_t data){
	uint8_t Xpos = prevSetX + offsetX;
	uint8_t Ypos = 8 * prevSetY + offsetY;

	// User-defined error
	// Will be deleted later
	if(Xpos > LCD_MAX_X_PIXEL || Ypos > LCD_MAX_Y_PIXEL) GPIOC_Warning();

	for(; Ypos < (8 * prevSetY + offsetY + 8); Ypos++){
		if((data & 1) == 1) bufferLCD[Ypos][Xpos] |= (1 << 0);
		else bufferLCD[Ypos][Xpos] &= ~(1 << 0);

		data = data >> 1;
	}
}

uint8_t LCD_IncrementContrast(void){
	contrastLCD++;
	if(contrastLCD >= LCD_MAX_CONTRAST) contrastLCD = LCD_MAX_CONTRAST;

	return contrastLCD;
}

uint8_t LCD_DecrementContrast(void){
	contrastLCD--;
	if(contrastLCD <= 0x00) contrastLCD = 0x00;

	return contrastLCD;
}
