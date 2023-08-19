/*
 * spi.h
 *
 *  Created on: Aug 11, 2023
 *      Author: Nguyen Duc Phu
 */

#ifndef SPI_H_
#define SPI_H_

void SPI1_Init(void);
void SPI1_Enable(void);
void SPI1_Disable(void);
void SPI1_WriteData(const uint8_t data);

void SPI2_Init(void);
void SPI2_Enable(void);
void SPI2_Disable(void);
void SPI2_WriteData(const uint8_t data);

#endif /* SPI_H_ */
