/* Simulation of a simple CPU
 *
 * 0	jmp A, B, addr32
 * 1	mov A, int32
 * 2	mov A, B
 * 3	lda A, B, C
 * 4	sto A, B, C
 * 5	shl A, B, C
 * 6	shr A, B, C
 * 7	not A, B
 * 8	and A, B, C
 * 9	or  A, B, C
 * 10	xor A, B, C
 * 11	add A, B, C
 * 12	mul A, B, C
 * 13	div A, B, C
 * 14	mod A, B, C
 *
 * for(x=0; x<256; x++) {..}		10 {..}
 *					100 if(x<256) jmp 10
 *
 * if(a!=b) {..}			if(a<b) jmp 20
 *					if(b<a) jmp 20
 *					jmp 100
 *					20 {...}
 *					100 ...
 *
 * if(a==b) {..}			if(a<b) jmp 100
 *					if(b<a) jmp 100
 *					{..}
 *					100...
 *
 * if(a<=b) {..}			if(b<a) jmp 100
 *					{..}
 *					100...
 */

#include <stdio.h>

int main() {
	unsigned char mem[65536];
	int reg[16], pc, steps = 0, data, p1, p2, p3, op;
	char regs[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};

	for(pc=0; pc<16; pc++) reg[pc] = 0;
	for(pc=0; pc<65536; pc++) mem[pc] = 0;

	FILE *f = fopen("executable.bin", "rb");
	fseek(f, 0, SEEK_END);
	pc = ftell(f);
	rewind(f);
	fread(mem, 1, pc, f);
	fclose(f);

	pc = 0;
	op = 255;
	printf("pc\topc\toperation\taction\n");
	for(steps=0; steps<24; steps++) {
		if(op== 0) { if((reg[p1]<reg[p2])|(p1==p2)) pc = data; else pc += 6;
			if(p1==p2) printf("\tjmp %d\t\tjmp %d\n", data, data);
			else printf("\tjmp %c, %c, %d\tif(%d<%d) jmp %d\n", regs[p1], regs[p2], data, reg[p1], reg[p2], data);
		}
		if(op== 1) { reg[p1] = data; pc += 6;
			printf("\tmov %c, %d\t%c = %d\n", regs[p1], data, regs[p1], data);
		}
		if(op== 2) { reg[p1] = reg[p2]; pc += 2;
			printf("\tmov %c, %c\t%c = %d\n", regs[p1], regs[p2], regs[p1], reg[p2]);
		}
		if(op== 3) { reg[p1] = mem[p2]; pc += 2;
			printf("\tlda %c, %c\t%c = mem[%d] = %d\n", regs[p1], regs[p2], regs[p1], p2, mem[p2]);
		}
		if(op== 4) { mem[p1] = reg[p2]; pc += 2;
			printf("\tsto %c, %c\tmem[%d] = %c = %d\n", regs[p1], regs[p2], p1, regs[p2], reg[p2]);
		}
		if(op== 5) { reg[p1] = reg[p2] << reg[p3]; pc += 2;
			printf("\tshl %c, %c, %c\t%c = %d << %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3]);
		}
		if(op== 6) { reg[p1] = reg[p2] >> reg[p3]; pc += 2;
			printf("\tshr %c, %c, %c\t%c = %d >> %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3]);
		}
		if(op== 7) { reg[p1] = ~reg[p2]; pc += 2; }
		if(op== 8) { reg[p1] = reg[p2]&reg[p3]; pc += 2; }
		if(op== 9) { reg[p1] = reg[p2]|reg[p3]; pc += 2; }
		if(op==10) { reg[p1] = reg[p2]^reg[p3];	pc += 2; }
		if(op==11) { reg[p1] = reg[p2] + reg[p3]; pc += 2;
			printf("\tadd %c, %c, %c\t%c = %d + %d = %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3], reg[p1]);
		}
		if(op==12) { reg[p1] = reg[p2] * reg[p3]; pc += 2;
			printf("\tmul %c, %c, %c\t%c = %d * %d = %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3], reg[p1]);
		}
		if(op==13) { reg[p1] = reg[p2] / reg[p3]; pc += 2;
			printf("\tdiv %c, %c, %c\t%c = %d / %d = %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3], reg[p1]);
		}
		if(op==14) { reg[p1] = reg[p2] % reg[p3]; pc += 2;
			printf("\tmod %c, %c, %c\t%c = %d %% %d = %d\n", regs[p1], regs[p2], regs[p3], regs[p1], reg[p2], reg[p3], reg[p1]);
		}
		op = mem[pc]>>4;
		p1 = mem[pc]&0b00001111;
		p2 = mem[pc+1]>>4;
		p3 = mem[pc+1]&0b00001111;
		data = (mem[pc+2]<<24) + (mem[pc+3]<<16) + (mem[pc+4]<<8) + (mem[pc+5]);
		printf("%d\t%d", pc, op);
	}
}
