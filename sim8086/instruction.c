#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "instruction.h"

const OpCode OpCodeTable[] = {
		[0x00] = {IT_ADD, ENC_MODRM},
		[0x01] = {IT_ADD, ENC_MODRM},
		[0x02] = {IT_ADD, ENC_MODRM},
		[0x03] = {IT_ADD, ENC_MODRM},
		[0x04] = {IT_ADD, ENC_ACC_IMM},
		[0x05] = {IT_ADD, ENC_ACC_IMM},
		[0x06] = {0},
		[0x07] = {0},
		[0x08] = {0},
		[0x09] = {0},
		[0x0A] = {0},
		[0x0B] = {0},
		[0x0C] = {0},
		[0x0D] = {0},
		[0x0E] = {0},
		[0x0F] = {0},
		[0x10] = {0},
		[0x11] = {0},
		[0x12] = {0},
		[0x13] = {0},
		[0x14] = {0},
		[0x15] = {0},
		[0x16] = {0},
		[0x17] = {0},
		[0x18] = {0},
		[0x19] = {0},
		[0x1A] = {0},
		[0x1B] = {0},
		[0x1C] = {0},
		[0x1D] = {0},
		[0x1E] = {0},
		[0x1F] = {0},
		[0x20] = {0},
		[0x21] = {0},
		[0x22] = {0},
		[0x23] = {0},
		[0x24] = {0},
		[0x25] = {0},
		[0x26] = {0},
		[0x27] = {0},
		[0x28] = {IT_SUB, ENC_MODRM},
		[0x29] = {IT_SUB, ENC_MODRM},
		[0x2A] = {IT_SUB, ENC_MODRM},
		[0x2B] = {IT_SUB, ENC_MODRM},
		[0x2C] = {IT_SUB, ENC_ACC_IMM},
		[0x2D] = {IT_SUB, ENC_ACC_IMM},
		[0x2E] = {0},
		[0x2F] = {0},
		[0x30] = {0},
		[0x31] = {0},
		[0x32] = {0},
		[0x33] = {0},
		[0x34] = {0},
		[0x35] = {0},
		[0x36] = {0},
		[0x37] = {0},
		[0x38] = {IT_CMP, ENC_MODRM},
		[0x39] = {IT_CMP, ENC_MODRM},
		[0x3A] = {IT_CMP, ENC_MODRM},
		[0x3B] = {IT_CMP, ENC_MODRM},
		[0x3C] = {IT_CMP, ENC_ACC_IMM},
		[0x3D] = {IT_CMP, ENC_ACC_IMM},
		[0x3E] = {0},
		[0x3F] = {0},
		[0x40] = {0},
		[0x41] = {0},
		[0x42] = {0},
		[0x43] = {0},
		[0x44] = {0},
		[0x45] = {0},
		[0x46] = {0},
		[0x47] = {0},
		[0x48] = {0},
		[0x49] = {0},
		[0x4A] = {0},
		[0x4B] = {0},
		[0x4C] = {0},
		[0x4D] = {0},
		[0x4E] = {0},
		[0x4F] = {0},
		[0x50] = {0},
		[0x51] = {0},
		[0x52] = {0},
		[0x53] = {0},
		[0x54] = {0},
		[0x55] = {0},
		[0x56] = {0},
		[0x57] = {0},
		[0x58] = {0},
		[0x59] = {0},
		[0x5A] = {0},
		[0x5B] = {0},
		[0x5C] = {0},
		[0x5D] = {0},
		[0x5E] = {0},
		[0x5F] = {0},
		[0x60] = {0},
		[0x61] = {0},
		[0x62] = {0},
		[0x63] = {0},
		[0x64] = {0},
		[0x65] = {0},
		[0x66] = {0},
		[0x67] = {0},
		[0x68] = {0},
		[0x69] = {0},
		[0x6A] = {0},
		[0x6B] = {0},
		[0x6C] = {0},
		[0x6D] = {0},
		[0x6E] = {0},
		[0x6F] = {0},
		[0x70] = {IT_JO, ENC_IP_INC8},
		[0x71] = {IT_JNO, ENC_IP_INC8},
		[0x72] = {IT_JB, ENC_IP_INC8},
		[0x73] = {IT_JAE, ENC_IP_INC8},
		[0x74] = {IT_JE, ENC_IP_INC8},
		[0x75] = {IT_JNE, ENC_IP_INC8},
		[0x76] = {IT_JBE, ENC_IP_INC8},
		[0x77] = {IT_JA, ENC_IP_INC8},
		[0x78] = {IT_JS, ENC_IP_INC8},
		[0x79] = {IT_JNS, ENC_IP_INC8},
		[0x7A] = {IT_JP, ENC_IP_INC8},
		[0x7B] = {IT_JPO, ENC_IP_INC8},
		[0x7C] = {IT_JL, ENC_IP_INC8},
		[0x7D] = {IT_JGE, ENC_IP_INC8},
		[0x7E] = {IT_JLE, ENC_IP_INC8},
		[0x7F] = {IT_JG, ENC_IP_INC8},
		[0x80] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x81] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x82] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x83] = {IT_GROUP_1, ENC_MODRM_IMM_SW},
		[0x84] = {0},
		[0x85] = {0},
		[0x86] = {0},
		[0x87] = {0},
		[0x88] = {IT_MOV, ENC_MODRM},
		[0x89] = {IT_MOV, ENC_MODRM},
		[0x8A] = {IT_MOV, ENC_MODRM},
		[0x8B] = {IT_MOV, ENC_MODRM},
		[0x8C] = {0},
		[0x8D] = {0},
		[0x8E] = {0},
		[0x8F] = {0},
		[0x90] = {0},
		[0x91] = {0},
		[0x92] = {0},
		[0x93] = {0},
		[0x94] = {0},
		[0x95] = {0},
		[0x96] = {0},
		[0x97] = {0},
		[0x98] = {0},
		[0x99] = {0},
		[0x9A] = {0},
		[0x9B] = {0},
		[0x9C] = {0},
		[0x9D] = {0},
		[0x9E] = {0},
		[0x9F] = {0},
		[0xA0] = {IT_MOV, ENC_ACC_MEM},
		[0xA1] = {IT_MOV, ENC_ACC_MEM},
		[0xA2] = {IT_MOV, ENC_ACC_MEM},
		[0xA3] = {IT_MOV, ENC_ACC_MEM},
		[0xA4] = {0},
		[0xA5] = {0},
		[0xA6] = {0},
		[0xA7] = {0},
		[0xA8] = {0},
		[0xA9] = {0},
		[0xAA] = {0},
		[0xAB] = {0},
		[0xAC] = {0},
		[0xAD] = {0},
		[0xAE] = {0},
		[0xAF] = {0},
		[0xB0] = {IT_MOV, ENC_REG_IMM},
		[0xB1] = {IT_MOV, ENC_REG_IMM},
		[0xB2] = {IT_MOV, ENC_REG_IMM},
		[0xB3] = {IT_MOV, ENC_REG_IMM},
		[0xB4] = {IT_MOV, ENC_REG_IMM},
		[0xB5] = {IT_MOV, ENC_REG_IMM},
		[0xB6] = {IT_MOV, ENC_REG_IMM},
		[0xB7] = {IT_MOV, ENC_REG_IMM},
		[0xB8] = {IT_MOV, ENC_REG_IMM},
		[0xB9] = {IT_MOV, ENC_REG_IMM},
		[0xBA] = {IT_MOV, ENC_REG_IMM},
		[0xBB] = {IT_MOV, ENC_REG_IMM},
		[0xBC] = {IT_MOV, ENC_REG_IMM},
		[0xBD] = {IT_MOV, ENC_REG_IMM},
		[0xBE] = {IT_MOV, ENC_REG_IMM},
		[0xBF] = {IT_MOV, ENC_REG_IMM},
		[0xC0] = {0},
		[0xC1] = {0},
		[0xC2] = {0},
		[0xC3] = {0},
		[0xC4] = {0},
		[0xC5] = {0},
		[0xC6] = {IT_MOV, ENC_MODRM_IMM},
		[0xC7] = {IT_MOV, ENC_MODRM_IMM},
		[0xC8] = {0},
		[0xC9] = {0},
		[0xCA] = {0},
		[0xCB] = {0},
		[0xCC] = {0},
		[0xCD] = {0},
		[0xCE] = {0},
		[0xCF] = {0},
		[0xD0] = {0},
		[0xD1] = {0},
		[0xD2] = {0},
		[0xD3] = {0},
		[0xD4] = {0},
		[0xD5] = {0},
		[0xD6] = {0},
		[0xD7] = {0},
		[0xD8] = {0},
		[0xD9] = {0},
		[0xDA] = {0},
		[0xDB] = {0},
		[0xDC] = {0},
		[0xDD] = {0},
		[0xDE] = {0},
		[0xDF] = {0},
		[0xE0] = {IT_LOOPNZ, ENC_IP_INC8},
		[0xE1] = {IT_LOOPZ, ENC_IP_INC8},
		[0xE2] = {IT_LOOP, ENC_IP_INC8},
		[0xE3] = {IT_JCXZ, ENC_IP_INC8},
		[0xE4] = {0},
		[0xE5] = {0},
		[0xE6] = {0},
		[0xE7] = {0},
		[0xE8] = {0},
		[0xE9] = {0},
		[0xEA] = {0},
		[0xEB] = {0},
		[0xEC] = {0},
		[0xED] = {0},
		[0xEE] = {0},
		[0xEF] = {0},
		[0xF0] = {0},
		[0xF1] = {0},
		[0xF2] = {0},
		[0xF3] = {0},
		[0xF4] = {0},
		[0xF5] = {0},
		[0xF6] = {0},
		[0xF7] = {0},
		[0xF8] = {0},
		[0xF9] = {0},
		[0xFA] = {0},
		[0xFB] = {0},
		[0xFC] = {0},
		[0xFD] = {0},
		[0xFE] = {0},
		[0xFF] = {0},
};

