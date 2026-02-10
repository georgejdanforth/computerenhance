#include <assert.h>
#include <stdio.h>

#include "instruction.h"

const OpCode OpCodeTable[] = {
		[0x00] = {IT_ADD, ENC_MODRM},
		[0x01] = {IT_ADD, ENC_MODRM},
		[0x02] = {IT_ADD, ENC_MODRM},
		[0x03] = {IT_ADD, ENC_MODRM},
		[0x04] = {IT_ADD, ENC_ACC_IMM},
		[0x05] = {IT_ADD, ENC_ACC_IMM},
		[0x06] = {0},
		[0x07] = {0},
		[0x08] = {0},
		[0x09] = {0},
		[0x0A] = {0},
		[0x0B] = {0},
		[0x0C] = {0},
		[0x0D] = {0},
		[0x0E] = {0},
		[0x0F] = {0},
		[0x10] = {0},
		[0x11] = {0},
		[0x12] = {0},
		[0x13] = {0},
		[0x14] = {0},
		[0x15] = {0},
		[0x16] = {0},
		[0x17] = {0},
		[0x18] = {0},
		[0x19] = {0},
		[0x1A] = {0},
		[0x1B] = {0},
		[0x1C] = {0},
		[0x1D] = {0},
		[0x1E] = {0},
		[0x1F] = {0},
		[0x20] = {0},
		[0x21] = {0},
		[0x22] = {0},
		[0x23] = {0},
		[0x24] = {0},
		[0x25] = {0},
		[0x26] = {0},
		[0x27] = {0},
		[0x28] = {IT_SUB, ENC_MODRM},
		[0x29] = {IT_SUB, ENC_MODRM},
		[0x2A] = {IT_SUB, ENC_MODRM},
		[0x2B] = {IT_SUB, ENC_MODRM},
		[0x2C] = {IT_SUB, ENC_ACC_IMM},
		[0x2D] = {IT_SUB, ENC_ACC_IMM},
		[0x2E] = {0},
		[0x2F] = {0},
		[0x30] = {0},
		[0x31] = {0},
		[0x32] = {0},
		[0x33] = {0},
		[0x34] = {0},
		[0x35] = {0},
		[0x36] = {0},
		[0x37] = {0},
		[0x38] = {IT_CMP, ENC_MODRM},
		[0x39] = {IT_CMP, ENC_MODRM},
		[0x3A] = {IT_CMP, ENC_MODRM},
		[0x3B] = {IT_CMP, ENC_MODRM},
		[0x3C] = {IT_CMP, ENC_ACC_IMM},
		[0x3D] = {IT_CMP, ENC_ACC_IMM},
		[0x3E] = {0},
		[0x3F] = {0},
		[0x40] = {0},
		[0x41] = {0},
		[0x42] = {0},
		[0x43] = {0},
		[0x44] = {0},
		[0x45] = {0},
		[0x46] = {0},
		[0x47] = {0},
		[0x48] = {0},
		[0x49] = {0},
		[0x4A] = {0},
		[0x4B] = {0},
		[0x4C] = {0},
		[0x4D] = {0},
		[0x4E] = {0},
		[0x4F] = {0},
		[0x50] = {0},
		[0x51] = {0},
		[0x52] = {0},
		[0x53] = {0},
		[0x54] = {0},
		[0x55] = {0},
		[0x56] = {0},
		[0x57] = {0},
		[0x58] = {0},
		[0x59] = {0},
		[0x5A] = {0},
		[0x5B] = {0},
		[0x5C] = {0},
		[0x5D] = {0},
		[0x5E] = {0},
		[0x5F] = {0},
		[0x60] = {0},
		[0x61] = {0},
		[0x62] = {0},
		[0x63] = {0},
		[0x64] = {0},
		[0x65] = {0},
		[0x66] = {0},
		[0x67] = {0},
		[0x68] = {0},
		[0x69] = {0},
		[0x6A] = {0},
		[0x6B] = {0},
		[0x6C] = {0},
		[0x6D] = {0},
		[0x6E] = {0},
		[0x6F] = {0},
		[0x70] = {IT_JO, ENC_IP_INC8},
		[0x71] = {IT_JNO, ENC_IP_INC8},
		[0x72] = {IT_JB, ENC_IP_INC8},
		[0x73] = {IT_JAE, ENC_IP_INC8},
		[0x74] = {IT_JE, ENC_IP_INC8},
		[0x75] = {IT_JNE, ENC_IP_INC8},
		[0x76] = {IT_JBE, ENC_IP_INC8},
		[0x77] = {IT_JA, ENC_IP_INC8},
		[0x78] = {IT_JS, ENC_IP_INC8},
		[0x79] = {IT_JNS, ENC_IP_INC8},
		[0x7A] = {IT_JP, ENC_IP_INC8},
		[0x7B] = {IT_JPO, ENC_IP_INC8},
		[0x7C] = {IT_JL, ENC_IP_INC8},
		[0x7D] = {IT_JGE, ENC_IP_INC8},
		[0x7E] = {IT_JLE, ENC_IP_INC8},
		[0x7F] = {IT_JG, ENC_IP_INC8},
		[0x80] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x81] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x82] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x83] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x84] = {0},
		[0x85] = {0},
		[0x86] = {0},
		[0x87] = {0},
		[0x88] = {IT_MOV, ENC_MODRM},
		[0x89] = {IT_MOV, ENC_MODRM},
		[0x8A] = {IT_MOV, ENC_MODRM},
		[0x8B] = {IT_MOV, ENC_MODRM},
		[0x8C] = {0},
		[0x8D] = {0},
		[0x8E] = {0},
		[0x8F] = {0},
		[0x90] = {0},
		[0x91] = {0},
		[0x92] = {0},
		[0x93] = {0},
		[0x94] = {0},
		[0x95] = {0},
		[0x96] = {0},
		[0x97] = {0},
		[0x98] = {0},
		[0x99] = {0},
		[0x9A] = {0},
		[0x9B] = {0},
		[0x9C] = {0},
		[0x9D] = {0},
		[0x9E] = {0},
		[0x9F] = {0},
		[0xA0] = {IT_MOV, ENC_ACC_MEM},
		[0xA1] = {IT_MOV, ENC_ACC_MEM},
		[0xA2] = {IT_MOV, ENC_ACC_MEM},
		[0xA3] = {IT_MOV, ENC_ACC_MEM},
		[0xA4] = {0},
		[0xA5] = {0},
		[0xA6] = {0},
		[0xA7] = {0},
		[0xA8] = {0},
		[0xA9] = {0},
		[0xAA] = {0},
		[0xAB] = {0},
		[0xAC] = {0},
		[0xAD] = {0},
		[0xAE] = {0},
		[0xAF] = {0},
		[0xB0] = {IT_MOV, ENC_REG_IMM},
		[0xB1] = {IT_MOV, ENC_REG_IMM},
		[0xB2] = {IT_MOV, ENC_REG_IMM},
		[0xB3] = {IT_MOV, ENC_REG_IMM},
		[0xB4] = {IT_MOV, ENC_REG_IMM},
		[0xB5] = {IT_MOV, ENC_REG_IMM},
		[0xB6] = {IT_MOV, ENC_REG_IMM},
		[0xB7] = {IT_MOV, ENC_REG_IMM},
		[0xB8] = {IT_MOV, ENC_REG_IMM},
		[0xB9] = {IT_MOV, ENC_REG_IMM},
		[0xBA] = {IT_MOV, ENC_REG_IMM},
		[0xBB] = {IT_MOV, ENC_REG_IMM},
		[0xBC] = {IT_MOV, ENC_REG_IMM},
		[0xBD] = {IT_MOV, ENC_REG_IMM},
		[0xBE] = {IT_MOV, ENC_REG_IMM},
		[0xBF] = {IT_MOV, ENC_REG_IMM},
		[0xC0] = {0},
		[0xC1] = {0},
		[0xC2] = {0},
		[0xC3] = {0},
		[0xC4] = {0},
		[0xC5] = {0},
		[0xC6] = {IT_MOV, ENC_MODRM_IMM},
		[0xC7] = {IT_MOV, ENC_MODRM_IMM},
		[0xC8] = {0},
		[0xC9] = {0},
		[0xCA] = {0},
		[0xCB] = {0},
		[0xCC] = {0},
		[0xCD] = {0},
		[0xCE] = {0},
		[0xCF] = {0},
		[0xD0] = {0},
		[0xD1] = {0},
		[0xD2] = {0},
		[0xD3] = {0},
		[0xD4] = {0},
		[0xD5] = {0},
		[0xD6] = {0},
		[0xD7] = {0},
		[0xD8] = {0},
		[0xD9] = {0},
		[0xDA] = {0},
		[0xDB] = {0},
		[0xDC] = {0},
		[0xDD] = {0},
		[0xDE] = {0},
		[0xDF] = {0},
		[0xE0] = {IT_LOOPNZ, ENC_IP_INC8},
		[0xE1] = {IT_LOOPZ, ENC_IP_INC8},
		[0xE2] = {IT_LOOP, ENC_IP_INC8},
		[0xE3] = {IT_JCXZ, ENC_IP_INC8},
		[0xE4] = {0},
		[0xE5] = {0},
		[0xE6] = {0},
		[0xE7] = {0},
		[0xE8] = {0},
		[0xE9] = {0},
		[0xEA] = {0},
		[0xEB] = {0},
		[0xEC] = {0},
		[0xED] = {0},
		[0xEE] = {0},
		[0xEF] = {0},
		[0xF0] = {0},
		[0xF1] = {0},
		[0xF2] = {0},
		[0xF3] = {0},
		[0xF4] = {0},
		[0xF5] = {0},
		[0xF6] = {0},
		[0xF7] = {0},
		[0xF8] = {0},
		[0xF9] = {0},
		[0xFA] = {0},
		[0xFB] = {0},
		[0xFC] = {0},
		[0xFD] = {0},
		[0xFE] = {0},
		[0xFF] = {0},
};

