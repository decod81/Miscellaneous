/* Compilation: gcc ccpu.c -o ccpu -lSDL2main -lSDL2
 * Example program: fill the screen with random (xorshift32) dots
 *
 * MOV A, 1             ; A: variable to hold pixel value (and initial value to xorshift pseudorandom)
 * MOV B, 255           ; B: variable to hold pixel position
 * MOV C, 1             ; C: variable used in adding the pixel position 
 * MOV D, 13            ; D: variable used in xorshift
 * MOV E, 17            ; E: variable used in xorshift
 * MOV F, 5             ; F: variable used in xorshift
 * MOV H, 256           ; H: variable used to limit the pixel values to 255
 * MOV I, 76800         ; I: variable indicating the memory location where filling of the screen should stop
 * SHL G, A, D          ; begin: standard xorshift32
 * XOR A, A, G          ; x ^= x << 13
 * SHR G, A, E
 * XOR A, A, G          ; x ^= x >> 17
 * SHL G, A, F
 * XOR A, A, G          ; x ^= x << 5 (end: standard xorshift32)
 * MOD G, A, H          ; limit the pixel values to 255
 * ADD B, B, C          ; increase memory location by 1
 * STO B, A             ; write pixel value
 * JL  B, I, 16         ; jump back to 16 if we havn't reached end of the screen yet
 * JMP 2                ; jump back to beginning (without resetting the random seed)
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

//#define DEBUG

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

char *prg = 
"MOV A, 1     ;"
"MOV B, 255   ;"
"MOV C, 1     ;"
"MOV D, 13    ;"
"MOV E, 17    ;"
"MOV F, 5     ;"
"MOV H, 256   ;"
"MOV I, 16000 ;"
"SHL G, A, D  ;"
"XOR A, A, G  ;"
"SHR G, A, E  ;"
"XOR A, A, G  ;"
"SHL G, A, F  ;"
"XOR A, A, G  ;"
"MOD G, A, H  ;"
"ADD B, B, C  ;"
"STO B, A     ;"
"JL  B, I, 16 ;"
"JMP 2        ;";


int assemble() {
  FILE *in = fopen("xorshift.asm", "r");
  FILE *out = fopen("ram.bin", "wb");
  unsigned char buf[256], nbuf[64], op[3], regs[3];
  int i, j, k, line = 0;

  printf("\nCompiling src.asm...\n\n");

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
    if(isOpcode(op, "JMP")) {
      i = JMP<<24;
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJMP %d\n", line, j);
      line += 2;
    }
    if(isOpcode(op, "JE ")) {
      i = (JE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJE  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "JNE")) {
      i = (JNE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJNE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "JG ")) {
      i = (JG<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJG  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "JGE")) {
      i = (JGE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJGE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "JL ")) {
      i = (JL<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJL  %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "JLE")) {
      i = (JLE<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tJLE %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "MOV")) {
      i = (MOV<<24)+(regs[0]<<16);
      fwrite(&i, 4, 1, out);
      fwrite(&j, 4, 1, out);
      printf("%d\tMOV %c, %d\n", line, regs[0]+'A', j);
      line += 2;
    }
    if(isOpcode(op, "CPY")) {
      i = (CPY<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tCPY %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line += 1;
    }
    if(isOpcode(op, "LDA")) {
      i = (LDA<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tLDA %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line += 1;
    }
    if(isOpcode(op, "STO")) {
      i = (STO<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tSTO %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line += 1;
    }
    if(isOpcode(op, "SHL")) {
      i = (SHL<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tSHL %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "SHR")) {
      i = (SHR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tSHR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "NOT")) {
      i = (NOT<<24)+(regs[0]<<16)+(regs[1]<<8);
      fwrite(&i, 4, 1, out);
      printf("%d\tNOT %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line += 1;
    }
    if(isOpcode(op, "AND")) {
      i = (AND<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tAND %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "OR ")) {
      i =  (OR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tOR  %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "XOR")) {
      i = (XOR<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tXOR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "ADD")) {
      i = (ADD<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tADD %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "MUL")) {
      i = (MUL<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tMUL %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "DIV")) {
      i = (DIV<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tDIV %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
    if(isOpcode(op, "MOD")) {
      i = (MOD<<24)+(regs[0]<<16)+(regs[1]<<8)+regs[2];
      fwrite(&i, 4, 1, out);
      printf("%d\tMOD %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line += 1;
    }
  }
  fflush(out);
  fclose(in);
  fseek(out, 0, SEEK_END);
  int len = ftell(out);
  printf("\nProgram length %d bytes\n\n", len);
  fclose(out);
}

int main(int argc, char* args[]) {
  unsigned char col;
  int *mem = malloc(65536*sizeof(int)*2);
  int reg[256], a, b, c, i, j, k, pc, data, OP, *p, t = 1, len;
  SDL_Event event;
  SDL_Window *window;
  SDL_Surface *surface;

  assemble();
  printf("Press enter to execute.\n");
  getch();

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CCPU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 240, 0);
  surface = SDL_GetWindowSurface(window);

  for(pc=0; pc<256; pc++) reg[pc] = 0;
  for(pc=0; pc<2*65536; pc++) mem[pc] = 0;

  FILE *f = fopen("ram.bin", "rb");
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  rewind(f);
  fread(mem, 1, len, f);
  fclose(f);
  printf("Program length %d bytes\n", len);
  
  /* ASCII hex conversion for verilog */
  FILE *outh = fopen("ram.hex", "w");
  for(i=0; i<512; i++)
  	if(i<len)
  		fprintf(outh, "%.8x\n", mem[i]);
  	else
  		fprintf(outh, "%.8x\n", 0);
  fclose(outh);
  
  FILE *outh2 = fopen("ram2.hex", "w");
  for(i=0; i<160*100; i++)
	fprintf(outh, "%.2x\n", rand()%256);
  fclose(outh2);
  
  pc = 0;
  OP = 256;

  while(t++) {
  	
  	OP = (mem[pc]&0b11111111000000000000000000000000)>>24;
    a = (mem[pc]&0b00000000111111110000000000000000)>>16;
    b = (mem[pc]&0b00000000000000001111111100000000)>>8;
    c = (mem[pc]&0b00000000000000000000000011111111);
    data = mem[pc+1];

    /* JMP */
    if(OP==JMP) {
      pc = data;
#ifdef DEBUG
      printf("JMP %d\n", data);
#endif
    }

    /* JE A, B, addr32 (if A==B jump to addr32) */
    if(OP==JE) {
      if(reg[a]==reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JE  %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* JNE A, B, addr32 */
    if(OP==JNE) {
      if(reg[a]!=reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JNE %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* JG A, B, addr32 */
    if(OP==JG) {
      if(reg[a]>reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JG  %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* JGE A, B, addr32 */
    if(OP==JGE) {
      if(reg[a]>=reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JGE %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* JL A, B, addr32 */
    if(OP==JL) {
      if(reg[a]<reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JL  %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* JLE A, B, addr32 */
    if(OP==JLE) {
      if(reg[a]<=reg[b])
		pc = data;
      else
		pc = pc + 2;
#ifdef DEBUG
      printf("JLE %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* MOV A, int32 (A = int32) */
    if(OP==MOV) {
      reg[a] = data;
      pc += 2;
#ifdef DEBUG
      printf("MOV %c, %d\n", a+'A', data);
#endif
    }

    /* CPY A, B (B = A) */
    if(OP==CPY) {
      reg[b] = reg[a];
      pc++;
#ifdef DEBUG
      printf("CPY %c, %c\n", a+'A', b+'A');
#endif
    }

    /* LDA A, B (A = MEM[B]) */
    if(OP==LDA) {
      reg[a] = mem[b];
      pc++;
#ifdef DEBUG
      printf("LDA %c, %c\n", a+'A', b+'A');
#endif
    }

    /* STO A, B (MEM[A] = B) */
    if(OP==STO) {
      mem[reg[a]] = reg[b];
      pc++;
#ifdef DEBUG
      printf("STO %c, %c (MEM[%d] = %d)\n", a+'A', b+'A', reg[a], reg[b]);
#endif
    }

    /* SHL A, B, C */
    if(OP==SHL) {
      reg[a] = reg[b] << reg[c];
      pc++;
#ifdef DEBUG
      printf("SHL %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* SHR A, B, C */
    if(OP==SHR) {
      reg[a] = reg[b] >> reg[c];
      pc++;
#ifdef DEBUG
      printf("SHR %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* NOT A, B */
    if(OP==NOT) {
      reg[a] = ~reg[b];
      pc++;
#ifdef DEBUG
      printf("NOT %c, %c\n", a+'A', b+'A');
#endif
    }

    /* AND A, B, C */
    if(OP==AND) {
      reg[a] = reg[b] & reg[c];
      pc++;
#ifdef DEBUG
      printf("AND %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* OR A, B, C */
    if(OP==OR) {
      reg[a] = reg[b] | reg[c];
      pc++;
#ifdef DEBUG
      printf("OR  %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* XOR A, B, C */
    if(OP==XOR) {
      reg[a] = reg[b] ^ reg[c];
      pc++;
#ifdef DEBUG
      printf("XOR %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* ADD A, B, C */
    if(OP==ADD) {
      reg[a] = reg[b] + reg[c];
      pc++;
#ifdef DEBUG
      printf("ADD %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* MUL A, B, C */
    if(OP==MUL) {
      reg[a] = reg[b] * reg[c];
      pc++;
#ifdef DEBUG
      printf("MUL %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* DIV A, B, C */
    if(OP==DIV) {
      reg[a] = reg[b] / reg[c];
      pc++;
#ifdef DEBUG
      printf("DIV %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* MOD A, B, C */
    if(OP==MOD) {
      reg[a] = reg[b] % reg[c];
      pc++;
#ifdef DEBUG
      printf("MOD %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    if(!(t%320)) {
      for(i=0; i<320*240; i++) {
		p = (int *)surface->pixels+i;
		col = mem[i];
		*p = 65536*col + 256*col + col;
      }
      SDL_UpdateWindowSurface(window);
    }
    if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
  }
  free(mem);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
