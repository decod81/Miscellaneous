/*
 * gcc -nostdlib -ffreestanding -c boot64b.c
 * ld -T boot64b.ld -nostdlib boot64b.o -o boot64b.bin
 * cat bootsect64b.bin > boot64b
 * cat boot64.bin >> boot64b
 *
 * boot64.ld
 *
 * OUTPUT_FORMAT(binary)
 * SECTIONS {
 *   . = 0x8000;
 *   .text : { *(.text) }
 *   .data : { *(.data) }
 *   .bss  : { *(.bss)  }
 * }
 */

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4
#define VGA_CRTC_DATA		0x3D5
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

unsigned short pos;
unsigned char *VGA = (unsigned char *)0xA0000;
unsigned char *TXT = (unsigned char *)0xB8000;
unsigned char MCGA[] = {
/* MISC */ 0x63,
/* SEQ */  0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */ 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
           0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
           0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
           0xFF,
/* GC */   0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
           0xFF,
/* AC */   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
           0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
           0x41, 0x00, 0x0F, 0x00, 0x00
};

void myhalt(void);
void outb(short port, char val);
unsigned char inb(unsigned short port);
void set_VGA_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
void clr(void);
void putchar(char c);
void write_regs(unsigned char *regs);
void write(char *data);

void main(void) {
  unsigned char x, y, z;
  int i, j;

  clr();

  /* 80x25 text mode */
  write("Hello, World!");
  pos = pos + 2*80;
  for(i=0; i<10; i++) {
    /* ps/2 keyboard example */
    while(!(inb(0x64)&1)); /* should be made interrupt driven */
    x = inb(0x60);
    if(x==0x10) {
      write("Q pressed ");
      x=0;
    }
    if(x==0x90) {
      write("Q released ");
      x=0;
    }
  }

  /* 320x200 @ 256 colors */
  write_regs(MCGA);
  for(i=0; i<64; i++) {
    set_VGA_palette(i, i, i, i);
    set_VGA_palette(64+i, i, 0, 0);
    set_VGA_palette(128+i, 0, i, 0);
    set_VGA_palette(192+i, 0, 0, i);
  }
  for(j=0; j<50; j++)
    for(i=0; i<320; i++) {
      *(VGA+i+j*320) = i%64;
      *(VGA+i+(50+j)*320) = 64+i%64;
      *(VGA+i+(100+j)*320) = 128+i%64;
      *(VGA+i+(150+j)*320) = 192+i%64;
    }
  myhalt();
}

void myhalt(void) {
  __asm__ __volatile__("hlt");
}

//static inline void outb(short port, char val) {
void outb(short port, char val) {
  __asm__ __volatile__("outb %0, %1;"::"a"(val), "d"(port));
}

//static inline unsigned char inb(unsigned short port) {
unsigned char inb(unsigned short port) {
  unsigned char ret;
  __asm__ __volatile__("inb %1, %0":"=a"(ret):"d"(port));
  return ret;
}

void set_VGA_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b) {
  outb(0x3C8, index);
  outb(0x3C9, r);
  outb(0x3C9, g);
  outb(0x3C9, b);
}

void set_VGA_cursor(unsigned char x, unsigned char y) {
  unsigned short a = x+y*80;

  outb(0x03D4, 0x0F);
  outb(0x03D5, a & 0xFF);
  outb(0x03D4, 0x0E);
  outb(0x03D5, (a >> 8) & 0xFF);
  pos = 2*(x+y*80);
}

void clr(void) {
  for(int i=0; i<80*25; i++)
    *(TXT+2*i) = 0;
  set_VGA_cursor(0, 0);
}

void put_VGA_char(char c) {
  *(TXT+pos) = c;
  pos = pos + 2;
}

void write(char *data) {
  for(int i=0; data[i]!=0; i++)
    put_VGA_char(data[i]);
  outb(0x03D4, 0x0F);
  outb(0x03D5, (pos>>1) & 0xFF);
  outb(0x03D4, 0x0E);
  outb(0x03D5, (pos>>9) & 0xFF);
}

void write_regs(unsigned char *regs) {
  unsigned char i;

  outb(VGA_MISC_WRITE, *regs);
  regs++;

  for(i=0; i<VGA_NUM_SEQ_REGS; i++) {
    outb(VGA_SEQ_INDEX, i);
    outb(VGA_SEQ_DATA, *regs);
    regs++;
  }

  outb(VGA_CRTC_INDEX, 0x03);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
  outb(VGA_CRTC_INDEX, 0x11);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);

  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;

  for(i=0; i<VGA_NUM_CRTC_REGS; i++) {
    outb(VGA_CRTC_INDEX, i);
    outb(VGA_CRTC_DATA, *regs);
    regs++;
  }

  for(i=0; i<VGA_NUM_GC_REGS; i++) {
    outb(VGA_GC_INDEX, i);
    outb(VGA_GC_DATA, *regs);
    regs++;
  }

  for(i=0; i<VGA_NUM_AC_REGS; i++) {
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, i);
    outb(VGA_AC_WRITE, *regs);
    regs++;
  }

  (void)inb(VGA_INSTAT_READ);
  outb(VGA_AC_INDEX, 0x20);
}
