/* Compilation: gcc ccpu8.c -o ccpu -lSDL2main -lSDL2 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

//#define DEBUG

#define NOP  0 /* do nothing               */
#define JMP  1 /* goto addr;               */
#define JML  2 /* if(reg1<reg2) goto addr; */
#define MOV  3 /* reg = val;               */
#define LDA  4 /* reg1 = mem[reg2];        */
#define STO  5 /* mem[reg1] = reg2;        */
#define SHL  6 /* reg1 = reg2 << reg3;     */
#define SHR  7 /* reg1 = reg2 >> reg3;     */
#define NOT  8 /* reg1 = ~reg2             */
#define AND  9 /* reg1 = reg2 & reg3;      */
#define IOR 10 /* reg1 = reg2 | reg3;      */
#define XOR 11 /* reg1 = reg2 ^ reg3;      */
#define ADD 12 /* reg1 = reg2 + reg3;      */

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

int assemble() {
  FILE *inp = fopen("src.asm", "r");
  FILE *out = fopen("dst.bin", "wb");
  unsigned char buf[256], nbuf[64], op[3], regs[3];
  int i, j, k, line = 0;

  while(fgets(buf, 256, inp)!=NULL) {

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
      fprintf(out, "%c", NOP);
      printf("%.10d\tNOP\n", line);
      line = line + 1;
    }
    if(isOpcode(op, "JMP")) {
      fprintf(out, "%c", JMP);
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tJMP %d\n", line, j);
      line += 5;
    }
    if(isOpcode(op, "JML")) {
      fprintf(out, "%c", JML);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tJML %c, %c, %d\n", line, regs[0]+'A', regs[1]+'A', j);
      line = line + 7;
    }
    if(isOpcode(op, "MOV")) {
      fprintf(out, "%c", MOV);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tMOV %c, %d\n", line, regs[0]+'A', j);
      line = line + 6;
    }
    if(isOpcode(op, "LDA")) {
      fprintf(out, "%c", LDA);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      printf("%.10d\tLDA %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line = line + 3;
    }
    if(isOpcode(op, "STO")) {
      fprintf(out, "%c", STO);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      printf("%.10d\tSTO %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line = line + 3;
    }   
    if(isOpcode(op, "SHL")) {
      fprintf(out, "%c", SHL);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tSHL %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
    if(isOpcode(op, "SHR")) {
      fprintf(out, "%c", SHR);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tSHR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
    if(isOpcode(op, "NOT")) {
      fprintf(out, "%c", NOT);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);   
      printf("%.10d\tNOT %c, %c\n", line, regs[0]+'A', regs[1]+'A');
      line = line + 3;
    }
    if(isOpcode(op, "AND")) {
	  fprintf(out, "%c", AND);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tAND %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
    if(isOpcode(op, "IOR")) {
      fprintf(out, "%c", IOR);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tOR  %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
    if(isOpcode(op, "XOR")) {
      fprintf(out, "%c", XOR);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tXOR %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
    if(isOpcode(op, "ADD")) {
      fprintf(out, "%c", ADD);
      fprintf(out, "%c", regs[0]);
      fprintf(out, "%c", regs[1]);
      fprintf(out, "%c", regs[2]);     
      printf("%.10d\tADD %c, %c, %c\n", line, regs[0]+'A', regs[1]+'A', regs[2]+'A');
      line = line + 4;
    }
  }
  fflush(out);
  fclose(inp);
  fclose(out);
}

int main(int argc, char* args[]) {
  unsigned char R, G, B;
  char *mem = malloc(4*65536*sizeof(char)); /* 256 kB */
  int reg[64], a, b, c, i, j, k, pc, data, OP, *p, t = 1, len;
  SDL_Event event;
  SDL_Window *window;
  SDL_Surface *surface;

  assemble();
  printf("\nPress enter to execute.\n");
  getch();

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CCPU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  surface = SDL_GetWindowSurface(window);

  for(pc=0; pc<64; pc++) reg[pc] = 0;
  for(pc=0; pc<4*65536; pc++) mem[pc] = 0;

  FILE *f = fopen("dst.bin", "rb");
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  rewind(f);
  fread(mem, 1, len, f);
  fclose(f);

  pc = 0;
  while(t++) {
  	
  	OP = mem[pc];
  	a = mem[pc+1];
  	b = mem[pc+2];
  	c = mem[pc+3];
  	i = mem[pc+4];
  	j = mem[pc+5];
  	k = mem[pc+6];

    /* JMP */
    if(OP==JMP) {
      pc = (a<<24)+(b<<16)+(c<<8)+i;
#ifdef DEBUG
      printf("JMP %d\n", data);
#endif
    }

    /* JML A, B, addr32 */
    if(OP==JML) {
      data = (c<<24)+(i<<16)+(j<<8)+k;
      if(reg[a]<reg[b])
		pc = data;
      else
		pc = pc + 7;
#ifdef DEBUG
      printf("JML %c, %c, %d\n", a+'A', b+'A', data);
#endif
    }

    /* MOV A, val */
    if(OP==MOV) {
      data = (b<<24)+(c<<16)+(i<<8)+j;
      reg[a] = data;
      pc = pc + 6;
#ifdef DEBUG
      printf("MOV %c, %d\n", a+'A', reg[a]);
#endif
    }

    /* LDA A, B (A = MEM[B]) */
    if(OP==LDA) {
      reg[a] = mem[reg[b]];
      pc = pc + 3;
#ifdef DEBUG
      printf("LDA %c, %c\n", a+'A', b+'A');
#endif
    }

    /* STO A, B (MEM[A] = B) */
    if(OP==STO) {
      mem[reg[a]] = reg[b];
      pc = pc + 3;
#ifdef DEBUG
      printf("STO %c, %c (MEM[%d] = %d)\n", a+'A', b+'A', reg[a], reg[b]);
#endif
    }

    /* SHL A, B, C */
    if(OP==SHL) {
      reg[a] = reg[b] << reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("SHL %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* SHR A, B, C */
    if(OP==SHR) {
      reg[a] = reg[b] >> reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("SHR %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* NOT A, B */
    if(OP==NOT) {
      reg[a] = ~reg[b];
      pc = pc + 4;
#ifdef DEBUG
      printf("NOT %c, %c\n", a+'A', b+'A');
#endif
    }

    /* AND A, B, C */
    if(OP==AND) {
      reg[a] = reg[b] & reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("AND %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* IOR A, B, C */
    if(OP==IOR) {
      reg[a] = reg[b] | reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("IOR %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* XOR A, B, C */
    if(OP==XOR) {
      reg[a] = reg[b] ^ reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("XOR %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* ADD A, B, C */
    if(OP==ADD) {
      reg[a] = reg[b] + reg[c];
      pc = pc + 4;
#ifdef DEBUG
      printf("ADD %c, %c, %c\n", a+'A', b+'A', c+'A');
#endif
    }

    /* draw MEM[65536:65536+320*240] */
	if(!(t%12800)) {
      for(j=0; j<480; j++)
      	for(i=0; i<640; i++) {
      	  p = (int *)surface->pixels+(i+j*640);
		  R = 85* (mem[65536+(i>>1)+(j>>1)*320]&0b00000011);
		  G = 85*((mem[65536+(i>>1)+(j>>1)*320]&0b00001100)>>2);
		  B = 85*((mem[65536+(i>>1)+(j>>1)*320]&0b00110000)>>4);
		  *p = (R<<16) + (G<<8) + B;
        }
      SDL_UpdateWindowSurface(window);
    }
    if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
    
  }
  free(mem);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

/* Example program (src.asm): xorshift random
MOV A, 1
MOV B, 65536
MOV C, 1
MOV D, 13
MOV E, 17
MOV F, 5
MOV H, 256
MOV I, 142336    ; 65536+320*240
SHL G, A, D
XOR A, A, G
SHR G, A, E
XOR A, A, G
SHL G, A, F
XOR A, A, G
ADD B, B, C
STO B, A
JML B, I, 48
JMP 6            ;
 */
