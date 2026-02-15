#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sim.h"

static const usize regOffsets[] = {
		[AX] = 0, [AH] = 0, [AL] = 1, [BX] = 2, [BH] = 2, [BL] = 3,  [CX] = 4,  [CH] = 4,
		[CL] = 5, [DX] = 6, [DH] = 6, [DL] = 7, [SP] = 8, [BP] = 10, [SI] = 12, [DI] = 14,
};

static void execMov(CPU* cpu, MovInstruction* instr);
static void execAdd(CPU* cpu, AddInstruction* instr);
static void execSub(CPU* cpu, SubInstruction* instr);
static void execCmp(CPU* cpu, CmpInstruction* instr);

void CPUInit(CPU* cpu) {
	assert(cpu != null);
	// Zero out flags
	cpu->flags = 0;
	// Zero out all registers to begin with
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
		case IT_ADD: EXEC(execAdd, (AddInstruction*)&instr->locPair);
		case IT_SUB: EXEC(execSub, (SubInstruction*)&instr->locPair);
		case IT_CMP: EXEC(execCmp, (CmpInstruction*)&instr->locPair);
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

static inline void setFlag(CPU* cpu, CPUFlag flag) {
	cpu->flags |= flag;
}

static inline void unsetFlag(CPU* cpu, CPUFlag flag) {
	cpu->flags &= ~flag;
}

static inline void handleZeroFlag(CPU* cpu, void* dst, bool isWord) {
	bool isZero = isWord ? *(u16*)dst == 0x0000 : *(u8*)dst == 0x00;
	if (isZero) {
		setFlag(cpu, ZF);
	} else {
		unsetFlag(cpu, ZF);
	}
}

static inline void handleSignFlag(CPU* cpu, void* dst, bool isWord) {
	bool isSign = isWord ? *(u16*)dst & 0x8000 : *(u8*)dst & 0x80;
	if (isSign) {
		setFlag(cpu, SF);
	} else {
		unsetFlag(cpu, SF);
	}
}

#define ASSERT_NOMEM(i)                                                                 \
	if (instr->locs.dst.type == LOC_MEM || instr->locs.src.type == LOC_MEM) {             \
		fprintf(stderr, "Only non-memory operations are supported");                        \
		assert(false);                                                                      \
	}

#define DO_MOV(_t, _dst, _src) *((_t*)_dst) = *((_t*)_src)
#define DO_ADD(_t, _dst, _src) *((_t*)_dst) += *((_t*)_src)
#define DO_SUB(_t, _dst, _src) *((_t*)_dst) -= *((_t*)_src)

#define EXEC_LOCPAIR(op)                                                                \
	void* dst = GetLocPtr(cpu, &(instr->locs.dst));                                       \
	void* src = GetLocPtr(cpu, &(instr->locs.src));                                       \
	if (instr->locs.isWord) {                                                             \
		op(u16, dst, src);                                                                  \
	} else {                                                                              \
		op(u8, dst, src);                                                                   \
	}

static void execMov(CPU* cpu, MovInstruction* instr) {
	ASSERT_NOMEM(instr);
	EXEC_LOCPAIR(DO_MOV);
}

static void execAdd(CPU* cpu, AddInstruction* instr) {
	ASSERT_NOMEM(instr);
	EXEC_LOCPAIR(DO_ADD);

	handleZeroFlag(cpu, dst, instr->locs.isWord);
	handleSignFlag(cpu, dst, instr->locs.isWord);
}

static void execSub(CPU* cpu, SubInstruction* instr) {
	ASSERT_NOMEM(instr);
	EXEC_LOCPAIR(DO_SUB);

	handleZeroFlag(cpu, dst, instr->locs.isWord);
	handleSignFlag(cpu, dst, instr->locs.isWord);
}

static void execCmp(CPU* cpu, CmpInstruction* instr) {
	ASSERT_NOMEM(instr);
	u8 res8;
	u16 res16;
	void* a = GetLocPtr(cpu, &(instr->locs.dst));
	void* b = GetLocPtr(cpu, &(instr->locs.src));
	void* dst;
	if (instr->locs.isWord) {
		res16 = *(u16*)a - *(u16*)b;
		dst = &res16;
	} else {
		res8 = *(u8*)a - *(u8*)b;
		dst = &res8;
	}

	handleZeroFlag(cpu, dst, instr->locs.isWord);
	handleSignFlag(cpu, dst, instr->locs.isWord);
}