// ============================================================================
// Decoding
// ============================================================================

typedef struct {
	File* f;
	u8 len;
	u8 bytes[6];
} DecodeContext;

static void readBytes(DecodeContext* ctx, usize n) {
	usize nread = fread(ctx->bytes + ctx->len, sizeof(u8), n, ctx->f);
	ctx->len += nread;
	assert(nread == n);
}

static Register getRegister(u8 b, bool w) {
	if (w) {
		// clang-format off
		switch (b) {
			case 0x00: return AX;
			case 0x01: return CX;
			case 0x02: return DX;
			case 0x03: return BX;
			case 0x04: return SP;
			case 0x05: return BP;
			case 0x06: return SI;
			case 0x07: return DI;
		}
		// clang-format on
	} else {
		// clang-format off
		switch (b) {
			case 0x00: return AL;
			case 0x01: return CL;
			case 0x02: return DL;
			case 0x03: return BL;
			case 0x04: return AH;
			case 0x05: return CH;
			case 0x06: return DH;
			case 0x07: return BH;
		}
		// clang-format on
	}
	fprintf(stderr, "Unhandled register (b=%02x, w=%d)\n", b, w);
	assert(false);
}

static void decodeMemoryLoc(DecodeContext* ctx, MemoryLoc* loc) {
#define SET_EA(e)                                                                       \
	loc->ea = e;                                                                          \
	break;

	loc->type = LOC_MEM;
	loc->disp = 0;

	u8 b2 = ctx->bytes[1];

	// clang-format off
	switch (b2 & 0x07) {
		case 0x00: SET_EA(EA_BX_SI);
		case 0x01: SET_EA(EA_BX_DI);
		case 0x02: SET_EA(EA_BP_SI);
		case 0x03: SET_EA(EA_BP_DI);
		case 0x04: SET_EA(EA_SI);
		case 0x05: SET_EA(EA_DI);
		case 0x06: SET_EA(EA_BP);
		case 0x07: SET_EA(EA_BX);
	}
	// clang-format on

	u8 mode = b2 & 0xC0;
	if (mode == 0x00 && loc->ea == EA_BP) {
		loc->ea = EA_NONE;
		mode = 0x80;
	}
	if (mode == 0x00) {
		return;
	} else if (mode == 0x40) {
		readBytes(ctx, 1);
		loc->disp = (u16)(i16)(i8)(ctx->bytes[ctx->len - 1]);
	} else if (mode == 0x80) {
		readBytes(ctx, 2);
		loc->disp = ((u16)(ctx->bytes[ctx->len - 2])) | ((u16)(ctx->bytes[ctx->len - 1]) << 8);
	} else {
		fprintf(stderr, "Unexpected mode value: %02x", mode);
		assert(false);
	}

#undef SET_EA
}

