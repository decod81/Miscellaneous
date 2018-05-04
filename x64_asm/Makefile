main: cmd.o write.o exit.o
	gcc -nostdlib -ffreestanding main.c cmd.o write.o exit.o -lc -o main

hello:
	nasm -f elf64 hello.s
	gcc -nostdlib hello.o -o hello

hellocmd:
	nasm -f elf64 hellocmd.s
	gcc -nostdlib hellocmd.o -o hellocmd -lc

cmd.o:
	nasm -f elf64 cmd.s

write.o:
	nasm -f elf64 write.s

exit.o:
	nasm -f elf64 exit.s

clean:
	rm -rf *.o
	rm -f hellocmd
	rm -f hello
	rm -f main
