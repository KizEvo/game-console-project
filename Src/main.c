/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Nguyen Duc Phu
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f1xx.h>

// Peripherals, clock, connectivity header files
#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h"
#include "lcd.h"
#include "interrupt.h"
// GUI header files
#include "gui.h"
#include "snake_game.h"
// Sound effect header files
#include "sound.h"

int main(void){
	Clock_Init();

	Timer2_Init();
	Timer1_Init();

	GPIOA_Init();
	GPIOB_Init();
	GPIOC_Init();

	SPI1_Init();
	SPI1_Enable();

	EXTI_Init();
	NVIC_Enable();

	LCD_Init();
//	Buzzer
//	for(int i = 0; i < 3; i++){
//		Sound_LostGame();
//		Timer2_Delay(2000);
//	}

	GUI_FirstUpdate();
	while(1){
		GUI_UpdatePointers();
		GUI_UpdateLCDPointer();
		GUI_ScreenSelection();
		GUI_SetGraphicStatus();
//		if(flagBtnAction0){
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(500);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			Timer2_Delay(500);
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(500);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			EXTI_ClearIRQFlag(&flagBtnAction0);
//		} else if(flagBtnAction3){
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			EXTI_ClearIRQFlag(&flagBtnAction3);
//		}
//		else if (flagBtnAction4) {
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			Timer2_Delay(200);
//			EXTI_ClearIRQFlag(&flagBtnAction4);
//		} else if(flagBtnAction5){
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << SET_PIN(13));
//			Timer2_Delay(200);
//			GPIOC->BSRR |= (1 << RESET_PIN(13));
//			EXTI_ClearIRQFlag(&flagBtnAction5);
//		}
	}
}
