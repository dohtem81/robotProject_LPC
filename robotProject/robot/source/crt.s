/* ***************************************************************************************************************

	crt.s						STARTUP  ASSEMBLY  CODE 
								-----------------------


	Module includes the interrupt vectors and start-up code.

  *************************************************************************************************************** */

/* Stack Sizes */
.set  UND_STACK_SIZE, 0x00000004		/* stack for "undefined instruction" interrupts is 4 bytes  */
.set  ABT_STACK_SIZE, 0x00000004		/* stack for "abort" interrupts is 4 bytes                  */
.set  FIQ_STACK_SIZE, 0x00000004		/* stack for "FIQ" interrupts  is 4 bytes         			*/
.set  IRQ_STACK_SIZE, 0X00000100		/* stack for "IRQ" normal interrupts is 4 bytes    			*/
.set  SVC_STACK_SIZE, 0x00000100		/* stack for "SVC" supervisor mode is 4 bytes  				*/



/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs (program status registers) */
.set  MODE_USR, 0x10            		/* Normal User Mode 										*/
.set  MODE_FIQ, 0x11            		/* FIQ Processing Fast Interrupts Mode 						*/
.set  MODE_IRQ, 0x12            		/* IRQ Processing Standard Interrupts Mode 					*/
.set  MODE_SVC, 0x13            		/* Supervisor Processing Software Interrupts Mode 			*/
.set  MODE_ABT, 0x17            		/* Abort Processing memory Faults Mode 						*/
.set  MODE_UND, 0x1B            		/* Undefined Processing Undefined Instructions Mode 		*/
.set  MODE_SYS, 0x1F            		/* System Running Priviledged Operating System Tasks  Mode	*/

.set  I_BIT, 0x80               		/* when I bit is set, IRQ is disabled (program status registers) */
.set  F_BIT, 0x40               		/* when F bit is set, FIQ is disabled (program status registers) */


/* Set some addresses in memory */

#.extern initialise
#.extern LEDOnTask
#.extern LEDOffTask
#.extern ping
.extern tick

.text
.arm

#.global mainLoop
#.global mainLoop2
.global IRQ_Routine
.global FIQ_Routine
.global SWI_Routine
.global	Reset_Handler
.global _startup
#.func   _startup


_startup:

# Exception Vectors

_vectors:       ldr     PC, Reset_Addr         
                ldr     PC, Undef_Addr
                ldr     PC, SWI_Addr
                ldr     PC, PAbt_Addr
                ldr     PC, DAbt_Addr
                nop							/* Reserved Vector (holds Philips ISP checksum) */
#                ldr     PC, [PC,#-0xFF0]	/* see page 71 of "Insiders Guide to the Philips ARM7-Based Microcontrollers" by Trevor Martin  */
				ldr		PC, IRQ_Addr
                ldr     PC, FIQ_Addr

Reset_Addr:     .word   Reset_Handler		/* defined in this module below  */
Undef_Addr:     .word   UNDEF_Routine		/* defined in main.c  */
SWI_Addr:       .word   SWI_Routine			/* defined in main.c  */
PAbt_Addr:      .word   UNDEF_Routine		/* defined in main.c  */
DAbt_Addr:      .word   UNDEF_Routine		/* defined in main.c  */
IRQ_Addr:       .word   IRQ_Routine			/* defined in main.c  */
FIQ_Addr:       .word   FIQ_Routine			/* defined in main.c  */
                .word   0					/* rounds the vectors and ISR addresses to 64 bytes total  */

						.=.+0x1C0					/* added 2/19/2011 */

# Reset Handler

Reset_Handler:  

				/* Setup a stack for each mode - note that this only sets up a usable stack
				for User mode.   Also each mode is setup with interrupts initially disabled. */
    			  
    			ldr   r0, =_stack_end
    			msr   CPSR_c, #MODE_UND|I_BIT|F_BIT 	/* Undefined Instruction Mode  */
    			mov   sp, r0
    			sub   r0, r0, #UND_STACK_SIZE
    			msr   CPSR_c, #MODE_ABT|I_BIT|F_BIT 	/* Abort Mode */
    			mov   sp, r0
    			sub   r0, r0, #ABT_STACK_SIZE
    			msr   CPSR_c, #MODE_FIQ|I_BIT|F_BIT 	/* FIQ Mode */
    			mov   sp, r0	
   				sub   r0, r0, #FIQ_STACK_SIZE
    			msr   CPSR_c, #MODE_IRQ|I_BIT|F_BIT	 	/* IRQ Mode */
    			mov   sp, r0
    			sub   r0, r0, #IRQ_STACK_SIZE
    			msr   CPSR_c, #MODE_SVC|I_BIT|F_BIT 	/* Supervisor Mode */
    			mov   sp, r0
    			sub   r0, r0, #SVC_STACK_SIZE
    			msr   CPSR_c, #MODE_SYS|F_BIT  	/* User Mode */
    			mov   sp, r0

				/* copy .data section (Copy from ROM to RAM) */
                ldr     R1, =_etext
                ldr     R2, =_data
                ldr     R3, =_edata
1:        		cmp     R2, R3
                ldrlo   R0, [R1], #4
                strlo   R0, [R2], #4
                blo     1b

				/* Clear .bss section (Zero init)  */
                mov     R0, #0
                ldr     R1, =_bss_start
                ldr     R2, =_bss_end
2:				cmp     R1, R2
                strlo   R0, [R1], #4
                blo     2b

				/* board initialisation */
                bl       initialise

				/* Enter the C code  */
                /* b       main */

                mov		R0, #0
                ldr		R1, =0x3f000000

				/* all IO as outputs */
				#ldr     r2, =0xe0028008
				#ldr     r3, =0xffffffff
				#str     r3, [r2]

				mov     r5, #0

# mainLoop
mainLoop:
				#bl		checkETH
				b       mainLoop

UNDEF_Routine:
				b       UNDEF_Routine

SWI_Routine:
				b       SWI_Routine

IRQ_Routine:
				sub		r14, r14, #4
				stmfd	r13!, {r0-r3, r12, r14}

				/* disable interrupts */
				#ldr		r2, =0xfffff014
				#ldr		r1, =0x00000010
				#ldr		r1, [r2]

				bl		tick

                /* flash LED */
				ldr		r2, =0xe0028000
				ldr		r2, [r2]
				tst		r2, #0x00000002
				ldreq   r2, =0xe0028004 @// set
				ldrne   r2, =0xe002800c @// clr
				ldr		r4, =0x00000002
				str     r4, [r2]
				/* end-of flash LED */

				/* reset t0ir */
				ldr		r2, =0x00000001
				ldr		r6, =0xe0004000
				str		r2, [r6]

				/* reset vvecaddr */
				ldr		r2, =0x00000010
				ldr		r6, =0xfffff030
				str		r2, [r6]

				/* enable interrupts */
				#ldr		r2, =0xfffff010
				#ldr		r1, =0x00000010
				#ldr		r1, [r2]

				ldmfd	r13!, {r0-r3, r12, pc}^

FIQ_Routine:
				b       FIQ_Routine

.end
