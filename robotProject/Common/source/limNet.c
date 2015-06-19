/*
 * limNet.c
 *
 *  Created on: Mar 3, 2011
 *      Author: strus
 */

#include "../headers/ethernet.h"
#include "../headers/limNet.h"
#include "../headers/lpc21xx.h"

_limNetData *limNetData;
struct _UDPMesg message;

/****************************************************************
 *
 * initialises data in the limNet
 * structure. no arguments passed
 *
 ***************************************************************/
void limNet_initialize(void){
	limNetData = (_limNetData *) ((message.mesgBody) - 2) ;

	limNetData->f_var1 = 1 ;
	limNetData->f_var2 = 2 ;
	limNetData->f_var3 = 3 ;
	limNetData->f_var4 = 4 ;
	limNetData->f_var5 = 5 ;
	/*
	limNetData->f_var6 = 6 ;
	limNetData->f_var7 = 7 ;
	limNetData->f_var8 = 8 ;
	limNetData->f_var9 = 9 ;
	limNetData->f_var10 = 10;
	limNetData->f_var11 = 11;
	limNetData->f_var12 = 12;
	limNetData->f_var13 = 13;
	limNetData->f_var14 = 14;
	limNetData->f_var15 = 15;
	limNetData->f_var16 = 16;
	limNetData->f_var17 = 17;
	limNetData->f_var18 = 18;
	limNetData->f_var19 = 19;
	limNetData->f_var20 = 20;
	limNetData->f_var21 = 21;
	limNetData->f_var22 = 22;
	limNetData->f_var23 = 23;
	limNetData->f_var24 = 24;
	limNetData->f_var25 = 25;
	*/
	limNetData->i_var1 = 1 ;
	limNetData->i_var2 = 2 ;
	limNetData->i_var3 = 3 ;
	limNetData->i_var4 = 4 ;
	limNetData->i_var5 = 5 ;
	limNetData->i_var6 = 6 ;
	limNetData->i_var7 = 7 ;
	limNetData->i_var8 = 8 ;
	limNetData->i_var9 = 9 ;
	limNetData->i_var10 = 10;
	limNetData->i_var11 = 11;
	limNetData->i_var12 = 12;
	limNetData->i_var13 = 13;
	limNetData->i_var14 = 14;
	limNetData->i_var15 = 15;
	limNetData->i_var16 = 16;
	limNetData->i_var17 = 17;
	limNetData->i_var18 = 18;
	limNetData->i_var19 = 19;
	limNetData->i_var20 = 20;
	/*
	limNetData->i_var21 = 21;
	limNetData->i_var22 = 22;
	limNetData->i_var23 = 23;
	limNetData->i_var24 = 24;
	limNetData->i_var25 = 25;
	*/
	limNetData->l_var1 = 1 ;
	limNetData->l_var2 = 2 ;
	limNetData->l_var3 = 3 ;
	limNetData->l_var4 = 4 ;
	limNetData->l_var5 = 5 ;
	limNetData->l_var6 = 6 ;
	limNetData->l_var7 = 7 ;
	limNetData->l_var8 = 8 ;
	limNetData->l_var9 = 9 ;
	limNetData->l_var10 = 10;
	/*
	limNetData->l_var11 = 11;
	limNetData->l_var12 = 12;
	limNetData->l_var13 = 13;
	limNetData->l_var14 = 14;
	limNetData->l_var15 = 15;
	limNetData->l_var16 = 16;
	limNetData->l_var17 = 17;
	limNetData->l_var18 = 18;
	limNetData->l_var19 = 19;
	limNetData->l_var20 = 20;
	limNetData->l_var21 = 21;
	limNetData->l_var22 = 22;
	limNetData->l_var23 = 23;
	limNetData->l_var24 = 24;
	limNetData->l_var25 = 25;
	*/
	limNetData->b_word1 = 1 ;
	limNetData->b_word2 = 2 ;
	limNetData->b_word3 = 3 ;
	limNetData->b_word4 = 4 ;
	limNetData->b_word5 = 5 ;
	/*
	limNetData->b_word6 = 6 ;
	limNetData->b_word7 = 7 ;
	limNetData->b_word8 = 8 ;
	limNetData->b_word9 = 9 ;
	limNetData->b_word10 = 10;
	*/


/*	limNetData.f_var1Func = limNet_postfloat ;
	limNetData.f_var2Func = limNet_postfloat ;
	limNetData.f_var3Func = limNet_postfloat ;
	limNetData.f_var4Func = limNet_postfloat ;
	limNetData.f_var5Func = limNet_postfloat ;
	limNetData.f_var6Func = limNet_postfloat ;
	limNetData.f_var7Func = limNet_postfloat ;
	limNetData.f_var8Func = limNet_postfloat ;
	limNetData.f_var9Func = limNet_postfloat ;
	limNetData.f_var10Func = limNet_postfloat ;
	limNetData.f_var11Func = limNet_postfloat ;
	limNetData.f_var12Func = limNet_postfloat ;
	limNetData.f_var13Func = limNet_postfloat ;
	limNetData.f_var14Func = limNet_postfloat ;
	limNetData.f_var15Func = limNet_postfloat ;
	limNetData.f_var16Func = limNet_postfloat ;
	limNetData.f_var17Func = limNet_postfloat ;
	limNetData.f_var18Func = limNet_postfloat ;
	limNetData.f_var19Func = limNet_postfloat ;
	limNetData.f_var20Func = limNet_postfloat ;
	limNetData.f_var21Func = limNet_postfloat ;
	limNetData.f_var22Func = limNet_postfloat ;
	limNetData.f_var23Func = limNet_postfloat ;
	limNetData.f_var24Func = limNet_postfloat ;
	limNetData.f_var25Func = limNet_postfloat ;
	limNetData.i_var1Func = limNet_postint ;
	limNetData.i_var2Func = limNet_postint ;
	limNetData.i_var3Func = limNet_postint ;
	limNetData.i_var4Func = limNet_postint ;
	limNetData.i_var5Func = limNet_postint ;
	limNetData.i_var6Func = limNet_postint ;
	limNetData.i_var7Func = limNet_postint ;
	limNetData.i_var8Func = limNet_postint ;
	limNetData.i_var9Func = limNet_postint ;
	limNetData.i_var10Func = limNet_postint ;
	limNetData.i_var11Func = limNet_postint ;
	limNetData.i_var12Func = limNet_postint ;
	limNetData.i_var13Func = limNet_postint ;
	limNetData.i_var14Func = limNet_postint ;
	limNetData.i_var15Func = limNet_postint ;
	limNetData.i_var16Func = limNet_postint ;
	limNetData.i_var17Func = limNet_postint ;
	limNetData.i_var18Func = limNet_postint ;
	limNetData.i_var19Func = limNet_postint ;
	limNetData.i_var20Func = limNet_postint ;
	limNetData.i_var21Func = limNet_postint ;
	limNetData.i_var22Func = limNet_postint ;
	limNetData.i_var23Func = limNet_postint ;
	limNetData.i_var24Func = limNet_postint ;
	limNetData.i_var25Func = limNet_postint ;
	limNetData.l_var1Func = limNet_postlong ;
	limNetData.l_var2Func = limNet_postlong ;
	limNetData.l_var3Func = limNet_postlong ;
	limNetData.l_var4Func = limNet_postlong ;
	limNetData.l_var5Func = limNet_postlong ;
	limNetData.l_var6Func = limNet_postlong ;
	limNetData.l_var7Func = limNet_postlong ;
	limNetData.l_var8Func = limNet_postlong ;
	limNetData.l_var9Func = limNet_postlong ;
	limNetData.l_var10Func = limNet_postlong ;
	limNetData.l_var11Func = limNet_postlong ;
	limNetData.l_var12Func = limNet_postlong ;
	limNetData.l_var13Func = limNet_postlong ;
	limNetData.l_var14Func = limNet_postlong ;
	limNetData.l_var15Func = limNet_postlong ;
	limNetData.l_var16Func = limNet_postlong ;
	limNetData.l_var17Func = limNet_postlong ;
	limNetData.l_var18Func = limNet_postlong ;
	limNetData.l_var19Func = limNet_postlong ;
	limNetData.l_var20Func = limNet_postlong ;
	limNetData.l_var21Func = limNet_postlong ;
	limNetData.l_var22Func = limNet_postlong ;
	limNetData.l_var23Func = limNet_postlong ;
	limNetData.l_var24Func = limNet_postlong ;
	limNetData.l_var25Func = limNet_postlong ;
	limNetData.b_word1Func = limNet_postunsignedint ;
	limNetData.b_word2Func = limNet_postunsignedint ;
	limNetData.b_word3Func = limNet_postunsignedint ;
	limNetData.b_word4Func = limNet_postunsignedint ;
	limNetData.b_word5Func = limNet_postunsignedint ;
	limNetData.b_word6Func = limNet_postunsignedint ;
	limNetData.b_word7Func = limNet_postunsignedint ;
	limNetData.b_word8Func = limNet_postunsignedint ;
	limNetData.b_word9Func = limNet_postunsignedint ;
	limNetData.b_word10Func = limNet_postunsignedint ;*/


}