void unparseLocPair(LocPair* instr, StringBuilder* sb);
void unparseSignedDisplacement(SignedDisplacementInstruction* instr, StringBuilder* sb);
void unparseLoc(Loc* loc, StringBuilder* sb);
void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb);
void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb);
void unparseImmediateLoc(ImmediateLoc* loc, StringBuilder* sb);

void InstructionUnparse(Instruction* instr, StringBuilder* sb) {
#define APPEND_INSTR(i)                                                                 \
	StringBuilderAppend(sb, i " ");                                                       \
	break;

	// clang-format off
	switch (instr->oc.type) {
		case IT_ADD: APPEND_INSTR("add");
		case IT_CMP: APPEND_INSTR("cmp");
		case IT_MOV: APPEND_INSTR("mov");
		case IT_SUB: APPEND_INSTR("sub");
		case IT_JA: APPEND_INSTR("ja");
		case IT_JAE: APPEND_INSTR("jae");
		case IT_JB: APPEND_INSTR("jb");
		case IT_JBE: APPEND_INSTR("jbe");
		case IT_JCXZ: APPEND_INSTR("jcxz");
		case IT_JE: APPEND_INSTR("je");
		case IT_JG: APPEND_INSTR("jg");
		case IT_JGE: APPEND_INSTR("jge");
		case IT_JL: APPEND_INSTR("jl");
		case IT_JLE: APPEND_INSTR("jle");
		case IT_JNE: APPEND_INSTR("jne");
		case IT_JNO: APPEND_INSTR("jno");
		case IT_JNS: APPEND_INSTR("jns");
		case IT_JO: APPEND_INSTR("jo");
		case IT_JP: APPEND_INSTR("jp");
		case IT_JPO: APPEND_INSTR("jpo");
		case IT_JS: APPEND_INSTR("js");
		case IT_LOOP: APPEND_INSTR("loop");
		case IT_LOOPNZ: APPEND_INSTR("loopnz");
		case IT_LOOPZ: APPEND_INSTR("loopz");
		default:
			// panic
			fprintf(stderr, "Unhandled instruction type: %02x\n", instr->oc.type);
			assert(false);
	}
	// clang-format on

	switch (instr->oc.type) {
		case IT_ADD:
		case IT_CMP:
		case IT_MOV:
		case IT_SUB:
			return unparseLocPair(&(instr->locPair.locs), sb);
		case IT_JA:
		case IT_JAE:
		case IT_JB:
		case IT_JBE:
		case IT_JCXZ:
		case IT_JE:
		case IT_JG:
		case IT_JGE:
		case IT_JL:
		case IT_JLE:
		case IT_JNE:
		case IT_JNO:
		case IT_JNS:
		case IT_JO:
		case IT_JP:
		case IT_JPO:
		case IT_JS:
		case IT_LOOP:
		case IT_LOOPNZ:
		case IT_LOOPZ:
			return unparseSignedDisplacement(&instr->signedDisp, sb);
		default:
			// panic
			fprintf(stderr, "Unhandled instruction type: %02x\n", instr->oc.type);
			assert(false);
	}
	// panic
	fprintf(stderr, "Unparse not defined for instruction type: %d\n", instr->oc.type);
	assert(false);

#undef APPEND_INSTR
}