static void decodeImmediateLoc(DecodeContext* ctx, ImmediateLoc* loc, bool s, bool w) {
	loc->type = LOC_IMM;

	if (s && w) {
		readBytes(ctx, 1);
		loc->data = (u16)(i16)(i8)(ctx->bytes[ctx->len - 1]);
		loc->isSigned = true;
	} else if (w) {
		readBytes(ctx, 2);
		loc->data = ((u16)ctx->bytes[ctx->len - 2]) | ((u16)ctx->bytes[ctx->len - 1] << 8);
		loc->isSigned = false;
	} else {
		readBytes(ctx, 1);
		loc->data = (u16)ctx->bytes[ctx->len - 1];
		loc->isSigned = false;
	}
}

static void decodeModRM(DecodeContext* ctx, LocPair* locs) {
	readBytes(ctx, 1);

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool d = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;
	locs->isWord = w;

	Loc regLoc;
	regLoc.reg.type = LOC_REG;
	regLoc.reg.reg = getRegister((b2 & 0x38) >> 3, w);

	Loc rmLoc;
	if ((b2 & 0xC0) == 0xC0) {
		rmLoc.reg.type = LOC_REG;
		rmLoc.reg.reg = getRegister(b2 & 0x07, w);
	} else {
		decodeMemoryLoc(ctx, &rmLoc.mem);
	}

	if (d) {
		locs->dst = regLoc;
		locs->src = rmLoc;
	} else {
		locs->dst = rmLoc;
		locs->src = regLoc;
	}
}

