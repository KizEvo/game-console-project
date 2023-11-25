/*
 * gui.c
 *
 *  Created on: Aug 22, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "gui.h"
#include "interrupt.h"
#include "lcd.h"
#include "gpio.h"
#include "timer.h"
#include "snake_game.h"
#include "sound.h"
#include "spi.h"

// Screen traversal history, with new screen selected
// the previous screen will be placed here
static uint8_t screenHistory[SCREEN_HISTORY_MAX] = {0};
// Screen index
static uint8_t screenIndex = 0;
// Pointer points to main screen by default
static uint8_t screenPointer = GUI_MAIN_SCREEN;

// Select options, with new screen selected the previous screen options
// will be cleared and the new one will be placed here
static uint8_t selectOptions[SELECT_OPTIONS_MAX] = {0};
// Select options pointer
static uint8_t selectPointer = 0;
// Selected options
static uint8_t selectedOption = 0;

// Graphic variables
static uint8_t prevSelectPointer = 1;
static uint8_t isFirstGraphicUpdate = 0;

void GUI_ScreenSelection(void){
	switch(screenPointer)
	{
		case GUI_MAIN_SCREEN:
			GUI_MainScreen();
			break;
		case GUI_SCREEN_GAMES:
			GUI_GamesScreen();
			break;
		case GAME_SNAKE:
			GameSnake_Start();
			GUI_ResetAllState();
			GUI_FirstUpdate();
			break;
		case GUI_SCREEN_SETTINGS:
			GUI_SettingsScreen();
			break;
		case SETTING_SOUND:
			GUI_Volume();
			GUI_ResetAllState();
			GUI_FirstUpdate();
			break;
		case SETTING_CONTRAST:
			GUI_Contrast();
			GUI_ResetAllState();
			GUI_FirstUpdate();
			break;
		default:
			break;
	}
}

void GUI_ResetAllState(void){
	for(uint8_t i = 0; i < SCREEN_HISTORY_MAX; i++)
		screenHistory[i] = 0;
	// Screen index
	screenIndex = 0;
	// Pointer points to main screen by default
	screenPointer = GUI_MAIN_SCREEN;

	for(uint8_t i = 0; i < SELECT_OPTIONS_MAX; i++)
		selectOptions[i] = 0;
	// Select options pointer
	selectPointer = 0;
	// Selected options
	selectedOption = 0;

	// Graphic variables
	prevSelectPointer = 1;
	isFirstGraphicUpdate = 0;
}

void GUI_FirstUpdate(void){
	GUI_UpdateLCDPointer();
	GUI_ScreenSelection();
	GUI_SetGraphicStatus();
}

void GUI_UpdatePointers(void){
	// Move pointer on LCD up
	if(flagBtnAction3) {
		if(selectPointer > 0) selectPointer--;
		if(selectPointer + 1 == 5) {
			GUI_ClearGraphicStatus();
			selectPointer = 0;
		}
		EXTI_ClearAllIRQFlag();
		Sound_MenuNavigate();
	}
	// Move pointer on LCD down
	else if(flagBtnAction4) {
		if(selectOptions[selectPointer + 1] > 0) selectPointer++;
		if(selectPointer - 1 == 4) GUI_ClearGraphicStatus();
		EXTI_ClearAllIRQFlag();
		Sound_MenuNavigate();
	}
	// Select / OK an option
	else if(flagBtnAction5) {
		if(!((selectOptions[selectPointer] & 1 << 7) == 0)) {
			screenHistory[screenIndex++] = screenPointer;
			screenPointer = selectOptions[selectPointer];
			selectPointer = 0;
			GUI_ClearGraphicStatus();
			GUI_ClearSelectOptions();
		} else {
			selectedOption = selectOptions[selectPointer];
		}
		EXTI_ClearAllIRQFlag();
		Sound_MenuNavigate();
	}
	// Return previous screen
	else if(flagBtnAction0){
		if(screenIndex > 0) {
			screenHistory[screenIndex--] = 0;
			screenPointer = screenHistory[screenIndex];
			selectPointer = 0;
			GUI_ClearGraphicStatus();
			GUI_ClearSelectOptions();
		}
		EXTI_ClearAllIRQFlag();
		Sound_MenuNavigate();
	}
}

void GUI_UpdateLCDPointer(void){
	if(!isFirstGraphicUpdate) {
		// Clear previous pointer arrow
		for(uint8_t row = 2; row <= 5; row++){
			LCD_SetPosition(5, row);
			LCD_WriteString(" ");
		}
		// When switching screen or on program startup
		// Arrow symbol in Ybank = 1
		LCD_SetPosition(5, 1);
		LCD_WriteString(">");
	} else if(prevSelectPointer != (selectPointer % 5) + 1){
		// Move select arrow pointer
		LCD_SetPosition(5, prevSelectPointer);
		LCD_WriteString(" ");
		LCD_SetPosition(5, (selectPointer % 5) + 1);
		LCD_WriteString(">");
	}

	prevSelectPointer = (selectPointer % 5) + 1;
}

void GUI_UpdateLCDOption(char *graphicStr[], const uint8_t *options){
	// Clear previous string on LCD
	for(uint8_t row = 1; row <= 5; row++){
		LCD_SetPosition(20, row);
		// 12 spaces => (5 (column per word) * 12) = 60 < 83 (LCD_MAX_X_PIXEL)
		// the last 3 columns are NOT cleared with each update
		LCD_WriteString("            ");
	}
	// Write new string
	uint8_t yBank = 1;
	for(uint8_t i = 5 * (selectPointer / 5);
			(i < 5 * (selectPointer / 5) + 5) && (options[i] != 0); i++){
		LCD_SetPosition(20, yBank++);
		LCD_WriteString(graphicStr[i]);
	}
}

void GUI_UpdateLCDScreenHeader(char *headerStr){
	LCD_SetPosition(5, 0);
	// 14 spaces => (5 (column per word) * 14) = 70 < 83 (LCD_MAX_X_PIXEL)
	// Clear previous header
	LCD_WriteString("              ");
	LCD_SetPosition(5, 0);
	// Write string header
	LCD_WriteString(headerStr);
}

void GUI_ClearSelectOptions(void){
	for(uint8_t i = 0; i < SELECT_OPTIONS_MAX; i++){
		if(selectOptions[i] <= 0) break;
		selectOptions[i] = 0;
	}
}

void GUI_FillCurrentScreenOptions(const uint8_t *optionsArr){
	// array[i] == 0 is the stop condition that the passed in array have to provide
	for(uint8_t i = 0; optionsArr[i] != 0; i++){
		selectOptions[i] = optionsArr[i];
	}
}

void GUI_MainScreen(void){
	uint8_t options[] = {GUI_SCREEN_GAMES, GUI_SCREEN_SETTINGS, 0};
	char *graphic[] = {"Games", "Settings"};

	if(!isFirstGraphicUpdate) {
		GUI_FillCurrentScreenOptions(options);
		GUI_UpdateLCDScreenHeader("Menu:");
		GUI_UpdateLCDOption(graphic, options);
	}
}

void GUI_GamesScreen(void){
	uint8_t options[] = {GAME_SNAKE, 0};
	char *graphic[] = {"Snake"};

	if(!isFirstGraphicUpdate) {
		GUI_FillCurrentScreenOptions(options);
		GUI_UpdateLCDScreenHeader("Games:");
		GUI_UpdateLCDOption(graphic, options);
	}
}

void GUI_SettingsScreen(void){
	uint8_t options[] = {SETTING_SOUND, SETTING_CONTRAST, 0};
	char *graphic[] = {"Volume", "Contrast"};

	if(!isFirstGraphicUpdate) {
		GUI_FillCurrentScreenOptions(options);
		GUI_UpdateLCDScreenHeader("Settings:");
		GUI_UpdateLCDOption(graphic, options);
	}
}

void GUI_Volume(void){
	EXTI_ClearAllIRQFlag();
	LCD_ClearScreen();

	Timer2_Delay(100);

	LCD_SetPosition(0, 0);
	LCD_WriteString("Change Sound");
	LCD_SetPosition(0, 1);
	LCD_WriteString("Settings");

	LCD_SetPosition(0, 3);
	LCD_WriteString("ON(L)/OFF(R)");

	while(1){
		if(flagBtnAction0){
			Sound_Config(SOUND_OFF);
			break;
		} else if (flagBtnAction5){
			Sound_Config(SOUND_ON);
			break;
		}
	}
	LCD_ClearScreen();
	Timer2_Delay(100);
	EXTI_ClearAllIRQFlag();
}

void GUI_Contrast(void){
	EXTI_ClearAllIRQFlag();
	LCD_ClearScreen();

	Timer2_Delay(100);

	LCD_SetPosition(0, 0);
	LCD_WriteString("LCD Contrast");
	LCD_SetPosition(0, 1);
	LCD_WriteString("Brightness");

	LCD_SetPosition(0, 3);
	LCD_WriteString("[1] <=> [2]");
	LCD_SetPosition(0, 5);
	LCD_WriteString("Return [5]");

	while(1){
		if(flagBtnAction1){
			LCD_DecrementContrast();
			EXTI_ClearIRQFlag(&flagBtnAction1);
			// Command for Function Set in extended instruction
			LCD_WriteCommand(LCD_COMMAND_FUNC_SET | EXTENDED_INSTRUCTION);
			// Set Voltage VOP (LCD contrast)
			LCD_WriteCommand(LCD_COMMAND_VOP | contrastLCD);
			Timer2_Delay(50);
		} else if (flagBtnAction2){
			LCD_IncrementContrast();
			EXTI_ClearIRQFlag(&flagBtnAction2);
			// Command for Function Set in extended instruction
			LCD_WriteCommand(LCD_COMMAND_FUNC_SET | EXTENDED_INSTRUCTION);
			// Set Voltage VOP (LCD contrast)
			LCD_WriteCommand(LCD_COMMAND_VOP | contrastLCD);
			Timer2_Delay(50);
		} else if (flagBtnAction5){
			break;
		}
	}
	LCD_ClearScreen();
	Timer2_Delay(100);
	EXTI_ClearAllIRQFlag();
}

void GUI_SetGraphicStatus(void){
	isFirstGraphicUpdate = 1;
}

void GUI_ClearGraphicStatus(void){
	isFirstGraphicUpdate = 0;
}
