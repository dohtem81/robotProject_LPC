/*
 * IAP.h
 *
 *  Created on: Jun 18, 2012
 *      Author: strus
 */

#ifndef IAP_H_
#define IAP_H_

#define IAP_LOCATION		0x7ffffff1

// IAP commands
#define 	IAP_PREP					50
#define 	IAP_CPY_RAM2FLASH			51
#define 	IAP_ERASE					52
#define		IAP_BLANK_CHK				53
#define		IAP_READ_PARTID				54
#define		IAP_READ_BOOTCODEVER		55
#define 	IAP_COMPARE					56

// IAP status codes
#define		IAP_CMD_SUCCESS				0
#define		IAP_INVALID_COMMAND			1
#define		IAP_SRC_ADDR_ERROR			2
#define		IAP_DST_ADDR_ERROR			3
#define		IAP_SRC_ADDR_NOT_MAPPED		4
#define		IAP_DST_ADDR_NOT_MAPPED		5
#define		IAP_COUNT_ERROR				6
#define		IAP_INVALID_SECTOR			7
#define		IAP_SECTOR_NOT_BLANK		8
#define		IAP_SECTOR_NOT_PREPARED		9
#define		IAP_COMPARE_ERROR			10
#define		IAP_BUSY					11

//functions
unsigned int IAP_checkID(void);

// other
typedef void (*IAP)(unsigned int[], unsigned int[]);

#endif

