#ifndef __SIM_H__
#define __SIM_H__

#include "common/types.h"
#include "instruction.h"

#define NUM_REGISTERS 8

typedef enum {
	CF = 0x01 << 0,
	PF = 0x01 << 2,
	AF = 0x01 << 4,
	ZF = 0x01 << 6,
	SF = 0x01 << 8,
	TF = 0x01 << 10,
} CPUFlag;

typedef struct {
	usize ip;
	u16 flags;
	u16 regs[NUM_REGISTERS];
} CPU;

void CPUInit(CPU* cpu);
void* CPUGetRegisterPtr(CPU* cpu, Register reg);
void CPUExec(CPU* cpu, Instruction* instr);

void* GetLocPtr(CPU* cpu, Loc* loc);

#endif // __SIM_H__
