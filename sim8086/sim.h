#ifndef __SIM_H__
#define __SIM_H__

#include "common/types.h"
#include "instruction.h"

#define NUM_REGISTERS 8

typedef struct {
	u16 regs[NUM_REGISTERS];
} CPU;

void CPUInit(CPU* cpu);
void* CPUGetRegisterPtr(CPU* cpu, Register reg);
void CPUExec(CPU* cpu, Instruction* instr);

void* GetLocPtr(CPU* cpu, Loc* loc);

#endif // __SIM_H__
