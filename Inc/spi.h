/*
 * spi.h
 *
 *  Created on: Aug 11, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef SPI_H_
#define SPI_H_

void SPI2_Init(void);
void SPI2_Enable(void);
void SPI2_Disable(void);
void SPI2_WriteData(uint8_t data);

#endif /* SPI_H_ */
