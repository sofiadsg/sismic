; using RAM memory to add two 16 bit numbers, using subroutines for setup and instructions
    CALL &SOMA
    JMP $

SOMA:   MOV.W   &0x2400, R10
        MOV.W   &0x2400, R10
        RET

        .data
        .word 0x2233, 0x5544