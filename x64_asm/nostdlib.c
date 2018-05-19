#include <stdio.h>

extern void write(long long, void *, long long);
// extern void exit(void);

void main(long long argc, long long arg1, long long arg2) {
	char msg[]="Hello!\n";
	write(1, &msg, sizeof(msg));
	printf("Number of arguments = %d\n", argc);
	printf("First argument = %s\n", (char*)(arg2));
	return;
}
