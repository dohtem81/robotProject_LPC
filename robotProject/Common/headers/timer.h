/*
 * timer.h
 *
 *  Created on: 2010-02-13
 *      Author: Piit
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef struct {
	char 	TCR;
	char 	CTCR;
	char 	IR;
	int		MCR;
	int 	EMR;
	int 	PC;
	int 	PR;
} TIMERSET;

//TCR register
#define TCR_EN		(0x1)
#define TCR_RST		(0x1<<1)

//CTCR register
#define CTCR_TMD	(0x0)	//timer mode
#define CTCR_CMDRE	(0x1)	//counter mode - rising edge
#define CTCR_CMDFE	(0x2)	//counter mode - falling edge
#define CTCR_CMDAE	(0x3)	//counter mode - any edge

//IR register
#define IR_MR0		(0x1)		//Interrupt flag for match channel 0
#define IR_MR1		(0x1<<1)	//Interrupt flag for match channel 1
#define IR_MR2		(0x1<<2)	//Interrupt flag for match channel 2
#define IR_MR3		(0x1<<3)	//Interrupt flag for match channel 3
#define IR_CR0		(0x1<<4)	//Interrupt flag for capture channel 0 event
#define IR_CR1		(0x1<<5)	//Interrupt flag for capture channel 1 event

//MCR register
#define MCR_MR0I	(0x1)		//Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
#define MCR_MR0R	(0x1<<1)	//the TC will be reset if MR0 matches it
#define MCR_MR0S	(0x1<<2)	//the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
#define MCR_MR1I	(0x1<<3)	//Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
#define MCR_MR1R	(0x1<<4)	//the TC will be reset if MR0 matches it
#define MCR_MR1S	(0x1<<5)	//the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
#define MCR_MR2I	(0x1<<6)	//Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
#define MCR_MR2R	(0x1<<7)	//the TC will be reset if MR0 matches it
#define MCR_MR2S	(0x1<<8)	//the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
#define MCR_MR3I	(0x1<<9)	//Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
#define MCR_MR3R	(0x1<<10)	//the TC will be reset if MR0 matches it
#define MCR_MR3S	(0x1<<11)	//the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC

//EMR register
#define	EMR_EM0		(0x1)		//when TC=MR0, this bit low, high, toggle depending on bits 5:4, goes out to MATn.0
#define	EMR_EM1		(0x1<<1)	//when TC=MR1, this bit low, high, toggle depending on bits 7:6, goes out to MATn.0
#define	EMR_EM2		(0x1<<2)	//when TC=MR2, this bit low, high, toggle depending on bits 9:8, goes out to MATn.0
#define	EMR_EM3		(0x1<<3)	//when TC=MR3, this bit low, high, toggle depending on bits 11:10, goes out to MATn.0
#define EMR_EMC0	4
#define EMR_EMC1	6
#define EMR_EMC2	8
#define EMR_EMC3	10

//valuse for EMCn
#define	EMC_NOP		0x0			//do nothing
#define EMC_CLR		0x1			//clear bit
#define EMC_SET		0x2			//set bit
#define	EMC_TGL		0x3			//toggle bit

#include "../headers/types.h"
uint initTimer0 (TIMERSET);
uint initTimer1 (TIMERSET);
uint startTimer(uint);
uint stopTimer(uint);

#endif /* TIMER_H_ */
