#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/strings.h"
#include "common/types.h"

#include "instruction.h"
#include "sim8086.h"

#define ERR_NONE 0
#define ERR_EOF 1
#define ERR_UNKNOWN 2

#define ERROR(e)                                                                        \
	ctx->err = e;                                                                         \
	return;

typedef struct {
	File* f;
	error err;
	u8 len;
	u8 bytes[6];
} DecodeContext;

static void printUsage(void);
static int decodeFile(File* f);
static Instruction decodeInstruction(DecodeContext* ctx);
static void decodeLocPair(DecodeContext* ctx, LocPairInstruction* instr);
static void decodeSignedDisplacement(DecodeContext* ctx,
                                     SignedDisplacementInstruction* instr);
static void decodeModRM(DecodeContext* ctx, LocPair* locs);
static void decodeModRMImm(DecodeContext* ctx, LocPair* locs);
static void decodeModRMImmSW(DecodeContext* ctx, LocPair* locs);
static void decodeAccImm(DecodeContext* ctx, LocPair* locs);
static void decodeRegImm(DecodeContext* ctx, LocPair* locs);
static void decodeMemoryLoc(DecodeContext* ctx, MemoryLoc* loc);
static void decodeImmediateLoc(DecodeContext* ctx, ImmediateLoc* loc, bool s, bool w);

int RunSim8086(int argc, char** argv) {
	if (argc != 3) {
		printUsage();
		return EXIT_FAILURE;
	}

	File* f = fopen(argv[2], "r");
	if (f == null) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		printUsage();
		return EXIT_FAILURE;
	}

	int result = decodeFile(f);

	fclose(f);
	return result;
}

static void printUsage(void) {
	fprintf(stderr, "Usage: computerenhance sim8086 <file>\n\n"
	                "Disassemble the given file.\n");
}

static int read(DecodeContext* ctx, usize n) {
	usize nread = fread(ctx->bytes + ctx->len, sizeof(u8), n, ctx->f);
	ctx->len += nread;
	if (nread == n) {
		return 0;
	}
	error err = errno;
	if (ferror(ctx->f)) {
		fprintf(stderr, "Error reading file: %s\n", strerror(err));
		return -1;
	}
	if (nread == 0) {
		return 1;
	}
	fprintf(stderr, "Malformed file. File length should be a multiple of 16.\n");
	return -1;
}

