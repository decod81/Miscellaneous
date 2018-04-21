#include <stdio.h>
#include <stdlib.h>

/* Branching */
#define NOP  0
#define JMP  1
#define JE   2
#define JNE  3
#define JG   4
#define JGE  5
#define JL   6
#define JLE  7

/* Loading */
#define MOV  8
#define CPY  9
#define LDA 10
#define STO 11

/* Logic */
#define SHL 12
#define SHR 13
#define NOT 14
#define AND 15
#define OR  16
#define XOR 17

/* Arithmetic */
#define ADD 18
#define MUL 19
#define DIV 20
#define MOD 21

int isOpcode(char *opc, char *cmp) {
  int i, j=0;
  for(i=0; i<3; i++)
    if(opc[i] == cmp[i])
      j++;
  if(j==3)
    return 1;
  else
    return 0;
}
int main() {
  FILE *in = fopen("xorshift.asm", "r");
  FILE *out = fopen("ram.bin", "wb");
  FILE *outh = fopen("ram.hex", "w");
  unsigned char buf[256], nbuf[64], op[3], regs[3];
  int i, j, k, line = 0;
  while(fgets(buf, 256, in)!=NULL) {

    /* find opcode */
    for(i=0; i<3; i++)
      op[i] = buf[i];
    for(i=0; i<64; i++)
      nbuf[i] = 0;
    i = 3;
    j = 0;
    while(buf[i]!='\n' && buf[i]!=';') {
      if(buf[i]>47 && buf[i]<58)
 		nbuf[j++] = buf[i];
      i++;
    }
    j = atoi(nbuf);

    /* find registers */
    i = 3;
    k = 0;
    while(buf[i]!='\n' && buf[i]!=';') {
      if(buf[i]>64 && buf[i]<91)
 		regs[k++] = buf[i]-'A';
      i++;
    }

    /* handle opcodes */
    if(isOpcode(op, "NOP")) {
      i = (NOP<<24);
      fwrite(&i, 4, 1, out);
      printf("%d\tNOP\n", line);
      fprintf(outh, "%.8x\n", &i);
      line += 1;
    }
    if(isOpcode(op, "JMP")) {
      i = JMP<<24;
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJMP %d\n", line, j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JE ")) {
      i = (JE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJE  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JNE")) {
      i = (JNE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJNE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JG ")) {
      i = (JG<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJG  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JGE")) {
      i = (JGE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJGE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JL ")) {
      i = (JL<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJL  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "JLE")) {
      i = (JLE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJLE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "MOV")) {
      i = (MOV<<24)+(regs[0]<<16);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tMOV %c, %d\n", line, regs[0]+'A', j);
      fprintf(outh, "%.8x\n", i);
      fprintf(outh, "%.8x\n", j);
      line += 2;
    }
    if(isOpcode(op, "CPY")) {
      i = (CPY<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tCPY %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "LDA")) {
      i = (LDA<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tLDA %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "STO")) {
      i = (STO<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tSTO %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "SHL")) {
      i = (SHL<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tSHL %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "SHR")) {
      i = (SHR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tSHR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "NOT")) {
      i = (NOT<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tNOT %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "AND")) {
      i = (AND<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tAND %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "OR ")) {
      i =  (OR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tOR  %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "XOR")) {
      i = (XOR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tXOR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "ADD")) {
      i = (ADD<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tADD %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "MUL")) {
      i = (MUL<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tMUL %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "DIV")) {
      i = (DIV<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tDIV %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
    if(isOpcode(op, "MOD")) {
      i = (MOD<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tMOD %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      fprintf(outh, "%.8x\n", i);
      line += 1;
    }
  }
  fflush(out);
  fflush(outh);
  fclose(in);
  fclose(out);
  fclose(outh);
}
