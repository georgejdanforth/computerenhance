#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sim.h"

static const usize regOffsets[] = {
		[AX] = 0, [AH] = 0, [AL] = 1, [BX] = 2, [BH] = 2, [BL] = 3,  [CX] = 4,  [CH] = 4,
		[CL] = 5, [DX] = 6, [DH] = 6, [DL] = 7, [SP] = 8, [BP] = 10, [SI] = 12, [DI] = 14,
};

static void execMov(CPU* cpu, MovInstruction* instr);

void CPUInit(CPU* cpu) {
	assert(cpu != null);
	// Zero out all registers to begin with.
	memset(cpu->regs, 0, sizeof(u16) * NUM_REGISTERS);
}

void* CPUGetRegisterPtr(CPU* cpu, Register reg) {
	return (void*)((u8*)cpu->regs + regOffsets[reg]);
}

void CPUExec(CPU* cpu, Instruction* instr) {
#define EXEC(_fn, _instr)                                                               \
	_fn(cpu, _instr);                                                                     \
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

static inline void* getRegLocPtr(CPU* cpu, RegisterLoc* loc) {
	return CPUGetRegisterPtr(cpu, loc->reg);
}

static inline void* getImmLocPtr(ImmediateLoc* loc) {
	return (void*)(&loc->data);
}

void* GetLocPtr(CPU* cpu, Loc* loc) {
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

static void execMov(CPU* cpu, MovInstruction* instr) {
	if (instr->locs.dst.type == LOC_MEM || instr->locs.src.type == LOC_MEM) {
		fprintf(stderr, "Only non-memory movs are supported");
		assert(false);
	}

	void* dst = GetLocPtr(cpu, &(instr->locs.dst));
	void* src = GetLocPtr(cpu, &(instr->locs.src));

	if (instr->locs.isWord) {
		DO_MOV(u16, dst, src);
	} else {
		DO_MOV(u8, dst, src);
	}
}
