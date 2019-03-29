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
			mov		#vetor, R5
			call	#smaller
			jmp	$

smaller:	mov.b	#127,R6
			mov.b 	@R5+,R8

loop:		cmp.b	@R5,R6
			jlo		lb
			jeq		increment
			mov.b 	@R5,R6
			mov		#1, R7

lb:			inc		R5
			dec.b	R8
			jnz		loop
			ret

increment:	inc		R7
			jmp		lb

;-------------------------------------------------------------------------------
; Data initialization
;-------------------------------------------------------------------------------

		.data
vetor:	.byte 10, "souzasofia"

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
            