void unparseLocPair(LocPair* locs, StringBuilder* sb) {
	unparseLoc(&locs->dst, sb);
	StringBuilderAppend(sb, ", ");
	unparseLoc(&locs->src, sb);
}

void unparseSignedDisplacement(SignedDisplacementInstruction* instr, StringBuilder* sb) {
	char buf[4];
	sprintf(buf, "%d", instr->disp);
	StringBuilderAppend(sb, buf);
}

void unparseLoc(Loc* loc, StringBuilder* sb) {
	switch (loc->type) {
		case LOC_REG:
			unparseRegisterLoc(&loc->reg, sb);
			return;
		case LOC_MEM:
			unparseMemoryLoc(&loc->mem, sb);
			return;
		case LOC_IMM:
			unparseImmediateLoc(&loc->imm, sb);
			return;
		default:
			fprintf(stderr, "Unparse not defined for mov loc: %d", loc->type);
			assert(false);
	}
}

void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb) {
#define APPEND_REGISTER(r)                                                              \
	StringBuilderAppend(sb, r);                                                           \
	return;

	// clang-format off
	switch (loc->reg) {
		case AX: APPEND_REGISTER("ax");
		case BX: APPEND_REGISTER("bx");
		case CX: APPEND_REGISTER("cx");
		case DX: APPEND_REGISTER("dx");
		case SP: APPEND_REGISTER("sp");
		case BP: APPEND_REGISTER("bp");
		case SI: APPEND_REGISTER("si");
		case DI: APPEND_REGISTER("di");
		case AL: APPEND_REGISTER("al");
		case BL: APPEND_REGISTER("bl");
		case CL: APPEND_REGISTER("cl");
		case DL: APPEND_REGISTER("dl");
		case AH: APPEND_REGISTER("ah");
		case BH: APPEND_REGISTER("bh");
		case CH: APPEND_REGISTER("ch");
		case DH: APPEND_REGISTER("dh");
	}
	// clang-format on
	// panic
	fprintf(stderr, "Unhandled register in unparse: %d", loc->reg);
	assert(false);
#undef APPEND_REGISTER
}

