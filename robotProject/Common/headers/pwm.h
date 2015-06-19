/*
 * pwm.h
 *
 *  Created on: Jul 21, 2012
 *      Author: strus
 */

#ifndef PWM_H_
#define PWM_H_

// definitions for pinsel's shifting
#define 	PWM_PWM1_PINSELSHIIFT		 0
#define 	PWM_PWM2_PINSELSHIIFT		14
#define 	PWM_PWM3_PINSELSHIIFT		 2
#define 	PWM_PWM4_PINSELSHIIFT		16
#define 	PWM_PWM5_PINSELSHIIFT		10
#define 	PWM_PWM6_PINSELSHIIFT		18

// bit value for PWMs 1-6 excluding 5
#define		PWM_PWM1_PINSEL_BITVALUE	0x2
#define		PWM_PWM2_PINSEL_BITVALUE	0x2
#define		PWM_PWM3_PINSEL_BITVALUE	0x2
#define		PWM_PWM4_PINSEL_BITVALUE	0x2
#define		PWM_PWM5_PINSEL_BITVALUE    0x1
#define		PWM_PWM6_PINSEL_BITVALUE	0x2

// PINSEL selection
#define		PWM_PWM1_PINSEL				0
#define		PWM_PWM2_PINSEL				0
#define		PWM_PWM3_PINSEL				0
#define		PWM_PWM4_PINSEL				0
#define		PWM_PWM5_PINSEL				1
#define		PWM_PWM6_PINSEL				0

// PCR settings
// single/double edge setting
#define		PWM_PWM0_SINGLE				0x0
#define		PWM_PWM0_DOUBLE				0x1
#define		PWM_PWM1_SINGLE				( 0xffffffff - 0x2 )
#define		PWM_PWM1_DOUBLE				0x2
#define		PWM_PWM2_SINGLE				( 0xffffffff - 0x4 )
#define		PWM_PWM2_DOUBLE				0x4
#define		PWM_PWM3_SINGLE				( 0xffffffff - 0x8 )
#define		PWM_PWM3_DOUBLE				0x8
#define		PWM_PWM4_SINGLE				( 0xffffffff - 0x10 )
#define		PWM_PWM4_DOUBLE				0x10
#define		PWM_PWM5_SINGLE				( 0xffffffff - 0x20 )
#define		PWM_PWM5_DOUBLE				0x20
#define		PWM_PWM6_SINGLE				( 0xffffffff - 0x40 )
#define		PWM_PWM6_DOUBLE				0x40

//output enable
#define		PWM_PWM1_OUTEN				( 0x1 << 9 )
#define		PWM_PWM2_OUTEN				( 0x1 << 10 )
#define		PWM_PWM3_OUTEN				( 0x1 << 11 )
#define		PWM_PWM4_OUTEN				( 0x1 << 12 )
#define		PWM_PWM5_OUTEN				( 0x1 << 13 )
#define		PWM_PWM6_OUTEN				( 0x1 << 14 )

// latch match register
#define		PWM_PMW0_LER				0x1
#define		PWM_PMW1_LER				0x2
#define		PWM_PMW2_LER				0x4
#define		PWM_PMW3_LER				0x8
#define		PWM_PMW4_LER				0x10
#define		PWM_PMW5_LER				0x20
#define		PWM_PMW6_LER				0x40

// PWM int / reset / stop on match
#define		PWM_PWM0_MRI				0x1
#define		PWM_PWM0_MRR				0x2
#define		PWM_PWM0_MRS				0x4
#define		PWM_PWM1_MRI				0x8
#define		PWM_PWM1_MRR				0x10
#define		PWM_PWM1_MRS				0x20
#define		PWM_PWM2_MRI				0x40
#define		PWM_PWM2_MRR				0x80
#define		PWM_PWM2_MRS				0x100
#define		PWM_PWM3_MRI				0x200
#define		PWM_PWM3_MRR				0x400
#define		PWM_PWM3_MRS				0x800
#define		PWM_PWM4_MRI				0x1000
#define		PWM_PWM4_MRR				0x2000
#define		PWM_PWM4_MRS				0x4000
#define		PWM_PWM5_MRI				0x8000
#define		PWM_PWM5_MRR				0x10000
#define		PWM_PWM5_MRS				0x20000
#define		PWM_PWM6_MRI				0x40000
#define		PWM_PWM6_MRR				0x80000
#define		PWM_PWM6_MRS				0x100000

// PWM interrupt channels for PWMIR
#define		PWM_PWM0_IR					0x1
#define		PWM_PWM1_IR					0x2
#define		PWM_PWM2_IR					0x4
#define		PWM_PWM3_IR					0x8
#define		PWM_PWM4_IR					0x100
#define		PWM_PWM5_IR					0x200
#define		PWM_PWM6_IR					0x400

// counter enable / reset / PWM enable
#define		PWM_TCR_EN					0x1
#define		PWM_TCR_RST					0x2
#define		PWM_TCR_PEN					0x8

// on reg match int / reset / stop
#define		PWM_INT						0x1
#define		PWM_RST						0x2
#define		PWM_STP						0x4

// power control to PWM block enable
#define		PWM_PCONP_EN				( 0x1 << 5 )

// PWM modes
#define		PWM_SINGLE					0
#define		PWM_DOUBLE					1

// function declarations
int pwm_initialize (int, int, unsigned long, int, int);
void pwm_initialiseMasterTimer(unsigned long);
void pwm_enable();
void pwm_disable();

#endif /* SPI_H_ */
