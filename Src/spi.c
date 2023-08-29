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

void SPI1_Init(void){
	RCC->APB2ENR |= (1 << 12);

	// Serial clock baud rate F-pclk
	SPI1->CR1 |= (0x3 << 3);	// 0x3

	// Set SSOE
	SPI1->CR2 |= (1 << 2);

	// Master mode
	SPI1->CR1 |= (1 << 2);
}

/************ DESCRIPTION *************
 * Enable SPI2 peripheral
***************************************/

void SPI1_Enable(void){
	SPI1->CR1 |= (1 << 6);
}

/************ DESCRIPTION *************
 * Disable SPI2 peripheral
 * Check for busy/empty flag
***************************************/

void SPI1_Disable(void){
	// Wait for Tx buffer to become empty
	while((SPI1->SR & 1 << 1) == 0);

	// Wait for SPI not busy flag
	while(!((SPI1->SR & 1 << 7) == 0));

	// Disable SPI
	SPI1->CR1 &= ~(1 << 6);
}

/************ DESCRIPTION *************
 * Write data to Tx buffer
 * Peripheral need to be enabled before a write
***************************************/

void SPI1_WriteData(const uint8_t data){
	// Data frame format is 8 bit => DR[0;7] is used for transmission/reception
	SPI1->DR = data;

	// Wait for Tx buffer to become empty
	while(!((SPI1->SR & 1 << 7) == 0));
}

/************ DESCRIPTION *************
 * Configure SPI2 in transmit-only master mode
***************************************/

void SPI2_Init(void){
	// Enable SPI2 clock
	RCC->APB1ENR |= (1 << 14);

	// BR bit, serial clock baud rate F-pclk
	SPI2->CR1 |= (0x6 << 3);

	// Set SSOE
	SPI2->CR2 |= (1 << 2);

	// Master mode
	SPI2->CR1 |= (1 << 2);
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
	// Wait for Tx buffer to become empty
	while((SPI2->SR & 1 << 1) == 0);

	// Wait for SPI not busy flag
	while(!((SPI2->SR & 1 << 7) == 0));

	// Disable SPI
	SPI2->CR1 &= ~(1 << 6);
}

/************ DESCRIPTION *************
 * Write data to Tx buffer
 * Peripheral need to be enabled before a write
***************************************/

void SPI2_WriteData(const uint8_t data){
	// Data frame format is 8 bit => DR[0;7] is used for transmission/reception
	SPI2->DR = data;

	// Wait for Tx buffer to become empty
	while(!((SPI2->SR & 1 << 7) == 0));
}
