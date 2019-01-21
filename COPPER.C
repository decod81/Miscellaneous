#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include <math.h>

#define pi 3.14159265358979

unsigned char pal[900];
unsigned short stab[256];
unsigned short bartab[3];
unsigned short i, m, n;

int round(float x) {
  if(x - floor(x) < 0.5)
    return floor(x);
  else
    return ceil(x);
}

int main() {
  for(i=0; i<3; i++)
    bartab[i] = 25*i;
  for(i=0; i<256; i++)
    stab[i] = round(sin(2*pi*i/255)*86) + 86;
  while(!kbhit()) {
    for(n=0; n<3; n++) {
      for(i=0; i<64; i++)
        pal[n%3 + 3*stab[bartab[n]] + 3*i] = i;
      for(i=0; i<64; i++)
        pal[n%3 + 3*stab[bartab[n]] + 3*64 + 3*i] = 63 - i;
      bartab[n] = 1 + bartab[n]&255;
    }
    asm { cli }
    while((inp(0x3da)&8)!=0);
    while((inp(0x3da)&8)==0);
    n = 0;
    for(m=0; m<300; m++) {
      outp(0x3c8, 0);
      outp(0x3c9, pal[n]);
      outp(0x3c9, pal[n+1]);
      while((inp(0x3da)&1)!=0);
      while((inp(0x3da)&1)==0);
      outp(0x3c9, pal[n+2]);
      n = n + 3;
    }
    asm { sti }
  }
}