nasm -f bin lm.asm -o lm
gcc -nostdlib -static -ffreestanding -c src.c
ld -T linker.ld -nostdlib -static src.o -o src.bin
cat longmode > kernel
cat src.bin >> kernel
