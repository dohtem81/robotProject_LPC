// --------------------------------------------------------------------------
//
//		System functions
//
// --------------------------------------------------------------------------

// include files - specific for CPU, board
#include "../headers/configuration.h"
#include "../../Common/headers/types.h"
#include "../../Common/headers/timer.h"
#include "../headers/System.h"
#include "../../Common/headers/ENC28J60.h"
#include "../../Common/headers/spi.h"
#include "../../Common/headers/ethernet.h"
#include "../../Common/headers/limNet.h"
#include "../../Common/headers/i2c.h"
#include "../../Common/headers/tmp102.h"
#include "../../Common/headers/pwm.h"
//#include "../../Common/headers/IAP.h"
unsigned int IAPRead(unsigned int[], unsigned int[]);
unsigned int command[5];
unsigned int result[2];
int fact;

// global variables used for configuration and data
BYTE 	frame[42];
WORD 	*ptr; 			//42 bytes is ARP frame


// external functions and variables
extern _limNetData *limNetData;

// definitions for local functions

uint initClock ();
uint initPher ();
uint setupGPIO () ;
uint initVICVect(uint) ;
uint initENC28J60 ();
uint tick ();
uint checkETH ();

// --------------------------------------------------------------------------
//
// performs CPU & board initialisation
// 1. sets-up clock
// 2. sets-up pheriphrials
//
// --------------------------------------------------------------------------
uint initialise(void){

	// call init clock
	if ( ! initClock() )
		return 0 ;

	long j;

	for (j=0; j<1000000; j++);

	// set up all pheripherials
	if ( ! initPher() )
		return 0 ;


	// initialise ENC28J60
	if ( ! initENC28J60() )
		return 0;

	spi_initialize(SETUP_SPI0);
	enc_initialise();
	initializeFrame();
	limNet_initialize();
	initI2C(0);


	pwm_initialiseMasterTimer(0x48000);
	pwm_initialize(1, PWM_SINGLE, 0x20000, PWM_RST, 1) ;
	pwm_initialize(6, PWM_SINGLE, 0x20000, PWM_RST, 1) ;

	pwm_enable();

	sendARP();

	if ( ! startTimer( 0 ) )
		return 0 ;
	// bit  4 - T0
	// bit 10 - SPI0
	VICIntEnable = 	0x1 << 4 ;//|| 0x1 << 10 ;

	// enable drive
	IO0SET	|= 0x1 << 30 ;

	// set direction
	IO0SET	|= 0x1 << 11 ;
	IO0CLR	|= 0x1 << 12 ;
	IO0SET  |= 0x1 << 15 ;
	IO0CLR	|= 0x1 << 14 ;

	fact = 0;

	return 1;
}


//--------------------------------------------------------------------------
//
// sets up clock
// no inputs
// returns 1 for success
// used addresses
//     - PLLCON
//     - PLLCFG
//     - PLLSTAT
//     - PLLFEED
//
//--------------------------------------------------------------------------
uint initClock () {

	PLLCON = 0x0;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	// ---------------------------------------------------------------------
	//M=2(MSEL 1),P=4(PSEL 2) FCCO = ~240Mhz, CCLK = ~30Mhz, OSC 14.7456
	PLLCFG = 0x00000021;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	// Enable the PLL
	PLLCON  = 0x01;
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	while(!(PLLSTAT & (0x1 << 10)));  // Wait for PLL to be enabled

	PLLCON 	|=	0x2;		// connect PLL
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
	while(!(PLLSTAT & (0x1 << 9)));

	//Init MAM & Flash memory fetch
	MAMCR = 0x02;
	MAMTIM = 0x04;
	// ---------------------------------------------------------------------


	return 1;
}


//--------------------------------------------------------------------------
//
// sets up pheripherials
// no inputs
// returns 1 for success
//
//--------------------------------------------------------------------------
uint initPher () {

	// set up direction for GPIO
	if ( ! setupGPIO() )
		return 0 ;

	TIMERSET tSetup;
	tSetup.TCR	=	0;	//not needed
	tSetup.CTCR	=	CTCR_TMD;
	tSetup.IR	=	IR_MR0;
	tSetup.MCR	=	MCR_MR0I + MCR_MR0R;
	tSetup.EMR	=	EMR_EM0 + (EMC_TGL << EMR_EMC0);
	tSetup.PC	=	0x1;
	tSetup.PR	=	0x100;

	//inits timer
	if ( ! initTimer0(tSetup) )
		return 0 ;

	return 1;
}

