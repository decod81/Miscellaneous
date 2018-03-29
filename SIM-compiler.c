/* Compiler for the simulation (SIM-CPU.c) of a simple CPU
 */
#include <stdio.h>

int isNumeric(char *s) {
	if(s == NULL || *s == '\0' || isspace(*s))
    	return 0;
    char *p;
    strtod(s, &p);
    return *p == '\0';
}

main() {
	FILE *f = fopen("source.asm", "r");
	FILE *g = fopen("executable.bin", "wb");
	char buf[256], cuf[256], *token, parameters[4][16];
	int a, b, c, d, i, j, k, l, row = 0, p, q;

	while(fgets(buf, 256, f)!=NULL) {
		printf("%d\n", row++);
		j = 0;
		for(i=0; i<256; i++) cuf[i] = 0;
		for(i=0; i<256; i++) if(buf[i]!=',' & buf[i]!='\n') cuf[j++] = buf[i];
		for(i=0; i<4; i++) parameters[i][0] = 0;
		token = strtok(cuf, " ");
		k = 0;
		l = 0;
		while(token!=NULL) {
			k++;
			if(isNumeric(token)) l++;
			strcpy(parameters[k-1], token);
			token = strtok(NULL, " ");
		}

		if(!strcmp(parameters[0], "jmp")) a = 0;
		if(!strcmp(parameters[0], "mov")) if(l>0) a = 1; else a = 2;
		if(!strcmp(parameters[0], "lda")) a = 3;
		if(!strcmp(parameters[0], "sto")) a = 4;
		if(!strcmp(parameters[0], "shl")) a = 5;
		if(!strcmp(parameters[0], "shr")) a = 6;
		if(!strcmp(parameters[0], "not")) a = 7;
		if(!strcmp(parameters[0], "and")) a = 8;
		if(!strcmp(parameters[0],  "or")) a = 9;
		if(!strcmp(parameters[0], "xor")) a = 10;
		if(!strcmp(parameters[0], "add")) a = 11;
		if(!strcmp(parameters[0], "mul")) a = 12;
		if(!strcmp(parameters[0], "div")) a = 13;
		if(!strcmp(parameters[0], "mod")) a = 14;

		b = parameters[1][0]-'A';
		c = parameters[2][0]-'A';

		// third parameter of command 0 is a number
		if(a==0) i = atoi(parameters[3]);

		// command 1 has only two parameters and the last one is a number
		if(a==1) { i = atoi(parameters[2]);	c = 0; }

		// commands 5-15 have a third parameter
		if(a>4)	d = parameters[3][0]-'A';

		// construct 16-bit command
		p = (a<<12) + (b<<8) + (c<<4) + d;

		// reverse endianness and write the command
		q =  ((p&0xff00)>>8)|((p&0x00ff)<<8);
		fwrite(&q, 2, 1, g);

		// commands 0 and 1 contain additional data
		if(a<2) {
			// reverse endianness and write the data
			j = ((i&0xff000000)>>24)|((i&0x00ff0000)>>8)|((i&0x0000ff00)<<8)|((i&0x000000ff)<<24);
			fwrite(&j, 4, 1, g);
		}
	}
	fclose(f);
	fclose(g);
}

/*
mov A, 110
mov B, 111
jmp B, C, 30
mov A, B
lda A, B
sto A, B
shl A, B, C
shr A, B, C
mov B, 2
mov C, -6
add A, B, C
mul A, B, C
div A, C, B
mov B, 4
mov C, 6
mod A, C, B
jmp A, A, 40
*/