static void decodeRegImm(DecodeContext* ctx, LocPair* locs) {
	u8 b1 = ctx->bytes[0];
	bool w = (b1 & 0x08) == 0x08;
	locs->isWord = w;

	locs->dst.reg.type = LOC_REG;
	locs->dst.reg.reg = getRegister(b1 & 0x07, w);

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
}

static void decodeModRMImm(DecodeContext* ctx, LocPair* locs) {
	readBytes(ctx, 1);

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool w = (b1 & 0x01) == 0x01;
	locs->isWord = w;

	if ((b2 & 0xC0) == 0xC0) {
		locs->dst.reg.type = LOC_REG;
		locs->dst.reg.reg = getRegister(b2 & 0x07, w);
	} else {
		decodeMemoryLoc(ctx, &(locs->dst.mem));
	}

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
}

static void decodeModRMImmSW(DecodeContext* ctx, LocPair* locs) {
	readBytes(ctx, 1);

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool s = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;
	locs->isWord = w;

	if ((b2 & 0xC0) == 0xC0) {
		locs->dst.reg.type = LOC_REG;
		locs->dst.reg.reg = getRegister(b2 & 0x07, w);
	} else {
		decodeMemoryLoc(ctx, &(locs->dst.mem));
	}

	decodeImmediateLoc(ctx, &(locs->src.imm), s, w);
}

static void decodeAccImm(DecodeContext* ctx, LocPair* locs) {
	bool w = (ctx->bytes[0] & 0x01) == 0x01;
	locs->isWord = w;

	locs->dst.reg.type = LOC_REG;
	locs->dst.reg.reg = w ? AX : AL;

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
}

static void decodeLocPair(DecodeContext* ctx, LocPairInstruction* instr) {
#define DECODE(fn)                                                                      \
	fn(ctx, &instr->locs);                                                                \
	break;

#define UPDATE_INSTR(i)                                                                 \
	instr->oc.type = i;                                                                   \
	break;

	// clang-format off
	switch (instr->oc.enc) {
		case ENC_MODRM: DECODE(decodeModRM);
		case ENC_REG_IMM: DECODE(decodeRegImm);
		case ENC_MODRM_IMM: DECODE(decodeModRMImm);
		case ENC_MODRM_IMM_SW: DECODE(decodeModRMImmSW);
		case ENC_ACC_IMM: DECODE(decodeAccImm);
		default:
			fprintf(stderr, "Unhandled encoding: %02x\n", instr->oc.enc);
			assert(false);
	}
	// clang-format on

	// clang-format off
	if (instr->oc.type == IT_GROUP_1) {
		u8 b = (ctx->bytes[1] & 0x38) >> 3;
		switch (b) {
			case 0x00: UPDATE_INSTR(IT_ADD);
			case 0x01: UPDATE_INSTR(IT_UNKNOWN); // OR
			case 0x02: UPDATE_INSTR(IT_UNKNOWN); // ADC
			case 0x03: UPDATE_INSTR(IT_UNKNOWN); // SBB
			case 0x04: UPDATE_INSTR(IT_UNKNOWN); // AND
			case 0x05: UPDATE_INSTR(IT_SUB);
			case 0x06: UPDATE_INSTR(IT_UNKNOWN); // XOR
			case 0x07: UPDATE_INSTR(IT_CMP);
			default:
				fprintf(stderr, "Unknown bit sequence for group 1: %02x\n", b);
				assert(false);
		}
	}
	// clang-format on

#undef UPDATE_INSTR
#undef DECODE
}

