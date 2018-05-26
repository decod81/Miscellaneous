/* gcc cpu.c -o cpu -lSDL2 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define JMP  0 /* goto addr;                                  */
#define JMR  1 /* goto rel addr;                              */
#define JML  2 /* if(reg1<reg2) goto rel addr;                */
#define MOV  3 /* reg = val;                                  */
#define LDA  4 /* reg1 = mem[reg2];                           */
#define STO  5 /* mem[reg1] = reg2;                           */
#define SHL  6 /* reg1 = reg2 << reg3;                        */
#define SHR  7 /* reg1 = reg2 >> reg3;                        */
#define NOT  8 /* reg1 = ~reg2                                */
#define AND  9 /* reg1 = reg2 & reg3;                         */
#define IOR 10 /* reg1 = reg2 | reg3;                         */
#define XOR 11 /* reg1 = reg2 ^ reg3;                         */
#define ADD 12 /* reg1 = reg2 + reg3;                         */
#define SUB 13 /* reg1 = reg2 - reg3;                         */
#define MUL 14 /* reg1 = reg2 * reg3;                         */
#define DIV 15 /* reg1 = reg2 / reg3; reg[3+1] = reg2 % reg3; */
#define SYN 16

char src[] = /* xorshift32 random noise generator */
  "JMP 37         \n"
  "DAT 43         \n" /* int 0 addr */
  "DAT 0          \n" /* int 1 addr */
  "DAT 0          \n" /* int 2 addr */
  "DAT 0          \n" /* int 3 addr */
  "DAT 0          \n" /* int 4 addr */
  "DAT 0          \n" /* int 5 addr */
  "DAT 0          \n" /* int 6 addr */
  "DAT 0          \n" /* int 7 addr */
  "MOV A, 1       \n"
  "MOV B, 65535   \n"
  "MOV C, 1       \n"
  "MOV D, 13      \n" 
  "MOV E, 17      \n"
  "MOV F, 5       \n"
  "MOV I, 142336  \n" /* 65536+320*240 */
  "SHL G, A, D    \n"
  "XOR A, A, G    \n"
  "SHR G, A, E    \n"
  "XOR A, A, G    \n"
  "SHL G, A, F    \n"
  "XOR A, A, G    \n"
  "ADD B, B, C    \n"
  "STO B, A       \n"
  "JML B, I, -31  \n"
  "SYN            \n";

int assemble() {
  FILE *out = fopen("dst.bin", "wb");
  unsigned char op[64], a[64], b[64], c[64], nline[80];
  unsigned char *lines[80];
  int i = 0, j, k, l = 0, line = 0, nlines = 0;

  /* read source code from a string */
  while(i<strlen(src)) {
    j = 0;
    while(src[i]!='\n')
      nline[j++] = src[i++];
    i++;
    lines[nlines] = malloc(80);
    strcpy(lines[nlines++], nline);
  }

  /* read source code from a file */
  /*
  FILE *inp = fopen("src.asm", "r");
  while(fgets(nline, 80, inp)!=NULL) {
    lines[nlines] = malloc(80);
    strcpy(lines[nlines++], nline);
  }
  fclose(inp);
  nlines--;
  */

  /* assemble into a binary file */
  while(l<nlines) {
    sscanf(lines[l], "%s %s %s %s", op, a, b, c);
    if(strspn(c, "-0123456789") == strlen(c)) j = atoi(c);
    if(strspn(b, "-0123456789") == strlen(b)) j = atoi(b);
    if(strspn(a, "-0123456789") == strlen(a)) j = atoi(a);
    l = l + 1;

  if(!strcmp(op, "DAT")) {
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tDATA %d\n", line, j);
      line += 4;
    }
    if(!strcmp(op, "JMP")) {
      fprintf(out, "%c", JMP);
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tJMP %d\n", line, j);
      line += 5;
    }
    if(!strcmp(op, "JMR")) {
      fprintf(out, "%c", JMR);
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tJMR %d (%d)\n", line, j, line+j);
      line += 5;
    }
    if(!strcmp(op, "JML")) {
      fprintf(out, "%c", JML);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tJML %c, %c, %d (%d)\n", line, a[0], b[0], j, line+j);
      line = line + 7;
    }
    if(!strcmp(op, "MOV")) {
      fprintf(out, "%c", MOV);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", (j&0b11111111000000000000000000000000)>>24);
      fprintf(out, "%c", (j&0b00000000111111110000000000000000)>>16);
      fprintf(out, "%c", (j&0b00000000000000001111111100000000)>>8);
      fprintf(out, "%c",  j&0b00000000000000000000000011111111);
      printf("%.10d\tMOV %c, %d\n", line, a[0], j);
      line = line + 6;
    }
    if(!strcmp(op, "LDA")) {
      fprintf(out, "%c", LDA);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      printf("%.10d\tLDA %c, %c\n", line, a[0], b[0]);
      line = line + 3;
    }
    if(!strcmp(op, "STO")) {
      fprintf(out, "%c", STO);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      printf("%.10d\tSTO %c, %c\n", line, a[0], b[0]);
      line = line + 3;
    }
    if(!strcmp(op, "SHL")) {
      fprintf(out, "%c", SHL);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tSHL %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "SHR")) {
      fprintf(out, "%c", SHR);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tSHR %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "NOT")) {
      fprintf(out, "%c", NOT);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      printf("%.10d\tNOT %c, %c\n", line, a[0], b[0]);
      line = line + 3;
    }
    if(!strcmp(op, "AND")) {
	  fprintf(out, "%c", AND);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tAND %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "IOR")) {
      fprintf(out, "%c", IOR);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tOR  %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "XOR")) {
      fprintf(out, "%c", XOR);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tXOR %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "ADD")) {
      fprintf(out, "%c", ADD);
      fprintf(out, "%c", a[0] - 'A');
      fprintf(out, "%c", b[0] - 'A');
      fprintf(out, "%c", c[0] - 'A');
      printf("%.10d\tADD %c, %c, %c\n", line, a[0], b[0], c[0]);
      line = line + 4;
    }
    if(!strcmp(op, "SYN")) {
      fprintf(out, "%c", SYN);
      printf("%.10d\tSYN\n", line);
      line = line + 1;
    }
  }
  fflush(out);
  fclose(out);

  l = 0;
  while(l<nlines) {
    free(lines[l]);
    l++;
  }
}

