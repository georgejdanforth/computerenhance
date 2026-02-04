#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>

#include "common/strings.h"
#include "common/types.h"

typedef enum {
	IT_MOV,
} InstructionType;

typedef enum {
	// Register/memory to/from register
	OC_MOV_RM = 0x88,
	// Immediate to register/memory
	OC_MOV_IRM = 0xC6,
	// Immediate to register
	OC_MOV_IR = 0xB0,
	// Memory to accumulator
	OC_MOV_MA = 0xA0,
	// Accumulator to memory
	OC_MOV_AM = 0xA2,
	// Register/memory to segment register
	OC_MOV_RMS = 0x8E,
	// Segment register to register/memory
	OC_MOV_SRM = 0x8C,
} OpCode;

static inline bool OpCodeIs(u8 byte, OpCode oc) {
	return (byte & oc) == oc;
}

typedef struct {
	InstructionType type;
	OpCode oc;
} Instruction;

typedef enum {
	ML_Register,
	ML_Memory,
	ML_Immediate,
} MovLocType;

typedef struct {
	MovLocType type;
} MovLoc;

typedef struct {
	InstructionType type;
	OpCode oc;
	MovLoc* src;
	MovLoc* dst;
} MovInstruction;

typedef enum { AX, BX, CX, DX, SP, BP, SI, DI, AL, AH, BL, BH, CL, CH, DL, DH } Register;

typedef struct {
	MovLocType type;
	Register reg;
} RegisterLoc;

typedef struct {
	MovLocType type;
} MemoryLoc;

void InstructionUnparse(Instruction* instr, StringBuilder* sb);

#endif // __INSTRUCTION_H__
