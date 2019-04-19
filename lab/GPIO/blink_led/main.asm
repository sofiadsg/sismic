;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

tempo0		.equ 65535
tempo1		.equ 131070
tempo2		.equ 262140
tempo3 		.equ 524280
			call	#main
			jmp	$

main: 		bis.b	#BIT0, &P1DIR
			bis.b	#BIT0, &P1OUT
			bis.b	#BIT7, &P4DIR
			bic.b	#BIT7, &P4OUT
			bic.b	#BIT1, &P1DIR
			bis.b	#BIT1, &P1REN
			bis.b 	#BIT1, &P1OUT
			mov.b	#0x00, R5
loop:		xor.b	#0x01, &P1OUT
			xor.b	#BIT7, &P4OUT
			bit.b	#0x02, &P1IN
			jz		incCont
			jmp		gastartemp

gastartemp: cmp.b #0,R5
			jne	caso1
			mov #tempo0, R4
			jmp looptempo
caso1:		cmp.b #1,R5
			jne caso2
			mov #tempo1, R4
			jmp looptempo
caso2:		cmp.b #2,R5
			jne caso3
			mov #tempo2, R4
			jmp looptempo
caso3:		cmp.b #3,R5
			jne gastartemp
			mov #tempo1, R4
			jmp looptempo
looptempo:	dec R4
			jnz looptempo
			jmp loop
			nop
                                            
incCont: inc  	R5
	     cmp.b	#4,R5
	     jge ZeroR5
	     jmp gastartemp

ZeroR5:	 mov.b #0, R5
		 jmp gastartemp
		 nop
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
