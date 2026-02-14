#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "sim.h"

static void execMov(CPU* cpu, MovInstruction* instr, StringBuilder* sb);

void CPUInit(CPU* cpu) {
	assert(cpu != null);
	// Zero out all registers to begin with.
	memset(cpu->regs, 0, sizeof(u16) * NUM_REGISTERS);
}

void CPUExec(CPU* cpu, Instruction* instr, StringBuilder* sb) {
#define EXEC(_fn, _instr)                                                               \
	_fn(cpu, _instr, sb);                                                                 \
	break;

	// clang-format off
	switch (instr->oc.type) {
		case IT_MOV: EXEC(execMov, (MovInstruction*)&instr->locPair);
		default:
			fprintf(stderr, "Unhandled instruction in CPUExec: %d\n", instr->oc.type);
			assert(false);
	}
		// clang-format on

#undef EXEC
}

static const usize regOffsets[] = {
		[AX] = 0, [AH] = 0, [AL] = 1, [BX] = 2, [BH] = 2, [BL] = 3,  [CX] = 4,  [CH] = 4,
		[CL] = 5, [DX] = 6, [DH] = 6, [DL] = 7, [SP] = 8, [BP] = 10, [SI] = 12, [DI] = 14,
};

static inline void* getRegLocPtr(CPU* cpu, RegisterLoc* loc) {
	return (void*)((u8*)cpu->regs + regOffsets[loc->reg]);
}

static inline void* getImmLocPtr(ImmediateLoc* loc) {
	return (void*)(&loc->data);
}

static void* getLocPtr(CPU* cpu, Loc* loc) {
	// clang-format off
	switch (loc->type) {
		case LOC_REG: return getRegLocPtr(cpu, &loc->reg);
		case LOC_IMM: return getImmLocPtr(&loc->imm);
		default:
			return getImmLocPtr(&loc->imm);
			fprintf(stderr, "Memory locs are not supported yet.");
			assert(false);
	}
	// clang-format on
}

#define DO_MOV(_t, _dst, _src) *((_t*)_dst) = *((_t*)_src)

static void execMov(CPU* cpu, MovInstruction* instr, StringBuilder* sb) {
	char debugBuf[32];
	if (instr->locs.dst.type == LOC_MEM || instr->locs.src.type == LOC_MEM) {
		fprintf(stderr, "Only non-memory movs are supported");
		assert(false);
	}

	void* dst = getLocPtr(cpu, &(instr->locs.dst));
	void* src = getLocPtr(cpu, &(instr->locs.src));

	if (instr->locs.isWord) {
		if (sb) {
			sprintf(debugBuf, " ; 0x%04x->0x%04x", *(u16*)src, *(u16*)dst);
			StringBuilderAppend(sb, debugBuf);
		}
		DO_MOV(u16, dst, src);
	} else {
		if (sb) {
			sprintf(debugBuf, " ; 0x%02x->0x%02x", *(u8*)src, *(u8*)dst);
			StringBuilderAppend(sb, debugBuf);
		}
		DO_MOV(u8, dst, src);
	}
}

void printRegister(CPU* cpu, Register reg) {
	switch (reg) {
		case AH:
		case AL:
		case BH:
		case BL:
		case CH:
		case CL:
		case DH:
		case DL: {
			u8 val = *((u8*)((u8*)cpu->regs + regOffsets[reg]));
			printf("%s: 0x%02x (%d)\n", RegisterName(reg), val, val);
			break;
		}
		default: {
			u16 val = *((u16*)((u8*)cpu->regs + regOffsets[reg]));
			printf("%s: 0x%04x (%d)\n", RegisterName(reg), val, val);
		}
	}
}

void CPUDumpRegisters(CPU* cpu) {
	for (int i = 0; i < 16; i++) {
		printRegister(cpu, (Register)i);
	}
}
