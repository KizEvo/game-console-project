/*
 * gpio.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef GPIO_H_
#define GPIO_H_

#define SET_PIN(x)		(x)
#define RESET_PIN(x)	((x) + (16))

void GPIOA_Init(void);
void GPIOB_Init(void);
void GPIOC_Init(void);

#endif /* GPIO_H_ */
