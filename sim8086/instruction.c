#include <assert.h>
#include <stdio.h>

#include "instruction.h"

void unparseMov(MovInstruction* instr, StringBuilder* sb);
void unparseMovLoc(MovLoc* loc, StringBuilder* sb);
void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb);
void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb);
void unparseImmediateLoc(ImmediateLoc* loc, StringBuilder* sb);

void InstructionUnparse(Instruction* instr, StringBuilder* sb) {
	switch (instr->type) {
		case IT_MOV:
			return unparseMov((MovInstruction*)instr, sb);
	}
	// panic
	fprintf(stderr, "Unparse not defined for instruction type: %d", instr->type);
	assert(false);
}

void unparseMov(MovInstruction* instr, StringBuilder* sb) {
	StringBuilderAppend(sb, "mov ");
	unparseMovLoc(instr->dst, sb);
	StringBuilderAppend(sb, ", ");
	unparseMovLoc(instr->src, sb);
}

void unparseMovLoc(MovLoc* loc, StringBuilder* sb) {
	switch (loc->type) {
		case ML_Register:
			unparseRegisterLoc((RegisterLoc*)loc, sb);
			return;
		case ML_Memory:
			unparseMemoryLoc((MemoryLoc*)loc, sb);
			return;
		case ML_Immediate:
			unparseImmediateLoc((ImmediateLoc*)loc, sb);
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
		char buf[9];
		if (loc->ea == EA_NONE) {
			sprintf(buf, "%d", loc->disp);
		} else {
			sprintf(buf, " + %d", loc->disp);
		}
		StringBuilderAppend(sb, buf);
	}

	StringBuilderAppend(sb, "]");

#undef APPEND_EA
}

void unparseImmediateLoc(ImmediateLoc* loc, StringBuilder* sb) {
	char buf[6];
	sprintf(buf, "%d", loc->data);
	StringBuilderAppend(sb, buf);
}
