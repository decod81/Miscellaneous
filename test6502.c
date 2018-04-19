/* 6502 */
#include <stdio.h>
#include <stdlib.h>

#define LDAa    0xAD
#define LDAax   0xBD
#define LDAay   0xB9
#define LDAim   0xA9
#define LDAzp   0xA5
#define LDAzpxi 0xA1
#define LDAzpx  0xB5
#define LDAzpiy 0xB1

#define LDXa    0xAE
#define LDXay   0xBE
#define LDXim   0xA2
#define LDXzp   0xA6
#define LDXzpy  0xB6

#define LDYa    0xAC
#define LDYay   0xBC
#define LDYim   0xA0
#define LDYzp   0xA4
#define LDYzpy  0xB4

#define STAa    0x8D
#define STAax   0x9D
#define STAay   0x99
#define STAzp   0x85
#define STAzpxi 0x81
#define STAzpx  0x95
#define STAzpiy 0x91

#define STXa    0x8E
#define STXzp   0x86
#define STXzpy  0x96

#define STYa    0x8C
#define STYzp   0x84
#define STYzpx  0x94

#define ADCa    0x6D
#define ADCax   0x7D
#define ADCay   0x79
#define ADCim   0x69
#define ADCzp   0x65
#define ADCzpxi 0x61
#define ADCzpx  0x76
#define ADCzpiy 0x71

#define SBCa    0xED
#define SBCax   0xFD
#define SBCay   0xF9
#define SBCim   0xE9
#define SBCzp   0xE5
#define SBCzpxi 0xE1
#define SBCzpx  0xF5
#define SBCzpiy 0xF1

#define INCa    0xEE
#define INCax   0xFE
#define INCzp   0xE6
#define INCzpx  0xF6

#define INX     0xE8
#define INY     0xC8

#define DECa    0xCE
#define DECax   0xDE
#define DECzp   0xC6
#define DECzpx  0xD6

#define DEX     0xCA
#define DEY     0x88

#define ASLa    0x0E
#define ASLax   0x1E
#define ASLA    0x0A
#define ASLzp   0x06
#define ASLzpx  0x16

#define LSRa    0x4E
#define LSRax   0x5E
#define LSRA    0x4A
#define LSRzp   0x46
#define LSRzpx  0x56

#define ROLa    0x2E
#define ROLax   0x3E
#define ROLA    0x2A
#define ROLzp   0x26
#define ROLzpx  0x36

#define RORa    0x6E
#define RORax   0x7E
#define RORA    0x6A
#define RORzp   0x66
#define RORzpx  0x76

#define ANDa    0x2D
#define ANDax   0x3D
#define ANDay   0x39
#define ANDim   0x29
#define ANDzp   0x25
#define ANDzpxi 0x21
#define ANDzpx  0x35
#define ANDzpiy 0x31

#define ORAa    0x0D
#define ORAax   0x1D
#define ORAay   0x19
#define ORAim   0x09
#define ORAzp   0x05
#define ORAzpxi 0x01
#define ORAzpx  0x15
#define ORAzpiy 0x11

#define EORa    0x4D
#define EORax   0x5D
#define EORay   0x59
#define EORim   0x49
#define EORzp   0x45
#define EORzpxi 0x41
#define EORzpx  0x55
#define EORzpiy 0x51

#define CMPa    0xCD
#define CMPax   0xDD
#define CMPay   0xD9
#define CMPim   0xC9
#define CMPzp   0xC5
#define CMPzpxi 0xC1
#define CMPzpx  0xD5
#define CMPzpiy 0xD1

#define CPXa    0xEC
#define CPXim   0xE0
#define CPXzp   0xE4

#define CPYa    0xCC
#define CPYim   0xC0
#define CPYzp   0xC4

#define BITa    0x2C
#define BITim   0x89
#define BITzp   0x24

#define BCC     0x90
#define BCS     0xB0
#define BEQ     0xF0
#define BMI     0x30
#define BNE     0xD0
#define BPL     0x10
#define BVC     0x50
#define BVS     0x70
#define TAX     0xAA
#define TXA     0x8A
#define TAY     0xA8
#define TYA     0x98
#define TSX     0xBA
#define TXS     0x9A
#define PHA     0x48
#define PLA     0x68
#define PHP     0x08
#define PLP     0x28
#define JMPa    0x4C
#define JMPi    0x6C
#define JSR     0x20
#define RTS     0x60
#define RTI     0x40
#define SEC     0x38
#define SED     0xF8
#define SEI     0x78
#define CLC     0x18
#define CLD     0xD8
#define CLI     0x58
#define CLV     0xB8
#define NOP     0xEA
#define BRK     0x00

#define END     256

