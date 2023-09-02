/*
 * interrupt.c
 *
 *  Created on: Aug 17, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "interrupt.h"

uint8_t flagBtnAction0 = 0;
uint8_t flagBtnAction1 = 0;
uint8_t flagBtnAction2 = 0;
uint8_t flagBtnAction3 = 0;
uint8_t flagBtnAction4 = 0;
uint8_t flagBtnAction5 = 0;

/************ DESCRIPTION *************
 * Initialize hardware external interrupt
 * Pin 0, 1, 2 & 3 of port A
***************************************/

void EXTI_Init(void){
	// Enable AFIO clock
	RCC->APB2ENR |= (1 << 0);
	// EXTI on pin 1, 2 of port A
	AFIO->EXTICR[0] &= ~(0x0FF0);
	// EXTI on pin 3 of port B
	AFIO->EXTICR[0] |= 0x1000;
	// EXTI on pin 4 & 5 of port B
	AFIO->EXTICR[1] |= 0x0011;
	// EXTI on pin 15 of port A
	AFIO->EXTICR[3] &= ~(0xF000);

	// Interrupt on falling edge on line 1, 2, 3, 4, 5 & 15
	EXTI->FTSR |= 0x3E << 0;
	EXTI->FTSR |= (1 << 15);
	// Set bit mask of 1, 2, 3, 4, 5 & 15
	EXTI->IMR |= 0x3E << 0;
	EXTI->IMR |= (1 << 15);

}

/************ DESCRIPTION *************
 * Enable NVIC for line 1, 2, 3, 4, 5 and 15
 * Default interrupt priority is 0 (highest)
***************************************/

void NVIC_Enable(void){
	uint32_t line = 0;
	// Set line 1 to 4
	line |= (0xF << 7);
	// Set line 5 (5-9)
	line |= (1 << 23);
	// Write to ISER[0] 0-31 register
	NVIC->ISER[0] = line;
	// Clear previous lines
	line = 0;
	// Set line 15 (10-15)
	// Offset = 32
	line |= (1 << (40 - 32));
	// Write to ISER[1] 32-63 register
	NVIC->ISER[1] = line;
}

/************ DESCRIPTION *************
 * IRQ Handlers
 * Get called whenever there is an external interrupt
***************************************/

void EXTI1_IRQHandler(void){
	flagBtnAction1 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 1;
}

void EXTI2_IRQHandler(void){
	flagBtnAction2 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 2;
}

void EXTI3_IRQHandler(void){
	flagBtnAction3 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 3;
}

void EXTI4_IRQHandler(void){
	flagBtnAction4 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 4;
}

void EXTI9_5_IRQHandler(void){
	flagBtnAction5 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 5;
}

void EXTI15_10_IRQHandler(void){
	flagBtnAction0 = 1;
	// Clear pending register
	EXTI->PR |= 1 << 15;
}

void EXTI_ClearIRQFlag(uint8_t *flag){
	*flag = 0;
}
