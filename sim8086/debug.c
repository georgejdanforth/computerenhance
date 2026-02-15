#include <assert.h>
#include <stdio.h>

#include "debug.h"
#include "sim.h"

static const char* InstructionTypeNames[] = {
		[IT_ADD] = "add",       [IT_CMP] = "cmp",     [IT_JA] = "ja",     [IT_JAE] = "jae",
		[IT_JB] = "jb",         [IT_JBE] = "jbe",     [IT_JCXZ] = "jcxz", [IT_JE] = "je",
		[IT_JG] = "jg",         [IT_JGE] = "jge",     [IT_JL] = "jl",     [IT_JLE] = "jle",
		[IT_JNE] = "jne",       [IT_JNO] = "jno",     [IT_JNS] = "jns",   [IT_JO] = "jo",
		[IT_JP] = "jp",         [IT_JPO] = "jpo",     [IT_JS] = "js",     [IT_LOOP] = "loop",
		[IT_LOOPNZ] = "loopnz", [IT_LOOPZ] = "loopz", [IT_MOV] = "mov",   [IT_SUB] = "sub",
};

const char* InstructionTypeName(InstructionType type) {
	const char* name = InstructionTypeNames[type];
	assert(name != NULL);
	return name;
}

static const char* RegisterNames[] = {
		[AX] = "ax", [BX] = "bx", [CX] = "cx", [DX] = "dx", [SP] = "sp", [BP] = "bp",
		[SI] = "si", [DI] = "di", [AL] = "al", [AH] = "ah", [BL] = "bl", [BH] = "bh",
		[CL] = "cl", [CH] = "ch", [DL] = "dl", [DH] = "dh",
};

const char* RegisterName(Register reg) {
	const char* name = RegisterNames[reg];
	assert(name != NULL);
	return name;
}

static void unparseLocPair(LocPair* locs, StringBuilder* sb);
static void unparseSignedDisplacement(SignedDisplacementInstruction* instr,
                                      StringBuilder* sb);
static void unparseLoc(Loc* loc, bool isWord, StringBuilder* sb);
static void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb);
static void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb);
static void unparseImmediateLoc(ImmediateLoc* loc, bool isWord, StringBuilder* sb);

static void cpuDiff(CPUPair* cpus, StringBuilder* sb);

void UnparseInstruction(Instruction* instr, CPUPair* cpus, StringBuilder* sb) {
	StringBuilderAppend(sb, InstructionTypeName(instr->oc.type));
	StringBuilderAppend(sb, " ");

	switch (instr->oc.type) {
		case IT_ADD:
		case IT_CMP:
		case IT_MOV:
		case IT_SUB:
			unparseLocPair(&(instr->locPair.locs), sb);
			break;
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
			unparseSignedDisplacement(&instr->signedDisp, sb);
			break;
		default:
			fprintf(stderr, "Unhandled instruction type: %02x\n", instr->oc.type);
			assert(false);
	}

	if (cpus != null) {
		cpuDiff(cpus, sb);
	}
}

static void unparseLocPair(LocPair* locs, StringBuilder* sb) {
	if (locs->dst.type == LOC_MEM && locs->src.type == LOC_IMM) {
		StringBuilderAppend(sb, locs->isWord ? "word " : "byte ");
	}
	unparseLoc(&locs->dst, locs->isWord, sb);
	StringBuilderAppend(sb, ", ");
	unparseLoc(&locs->src, locs->isWord, sb);

	/*
	if (cpu != null) {
	  char buf[18];
	  StringBuilderAppend(sb, " ; ");
	  void* dst = GetLocPtr(cpu, &locs->dst);
	  void* src = GetLocPtr(cpu, &locs->src);

	  if (locs->isWord) {
	    sprintf(buf, "0x%04x -> 0x%04x", *(u16*)dst, *(u16*)src);
	  } else {
	    sprintf(buf, "0x%02x -> 0x%02x", *(u8*)dst, *(u8*)src);
	  }

	  StringBuilderAppend(sb, buf);
	}
	*/
}