static int decodeFile(File* f) {
	StringBuilder sb = StringBuilderCreate();
	Instruction instr;
	DecodeContext ctx;
	ctx.f = f;
	while (true) {
		ctx.err = ERR_NONE;
		ctx.len = 0;
		instr = decodeInstruction(&ctx);
		if (ctx.err != 0) {
			break;
		}
		InstructionUnparse(&instr, &sb);
		char* str = StringBuilderString(&sb);
		printf("%s\n", str);
		StringBuilderRewind(&sb);
	}

	if (ctx.err == ERR_NONE || ctx.err == ERR_EOF) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

static Instruction decodeInstruction(DecodeContext* ctx) {
	Instruction instr = {0};

	int result;
	if ((result = read(ctx, 1)) != 0) {
		ctx->err = result == 1 ? ERR_EOF : ERR_UNKNOWN;
		return instr;
	}

	u8 b1 = ctx->bytes[0];
	instr.oc = OpCodeTable[b1];

	switch (instr.oc.type) {
		case IT_ADD:
		case IT_CMP:
		case IT_MOV:
		case IT_SUB:
		case IT_GROUP_1:
			decodeLocPair(ctx, &instr.locPair);
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
			decodeSignedDisplacement(ctx, &instr.signedDisp);
			break;
		default:
			fprintf(stderr, "Unhandled opcode: %02x\n", b1);
			ctx->err = ERR_UNKNOWN;
			return instr;
	}

	return instr;
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
			fprintf(stderr, "Unhandled opcode: %02x\n", ctx->bytes[0]);
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
			case 0x06: UPDATE_INSTR(IT_UNKNOWN); // XOR (?)
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

static void decodeSignedDisplacement(DecodeContext* ctx,
                                     SignedDisplacementInstruction* instr) {
	if (read(ctx, 1) != 0) {
		ERROR(ERR_UNKNOWN);
	}

	instr->disp = ctx->bytes[1];
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
	// panic
	fprintf(stderr, "Unhandled register (b=%02x, w=%d)\n", b, w);
	assert(false);
}

static void decodeModRM(DecodeContext* ctx, LocPair* locs) {
	if (read(ctx, 1) != 0) {
		ERROR(ERR_UNKNOWN);
	}

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool d = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;

	Loc regLoc;
	regLoc.reg.type = LOC_REG;
	regLoc.reg.reg = getRegister((b2 & 0x38) >> 3, w);

	Loc rmLoc;
	if ((b2 & 0xC0) == 0xC0) {
		// Register mode
		rmLoc.reg.type = LOC_REG;
		rmLoc.reg.reg = getRegister(b2 & 0x07, w);
	} else {
		// Memory mode
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

	locs->dst.reg.type = LOC_REG;
	locs->dst.reg.reg = getRegister(b1 & 0x07, w);

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
}

static void decodeModRMImm(DecodeContext* ctx, LocPair* locs) {
	if (read(ctx, 1) != 0) {
		ERROR(ERR_UNKNOWN);
	}

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool w = (b1 & 0x01) == 0x01;

	if ((b2 & 0xC0) == 0xC0) {
		locs->dst.reg.type = LOC_REG;
		locs->dst.reg.reg = getRegister(b2 & 0x07, w);
	} else {
		decodeMemoryLoc(ctx, &(locs->dst.mem));
	}

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
}

static void decodeModRMImmSW(DecodeContext* ctx, LocPair* locs) {
	if (read(ctx, 1) != 0) {
		ERROR(ERR_UNKNOWN);
	}

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool s = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;

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

	locs->dst.reg.type = LOC_REG;
	locs->dst.reg.reg = w ? AX : AL;

	decodeImmediateLoc(ctx, &(locs->src.imm), false, w);
	locs->src.imm.type = LOC_IMM;
	locs->src.imm.isSigned = false;
};

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
	//clang-format on

	u8 mode = b2 & 0xC0;
	if (mode == 0x00 && loc->ea == EA_BP) {
		// Handle direct address case by setting the effective address to EA_NONE,
		// then switch mode to 0x80 (16-bit displacement) so that we can consume
		// the direct address using displacement info
		loc->ea = EA_NONE;
		mode = 0x80;
	}
	if (mode == 0x00) {
		return;
	} else if (mode == 0x40) {
		if(read(ctx, 1) != 0) {
			ERROR(ERR_UNKNOWN);
		}
		loc->disp = (u16)(i16)(i8)(ctx->bytes[ctx->len - 1]);
	} else if (mode == 0x80) {
		if(read(ctx, 2) != 0) {
			ERROR(ERR_UNKNOWN);
		}
		loc->disp = ((u16)(ctx->bytes[ctx->len - 2])) | ((u16)(ctx->bytes[ctx->len - 1]) << 8);
	} else {
		// panic
		fprintf(stderr, "Unexpected mode value: %02x", mode);
		assert(false);
	}

#undef SET_EA
}

static void decodeImmediateLoc(DecodeContext* ctx, ImmediateLoc* loc, bool s, bool w) {
	loc->type = LOC_IMM;

	if (s && w) {
		// 8-bit immediate, sign-extended to 16-bit
		if (read(ctx, 1) != 0) {
			ERROR(ERR_UNKNOWN);
		}
		loc->data = (u16)(i16)(i8)(ctx->bytes[ctx->len - 1]);
		loc->isSigned = true;
	} else if (w) {
		// 16-bit immediate
		if (read(ctx, 2) != 0) {
			ERROR(ERR_UNKNOWN);
		}
		loc->data = ((u16)ctx->bytes[ctx->len - 2]) | ((u16)ctx->bytes[ctx->len - 1] << 8);
		loc->isSigned = false;
	} else {
		// 8-bit immediate
		if (read(ctx, 1) != 0) {
			ERROR(ERR_UNKNOWN);
		}
		loc->data = (u16)ctx->bytes[ctx->len - 1];
		loc->isSigned = false;
	}
}
