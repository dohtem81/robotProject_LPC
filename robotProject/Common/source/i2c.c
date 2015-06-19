#include "../headers/lpc21xx.h"
#include "../headers/i2c.h"


//unsigned char *G_I2Cdata;

/////////////////////////////////////
//
//initializes selected I2C port
//
/////////////////////////////////////
void initI2C(unsigned int port){
	
	//checks if argument of the function is correct
	//if ((port != I2C0) & (port != I2C1) & (port != I2C2))
	//	return;
	
//	PCONP |= port;
	//clksel is set in traget.h to 75MHz/4 = 18.75MHz
	
	//pinsel seleection - based on port
//	switch (port){
	
//	case I2C0:
		PINSEL0		=	PINSEL0 | (0x5 << 4);//(I2C0_SDA | I2C0_SCL);
		//PINMODE1	|=	(0xa << 22);
		I20CONCLR	=	(I2CONSET_AA | I2CONSET_SI | I2CONSET_STA | I2CONSET_I2EN);
		I20ADR		=	I2C0_SLAVE;
		I20CONSET	=	I2CONSET_I2EN;
		I20SCLH		=	0x8;//0x0032;
		I20SCLL		=	0x8;//0x0032;
//		break;
/*
	case I2C1:
		PINSEL0 	|= (I2C1_SDA | I2C1_SCL);
		PINMODE0	|=	0xa;
		I20CONCLR	=	(I2CONSET_AA | I2CONSET_SI | I2CONSET_STA | I2CONSET_I2EN);
		I21ADR		=	I2C1_SLAVE;
		I21CONSET	=	I2CONSET_I2EN;
		I21SCLH		=	0x0032;
		I21SCLL		=	0x0032;
		break;
	
	case I2C2:
		PINSEL0 	|=	(I2C2_SDA | I2C2_SCL);
		PINMODE0	|=	(0xa << 20);
		I20CONCLR	=	(I2CONSET_AA | I2CONSET_SI | I2CONSET_STA | I2CONSET_I2EN);
		I22ADR		=	I2C2_SLAVE;
		I22CONSET	=	I2CONSET_I2EN;
		I22SCLH		=	0x0032;
		I22SCLL		=	0x0032;
		break;*/
		
//	default:
//		return;
//		break;
//	}
}


/////////////////////////////////////
//
//	send start
//
/////////////////////////////////////
void writeI2CStart(){
	I20CONCLR	=	I2CONSET_SI | I2CONSET_STO;
	I20CONSET	=	I2CONSET_STA | I2CONSET_I2EN;
	return;
}


/////////////////////////////////////
//
//	send stop
//
/////////////////////////////////////
void writeI2CStop(){
	I20CONSET	=	I2CONSET_STO;
	I20CONCLR	=	I2CONSET_SI;
	return;
}


/////////////////////////////////////
//
//	initialize master data cnt
//
/////////////////////////////////////
void writeI2C(unsigned char data){
	I20DAT		=	data;
	I20CONCLR	=	(I2CONSET_STA | I2CONSET_SI);
	return;
}


/////////////////////////////////////
//
//	creates delay
//
/////////////////////////////////////
void waitForComplete(void){
//need to wait how to achieve that
}


//////////////////////////////////////////////////////////////////////
//
//	reads register indicated by reg
//
//////////////////////////////////////////////////////////////////////
int I2CreadRegister(unsigned short reg, unsigned short address){
	unsigned char msb, lsb;
	int i;
	int mult, retVal;
	
	i = 0;
	writeI2CStart();
	while ((I20STAT != 0x08) & (i++ < 0xff));
	
	i = 0;
	writeI2C(address);
	while ((I20STAT != 0x18) & (i++ < 0xff));
	
	i = 0;
	writeI2C(reg);
	while ((I20STAT != 0x28) & (i++ < 0xff));
	
	i = 0;
	writeI2CStart();
	while ((I20STAT != 0x10) & (i++ < 0xff));
	
	i = 0;
	writeI2C(address | 0x1);
	while ((I20STAT != 0x40) & (i++ < 0xff));
	I20CONSET	=	I2CONSET_AA;
	I20CONCLR	=	I2CONSET_SI;
	
	i = 0;
	while ((I20STAT != 0x50) & (i++ < 0xff));
	msb = I20DAT & 0xff;
	I20CONCLR	=	I2CONSET_SI | I2CONSET_AA | I2CONSET_STO | I2CONSET_STA;
	
	i = 0;
	while ((I20STAT != 0x58) & (i++ < 0xff));
	lsb = I20DAT & 0xff;
	
	writeI2CStop();
	while(I20CONSET & I2CONSET_STO);
	
	//msb = 0x1c;
	//lsb = 0x90;
	//write what's on board :)
	retVal = (((msb & 0x7f) << 4) | ((lsb & 0xf0) >> 4));
	if ((msb & 0x80) != 0)
		retVal *= -1;

	return retVal;
}
