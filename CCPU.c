#include <stdio.h>
#include <stdlib.h>

/* Branching */
#define JMP  0
#define JE   1
#define JNE  2
#define JG   3
#define JGE  4
#define JL   5
#define JLE  6

/* Loading */
#define MOV  7
#define CPY  8
#define LDA  9
#define STO 10

/* Logic */
#define SHL 11
#define SHR 12
#define NOT 13
#define AND 14
#define OR  15
#define XOR 16

/* Arithmetic */
#define ADD 17
#define MUL 18
#define DIV 19
#define MOD 20

/* Debug codes */
#define DIE 255

int main() {
  int mem[65536], reg[256], a, b, c, pc, steps = 0, data, OP;

  for(pc=0; pc<256; pc++) reg[pc] = 0;
  for(pc=0; pc<65536; pc++) mem[pc] = 0;

  FILE *f = fopen("ram.bin", "rb");
  fseek(f, 0, SEEK_END);
  int len = ftell(f);
  rewind(f);
  fread(mem, 1, len, f);
  fclose(f);
  printf("Program length %d bytes\n", len);

  pc = 0;
  OP = 256;
  for(steps=0; steps<len; steps++) {

    /* JMP */
    if(OP==JMP) {
      pc = data;
      printf("JMP %d\n", data);
    }

    /* JE A, B, addr32 (if A==B jump to addr32) */
    if(OP==JE) {
      if(reg[a]==reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JE  %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* JNE A, B, addr32 */
    if(OP==JNE) {
      if(reg[a]!=reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JNE %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* JG A, B, addr32 */
    if(OP==JG) {
      if(reg[a]>reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JG  %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* JGE A, B, addr32 */
    if(OP==JGE) {
      if(reg[a]>=reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JGE %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* JL A, B, addr32 */
    if(OP==JL) {
      if(reg[a]<reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JL  %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* JLE A, B, addr32 */
    if(OP==JLE) {
      if(reg[a]<=reg[b])
	pc = data;
      else
	pc = pc + 2;
      printf("JLE %c, %c, %d\n", a+'A', b+'A', data);
    }

    /* MOV A, int32 (A = int32) */
    if(OP==MOV) {
      reg[a] = data;
      pc += 2;
      printf("MOV %c, %d\n", a+'A', data);
    }

    /* CPY A, B (B = A) */
    if(OP==CPY) {
      reg[b] = reg[a];
      pc++;
      printf("CPY %c, %c\n", a+'A', b+'A');
    }

    /* LDA A, B (A = MEM[B]) */
    if(OP==LDA) {
      reg[a] = mem[b];
      pc++;
      printf("LDA %c, %c\n", a+'A', b+'A');
    }

    /* STO A, B (MEM[A] = B) */
    if(OP==STO) {
      mem[a] = reg[b];
      pc++;
      printf("STO %c, %c\n", a+'A', b+'A');
    }

    /* SHL A, B, C */
    if(OP==SHL) {
      reg[a] = reg[b] << reg[c];
      pc++;
      printf("SHL %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* SHR A, B, C */
    if(OP==SHR) {
      reg[a] = reg[b] >> reg[c];
      pc++;
      printf("SHR %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* NOT A, B */
    if(OP==NOT) {
      reg[a] = ~reg[b];
      pc++;
      printf("NOT %c, %c\n", a+'A', b+'A');
    }

    /* AND A, B, C */
    if(OP==AND) {
      reg[a] = reg[b] & reg[c];
      pc++;
      printf("AND %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* OR A, B, C */
    if(OP==OR) {
      reg[a] = reg[b] | reg[c];
      pc++;
      printf("OR  %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* XOR A, B, C */
    if(OP==XOR) {
      reg[a] = reg[b] ^ reg[c];
      pc++;
      printf("XOR %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* ADD A, B, C */
    if(OP==ADD) {
      reg[a] = reg[b] + reg[c];
      pc++;
      printf("ADD %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* MUL A, B, C */
    if(OP==MUL) {
      reg[a] = reg[b] * reg[c];
      pc++;
      printf("MUL %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* DIV A, B, C */
    if(OP==DIV) {
      reg[a] = reg[b] / reg[c];
      pc++;
      printf("DIV %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* MOD A, B, C */
    if(OP==MOD) {
      reg[a] = reg[b] % reg[c];
      pc++;
      printf("MOD %c, %c, %c\n", a+'A', b+'A', c+'A');
    }

    /* DIE */
    if(OP==DIE) {
      printf("DIE\n");
      printf("PC=%d, A=%d, B=%d, C=%d\n", pc, reg[0], reg[1], reg[2]);
      exit(0);
    }
      OP = (mem[pc]&0b11111111000000000000000000000000)>>24;
       a = (mem[pc]&0b00000000111111110000000000000000)>>16;
       b = (mem[pc]&0b00000000000000001111111100000000)>>8;
       c = (mem[pc]&0b00000000000000000000000011111111);
    data = mem[pc+1];
    // printf("PC=%d OP=%d %d %d %d\n", pc, OP>>24, a>>16, b>>8, c);
    // printf("%d\n", data);
  }
}
