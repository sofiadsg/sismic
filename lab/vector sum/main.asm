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
			mov		#Vetor1, R5
			mov		#Vetor2, R6
			mov		#0x2420, R7
			call	#vecsum
			jmp	$

vecsum:		mov.w 	@R5+,R8
			mov.w	R8,0(R7)
			add		#2,R6
			add		#2,R7
loop:		mov.w 	#0,0(R7)
			mov.w 	@R5,R9
			add 	@R6,R9
			mov.w	R9,0(R7)
			add		#2,R7
			dec.b	R8
			jnz 	loop
			ret

;-------------------------------------------------------------------------------
; Data initialization
;-------------------------------------------------------------------------------
			.data
; Declarar os vetores
Vetor1:		.word	7, 2021, 2223, 2425, 2627, 2829, 3031, 3233
Vetor2:		.word	7, 6867, 6665, 6463, 6261, 6059, 5857, 5655
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
            
