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

typedef struct {
	InstructionType type;
	OpEncoding enc;
} OpCode;

extern const OpCode OpCodeTable[];

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

typedef struct {
	MovLocType type;
	EffectiveAddress ea;
	u16 disp;
} MemoryLoc;

typedef struct {
	MovLocType type;
	u16 data;
} ImmediateLoc;

void InstructionUnparse(Instruction* instr, StringBuilder* sb);

#endif // __INSTRUCTION_H__
