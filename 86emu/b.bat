set path=.\mingw64\bin
nasm -O0 -fbin mybios.asm -o bios
MinGW64\bin\gcc -m32 8086tiny.c -I.\SDL-1.2.15\include\SDL -L.\SDL-1.2.15\lib -lSDL
pause