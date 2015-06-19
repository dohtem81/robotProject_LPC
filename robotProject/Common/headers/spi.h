/*
 * spi.h
 *
 *  Created on: Feb 12, 2011
 *      Author: strus
 */

#ifndef SPI_H_
#define SPI_H_

#define SETUP_SPI0				0
#define SETUP_SPI1				1 //to be implemented

#define PINSEL_SPI0_SCK			0x01 << 8
#define PINSEL_SPI0_MISO		0x01 << 10
#define	PINSEL_SPI0_MOSI		0x01 << 12
#define PINSEL_SPI0_SSEL0		0x01 << 14
#define PCONP_SPI0				0x1 << 8


// function declarations
void spi_initialize (int);
void spi_write (int, unsigned char *, unsigned short);
void spi_read  (int, unsigned char *, unsigned short);

#endif /* SPI_H_ */
