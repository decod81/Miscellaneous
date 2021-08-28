#include "SDL.h"
#include "cputable.h"
#include <sys/time.h>

#define IO_PORT_COUNT 0x10000
#define RAM_SIZE 0x10FFF0
#define REGS_BASE 0xF0000
#define VIDEO_RAM_SIZE 0x10000
#define GRAPHICS_UPDATE_DELAY 360000
#define KEYBOARD_TIMER_UPDATE_DELAY 20000

#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7
#define REG_ES 8
#define REG_CS 9
#define REG_SS 10
#define REG_DS 11
#define REG_ZERO 12
#define REG_SCRATCH 13
#define REG_AL 0
#define REG_AH 1
#define REG_CL 2
#define REG_CH 3
#define REG_DL 4
#define REG_DH 5
#define REG_BL 6
#define REG_BH 7
#define FLAG_CF 40
#define FLAG_PF 41
#define FLAG_AF 42
#define FLAG_ZF 43
#define FLAG_SF 44
#define FLAG_IF 46
#define FLAG_DF 47
#define FLAG_OF 48

#define TABLE_XLAT_OPCODE 8
#define TABLE_XLAT_SUBFUNCTION 9
#define TABLE_STD_FLAGS 10
#define TABLE_PARITY_FLAG 11
#define TABLE_BASE_INST_SIZE 12
#define TABLE_I_W_SIZE 13
#define TABLE_I_MOD_SIZE 14
#define TABLE_COND_JUMP_DECODE_A 15
#define TABLE_COND_JUMP_DECODE_B 16
#define TABLE_COND_JUMP_DECODE_C 17
#define TABLE_COND_JUMP_DECODE_D 18
#define TABLE_FLAGS_BITFIELDS 19

#define FLAGS_UPDATE_SZP 1
#define FLAGS_UPDATE_AO_ARITH 2
#define FLAGS_UPDATE_OC_LOGIC 4

#define DECODE_RM_REG scratch2_uint = 4 * !i_mod, op_to_addr = rm_addr = i_mod < 3 ? SEGREG(seg_override_en ? seg_override : instructions[((scratch2_uint + 3)<<8)+i_rm], instructions[(scratch2_uint<<8)+i_rm], regs16[instructions[((scratch2_uint + 1)<<8)+i_rm]] + instructions[((scratch2_uint + 2)<<8)+i_rm] * i_data1+) : GET_REG_ADDR(i_rm), op_from_addr = GET_REG_ADDR(i_reg), i_d && (scratch_uint = op_from_addr, op_from_addr = rm_addr, op_to_addr = scratch_uint)
#define GET_REG_ADDR(reg_id) (REGS_BASE + (i_w ? 2 * reg_id : 2 * reg_id + reg_id / 4 & 7))
#define TOP_BIT 8*(i_w + 1)
#define MUL_MACRO(op_data_type,out_regs) (set_opcode(0x10), out_regs[i_w + 1] = (op_result = CAST(op_data_type)mem[rm_addr] * (op_data_type)*out_regs) >> 16, regs16[REG_AX] = op_result, set_OF(set_CF(op_result - (op_data_type)op_result)))
#define DIV_MACRO(out_data_type,in_data_type,out_regs) (scratch_int = CAST(out_data_type)mem[rm_addr]) && !(scratch2_uint = (in_data_type)(scratch_uint = (out_regs[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (out_data_type)scratch2_uint) ? out_regs[i_w+1] = scratch_uint - scratch_int * (*out_regs = scratch2_uint) : pc_interrupt(0)
#define DAA_DAS(op1,op2,mask,min) set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF]) && (op_result = regs8[REG_AL] op1 6, set_CF(regs8[FLAG_CF] || (regs8[REG_AL] op2 scratch2_uint))), set_CF((((mask & 1 ? scratch2_uint : regs8[REG_AL]) & mask) > min) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] op1 0x60)
#define ADC_SBB_MACRO(a) OP(a##= regs8[FLAG_CF] +), set_CF(regs8[FLAG_CF] && (op_result == op_dest) || (a op_result < a(int)op_dest)), set_AF_OF_arith()
#define R_M_OP(dest,op,src) (i_w ? op_dest = CAST(unsigned short)dest, op_result = CAST(unsigned short)dest op (op_source = CAST(unsigned short)src) : (op_dest = dest, op_result = dest op (op_source = CAST(unsigned char)src)))
#define MEM_OP(dest,op,src) R_M_OP(mem[dest],op,mem[src])
#define OP(op) MEM_OP(op_to_addr,op,op_from_addr)
#define INDEX_INC(reg_id) (regs16[reg_id] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1))
#define R_M_PUSH(a) (i_w = 1, R_M_OP(mem[SEGREG(REG_SS, REG_SP, --)], =, a))
#define R_M_POP(a) (i_w = 1, regs16[REG_SP] += 2, R_M_OP(a, =, mem[SEGREG(REG_SS, REG_SP, -2+)]))
#define SEGREG(reg_seg,reg_ofs,op) 16 * regs16[reg_seg] + (unsigned short)(op regs16[reg_ofs])
#define SIGN_OF(a) (1 & (i_w ? CAST(short)a : a) >> (TOP_BIT - 1))
#define CAST(a) *(a*)&