static void decodeSignedDisplacement(DecodeContext* ctx, SignedDisplacementInstruction* instr) {
	readBytes(ctx, 1);
	instr->disp = ctx->bytes[1];
}

DecodeResult InstructionDecodeFromFile(File* f) {
	DecodeResult result = {0};
	DecodeContext ctx = {0};
	ctx.f = f;

	usize nread = fread(ctx.bytes, sizeof(u8), 1, f);
	if (nread == 0) {
		result.eof = true;
		return result;
	}
	ctx.len = 1;

	u8 b1 = ctx.bytes[0];
	result.instr.oc = OpCodeTable[b1];

	switch (result.instr.oc.type) {
		case IT_ADD:
		case IT_CMP:
		case IT_MOV:
		case IT_SUB:
		case IT_GROUP_1:
			decodeLocPair(&ctx, &result.instr.locPair);
			break;
		case IT_JA:
		case IT_JAE:
		case IT_JB:
		case IT_JBE:
		case IT_JCXZ:
		case IT_JE:
		case IT_JG:
		case IT_JGE:
		case IT_JL:
		case IT_JLE:
		case IT_JNE:
		case IT_JNO:
		case IT_JNS:
		case IT_JO:
		case IT_JP:
		case IT_JPO:
		case IT_JS:
		case IT_LOOP:
		case IT_LOOPNZ:
		case IT_LOOPZ:
			decodeSignedDisplacement(&ctx, &result.instr.signedDisp);
			break;
		default:
			fprintf(stderr, "Unhandled opcode: %02x\n", b1);
			assert(false);
	}

	return result;
}

// ============================================================================
// Unparsing
// ============================================================================

static const char* InstructionTypeNames[] = {
	[IT_ADD] = "add",
	[IT_CMP] = "cmp",
	[IT_JA] = "ja",
	[IT_JAE] = "jae",
	[IT_JB] = "jb",
	[IT_JBE] = "jbe",
	[IT_JCXZ] = "jcxz",
	[IT_JE] = "je",
	[IT_JG] = "jg",
	[IT_JGE] = "jge",
	[IT_JL] = "jl",
	[IT_JLE] = "jle",
	[IT_JNE] = "jne",
	[IT_JNO] = "jno",
	[IT_JNS] = "jns",
	[IT_JO] = "jo",
	[IT_JP] = "jp",
	[IT_JPO] = "jpo",
	[IT_JS] = "js",
	[IT_LOOP] = "loop",
	[IT_LOOPNZ] = "loopnz",
	[IT_LOOPZ] = "loopz",
	[IT_MOV] = "mov",
	[IT_SUB] = "sub",
};

const char* InstructionTypeName(InstructionType type) {
	const char* name = InstructionTypeNames[type];
	assert(name != NULL);
	return name;
}

static const char* RegisterNames[] = {
	[AX] = "ax",
	[BX] = "bx",
	[CX] = "cx",
	[DX] = "dx",
	[SP] = "sp",
	[BP] = "bp",
	[SI] = "si",
	[DI] = "di",
	[AL] = "al",
	[AH] = "ah",
	[BL] = "bl",
	[BH] = "bh",
	[CL] = "cl",
	[CH] = "ch",
	[DL] = "dl",
	[DH] = "dh",
};

const char* RegisterName(Register reg) {
	const char* name = RegisterNames[reg];
	assert(name != NULL);
	return name;
}

void unparseLocPair(LocPair* locs, StringBuilder* sb);
void unparseSignedDisplacement(SignedDisplacementInstruction* instr, StringBuilder* sb);
void unparseLoc(Loc* loc, bool isWord, StringBuilder* sb);
void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb);
void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb);
void unparseImmediateLoc(ImmediateLoc* loc, bool isWord, StringBuilder* sb);