int main(int argc, char* args[]) {
  unsigned char R, G, B;
  char *mem = malloc(1024*1024*sizeof(char)); /* 1 MB */
  int reg[64], pc, data, *p, t = 1, len, OP, a, b, c, i, j, k;
  SDL_Event event;
  SDL_Window *window;
  SDL_Surface *surface;

  assemble();

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CCPU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  surface = SDL_GetWindowSurface(window);

  for(pc=0; pc<64; pc++) reg[pc] = 0;
  for(pc=0; pc<1024*1024; pc++) mem[pc] = 0;

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
      pc = ((unsigned char)(a)<<24)+((unsigned char)(b)<<16)+((unsigned char)(c)<<8)+(unsigned char)(i);
    }

    /* JML A, B, addr32 */
    if(OP==JML) {
      data = ((signed char)(c)<<24)+((unsigned char)(i)<<16)+((unsigned char)(j)<<8)+(unsigned char)(k);
      if(reg[a]<reg[b])
		pc = pc + data;
      else
		pc = pc + 7;
    }

    /* MOV A, val */
    if(OP==MOV) {
      data = (b<<24)+((unsigned char)(c)<<16)+((unsigned char)(i)<<8)+(unsigned char)(j);
      reg[a] = data;
      pc = pc + 6;
    }

    /* LDA A, B (A = MEM[B]) */
    if(OP==LDA) {
      reg[a] = mem[reg[b]];
      pc = pc + 3;
    }

    /* STO A, B (MEM[A] = B) */
    if(OP==STO) {
      mem[reg[a]] = reg[b];
      pc = pc + 3;
    }

    /* SHL A, B, C */
    if(OP==SHL) {
      reg[a] = reg[b] << reg[c];
      pc = pc + 4;
    }

    /* SHR A, B, C */
    if(OP==SHR) {
      reg[a] = reg[b] >> reg[c];
      pc = pc + 4;
    }

    /* NOT A, B */
    if(OP==NOT) {
      reg[a] = ~reg[b];
      pc = pc + 4;
    }

    /* AND A, B, C */
    if(OP==AND) {
      reg[a] = reg[b] & reg[c];
      pc = pc + 4;
    }

    /* IOR A, B, C */
    if(OP==IOR) {
      reg[a] = reg[b] | reg[c];
      pc = pc + 4;
    }

    /* XOR A, B, C */
    if(OP==XOR) {
      reg[a] = reg[b] ^ reg[c];
      pc = pc + 4;
    }

    /* ADD A, B, C */
    if(OP==ADD) {
      reg[a] = reg[b] + reg[c];
      pc = pc + 4;
    }

    if(OP==SYN) {
      pc = pc + 1;
      for(j=0; j<480; j++)
        for(i=0; i<640; i++) {
          p = (int *)surface->pixels+(i+j*640);
                  R = 85* (mem[65536+(i>>1)+(j>>1)*320]&0b00000011);
                  G = 85*((mem[65536+(i>>1)+(j>>1)*320]&0b00001100)>>2);
                  B = 85*((mem[65536+(i>>1)+(j>>1)*320]&0b00110000)>>4);
                  *p = (R<<16) + (G<<8) + B;
        }
      SDL_UpdateWindowSurface(window);
      pc = (mem[5]<<24)+(mem[6]<<16)+(mem[7]<<8)+(mem[8]);
      if(SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
    }
  }
  free(mem);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
