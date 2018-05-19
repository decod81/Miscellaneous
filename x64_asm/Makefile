kernel64:
	nasm -f bin bootsect64.asm -o bootsect64.bin
	gcc -nostdlib -ffreestanding -c boot64.c
	ld -T boot64.ld boot64.o -o boot64.bin
	cat bootsect64.bin > kernel64.bin
	cat boot64.bin >> kernel64.bin

kernel16:
	nasm -f bin bootsect16.asm -o kernel16.bin

hello:
	nasm -f elf64 hello.s -o hello.o
	gcc -nostdlib hello.o -o hello

nostdlib:
	nasm -f elf64 cmd.s
	nasm -f elf64 write.s
	nasm -f elf64 exit.s
	gcc -nostdlib -ffreestanding nostdlib.c cmd.o write.o exit.o -lc -o nostdlib

clean:
	rm -rf *.o
	rm -rf *.bin
	rm -f nostdlib
	rm -f hello