void InstructionUnparse(Instruction* instr, StringBuilder* sb) {
	StringBuilderAppend(sb, InstructionTypeName(instr->oc.type));
	StringBuilderAppend(sb, " ");

	switch (instr->oc.type) {
		case IT_ADD:
		case IT_CMP:
		case IT_MOV:
		case IT_SUB:
			return unparseLocPair(&(instr->locPair.locs), sb);
		case IT_JA:
		case IT_JAE:
		case IT_JB:
		case IT_JBE:
		case IT_JCXZ:
		case IT_JE:
		case IT_JG:
		case IT_JGE:
		case IT_JL:
		case IT_JLE:
		case IT_JNE:
		case IT_JNO:
		case IT_JNS:
		case IT_JO:
		case IT_JP:
		case IT_JPO:
		case IT_JS:
		case IT_LOOP:
		case IT_LOOPNZ:
		case IT_LOOPZ:
			return unparseSignedDisplacement(&instr->signedDisp, sb);
		default:
			// panic
			fprintf(stderr, "Unhandled instruction type: %02x\n", instr->oc.type);
			assert(false);
	}
	// panic
	fprintf(stderr, "Unparse not defined for instruction type: %d\n", instr->oc.type);
	assert(false);

#undef APPEND_INSTR
}

void unparseLocPair(LocPair* locs, StringBuilder* sb) {
	// Output "byte" or "word" when dst is memory and src is immediate
	if (locs->dst.type == LOC_MEM && locs->src.type == LOC_IMM) {
		StringBuilderAppend(sb, locs->isWord ? "word " : "byte ");
	}
	unparseLoc(&locs->dst, locs->isWord, sb);
	StringBuilderAppend(sb, ", ");
	unparseLoc(&locs->src, locs->isWord, sb);
}

void unparseSignedDisplacement(SignedDisplacementInstruction* instr, StringBuilder* sb) {
	char buf[4];
	sprintf(buf, "%d", instr->disp);
	StringBuilderAppend(sb, buf);
}

void unparseLoc(Loc* loc, bool isWord, StringBuilder* sb) {
	switch (loc->type) {
		case LOC_REG:
			unparseRegisterLoc(&loc->reg, sb);
			return;
		case LOC_MEM:
			unparseMemoryLoc(&loc->mem, sb);
			return;
		case LOC_IMM:
			unparseImmediateLoc(&loc->imm, isWord, sb);
			return;
		default:
			fprintf(stderr, "Unparse not defined for mov loc: %d", loc->type);
			assert(false);
	}
}

void unparseRegisterLoc(RegisterLoc* loc, StringBuilder* sb) {
	StringBuilderAppend(sb, RegisterName(loc->reg));
}

void unparseMemoryLoc(MemoryLoc* loc, StringBuilder* sb) {
#define APPEND_EA(ea)                                                                   \
	StringBuilderAppend(sb, ea);                                                          \
	break;

	StringBuilderAppend(sb, "[");

	// clang-format off
	switch(loc->ea) {
		case EA_BX_SI: APPEND_EA("bx + si");
		case EA_BX_DI: APPEND_EA("bx + di");
		case EA_BP_SI: APPEND_EA("bp + si");
		case EA_BP_DI: APPEND_EA("bp + di");
		case EA_SI: APPEND_EA("si");
		case EA_DI: APPEND_EA("di");
		case EA_BP: APPEND_EA("bp");
		case EA_BX: APPEND_EA("bx");
		case EA_NONE: break;
	}
	// clang-format on

	if (loc->disp != 0) {
		char buf[12];
		i16 disp = (i16)loc->disp;
		if (loc->ea == EA_NONE) {
			sprintf(buf, "%d", loc->disp);
		} else if (disp < 0) {
			sprintf(buf, " - %d", -disp);
		} else {
			sprintf(buf, " + %d", disp);
		}
		StringBuilderAppend(sb, buf);
	}

	StringBuilderAppend(sb, "]");

#undef APPEND_EA
}

void unparseImmediateLoc(ImmediateLoc* loc, bool isWord, StringBuilder* sb) {
	char buf[8];
	if (loc->isSigned) {
		// Sign-extended immediate (s=1, w=1 case)
		sprintf(buf, "%d", (i16)loc->data);
	} else if (!isWord && (loc->data & 0x80)) {
		// 8-bit immediate with high bit set - display as signed
		sprintf(buf, "%d", (i8)loc->data);
	} else {
		sprintf(buf, "%d", loc->data);
	}
	StringBuilderAppend(sb, buf);
}
