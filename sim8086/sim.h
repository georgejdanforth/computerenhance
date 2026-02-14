#ifndef __SIM_H__
#define __SIM_H__

#include "common/types.h"
#include "instruction.h"

#define NUM_REGISTERS 8

typedef struct {
	u16 regs[NUM_REGISTERS];
} CPU;

void CPUInit(CPU* cpu);
void CPUDumpRegisters(CPU* cpu);
void CPUExec(CPU* cpu, Instruction* instr, StringBuilder* sb);

#endif // __SIM_H__
