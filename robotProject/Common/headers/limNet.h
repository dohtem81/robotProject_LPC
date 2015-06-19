/*
 * limNet.h
 *
 *  Created on: Mar 3, 2011
 *      Author: strus
 */

#ifndef LIMNET_H_
#define LIMNET_H_

typedef struct {
	unsigned short index;

	float     f_var1 ;     // comment f_var1
	float     f_var2 ;     // comment f_var2
	float     f_var3 ;     // comment f_var3
	float     f_var4 ;     // comment f_var4
	float     f_var5 ;     // comment f_var5
//	float     f_var6 ;     // comment f_var6
//	float     f_var7 ;     // comment f_var7
//	float     f_var8 ;     // comment f_var8
//	float     f_var9 ;     // comment f_var9
//	float     f_var10 ;     // comment f_var10
//	float     f_var11 ;     // comment f_var11
//	float     f_var12 ;     // comment f_var12
//	float     f_var13 ;     // comment f_var13
//	float     f_var14 ;     // comment f_var14
//	float     f_var15 ;     // comment f_var15
//	float     f_var16 ;     // comment f_var16
//	float     f_var17 ;     // comment f_var17
//	float     f_var18 ;     // comment f_var18
//	float     f_var19 ;     // comment f_var19
//	float     f_var20 ;     // comment f_var20
//	float     f_var21 ;     // comment f_var21
//	float     f_var22 ;     // comment f_var22
//	float     f_var23 ;     // comment f_var23
//	float     f_var24 ;     // comment f_var24
//	float     f_var25 ;     // comment f_var25
	int     i_var1 ;     // comment i_var1
	int     i_var2 ;     // comment i_var2
	int     i_var3 ;     // comment i_var3
	int     i_var4 ;     // comment i_var4
	int     i_var5 ;     // comment i_var5
	int     i_var6 ;     // comment i_var6
	int     i_var7 ;     // comment i_var7
	int     i_var8 ;     // comment i_var8
	int     i_var9 ;     // comment i_var9
	int     i_var10 ;     // comment i_var10
	int     i_var11 ;     // comment i_var11
	int     i_var12 ;     // comment i_var12
	int     i_var13 ;     // comment i_var13
	int     i_var14 ;     // comment i_var14
	int     i_var15 ;     // comment i_var15
	int     i_var16 ;     // comment i_var16
	int     i_var17 ;     // comment i_var17
	int     i_var18 ;     // comment i_var18
	int     i_var19 ;     // comment i_var19
	int     i_var20 ;     // comment i_var20
//	int     i_var21 ;     // comment i_var21
//	int     i_var22 ;     // comment i_var22
//	int     i_var23 ;     // comment i_var23
//	int     i_var24 ;     // comment i_var24
//	int     i_var25 ;     // comment i_var25
	long     l_var1 ;     // comment l_var1
	long     l_var2 ;     // comment l_var2
	long     l_var3 ;     // comment l_var3
	long     l_var4 ;     // comment l_var4
	long     l_var5 ;     // comment l_var5
	long     l_var6 ;     // comment l_var6
	long     l_var7 ;     // comment l_var7
	long     l_var8 ;     // comment l_var8
	long     l_var9 ;     // comment l_var9
	long     l_var10 ;     // comment l_var10
//	long     l_var11 ;     // comment l_var11
//	long     l_var12 ;     // comment l_var12
//	long     l_var13 ;     // comment l_var13
//	long     l_var14 ;     // comment l_var14
//	long     l_var15 ;     // comment l_var15
//	long     l_var16 ;     // comment l_var16
//	long     l_var17 ;     // comment l_var17
//	long     l_var18 ;     // comment l_var18
//	long     l_var19 ;     // comment l_var19
//	long     l_var20 ;     // comment l_var20
//	long     l_var21 ;     // comment l_var21
//	long     l_var22 ;     // comment l_var22
//	long     l_var23 ;     // comment l_var23
//	long     l_var24 ;     // comment l_var24
//	long     l_var25 ;     // comment l_var25
	unsigned char     b_word1 ;     // comment b_word1
	unsigned char     b_word2 ;     // comment b_word2
	unsigned char     b_word3 ;     // comment b_word3
	unsigned char     b_word4 ;     // comment b_word4
	unsigned char     b_word5 ;     // comment b_word5

	void (*f_var1Func)(void*, void*) ;
	void (*f_var2Func)(void*, void*) ;
	void (*f_var3Func)(void*, void*) ;
	void (*f_var4Func)(void*, void*) ;
	void (*f_var5Func)(void*, void*) ;
/*	void (*f_var6Func)(void*, void*) ;
	void (*f_var7Func)(void*, void*) ;
	void (*f_var8Func)(void*, void*) ;
	void (*f_var9Func)(void*, void*) ;
	void (*f_var10Func)(void*, void*) ;
	void (*f_var11Func)(void*, void*) ;
	void (*f_var12Func)(void*, void*) ;
	void (*f_var13Func)(void*, void*) ;
	void (*f_var14Func)(void*, void*) ;
	void (*f_var15Func)(void*, void*) ;
	void (*f_var16Func)(void*, void*) ;
	void (*f_var17Func)(void*, void*) ;
	void (*f_var18Func)(void*, void*) ;
	void (*f_var19Func)(void*, void*) ;
	void (*f_var20Func)(void*, void*) ;
	void (*f_var21Func)(void*, void*) ;
	void (*f_var22Func)(void*, void*) ;
	void (*f_var23Func)(void*, void*) ;
	void (*f_var24Func)(void*, void*) ;
	void (*f_var25Func)(void*, void*) ;*/
	void (*i_var1Func)(void*, void*) ;
	void (*i_var2Func)(void*, void*) ;
	void (*i_var3Func)(void*, void*) ;
	void (*i_var4Func)(void*, void*) ;
	void (*i_var5Func)(void*, void*) ;
	void (*i_var6Func)(void*, void*) ;
	void (*i_var7Func)(void*, void*) ;
	void (*i_var8Func)(void*, void*) ;
	void (*i_var9Func)(void*, void*) ;
	void (*i_var10Func)(void*, void*) ;
	void (*i_var11Func)(void*, void*) ;
	void (*i_var12Func)(void*, void*) ;
	void (*i_var13Func)(void*, void*) ;
	void (*i_var14Func)(void*, void*) ;
	void (*i_var15Func)(void*, void*) ;
	void (*i_var16Func)(void*, void*) ;
	void (*i_var17Func)(void*, void*) ;
	void (*i_var18Func)(void*, void*) ;
	void (*i_var19Func)(void*, void*) ;
	void (*i_var20Func)(void*, void*) ;
/*	void (*i_var21Func)(void*, void*) ;
	void (*i_var22Func)(void*, void*) ;
	void (*i_var23Func)(void*, void*) ;
	void (*i_var24Func)(void*, void*) ;
	void (*i_var25Func)(void*, void*) ;*/
	void (*l_var1Func)(void*, void*) ;
	void (*l_var2Func)(void*, void*) ;
	void (*l_var3Func)(void*, void*) ;
	void (*l_var4Func)(void*, void*) ;
	void (*l_var5Func)(void*, void*) ;
	void (*l_var6Func)(void*, void*) ;
	void (*l_var7Func)(void*, void*) ;
	void (*l_var8Func)(void*, void*) ;
	void (*l_var9Func)(void*, void*) ;
	void (*l_var10Func)(void*, void*) ;
/*	void (*l_var11Func)(void*, void*) ;
	void (*l_var12Func)(void*, void*) ;
	void (*l_var13Func)(void*, void*) ;
	void (*l_var14Func)(void*, void*) ;
	void (*l_var15Func)(void*, void*) ;
	void (*l_var16Func)(void*, void*) ;
	void (*l_var17Func)(void*, void*) ;
	void (*l_var18Func)(void*, void*) ;
	void (*l_var19Func)(void*, void*) ;
	void (*l_var20Func)(void*, void*) ;
	void (*l_var21Func)(void*, void*) ;
	void (*l_var22Func)(void*, void*) ;
	void (*l_var23Func)(void*, void*) ;
	void (*l_var24Func)(void*, void*) ;
	void (*l_var25Func)(void*, void*) ;*/
	void (*b_word1Func)(void*, void*) ;
	void (*b_word2Func)(void*, void*) ;
	void (*b_word3Func)(void*, void*) ;
	void (*b_word4Func)(void*, void*) ;
	void (*b_word5Func)(void*, void*) ;
} _limNetData ;


void inline limNet_initialize(void);
void inline limNet_postfloat(void *, void *);
void inline limNet_postint(void *, void *);
void inline limNet_postlong(void *, void *);
void inline limNet_postunsignedint(void *, void *);

#endif /* LIMNET_H_ */
