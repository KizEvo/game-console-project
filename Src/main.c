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

int main(void){

	Clock_Init();
	Timer2_Init();
	SPI2_Init();
	GPIOB_Init();

	/* Testing 		*/
//	SPI2_Enable();
//	SPI2_WriteData(0xAA);

    /* Loop forever */
	while(1){
	}
}
