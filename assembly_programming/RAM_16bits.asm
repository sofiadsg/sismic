; using RAM memory to add two 16 bit numbers
MOV.W   &0x2400, R10
MOV.W   &0x2400, R10
JMP $

.data
.word 0x2233, 0x5544