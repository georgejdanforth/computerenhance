#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common/strings.h"
#include "instruction.h"
#include "sim.h"

const char* InstructionTypeName(InstructionType type);
const char* RegisterName(Register reg);

void UnparseInstruction(Instruction* instr, CPU* cpu, StringBuilder* sb);
void DumpCPURegisters(CPU* cpu);

#endif // __DEBUG_H__