int main() {
  int MEM[65536] = {NOP, NOP, LDAa, 2, END, 0, };
  int OP=0; /* opcode */
  int PC=0; /* program counter */
  int A=0;  /* accumulator */
  int X=0;  /* register X */
  int Y=0;  /* register Y */
  int C=0;  /* carry */
  int V=0;  /* overflow flag */
  int I=0;  /* interrupt disable status */
  int D=0;  /* decimal mode */
  int P=0;  /* processor status */
  int S=0;  /* stack */
  int Z=0;  /* zero flag */
  int N=0;  /* negative flag */
  int M=0;  /* contents of memory next to PC */

  /*
    FILE *f = fopen("ram.bin", "rb");
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);
    fread(MEM, 1, len, f);
    fclose(f);
  */

  PC = 0;
  OP = 0;
  for(int steps=0; steps<65536; steps++) {
    OP = MEM[PC];
    M = MEM[PC+1];

    /* LDA */
    if(OP==LDAa) { /* LDA a */
      A = MEM[M];
      PC+=2;
      printf("LDA $%.8x\n", M);
    }

    /* LDX */
    if(OP==LDXa) { /* LDX a */
      X = MEM[M];
      PC+=2;
      printf("LDX $%.8x\n", M);
    }

    /* LDY */
    if(OP==LDYa) { /* LDY a */
      Y = MEM[M];
      PC+=2;
      printf("LDY $%.8x\n", M);
    }

    /* STA */
    if(OP==STAa) { /* STA a */
      MEM[M] = A;
      PC+=2;
      printf("STA $%.8x\n", M);
    }

    /* STX */
    if(OP==STXa) { /* STX a */
      MEM[M] = X;
      PC+=2;
      printf("STX $%.8x\n", M);
    }

    /* STY */
    if(OP==STYa) { /* STY a */
      MEM[M] = Y;
      PC+=2;
      printf("STY $%.8x\n", M);
    }

    /* ADC */
    if(OP==ADCa) { /* ADC a */
      A = A + MEM[M];
      PC+=2;
      printf("ADC $%.8x\n", M);
    }
    if(OP==ADCim) { /* ADC # */
      A = A + M;
      PC+=2;
      printf("ADC #$%.8x\n", M);
    }

    /* SBC  */
    if(OP==SBCa) { /* SBC a */
      A = A - MEM[M] - (~C);
      PC+=2;
      printf("SBC $%.8x\n", M);
    }
    if(OP==SBCim) { /* SBC # */
      A = A - M - (~C);
      PC+=2;
      printf("SBC #$%.8x\n", M);
    }

    /* INC */
    if(OP==INCa) { /* INC a */
      MEM[M]++;
      PC+=2;
      printf("INC $%.8x\n", M);
    }

    /* INX */
    if(OP==INX) {
      X++;
      PC+=2;
      printf("INX\n");
    }

    /* INY */
    if(OP==INY) {
      Y++;
      PC+=2;
      printf("INY\n");
    }

    /* DEC */
    if(OP==DECa) {
      MEM[M]--;
      PC+=2;
      printf("DEC $%.8x\n", M);
    }

    /* DEX */
    if(OP==DEX) {
      X--;
      PC+=2;
      printf("DEX\n");
    }

    /* DEY */
    if(OP==DEY) {
      Y--;
      PC+=2;
      printf("DEY\n");
    }

    /* ASL */
    if(OP==ASLa) { /* ASL a */
      A = A << 1;
      PC++;
      printf("ASL A\n");
    }

    /* LSR */
    if(OP==LSRa) { /* LSR a */
      A = A >> 1;
      PC++;
      printf("LSR A\n");
    }

    /* ROL */
    /* ROR */

    /* AND */
    if(OP==ANDa) { /* AND a */
      A = A & MEM[M];
      PC+=2;
      printf("AND $%.8x\n", M);
    }
    if(OP==ANDim) { /* AND # */
      A = A & M;
      PC+=2;
      printf("AND #$%.8x\n", M);
    }

    /* ORA */
    if(OP==ORAa) { /* ORA a */
      A = A|MEM[M];
      PC+=2;
      printf("ORA $%.8x\n", M);
    }
    if(OP==ORAim) { /* ORA # */
      A = A|M;
      PC+=2;
      printf("ORA #$%.8x\n", M);
    }

    /* EOR */
    if(OP==EORa) { /* EOR a */
      A = A^MEM[M];
      PC+=2;
      printf("EOR $%.8x\n", M);
    }
    if(OP==0x49) { /* EUR # */
      A = A^M;
      PC+=2;
      printf("EOR #$%.8x\n", M);
    }

    /* CMP */
    if(OP==CMPa) { /* CMP a */
      if(A<MEM[M]) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(A==MEM[M]) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(A>MEM[M]) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
      printf("CMP $%.8x\n", M);
    }
    if(OP==CMPim) { /* CMP # */
      if(A<M) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(A==M) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(A>M) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
      printf("CMP #$%.8x\n", M);
    }

    /* CPX */
    if(OP==CPXa) { /* CPX a */
      if(X<MEM[M]) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(X==MEM[M]) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(X>MEM[M]) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
      printf("CPX $%.8x\n", M);
    }
    if(OP==CPXim) { /* CPX # */
      if(X<M) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(X==M) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(X>M) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
    }

    /* CPY */
    if(OP==CPYa) { /* CPY a */
      if(Y<MEM[M]) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(Y==MEM[M]) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(Y>MEM[M]) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
    }
    if(OP==CPYim) { /* CPY # */
      if(Y<M) {
	N = 1;
	Z = 0;
	C = 0;
      }
      if(Y==M) {
	N = 0;
	Z = 1;
	C = 1;
      }
      if(Y>M) {
	N = 0;
	Z = 0;
	C = 1;
      }
      PC+=2;
    }

    /* BIT */
    if(OP==BITa) { /* BIT a */
      PC+=2;
    }
    if(OP==BITim) { /* BIT # */
      PC+=2;
    }
    if(OP==BITzp) { /* BIT zp */
      PC+=2;
    }

    /* BCC */
    if(OP==BCC) {
      if(C==0)
	PC=M;
      else
	PC+=2;
    }

    /* BCS */
    if(OP==BCS) {
      if(C==1)
	PC=M;
      else
	PC+=2;
    }

    /* BEQ */
    if(OP==BEQ) {
      if(Z==1)
	PC=M;
      else
	PC+=2;
    }

    /* BMI */
    if(OP==BMI) {
      if(N==1)
	PC=M;
      else
	PC+=2;
    }

    /* BNE */
    if(OP==BNE) {
      if(Z==0)
	PC=M;
      else
	PC+=2;
    }

    /* BPL */
    if(OP==BPL) {
      if(N==0)
	PC=M;
      else
	PC+=2;
    }

    /* BVC */
    if(OP==BVC) {
      if(V==0)
	PC=M;
      else
	PC+=2;
    }

    /* BVS */
    if(OP==BVS) {
      if(V==1)
	PC=M;
      else
	PC+=2;
    }

    /* TAX */
    if(OP==TAX) {
      X = A;
    }

    /* TXA */
    if(OP==TXA) {
      A = X;
    }

    /* TAY */
    if(OP==TAY) {
      Y = A;
    }

    /* TYA */
    if(OP==TYA) {
      A = Y;
    }

    /* TSX */
    if(OP==TSX) {
      X = S;
    }

    /* TXS */
    if(OP==TXS) {
      S = X;
    }

    /* PHA */
    if(OP==PHA) {
      S = A;
    }

    /* PLA */
    if(OP==PLA) {
      A = S;
    }

    /* PHP */
    if(OP==PHP) {
      S = P;
    }

    /* PLP */
    if(OP==PLP) {
      P = S;
    }

    /* JMP */
    if(OP==JMPa) { /* JMP a */
      PC=M;
    }
    if(OP==JMPi) { /* JMP (a) */
      PC=MEM[M];
    }

    /* JSR (Jump to New Location Saving Return Address) */
    if(OP==JSR) { /* JSR a */
      PC=M;
    }

    /* RTS (Return from Subroutine) */
    if(OP==RTS) {
    }

    /* RTI (Return from Interrupt) */
    if(OP==RTI) {
    }

    /* SEC (Set Carry Flag) */
    if(OP==SEC) {
      C = 1;
      PC++;
    }

    /* SED (Set Decimal Mode) */
    if(OP==SED) {
      D = 1;
      PC++;
    }

    /* SEI (Set Interrupt Disable) */
    if(OP==SEI) {
      I = 1;
      PC++;
    }

    /* CLC (Clear Carry Flag) */
    if(OP==CLC) {
      C = 0;
      PC++;
    }

    /* CLD (Clear Decimal Mode) */
    if(OP==CLD) {
      D = 0;
      PC++;
    }

    /* CLI (Clear Interrupt Disable Status) */
    if(OP==CLI) {
      I = 0;
      PC++;
    }

    /* CLV (Clear Overflow Flag) */
    if(OP==CLV) {
      V = 0;
      PC++;
    }

    /* NOP (No Operation) */
    if(OP==NOP) {
      printf("NOP\n");
      PC++;
    }

    /* BRK (Break: Force an Interrupt)*/
    if(OP==BRK) {
      printf("BRK\n");
      PC++;
    }

    /* END */
    if(OP==256) {
      printf("END\n");
      printf("A = %d\n", A);
      printf("X = %d\n", X);
      printf("Y = %d\n", Y);
      printf("Decimal Mode = %d\n", D);
      printf("Overflow Flag = %d\n", V);
      printf("Stack Pointer = %d\n", S);
      printf("N = %d, Z = %d, C = %d\n", N, Z, C);
      exit(0);
    }
  }
}
