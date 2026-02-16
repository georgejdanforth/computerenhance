#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>

#include "common/buffer.h"
#include "common/types.h"

typedef enum {
	IT_UNKNOWN = 0,

	IT_ADD,
	IT_CMP,
	IT_JA,
	IT_JAE,
	IT_JB,
	IT_JBE,
	IT_JCXZ,
	IT_JE,
	IT_JG,
	IT_JGE,
	IT_JL,
	IT_JLE,
	IT_JNE,
	IT_JNO,
	IT_JNS,
	IT_JO,
	IT_JP,
	IT_JPO,
	IT_JS,
	IT_LOOP,
	IT_LOOPNZ,
	IT_LOOPZ,
	IT_MOV,
	IT_SUB,

	IT_GROUP_1, // ADD, ADC, SUB, SBB, CMP, XOR, OR, AND
} InstructionType;

typedef enum {
	ENC_NONE = 0,     // No operands
	ENC_MODRM,        // Standard MOD + R/M byte
	ENC_MODRM_IMM8,   // MOD + R/M w/ 8-bit immediate
	ENC_MODRM_IMM,    // MOD + R/M w/ immediate sized by W bit
	ENC_MODRM_IMM_SW, // MOD + R/M w/ immediate sized by W bit and with sign-extension bit
	ENC_ACC_MEM,      // Accumulator + direct address sized by W bit
	ENC_ACC_IMM,      // Immediate to accumulator w/ immediate sized by w bit
	ENC_REG_IMM,      // Register in low 3 bits w/ immediate sized by W bit
	ENC_IP_INC8,      // Signed 8-bit displacement following opcode
} OpEncoding;

typedef enum {
	AX = 0,
	BX,
	CX,
	DX,
	SP,
	BP,
	SI,
	DI,
	AL,
	AH,
	BL,
	BH,
	CL,
	CH,
	DL,
	DH
} Register;

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
	bool isSigned;
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
	bool isWord;
} LocPair;

typedef struct {
	OpCode oc;
	LocPair locs;
} LocPairInstruction;

typedef LocPairInstruction MovInstruction;
typedef LocPairInstruction AddInstruction;
typedef LocPairInstruction SubInstruction;
typedef LocPairInstruction CmpInstruction;

typedef struct {
	OpCode oc;
	i8 disp;
} SignedDisplacementInstruction;

typedef SignedDisplacementInstruction JmpInstruction;

typedef union {
	OpCode oc;
	LocPairInstruction locPair;
	SignedDisplacementInstruction signedDisp;
} Instruction;

typedef struct {
	Instruction instr;
	usize sizeBytes;
} DecodeResult;
DecodeResult InstructionDecode(Buffer instrBuf);

#endif // __INSTRUCTION_H__
