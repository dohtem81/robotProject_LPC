/*
 * spi.c
 *
 *  Created on: Feb 12, 2011
 *      Author: strus
 */

#include "../headers/spi.h"
#include "../headers/lpc21xx.h"

//**************************************************************************/
//
//	initializes selected SPI
//	currently only SPI0 implemented
//	PARAMETERS:
//		int port	-	which SPI to be initialized
//
//**************************************************************************/
void spi_initialize (int port){
	unsigned short dump;

	if (port == SETUP_SPI0) {
		//PINSEL0 	|= 	PINSEL_SPI0_SCK +
		//				PINSEL_SPI0_MISO +
		//				PINSEL_SPI0_MOSI +
		//				PINSEL_SPI0_SSEL0 ;
		PINSEL0		|=	0x5500;
		//PCONP 		|=	PCONP_SPI0 ;

		SPI_SPCR	=	0x1 << 5 ;	//in master mode
		/* in master mode this value min 8;
		 * rate = PCLK / SPCCR, where PCLK = CCLK / APBDIV
		 * as default APBDIV = 4 (register = 0)
		 * PCLK = ~60MHz / 4 = 15MHz
		 * rate = 15MHz / 8 = 1.875 MHz
		 */
		SPI_SPCCR	=	0xf0;//0x20;

		dump = SPI_SPSR;
	}

}


//**************************************************************************/
//
//	writes data (future: into selected SPI)
//	currently only SPI0 implemented
//	PARAMETERS:
//		int port	-	which SPI to be initialized
//		short *data	-	pointer to data to be written to SPI
//		short length-	length of the message (bytes)
//
//**************************************************************************/
void spi_write (int port, unsigned char *data, unsigned short length){
	unsigned short idx; //index pointer

	for (idx = 0; idx < length; idx++){
		SPI_SPDR = *data++;
		while (!(SPI_SPSR));
	}
}

//**************************************************************************/
//
//	reads data (future: into selected SPI)
//	currently only SPI0 implemented
//	PARAMETERS:
//		int port	-	which SPI to be initialized
//		short *data	-	pointer for data to be read from SPI
//
//**************************************************************************/
void spi_read (int port, unsigned char *data, unsigned short length){
	unsigned short idx, status;

	status = SPI_SPSR;

	for (idx = 0; idx < length; idx++){
		SPI_SPDR = 0xff;
		while(!(SPI_SPSR));
		*data++		=	SPI_SPDR ;
	}
}