static void unparseSignedDisplacement(SignedDisplacementInstruction* instr,
                                      StringBuilder* sb) {
	char buf[4];
	sprintf(buf, "%d", instr->disp);
	StringBuilderAppend(sb, buf);
}

static void unparseLoc(Loc* loc, bool isWord, StringBuilder* sb) {
	switch (loc->type) {
		case LOC_REG:
			unparseRegisterLoc(&loc->reg, sb);
			return;
		case LOC_MEM:
			unparseMemoryLoc(&loc->mem, sb);
			return;
		case LOC_IMM:
			unparseImmediateLoc(&loc->imm, isWord, sb);
			return;
		default:
			fprintf(stderr, "Unparse not defined for mov loc: %d", loc->type);
			assert(false);
	}
}

static void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb) {
	StringBuilderAppend(sb, RegisterName(loc->reg));
}

static void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb) {
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

static void unparseImmediateLoc(ImmediateLoc* loc, bool isWord, StringBuilder* sb) {
	char buf[8];
	if (loc->isSigned) {
		sprintf(buf, "%d", (i16)loc->data);
	} else if (!isWord && (loc->data & 0x80)) {
		sprintf(buf, "%d", (i8)loc->data);
	} else {
		sprintf(buf, "%d", loc->data);
	}
	StringBuilderAppend(sb, buf);
}

static void printFlags(char* buf, u16 flags) {
#define IS_SET(f) (flags & f) == f
	int i = 0;
	// clang-format off
	if (IS_SET(CF)) buf[i++] = 'C';
	if (IS_SET(PF)) buf[i++] = 'P';
	if (IS_SET(AF)) buf[i++] = 'A';
	if (IS_SET(ZF)) buf[i++] = 'Z';
	if (IS_SET(SF)) buf[i++] = 'S';
	if (IS_SET(TF)) buf[i++] = 'T';
	// clang-format on
	buf[i] = '\0';
#undef IS_SET
}

static void cpuDiff(CPUPair* cpus, StringBuilder* sb) {
	char buf[32];
	bool hasDiff;
	StringBuilderAppend(sb, " ;");
	for (int i = 0; i < 16; i++) {
		Register reg = (Register)i;
		const char* regName = RegisterName(reg);
		void* prev = CPUGetRegisterPtr(&cpus->prev, reg);
		void* curr = CPUGetRegisterPtr(&cpus->curr, reg);
		switch (reg) {
			case AH:
			case AL:
			case BH:
			case BL:
			case CH:
			case CL:
			case DH:
			case DL:
				continue;
			default: {
				if (*(u16*)prev != *(u16*)curr) {
					sprintf(buf, " %s:0x%04x->0x%04x", regName, *(u16*)prev, *(u16*)curr);
					StringBuilderAppend(sb, buf);
					hasDiff = true;
				}
			}
		}
	}

	if (cpus->prev.flags != cpus->curr.flags) {
		printFlags(buf, cpus->prev.flags);
		StringBuilderAppend(sb, " flags:");
		StringBuilderAppend(sb, buf);
		StringBuilderAppend(sb, "->");
		printFlags(buf, cpus->curr.flags);
		StringBuilderAppend(sb, buf);
		hasDiff = true;
	}

	if (!hasDiff) {
		StringBuilderAppend(sb, " no diff");
	}
}

void printRegister(CPU* cpu, Register reg) {
	void* regPtr = CPUGetRegisterPtr(cpu, reg);
	switch (reg) {
		case AH:
		case AL:
		case BH:
		case BL:
		case CH:
		case CL:
		case DH:
		case DL:
			break;
		default: {
			u16 val = *(u16*)regPtr;
			printf("%s: 0x%04x (%d)\n", RegisterName(reg), val, val);
		}
	}
}

void DumpCPURegisters(CPU* cpu) {
	char buf[16];
	printf("Final registers:\n");
	for (int i = 0; i < 16; i++) {
		printRegister(cpu, (Register)i);
	}
	printFlags(buf, cpu->flags);
	printf("\nFlags: %s\n", buf);
}