unsigned char mem[RAM_SIZE], io_ports[IO_PORT_COUNT], *opcode_stream, *regs8, i_rm, i_w, i_reg, i_mod, i_mod_size, i_d, i_reg4bit, raw_opcode_id, xlat_opcode_id, extra, rep_mode, seg_override_en, rep_override_en, trap_flag, scratch_uchar, io_hi_lo, *vid_mem_base, spkr_en;
unsigned short *regs16, reg_ip, seg_override, file_index, wave_counter;
unsigned int op_source, op_dest, rm_addr, op_to_addr, op_from_addr, i_data0, i_data1, i_data2, scratch_uint, scratch2_uint, inst_counter, set_flags_type, pixel_colors[256], vmem_ctr;
int op_result, scratch_int;
SDL_AudioSpec sdl_audio = {44100, AUDIO_U8, 1, 0, 128};
SDL_Surface *sdl_screen;
SDL_Color sdl_colors[256];
SDL_Event sdl_event;
unsigned char *disk;

char set_CF(int new_CF)
{
	return regs8[FLAG_CF] = !!new_CF;
}

char set_AF(int new_AF)
{
	return regs8[FLAG_AF] = !!new_AF;
}

char set_OF(int new_OF)
{
	return regs8[FLAG_OF] = !!new_OF;
}

char set_AF_OF_arith()
{
	set_AF((op_source ^= op_dest ^ op_result) & 0x10);
	if(op_result == op_dest)
		return set_OF(0);
	else
		return set_OF(1 & (regs8[FLAG_CF] ^ op_source >> (TOP_BIT - 1)));
}

void make_flags()
{
	int i;
	scratch_uint = 0xF002;
	for(i=9; i--;)
		scratch_uint += regs8[FLAG_CF + i] << instructions[(TABLE_FLAGS_BITFIELDS<<8)+i];
}

void set_flags(int new_flags)
{
	int i;
	for(i=9; i--;)
		regs8[FLAG_CF + i] = !!(1 << instructions[(TABLE_FLAGS_BITFIELDS<<8)+i] & new_flags);
}

void set_opcode(unsigned char opcode)
{
	xlat_opcode_id = instructions[(TABLE_XLAT_OPCODE<<8)+(raw_opcode_id = opcode)];
	extra = instructions[(TABLE_XLAT_SUBFUNCTION<<8)+opcode];
	i_mod_size = instructions[(TABLE_I_MOD_SIZE<<8)+opcode];
	set_flags_type = instructions[(TABLE_STD_FLAGS<<8)+opcode];
}

char pc_interrupt(unsigned char interrupt_num)
{
	set_opcode(0xCD);
	make_flags();
	R_M_PUSH(scratch_uint);
	R_M_PUSH(regs16[REG_CS]);
	R_M_PUSH(reg_ip);
	MEM_OP(REGS_BASE + 2 * REG_CS, =, 4 * interrupt_num + 2);
	R_M_OP(reg_ip, =, mem[4 * interrupt_num]);

	return regs8[FLAG_IF] = 0;
}

