/* gcc -fno-pie -std=gnu99 -Os -nostdlib -m32 -march=i386 -ffreestanding -o hello.com -Wl,--nmagic,--script=com.ld hellodos.c */

/* com.ld
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0x0100;
    .text :
    {
        *(.text);
    }
    .data :
    {
        *(.data);
        *(.bss);
        *(.rodata);
    }
    _heap = ALIGN(4);
}
*/
__asm__ (".code16gcc\n"
                      "call  dosmain\n"
                      "mov   $0x4C,%ah\n"
                      "int   $0x21\n");

static void print(char *string) {
  __asm__ __volatile__ ("mov   $0x09, %%ah\n"
                        "int   $0x21\n"
                        : /* no output */
                        : "d"(string)
                        : "ah");
}

int dosmain(void) {
  print("Hello, World!\n$");
  return 0;
}
