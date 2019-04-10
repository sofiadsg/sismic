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


;----------------------------------------------------------------------------
; Main loop here
;----------------------------------------------------------------------------
			mov 	#vetor,R5               ;incializar R5 com o endereço do vetor
			call 	#ORDENA                 ;chamar subrotina
			jmp 	$                       ;travar execução
			nop 	                        ;exigido pelo montador

ORDENA:		mov.b   @R5+,R4                  ;Criamos o contador R4
            mov.b   R4, R6                 ;Precisaremos novamente saber o valor do tamanho do vetor, então vamos guardá-lo
         		                            ;em dois registradores, que serão decrementados em momentos diferentes
outerLoop:  mov     R5, R7					;Também precisamos guardar o endereço da primeira letra
			mov.b	R4, R6
            
innerLoop:  cmp.b   @R5, 0(R7)
            jge     fimInLoop
            mov.b   @R7, R9
            mov.b   @R5, 0(R7)
            mov.b   R9, 0(R5)
fimInLoop:  cmp.b	@R5, -1(R5)				;Evita que sejam feitas repetições desnecessárias do loop
            jeq		fimOutLoop
			inc		R7
            dec		R6
            jnz 	innerLoop
            jmp		fimOutLoop
            nop
fimOutLoop:	inc 	R5
			dec 	R4
			jnz 	outerLoop
			ret



			.data                           ; Declarar vetor com a concatenação dos nomes completos da equipe
vetor:		.byte 51, "sofiacatharinadisegnaviniciusmarconcinidesouzaleite"
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
            
