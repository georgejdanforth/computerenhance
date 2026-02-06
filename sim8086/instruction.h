#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>

#include "common/strings.h"
#include "common/types.h"

typedef enum {
	IT_UNKNOWN = 0,
	IT_MOV,
} InstructionType;

typedef enum {
	ENC_NONE = 0,   // No operands
	ENC_MODRM,      // Standard MOD + R/M byte
	ENC_MODRM_IMM8, // MOD + R/M w/ 8-bit immediate
	ENC_MODRM_IMM,  // MOD + R/M w/ immediate sized by W bit
	ENC_ACC_MEM,    // Accumulator + direct address sized by W bit
	ENC_REG_IMM,    // Register in low 3 bits w/ immediate sized by W bit
} OpEncoding;

typedef enum { AX, BX, CX, DX, SP, BP, SI, DI, AL, AH, BL, BH, CL, CH, DL, DH } Register;

typedef enum {
	EA_BX_SI,
	EA_BX_DI,
	EA_BP_SI,
	EA_BP_DI,
	EA_SI,
	EA_DI,
	EA_BP,
	EA_BX,
	// Used for direct address
	EA_NONE,
} EffectiveAddress;

typedef enum {
	LOC_REG,
	LOC_IMM,
	LOC_MEM,
	LOC_ACC,
} LocType;

typedef struct {
	InstructionType type;
	OpEncoding enc;
} OpCode;

extern const OpCode OpCodeTable[];

typedef struct {
	LocType type;
	Register reg;
} RegisterLoc;

typedef struct {
	LocType type;
	u16 data;
} ImmediateLoc;

typedef struct {
	LocType type;
	EffectiveAddress ea;
	u16 disp;
} MemoryLoc;

typedef union {
	LocType type;
	RegisterLoc reg;
	ImmediateLoc imm;
	MemoryLoc mem;
} Loc;

typedef struct {
	Loc src;
	Loc dst;
} LocPair;

typedef struct {
	OpCode oc;
	LocPair locs;
} MovInstruction;

typedef union {
	OpCode oc;
	MovInstruction mov;
} Instruction;

void InstructionUnparse(Instruction* instr, StringBuilder* sb);

#endif // __INSTRUCTION_H__
