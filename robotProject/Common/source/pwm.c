/*
 * pwm.c
 *
 *  Created on: Jul 21, 2012
 *      Author: strus
 */

#include "../headers/pwm.h"
#include "../headers/lpc21xx.h"

// i- interrupt, r- reset, s- stop (irs)
int pwm_initialize (int port, int mode, unsigned long match_cnt, int irs, int out) {

	//currently only single mode implemented
	if (mode == PWM_DOUBLE)
		return 0 ;
	else if (mode == PWM_SINGLE) {
		// setting up single mode

		if ( (port < 0) || (port > 6) )
			// incorrect port
			return 0;

		int whichPinSel ;
		int bitOffsetForPinSel ;
		int bitValueForPinSel ;
		unsigned long mcrValue = 0 ;
		unsigned long outEnable = 0;
		unsigned long latch = 0;
		unsigned long * MR ;

		switch (port){
		case 1:
			whichPinSel 		= PWM_PWM1_PINSEL;
			bitOffsetForPinSel 	= PWM_PWM1_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM1_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM1_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM1_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM1_MRS ;
			if (out > 0)
				outEnable = PWM_PWM1_OUTEN;
			latch = PWM_PMW1_LER ;
			MR = &PWM_MR1;
			break;
		case 2:
			whichPinSel 		= PWM_PWM2_PINSEL;
			bitOffsetForPinSel 	= PWM_PWM2_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM2_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM2_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM2_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM2_MRS ;
			if (out > 0)
				outEnable = PWM_PWM2_OUTEN;
			latch = PWM_PMW2_LER ;
			MR = &PWM_MR2;
			break;
		case 3:
			whichPinSel 		= PWM_PWM3_PINSEL;
			bitOffsetForPinSel 	= PWM_PWM3_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM3_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM3_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM3_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM3_MRS ;
			if (out > 0)
				outEnable = PWM_PWM3_OUTEN;
			latch = PWM_PMW3_LER ;
			MR = &PWM_MR3;
			break;
		case 4:
			whichPinSel 		= PWM_PWM4_PINSEL;
			bitOffsetForPinSel	= PWM_PWM4_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM4_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM4_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM4_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM4_MRS ;
			if (out > 0)
				outEnable = PWM_PWM4_OUTEN;
			latch = PWM_PMW4_LER ;
			MR = &PWM_MR4;
			break;
		case 5:
			whichPinSel 		= PWM_PWM5_PINSEL;
			bitOffsetForPinSel 	= PWM_PWM5_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM5_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM5_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM5_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM5_MRS ;
			if (out > 0)
				outEnable = PWM_PWM5_OUTEN;
			latch = PWM_PMW5_LER ;
			MR = &PWM_MR5;
			break;
		case 6:
			whichPinSel 		= PWM_PWM6_PINSEL;
			bitOffsetForPinSel 	= PWM_PWM6_PINSELSHIIFT ;
			bitValueForPinSel 	= PWM_PWM6_PINSEL_BITVALUE ;
			if ((irs & 0x1) > 0)
				mcrValue |= PWM_PWM6_MRI ;
			if ((irs & 0x2) > 0)
				mcrValue |= PWM_PWM6_MRR ;
			if ((irs & 0x4) > 0)
				mcrValue |= PWM_PWM6_MRS ;
			if (out > 0)
				outEnable = PWM_PWM6_OUTEN;
			latch = PWM_PMW6_LER ;
			MR = &PWM_MR6;
			break;
		}

		switch (whichPinSel){
		case 0:
			PINSEL0 |=	bitValueForPinSel << bitOffsetForPinSel;
			break;
		case 1:
			PINSEL1 |=	bitValueForPinSel << bitOffsetForPinSel;
			break;
		}
		// enable output
		PWM_PCR |= outEnable ;

		// set match counter
		*MR = match_cnt;

		// latch
		PWM_LER |= latch ;

		return 1 ;

	} else
		//incorrect mode
		return 0;

}


void pwm_enable(void) {

	//enable counter and PWM
	PWM_TCR = PWM_TCR_EN + PWM_TCR_PEN ;

}



void pwm_disable(void) {

	// reset timer counter
	PWM_TCR = 0;

}

void pwm_reset(void) {

	// reset timer counter
	PWM_TCR = PWM_TCR_RST;

}


void pwm_initialiseMasterTimer(unsigned long counterMaxValue){
	// reset timer and disable
	PWM_TCR = PWM_TCR_RST;
	// set prescale counter
	PWM_PR = 0x0;
	// set MCR bit: 0-interrupt, 1-reset, 2-stop on match
	PWM_MCR |= PWM_RST;
	// counter
	PWM_MR0 = counterMaxValue;
	// latch valuse
	PWM_LER |= PWM_PMW0_LER ;
	// enable PWM
	PWM_TCR = PWM_TCR_EN | PWM_TCR_PEN;
}

