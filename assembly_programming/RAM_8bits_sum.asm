; Cretes a subroutine to add multiple numbers
    CALL &SOMA
    JMP $

SOMA:   CLR.W   R10
        MOV.W   &0x2400, R10
LOOP:   ADD.B   @R5,R10
        INC.W   R5
        DEC.W   R6
        JNZ LOOP
        RET