#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val);

main() {
  unsigned char *VGA = (unsigned char *)0xA0000;
  int i, j;

  for(i=0; i<64; i++) {
    outb(0x3C8, i);
    outb(0x3C9, i);
    outb(0x3C9, i);
    outb(0x3C9, i);
  }

  for(j=0; j<200; j++)
    for(i=0; i<320; i++) {
      *(VGA+(i+j*320)) = i%64;
    }

  while(1);
}

static inline void outb(uint16_t port, uint8_t val) {
  __asm__ __volatile__("outb %0, %1;"::"a"(val), "d"(port):);
}
