#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common/strings.h"
#include "instruction.h"

const char* InstructionTypeName(InstructionType type);
const char* RegisterName(Register reg);
void InstructionUnparse(Instruction* instr, StringBuilder* sb);

#endif // __DEBUG_H__