/****************************************************************
 *
 * copy and post data on the network
 *
 ***************************************************************/
void limNet_postData(void){
	//struct _UDPMesg message;

	struct _MACAddress serverMAC = {SERVER_MAC1, SERVER_MAC2, SERVER_MAC3};
	struct _IPFrame *frame;

	// index is same as length;
	message.length= 150 ;
	sendUDPFrame(frame, serverMAC, 0xc2f9, 0xc2f7, &message);
}


/****************************************************************
 *
 * post float number on network
 *
 ***************************************************************/
void inline limNet_postfloat(void *passedData, void *msg){
	float *data, *pointer;
	struct _UDPMesg *message;

	message = (struct _UDPMesg *)msg;

	pointer = (float *)(&message->mesgBody[limNetData->index]);
	*pointer = *((float *)passedData);

	//message->length += 4;
	limNetData->index += 4;
}


/****************************************************************
 *
 * post int number on network
 *
 ***************************************************************/
void inline limNet_postint(void *passedData, void *msg){
	int *data, *pointer;
	struct _UDPMesg *message;

	message = (struct _UDPMesg *)msg;

	pointer = (int *)(&message->mesgBody[limNetData->index]);
	*pointer = *((int *)passedData);

	//message->length += 2;
	limNetData->index += 4;
}


/****************************************************************
 *
 * post long number on network
 *
 ***************************************************************/
void inline limNet_postlong(void *passedData, void *msg){
	long *data, *pointer;
	struct _UDPMesg *message;

	message = (struct _UDPMesg *)msg;

	pointer = (long *)(&message->mesgBody[limNetData->index]);
	*pointer = *((long *)passedData);

	//message->length += 4;
	limNetData->index += 8;
}

/****************************************************************
 *
 * post unsigned int number on network
 *
 ***************************************************************/
void inline limNet_postunsignedint(void *passedData, void *msg){
	unsigned int *data, *pointer;
	struct _UDPMesg *message;

	message = (struct _UDPMesg *)msg;

	pointer = (unsigned int *)(&message->mesgBody[limNetData->index]);
	*pointer = *((unsigned int *)passedData);

	//message->length += 2;
	limNetData->index += 2;
}

