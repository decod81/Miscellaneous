//#include <stdio.h>

extern void write(long long, void *, long long, long long);
extern void exit(void);

void main(long long argc, char *argv[]) {
	char msg[]="Hello!\n";
	write(1, &msg, sizeof(msg), 1);
	printf("%d\n", argc);
//	if(argc>1) printf("%s\n", &argv[2]);
//	exit();
	return;
}
