nasm -f bin lm.asm -o lm
#gcc -fno-pie -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -O0 -nostdlib -ffreestanding -c src.c
gcc -nostdlib -ffreestanding -c src.c
ld -T linker.ld -nostdlib src.o -o src.bin
cat lm > kernel
cat src.bin >> kernel
#dd if=/dev/zero of=./zero.bin count=8
#cat zero.bin >> kernel
