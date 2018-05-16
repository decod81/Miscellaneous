static inline void outb(short port, char val);

void main(void) {
  unsigned char *VGA = (unsigned char *)0xA0000;
  int i, j;

  for(i=0; i<64; i++) {
    outb(0x3C8, i);
    outb(0x3C9, i);
    outb(0x3C9, i);
    outb(0x3C9, i);

    outb(0x3C8, 64+i);
    outb(0x3C9, i);
    outb(0x3C9, 0);
    outb(0x3C9, 0);

    outb(0x3C8, 128+i);
    outb(0x3C9, 0);
    outb(0x3C9, i);
    outb(0x3C9, 0);

    outb(0x3C8, 192+i);
    outb(0x3C9, 0);
    outb(0x3C9, 0);
    outb(0x3C9, i);
  }

  for(j=0; j<50; j++)
    for(i=0; i<320; i++) {
      *(VGA+i+j*320) = i%64;
      *(VGA+i+(50+j)*320) = 64+i%64;
      *(VGA+i+(100+j)*320) = 128+i%64;
      *(VGA+i+(150+j)*320) = 192+i%64;
    }

  for(;;);
}

static inline void outb(short port, char val) {
  __asm__ __volatile__("outb %0, %1;"::"a"(val), "d"(port):);
}