int AAA_AAS(char which_operation)
{
	return (regs16[REG_AX] += 262 * which_operation*set_AF(set_CF(((regs8[REG_AL] & 0x0F) > 9) || regs8[FLAG_AF])), regs8[REG_AL] &= 0x0F);
}

void audio_callback(void *data, unsigned char *stream, int len)
{
	int i;
	for(i=0; i<len; i++)
		stream[i] = (spkr_en == 3) && CAST(unsigned short)mem[0x4AA] ? -((54 * wave_counter++ / CAST(unsigned short)mem[0x4AA]) & 1) : sdl_audio.silence;
	spkr_en = io_ports[0x61] & 3;
}

int WinMain()
{

        struct timeval t, u;
        double elapsed;
	float pit_tick;
	unsigned char pit, video;
	unsigned int i, j, k;
	int c, h, s;
	FILE *f;

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	sdl_audio.callback = audio_callback;
	sdl_audio.samples = 512;
	SDL_OpenAudio(&sdl_audio, 0);

	sdl_screen = SDL_SetVideoMode(640, 400, 8, 0);
	SDL_EnableUNICODE(1);

	f = fopen("bios", "rb");
	fread(mem + 0xF0000, 65536, 1, f);
	fclose(f);

	disk = (char *)malloc(10653697);
	f = fopen("xt.img", "rb");
	fread(disk, 10653696, 1, f);
	fclose(f);

	regs16 = (unsigned short *)(regs8 = mem + REGS_BASE);
	regs16[REG_CS] = 0xF000;
	reg_ip = 0x100;

	// terminate if CS:IP = 0:0
	for(; opcode_stream = mem + 16 * regs16[REG_CS] + reg_ip, opcode_stream != mem;)
	{
		set_opcode(*opcode_stream);
		i_w = (i_reg4bit = raw_opcode_id & 7) & 1;
		i_d = i_reg4bit / 2 & 1;
		i_data0 = CAST(short)opcode_stream[1];
		i_data1 = CAST(short)opcode_stream[2];
		i_data2 = CAST(short)opcode_stream[3];
		if(seg_override_en) seg_override_en--;
		if(rep_override_en) rep_override_en--;

		if(i_mod_size) // i_mod_size > 0 indicates use of i_mod/i_rm/i_reg
		{
			i_mod = (i_data0 & 0xFF) >> 6;
			i_rm = i_data0 & 7;
			i_reg = i_data0 / 8 & 7;

			if((!i_mod && i_rm == 6) || (i_mod == 2))
				i_data2 = CAST(short)opcode_stream[4];
			else
			if(i_mod != 1)
				i_data2 = i_data1;
			else
				i_data1 = (char)i_data1;

			DECODE_RM_REG;
		}

		switch(xlat_opcode_id)
		{
			case 0: // Conditional jump (JAE, JNAE, etc.)
				// i_w is the invert flag, e.g. i_w == 1 means JNAE, whereas i_w == 0 means JAE
				scratch_uchar = raw_opcode_id / 2 & 7;
				reg_ip += (char)i_data0 * (i_w ^ (regs8[instructions[(TABLE_COND_JUMP_DECODE_A<<8)+scratch_uchar]] || regs8[instructions[(TABLE_COND_JUMP_DECODE_B<<8)+scratch_uchar]] || regs8[instructions[(TABLE_COND_JUMP_DECODE_C<<8)+scratch_uchar]] ^ regs8[instructions[(TABLE_COND_JUMP_DECODE_D<<8)+scratch_uchar]]));
				break;
			case 1: // MOV reg, imm
				i_w = !!(raw_opcode_id & 8);
				R_M_OP(mem[GET_REG_ADDR(i_reg4bit)], =, i_data0);
				break;
			case 3: // PUSH regs16
				R_M_PUSH(regs16[i_reg4bit]);
				break;
			case 4: // POP regs16
				R_M_POP(regs16[i_reg4bit]);
				break;
			case 2: // INC|DEC regs16
				i_w = 1;
				i_d = 0;
				i_reg = i_reg4bit;
				DECODE_RM_REG;
				i_reg = extra;
			case 5: // INC|DEC|JMP|CALL|PUSH
				if(i_reg < 2) // INC|DEC
					MEM_OP(op_from_addr, += 1 - 2 * i_reg +, REGS_BASE + 2 * REG_ZERO),
					op_source = 1,
					set_AF_OF_arith(),
					set_OF(op_dest + 1 - i_reg == 1 << (TOP_BIT - 1)),
					(xlat_opcode_id == 5) && (set_opcode(0x10), 0); // Decode like ADC
				else if(i_reg != 6) // JMP|CALL
					i_reg - 3 || R_M_PUSH(regs16[REG_CS]), // CALL (far)
					i_reg & 2 && R_M_PUSH(reg_ip + 2 + i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6)), // CALL (near or far)
					i_reg & 1 && (regs16[REG_CS] = CAST(short)mem[op_from_addr + 2]), // JMP|CALL (far)
					R_M_OP(reg_ip, =, mem[op_from_addr]),
					set_opcode(0x9A); // Decode like CALL
				else // PUSH
					R_M_PUSH(mem[rm_addr]);
				break;
			case 6: // TEST r/m, imm16 / NOT|NEG|MUL|IMUL|DIV|IDIV reg
				op_to_addr = op_from_addr;
				if(i_reg==0)
				{
					set_opcode(0x20); // Decode like AND
					reg_ip += i_w + 1;
					R_M_OP(mem[op_to_addr], &, i_data2);
				} else
				if(i_reg==2)
				{
					OP(=~);
				} else
				if(i_reg==3)
				{
					OP(=-);
					op_dest = 0;
					set_opcode(0x28); // Decode like SUB
					set_CF(op_result > op_dest);
				} else
				if(i_reg==4)
				{
					if(i_w) MUL_MACRO(unsigned short, regs16);
					else MUL_MACRO(unsigned char, regs8);
				} else
				if(i_reg==5)
				{
					if(i_w) MUL_MACRO(short, regs16);
					else MUL_MACRO(char, regs8);
				} else
				if(i_reg==6)
				{
					if(i_w) DIV_MACRO(unsigned short, unsigned, regs16);
					else DIV_MACRO(unsigned char, unsigned short, regs8);
				} else
				if(i_reg==7)
				{
					if(i_w) DIV_MACRO(short, int, regs16);
					else DIV_MACRO(char, short, regs8);
				}
				break;
			case 7: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP AL/AX, immed
				rm_addr = REGS_BASE;
				i_data2 = i_data0;
				i_mod = 3;
				i_reg = extra;
				reg_ip--;
			case 8: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP reg, immed
				op_to_addr = rm_addr;
				regs16[REG_SCRATCH] = (i_d |= !i_w) ? (char)i_data2 : i_data2;
				op_from_addr = REGS_BASE + 2 * REG_SCRATCH;
				reg_ip += !i_d + 1;
				set_opcode(0x08 * (extra = i_reg));
			case 9: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP|MOV reg, r/m
				if(extra==0)
				{
					OP(+=);
					set_CF(op_result < op_dest);
				} else
				if(extra==1)
				{
					OP(|=);
				} else
				if(extra==2)
				{
					ADC_SBB_MACRO(+);
				} else
				if(extra==3)
				{
					ADC_SBB_MACRO(-);
				} else
				if(extra==4)
				{
					OP(&=);
				} else
				if(extra==5)
				{
					OP(-=);
					set_CF(op_result > op_dest);
				} else
				if(extra==6)
				{
					OP(^=);
				} else
				if(extra==7)
				{
					OP(-);
					set_CF(op_result > op_dest);
				} else
				if(extra==8)
				{
					OP(=);
				}
				break;
			case 10: // MOV sreg, r/m | POP r/m | LEA reg, r/m
				if(!i_w) // MOV
				{
					i_w = 1;
					i_reg += 8;
					DECODE_RM_REG;
					OP(=);
				} else
				if(!i_d) // LEA
				{
					seg_override_en = 1;
					seg_override = REG_ZERO;
					DECODE_RM_REG;
					R_M_OP(mem[op_from_addr], =, rm_addr);
				}
				else // POP
					R_M_POP(mem[rm_addr]);
				break;
			case 11: // MOV AL/AX, [loc]
				i_mod = i_reg = 0;
				i_rm = 6;
				i_data1 = i_data0;
				DECODE_RM_REG;
				MEM_OP(op_from_addr, =, op_to_addr);
				break;
			case 12: // ROL|ROR|RCL|RCR|SHL|SHR|???|SAR reg/mem, 1/CL/imm (80186)
				scratch2_uint = SIGN_OF(mem[rm_addr]);
				if(extra)
				{
					++reg_ip;
					scratch_uint = (char)i_data1;
				} else
				{
					if(i_d) scratch_uint = 31 & regs8[REG_CL];
					else scratch_uint = 1;
				}
				if(scratch_uint)
				{
					if(i_reg < 4) // Rotate operations
					{
						scratch_uint %= i_reg / 2 + TOP_BIT;
						R_M_OP(scratch2_uint, =, mem[rm_addr]);
					}
					if(i_reg & 1) // Rotate/shift right operations
						R_M_OP(mem[rm_addr], >>=, scratch_uint);
					else // Rotate/shift left operations
						R_M_OP(mem[rm_addr], <<=, scratch_uint);
					if(i_reg > 3) // Shift operations
						set_opcode(0x10); // Decode like ADC
					if(i_reg > 4) // SHR or SAR
						set_CF(op_dest >> (scratch_uint - 1) & 1);
				}
				if(i_reg==0) // ROL
				{
					R_M_OP(mem[rm_addr], += , scratch2_uint >> (TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result) ^ set_CF(op_result & 1));
				} else
				if(i_reg==1) // ROR
				{
					scratch2_uint &= (1 << scratch_uint) - 1;
					R_M_OP(mem[rm_addr], += , scratch2_uint << (TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result * 2) ^ set_CF(SIGN_OF(op_result)));
				} else
				if(i_reg==2) // RCL
				{
					R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (scratch_uint - 1)) + , scratch2_uint >> (1 + TOP_BIT - scratch_uint));
					set_OF(SIGN_OF(op_result) ^ set_CF(scratch2_uint & 1 << (TOP_BIT - scratch_uint)));
				} else
				if(i_reg==3) // RCR
				{
					R_M_OP(mem[rm_addr], += (regs8[FLAG_CF] << (TOP_BIT - scratch_uint)) + , scratch2_uint << (1 + TOP_BIT - scratch_uint));
					set_CF(scratch2_uint & 1 << (scratch_uint - 1));
					set_OF(SIGN_OF(op_result) ^ SIGN_OF(op_result * 2));
				} else
				if(i_reg==4) // SHL
				{
					set_OF(SIGN_OF(op_result) ^ set_CF(SIGN_OF(op_dest << (scratch_uint - 1))));
				} else
				if(i_reg==5) // SHR
				{
					set_OF(SIGN_OF(op_dest));
				} else
				if(i_reg==7) // SAR
				{
					scratch_uint < TOP_BIT || set_CF(scratch2_uint);
					set_OF(0);
					R_M_OP(mem[rm_addr], +=, scratch2_uint *= ~(((1 << TOP_BIT) - 1) >> scratch_uint));
				}
				break;
			case 13: // LOOPxx|JCZX
				regs16[REG_CX]--;
				scratch_uint = (regs16[REG_CX]!=0);
				if(i_reg4bit==0) // LOOPNZ
					scratch_uint &= !regs8[FLAG_ZF];
				else
				if(i_reg4bit==1) // LOOPZ
					scratch_uint &= regs8[FLAG_ZF];
				else
				if(i_reg4bit==3) // JCXXZ
					scratch_uint = !++regs16[REG_CX];
				reg_ip += scratch_uint*(char)i_data0;
				break;
			case 14: // JMP | CALL short/near
				reg_ip += 3 - i_d;
				if(!i_w)
				{
					if(i_d) // JMP far
					{
						reg_ip = 0;
						regs16[REG_CS] = i_data2;
					}
					else // CALL
						R_M_PUSH(reg_ip);
				}
				if(i_d && i_w) reg_ip += (char)i_data0;
				else reg_ip += i_data0;
				break;
			case 15: // TEST reg, r/m
				MEM_OP(op_from_addr, &, op_to_addr);
				break;
			case 16: // XCHG AX, regs16
				i_w = 1;
				op_to_addr = REGS_BASE;
				op_from_addr = GET_REG_ADDR(i_reg4bit);
			case 24: // NOP|XCHG reg, r/m
				if(op_to_addr != op_from_addr)
				{
					OP(^=);
					MEM_OP(op_from_addr, ^=, op_to_addr);
					OP(^=);
				}
				break;
			case 17: // MOVSx (extra=0)|STOSx (extra=1)|LODSx (extra=2)
				scratch2_uint = seg_override_en ? seg_override : REG_DS;
				for(scratch_uint = rep_override_en ? regs16[REG_CX] : 1; scratch_uint; scratch_uint--)
				{
					MEM_OP(extra < 2 ? SEGREG(REG_ES, REG_DI,) : REGS_BASE, =, extra & 1 ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,)),
					extra & 1 || INDEX_INC(REG_SI),
					extra & 2 || INDEX_INC(REG_DI);
				}

				if(rep_override_en) regs16[REG_CX] = 0;
				break;
			case 18: // CMPSx (extra=0)|SCASx (extra=1)
				scratch2_uint = seg_override_en ? seg_override : REG_DS;
				if((scratch_uint = rep_override_en ? regs16[REG_CX] : 1))
				{
					for(; scratch_uint; rep_override_en || scratch_uint--)
					{
						MEM_OP(extra ? REGS_BASE : SEGREG(scratch2_uint, REG_SI,), -, SEGREG(REG_ES, REG_DI,)),
						extra || INDEX_INC(REG_SI),
						INDEX_INC(REG_DI), rep_override_en && !(--regs16[REG_CX] && (!op_result == rep_mode)) && (scratch_uint = 0);
					}
					set_flags_type = FLAGS_UPDATE_SZP | FLAGS_UPDATE_AO_ARITH; // Funge to set SZP/AO flags
					set_CF(op_result > op_dest);
				}
				break;
			case 19: // RET|RETF|IRET
				i_d = i_w;
				R_M_POP(reg_ip);
				if(extra) // IRET|RETF|RETF imm16
					R_M_POP(regs16[REG_CS]);
				if(extra & 2) // IRET
					set_flags(R_M_POP(scratch_uint));
				else if(!i_d) // RET|RETF imm16
					regs16[REG_SP] += i_data0;
				break;
			case 20: // MOV r/m, immed
				R_M_OP(mem[op_from_addr], =, i_data2);
				break;
			case 21: // IN AL/AX, DX/imm8
				io_ports[0x42] = --io_ports[0x40]; // PIT channel 0/2 read placeholder
				io_ports[0x3DA] ^= 9; // CGA refresh
				scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
				R_M_OP(regs8[REG_AL], =, io_ports[scratch_uint]);
				break;
			case 22: // OUT DX/imm8, AL/AX
				scratch_uint = extra ? regs16[REG_DX] : (unsigned char)i_data0;
				R_M_OP(io_ports[scratch_uint], =, regs8[REG_AL]);
				scratch_uint == 0x61 && (io_hi_lo = 0, spkr_en |= regs8[REG_AL] & 3); // PC speaker control
				(scratch_uint == 0x42) && (io_ports[0x43] & 6) && (mem[0x469 + scratch_uint - (io_hi_lo ^= 1)] = regs8[REG_AL]); // PIT channel 2, used with PC speaker
				if(scratch_uint == 0x43 && regs8[REG_AL]==0x36) pit = 0;
				if(scratch_uint == 0x40)
				{
					if(pit==0) pit_tick = 1.0*regs8[REG_AL];
					if(pit++)
					{
						pit_tick += 256.0*regs8[REG_AL];
						if(pit_tick==0.0)
							pit_tick = 65536.0/1193.18;
						else
							pit_tick = pit_tick/1193.18;
					}
				}
				scratch_uint == 0x43 && (io_hi_lo = 0, regs8[REG_AL] >> 6 == 2) && (SDL_PauseAudio((regs8[REG_AL] & 0xF7) != 0xB6), 0); // Speaker enable
				break;
			case 23: // REPxx
				rep_override_en = 2;
				rep_mode = i_w;
				seg_override_en && seg_override_en++;
				break;
			case 25: // PUSH reg
				R_M_PUSH(regs16[extra]);
				break;
			case 26: // POP reg
				R_M_POP(regs16[extra]);
				break;
			case 27: // xS: segment overrides
				seg_override_en = 2;
				seg_override = extra;
				rep_override_en && rep_override_en++;
				break;
			case 28: // DAA/DAS
				i_w = 0;
				if(extra) DAA_DAS(-=, >=, 0xFF, 0x99);
				else DAA_DAS(+=, <, 0xF0, 0x90);
				break;
			case 29: // AAA/AAS
				op_result = AAA_AAS(extra - 1);
				break;
			case 30: // CBW
				regs8[REG_AH] = -SIGN_OF(regs8[REG_AL]);
				break;
			case 31: // CWD
				regs16[REG_DX] = -SIGN_OF(regs16[REG_AX]);
				break;
			case 32: // CALL FAR imm16:imm16
				R_M_PUSH(regs16[REG_CS]);
				R_M_PUSH(reg_ip + 5);
				regs16[REG_CS] = i_data2;
				reg_ip = i_data0;
				break;
			case 33: // PUSHF
				make_flags();
				R_M_PUSH(scratch_uint);
				break;
			case 34: // POPF
				set_flags(R_M_POP(scratch_uint));
				break;
			case 35: // SAHF
				make_flags();
				set_flags((scratch_uint & 0xFF00) + regs8[REG_AH]);
				break;
			case 36: // LAHF
				make_flags(),
				regs8[REG_AH] = scratch_uint;
				break;
			case 37: // LES|LDS reg, r/m
				i_w = i_d = 1;
				DECODE_RM_REG;
				OP(=);
				MEM_OP(REGS_BASE + extra, =, rm_addr + 2);
				break;
			case 39: // INT imm8
				reg_ip += 2;
				pc_interrupt(i_data0);
				break;
			case 41: // AAM
				if(i_data0 &= 0xFF)
				{
					regs8[REG_AH] = regs8[REG_AL] / i_data0;
					op_result = regs8[REG_AL] %= i_data0;
				}
				else // Divide by zero
					pc_interrupt(0);
				break;
			case 42: // AAD
				i_w = 0;
				regs16[REG_AX] = op_result = 0xFF & regs8[REG_AL] + i_data0 * regs8[REG_AH];
				break;
			case 43: // SALC
				regs8[REG_AL] = -regs8[FLAG_CF];
				break;
			case 44: // XLAT
				regs8[REG_AL] = mem[SEGREG(seg_override_en ? seg_override : REG_DS, REG_BX, regs8[REG_AL] +)];
				break;
			case 45: // CMC
				regs8[FLAG_CF] ^= 1;
				break;
			case 46: // CLC|STC|CLI|STI|CLD|STD
				regs8[extra / 2] = extra & 1;
				break;
			case 47: // TEST AL/AX, immed
				R_M_OP(regs8[REG_AL], &, i_data0);
				break;
			case 48: // Emulator-specific 0F xx opcodes
				if((char)i_data0==0) printf("%c", regs8[REG_AL]); // 0f00 = putchar(al)
				else
				if((char)i_data0==2) // 0f02 read sectors
				{
					// disk is type 1: 17 sectors, 4 heads, 306 cylinders
					c = ((regs8[REG_CL]&0xC0)<<2)|regs8[REG_CH];
					h = regs8[REG_DH];
					s = regs8[REG_CL]&63;
					j = 512*(17*(4*c+h)+s-1);
					for(i=0; i<512*regs8[REG_AL]; i++)
						mem[SEGREG(REG_ES, REG_BX,)+i] = disk[j + i];
				} else
				if((char)i_data0==3) // 0f03 change video mode
				{
					video = regs8[REG_AL];
					if(video==0x04)
					{
						sdl_colors[0].r = 0;
						sdl_colors[0].g = 0;
						sdl_colors[0].b = 0;

						sdl_colors[1].r = 0xC0;
						sdl_colors[1].g = 0xC0;
						sdl_colors[1].b = 0xC0;

						sdl_colors[2].r = 0x80;
						sdl_colors[2].g = 0x80;
						sdl_colors[2].b = 0x80;

						sdl_colors[3].r = 0xFF;
						sdl_colors[3].g = 0xFF;
						sdl_colors[3].b = 0xFF;

						SDL_SetColors(sdl_screen, sdl_colors, 0, 4);
					}
					if(video==0x0d)
					{
					}
					if(video==0x13)
					{
						for(i=0; i<256; i++)
						{
							sdl_colors[i].r = i<<2;
							sdl_colors[i].g = i<<2;
							sdl_colors[i].b = i<<2;
						}
						SDL_SetColors(sdl_screen, sdl_colors, 0, 256);
					}
				}
		}

		// increment instruction pointer by computed instruction length
		reg_ip += (i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6))*i_mod_size + instructions[(TABLE_BASE_INST_SIZE<<8)+raw_opcode_id] + instructions[(TABLE_I_W_SIZE<<8)+raw_opcode_id]*(i_w + 1);

		// update SF, ZF and PF as needed
		if(set_flags_type & FLAGS_UPDATE_SZP)
		{
			regs8[FLAG_SF] = SIGN_OF(op_result);
			regs8[FLAG_ZF] = !op_result;
			regs8[FLAG_PF] = instructions[(unsigned char)op_result + (TABLE_PARITY_FLAG<<8)];
			if(set_flags_type & FLAGS_UPDATE_AO_ARITH) set_AF_OF_arith();
			if(set_flags_type & FLAGS_UPDATE_OC_LOGIC) set_CF(0), set_OF(0);
		}

                if(!(inst_counter % GRAPHICS_UPDATE_DELAY))
                {
			if(video == 0x04)
                        {
				vid_mem_base = mem + 0xB8000;
				for(j=0; j<400; j++)
					for(i=0; i<640; i++)
					{
						k = (i>>1)+(j>>1)*640;
						((unsigned char*)sdl_screen->pixels)[i+j*640] = ( vid_mem_base[0x1000*(j&2)+(i>>3)+(j>>2)*80] >> (~i&6) ) & 3;
					}
                        }
			if(video == 0x0d)
			{
				vid_mem_base = mem + 0xA0000;
			}
			if(video == 0x13)
			{
				vid_mem_base = mem + 0xA0000;
				for(j=0; j<400; j++)
					for(i=0; i<640; i++)
						((unsigned char*)sdl_screen->pixels)[i+j*640] = vid_mem_base[(i>>1)+(j>>1)*320];
			}
			SDL_Flip(sdl_screen);
                        SDL_PumpEvents();
                }

		if(!(++inst_counter % 20000) && !seg_override_en && !rep_override_en && regs8[FLAG_IF])
		{
			gettimeofday(&u, 0);
			elapsed = (u.tv_sec - t.tv_sec)*1000.0 + (u.tv_usec - t.tv_usec)*0.001;
			if( elapsed > pit_tick )
			{
				pc_interrupt(8);
				t = u;
			}
			if(SDL_PollEvent(&sdl_event))
			{
				if(sdl_event.type == SDL_KEYDOWN) io_ports[0x60] = sdl_event.key.keysym.scancode;
				if(sdl_event.type == SDL_KEYUP) io_ports[0x60] = sdl_event.key.keysym.scancode|0x80;
				if(sdl_event.key.keysym.scancode==1)
				{
					SDL_Quit();
					exit(0);
				}
				pc_interrupt(9);
			}
		}
	}
	SDL_Quit();
	return 0;
}