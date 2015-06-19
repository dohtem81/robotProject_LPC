/*
 * timer.c
 *
 *  Created on: 2010-02-13
 *      Author: Piit
 */

#include "../headers/lpc21xx.h"
#include "../headers/timer.h"
#include "../headers/types.h"


//--------------------------------------------------------------------------
//
// inits timer 0
// TIMERSET struct
// returns 1 for success
//
//--------------------------------------------------------------------------
uint initTimer0(TIMERSET ts){
	TIMER0_TCR	=	TCR_RST; 	//reset & disable TIMER0_
	TIMER0_IR	=	ts.IR;
	TIMER0_CTCR	=	ts.CTCR; 	//timer mode
	TIMER0_PR	=	ts.PR; 	//max value for prescale counter
	TIMER0_MR0	=	0x1; 		//
	TIMER0_MCR	=	ts.MCR;		//when MR0_ macthes TC
	TIMER0_EMR	=	ts.EMR;

	return 1 ;
}

//--------------------------------------------------------------------------
//
// inits timer 1
// TIMERSET struct
// returns 1 for success
//
//--------------------------------------------------------------------------
uint initTimer1(TIMERSET ts){
	TIMER1_TCR	=	TCR_RST; 	//reset & disable TIMER0_
	TIMER1_IR	=	ts.IR;
	TIMER1_CTCR	=	ts.CTCR; 	//timer mode
	TIMER1_PR	=	ts.PR; 	//max value for prescale counter
	TIMER1_MR0	=	0x1; 		//
	TIMER1_MCR	=	ts.MCR;		//when MR0_ macthes TC
	TIMER1_EMR	=	ts.EMR;

	return 1 ;
}

//--------------------------------------------------------------------------
//
// starts timer specified by input
// uint timer
// returns 1 for success
//
//--------------------------------------------------------------------------
uint startTimer(uint timer) {
	//evaluate timer number
	if ( ( timer < 0 ) || ( timer > 1 ) )
		return 0 ;

	if ( timer == 0 ) {
		TIMER0_TCR	=	TCR_EN;
		return 1 ;
	}

	if ( timer == 1 ) {
		TIMER1_TCR	=	TCR_EN;
		return 1 ;
	}

	return 0 ; // should never reach that point
}


//--------------------------------------------------------------------------
//
// stops timer specified by input
// uint timer
// returns 1 for success
//
//--------------------------------------------------------------------------
uint stopTimer(uint timer) {
	//evaluate timer number
	if ( ( timer < 0 ) || ( timer > 1 ) )
		return 0 ;

	if ( timer == 0 ) {
		TIMER0_TCR	=	0;
		return 1 ;
	}

	if ( timer == 1 ) {
		TIMER1_TCR	=	0;
		return 1 ;
	}

	return 0 ; // should never reach that point
}
