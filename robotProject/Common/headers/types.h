/*
 * types.h
 *
 *  Created on: Jun 9, 2011
 *      Author: strus
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned int 			uint ;
typedef unsigned int			BOOL ;
typedef unsigned char 			BYTE ;
typedef unsigned short			WORD ;
typedef unsigned long           DWORD ;


#define 	TRUE				1
#define 	FALSE				0
// structures

typedef struct
{
	BYTE dstAddress[6];
	BYTE srcAddress[6];
	BYTE Type;
	BYTE bytData[1000];
} tMACPacket;


typedef struct {
	WORD suiHTYPE;  //2
	WORD suiPTYPE;  //4
	BYTE bytHLEN;   //5
	BYTE bytPLEN;   //6
	WORD suiOPER;   //8
	BYTE bytSHA[6]; //14
	BYTE bytSPA[4]; //16
	BYTE bytTHA[6]; //22
	BYTE bytTPA[4]; //26
} tARP;

#endif /* TYPES_H_ */