void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb) {
#define APPEND_EA(ea)                                                                   \
	StringBuilderAppend(sb, ea);                                                          \
	break;

	StringBuilderAppend(sb, "[");

	// clang-format off
	switch(loc->ea) {
		case EA_BX_SI: APPEND_EA("bx + si");
		case EA_BX_DI: APPEND_EA("bx + di");
		case EA_BP_SI: APPEND_EA("bp + si");
		case EA_BP_DI: APPEND_EA("bp + di");
		case EA_SI: APPEND_EA("si");
		case EA_DI: APPEND_EA("di");
		case EA_BP: APPEND_EA("bp");
		case EA_BX: APPEND_EA("bx");
		case EA_NONE: break;
	}
	// clang-format on

	if (loc->disp != 0) {
		char buf[12];
		i16 disp = (i16)loc->disp;
		if (loc->ea == EA_NONE) {
			sprintf(buf, "%d", loc->disp);
		} else if (disp < 0) {
			sprintf(buf, " - %d", -disp);
		} else {
			sprintf(buf, " + %d", disp);
		}
		StringBuilderAppend(sb, buf);
	}

	StringBuilderAppend(sb, "]");

#undef APPEND_EA
}

void unparseImmediateLoc(ImmediateLoc* loc, StringBuilder* sb) {
	char buf[8];
	if (loc->isSigned) {
		sprintf(buf, "%d", (i16)loc->data);
	} else {
		sprintf(buf, "%d", loc->data);
	}
	StringBuilderAppend(sb, buf);
}