//--------------------------------------------------------------------------
//
// sets up GPIO
// no inputs
// returns 1 for success
//
//--------------------------------------------------------------------------
uint setupGPIO () {

	// setup GPIO direction
	IO0DIR		=	IO0DIR_DEF ;
	IO1DIR		=	IO1DIR_DEF ;

	//PINSEL0 = 	0x000c0000;
	PINSEL1 = 	0x00500000;
	return 1 ;
}


//--------------------------------------------------------------------------
//
// sets up interrupt vectors
// vector number
// returns 1 for success
//
//--------------------------------------------------------------------------
uint initVICVect(uint vectNo) {

	//evaluate input
	if ( ( vectNo < 0 ) || (vectNo > 31) )
		return 0 ;

	VICIntSelect = 0x00000000;
	VICIntEnable = 0x00020010;

	//VICVectAddr4 = /* function for TIMER0 interrupt

	return 1;
}


//--------------------------------------------------------------------------
//
// sets up interrupt vectors
// vector number
// returns 1 for success
//
//--------------------------------------------------------------------------
uint initENC28J60 () {

	//init ARP packet

	//sets pointer at begining fo the frame
	ptr 		=	(unsigned short *)frame;
	//first 3 words is destination MAC - broadcast	[0-2]
	*(ptr++)		=	0xffff;
	*(ptr++)		=	0xffff;
	*(ptr++)		=	0xffff;

	//3 wrods source MAC (this station)	[3-5]
	*(ptr++)		=	ETH_MAC1;
	*(ptr++)		=	ETH_MAC2;
	*(ptr++)		=	ETH_MAC3;

	//sets type as ARP	[6]
	*(ptr++)		=	TYPE_ARP;

	//hardware type - ethernet	[7]
	*(ptr++)		=	ARP_ETH;

	//protocol type IP	[8]
	*(ptr++)		=	TYPE_IP;

	//hardware size and protocol size (contant)		[9]
	*(ptr++)		=	ARP_HDPS;

	//opcode (is gratuitous)	[10]
	*(ptr++)		=	0x0001;

	//sorce MAC again...	[11-13]
	*(ptr++)		=	ETH_MAC1;
	*(ptr++)		=	ETH_MAC2;
	*(ptr++)		=	ETH_MAC3;

	//source IP address		[14-15]
	*(ptr++)		=	(IP1 << 8) | IP2;
	*(ptr++)		=	(IP3 << 8) | IP4;

	//destination MAC - this time 0s	[16-18]
	*(ptr++)		=	0x0000;
	*(ptr++)		=	0x0000;
	*(ptr++)		=	0x0000;

	//destination IP
	*(ptr++)		=	(192 << 8) + 168;
	*ptr			=	(100 << 8) + 1;

	return 1;
}

//--------------------------------------------------------------------------
//
//	not sure what it does ;)
//
//--------------------------------------------------------------------------
uint tick () {
	// check for pending messages
	checkETH();

	// motor control
	//=========================================================================================
	PWM_MR1 = PWM_MR1 + 0x1000;
	PWM_MR6 = PWM_MR6 + 0x1000;

	if (PWM_MR1 > 0x40000) {
		PWM_MR1 = 0x1000;
		PWM_MR6 = 0x1000;
	}

	if (fact > 100){
		IO0CLR |= 0x1 << 11 ;
		IO0SET |= 0x1 << 12 ;
		IO0CLR |= 0x1 << 15 ;
		IO0SET |= 0x1 << 14 ;
	} else {
		IO0SET |= 0x1 << 11 ;
		IO0CLR |= 0x1 << 12 ;
		IO0SET |= 0x1 << 15 ;
		IO0CLR |= 0x1 << 14 ;
	}

	PWM_LER	=	PWM_PMW1_LER | PWM_PMW6_LER;

	//network activities
	if (!(fact % 10)) {
		//==========================================================================================
		limNetData->f_var1 = (float)I2CreadRegister(TMP_REG, TMP_WR) * 0.0625 ;
		limNetData->f_var2 = (float)I2CreadRegister(TMP_REG, TMP_WR + 2) * 0.0625 ;
		limNetData->f_var3 = (float)I2CreadRegister(TMP_REG, TMP_WR + 4) * 0.0625 ;

		limNetData->f_var1 = (float)IAPRead(command, result);

		limNet_postData();
	}


	fact++;
	if (fact>200) fact = 0;

	return 1;
}


//--------------------------------------------------------------------------
//
//	checks if anything in RX buffer
//
//--------------------------------------------------------------------------
uint checkETH () {

	unsigned char que;

	while ((que = enc_read((unsigned char *)returnRXFramePointer())) > 0){
		processFrame(0, (unsigned short*)&que);

	}

	return 0;
}
