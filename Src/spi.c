/*
 * spi.c
 *
 *  Created on: Aug 11, 2023
 *      Author: Nguyen Duc Phu
 */

#include <stm32f1xx.h>
#include "spi.h"
#include "timer.h"

/************ DESCRIPTION *************
 * Configure SPI2 in transmit-only master mode
***************************************/

void SPI2_Init(void){
	// Enable SPI2 clock
	RCC->APB1ENR |= (1 << 14);

	// BR bit, serial clock baud rate F-pclk / 8
	SPI2->CR1 |= (0x2 << 3);

	// CPHA bit, 2nd clock transition is the 1st data capture
	SPI2->CR1 |= (1 << 0);

	// CPOL bit, clock polarity to 1 when idle
	SPI2->CR1 |= (1 << 1);

	// Data frame format 8-bit
	SPI2->CR1 &= ~(1 << 11);

	// Frame format MSB first
	SPI2->CR1 &= ~(1 << 7);

	// Enabled software slave management
	SPI2->CR1 |= (1 << 9);

	// Set SSI bit, IO value of NSS pin is ignored
	SPI2->CR1 |= (1 << 8);

	// Master mode
	SPI2->CR1 |= (1 << 2);

	// Transmit-only
	SPI2->CR1 &= ~(1 << 10);
	SPI2->CR1 &= ~(1 << 15);
}


/************ DESCRIPTION *************
 * Enable SPI2 peripheral
***************************************/

void SPI2_Enable(void){
	SPI2->CR1 |= (1 << 6);
}

/************ DESCRIPTION *************
 * Disable SPI2 peripheral
 * Check for busy/empty flag
***************************************/

void SPI2_Disable(void){
	// Check if Tx buffer is empty
	if((SPI2->SR & 1 << 1) == 0) return;

	// Check SPI busy flag
	if(!((SPI2->SR & 1 << 7) == 0)) return;

	// Disable SPI
	SPI2->CR1 &= ~(1 << 6);
}

/************ DESCRIPTION *************
 * Write data to Tx buffer
 * Peripheral need to be enabled before a write
***************************************/

void SPI2_WriteData(uint8_t data){
	// Check if SPI2 is enabled
	if((SPI2->CR1 & 1 << 6) == 0) return;

	// Data frame format is 8 bit => DR[0;7] is used for transmission/reception
	SPI2->DR = (0x0000 | data);

	// Wait 10ms before next write
	Timer2_Delay(10);
}
