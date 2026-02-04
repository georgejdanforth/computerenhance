#include <assert.h>
#include <stdio.h>

#include "instruction.h"

void unparseMov(MovInstruction* instr, StringBuilder* sb);
void unparseMovLoc(MovLoc* loc, StringBuilder* sb);
void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb);

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
