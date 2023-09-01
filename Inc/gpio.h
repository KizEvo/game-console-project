/*
 * gpio.h
 *
 *  Created on: Aug 10, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef GPIO_H_
#define GPIO_H_

#define SET_PIN(x)				(x)
#define RESET_PIN(x)			((x) + (16))

#define INPUT_ANALOG			0x0U
#define INPUT_FLOATING			0x4U
#define INPUT_PULLDOWN			0x8U
#define INPUT_PULLUP			0x18U

#define GP_OUTPUT_PUSHPULL		0x0U
#define GP_OUTPUT_OPENDRAIN		0x4U
#define ALT_OUTPUT_PUSHPULL		0x8U
#define ALT_OUTPUT_OPENDRAIN	0xCU

#define OUTPUT_SPD_MED			0x1U
#define OUTPUT_SPD_LOW			0x2U
#define OUTPUT_SPD_HIGH			0x3U

#define NO_REMAP				0xFFU

void GPIOA_Init(void);
void GPIOB_Init(void);
void GPIOC_Init(void);

void GPIO_Config(GPIO_TypeDef *GPIO, uint8_t pin, uint8_t mode, uint8_t afioPos, uint32_t remapOption);
void GPIO_EnableClock(uint8_t portPos);

void GPIOC_Warning(void);

#endif /* GPIO_H_ */
