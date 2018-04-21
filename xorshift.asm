MOV A, 1       ; this program will do xorshift32 
MOV B, 0       ; displayed as noise on CCPU
MOV C, 1
MOV D, 13
MOV E, 17
MOV F, 5
MOV H, 256
MOV I, 64000
SHL G, A, D
XOR A, A, G    ; A ^= A << 13
SHR G, A, E
XOR A, A, G    ; A ^= A >> 17
SHL G, A, F
XOR A, A, G    ; A ^= A << 5
ADD B, B, C.   ; B  = B + 1
STO B, A       ; MEM[B] = A
JL  B, I, 16.  ; if(B==64000) goto 16;
JMP 2          ; goto 2;