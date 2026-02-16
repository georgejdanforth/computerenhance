#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common/strings.h"
#include "instruction.h"
#include "sim.h"

typedef struct {
	CPU prev;
	CPU curr;
} CPUPair;

const char* InstructionTypeName(InstructionType type);
const char* RegisterName(Register reg);

void UnparseInstruction(Instruction* instr, CPUPair* cpus, StringBuilder* sb);
void DumpCPURegisters(CPU* cpu);

#endif // __DEBUG_H__
