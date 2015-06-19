/*
 * IAP.h
 *
 *  Created on: Jun 18, 2012
 *      Author: strus
 */

#include "../headers/IAP.h"
#include "../../Common/headers/lpc21xx.h"

unsigned int command[5];
unsigned int result[2];

IAP iap_entry = (IAP) IAP_LOCATION;

void initArrays(){
	command[0] = 0;
	command[1] = 0;
	command[2] = 0;
	command[3] = 0;
	command[4] = 0;

	result[0] = 0;
	result[1] = 0;
}


unsigned int IAP_checkID(void){
	initArrays();
	unsigned long intRegister;

	intRegister = VICIntEnable;

	VICIntEnable = 	0x0;

	command[0]	=	IAP_READ_PARTID;

//	iap_entry(command, result);

	VICIntEnable = 	intRegister;
	return 0;// result[1];
}
