#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/arena.h"
#include "common/strings.h"
#include "common/types.h"

#include "instruction.h"
#include "sim8086.h"

#define ARENA_SIZE 1024;

typedef struct {
	File* f;
	Arena* allocator;
	u8 len;
	u8 bytes[6];
} DecodeContext;

static void printUsage(void);
static int decodeFile(File* f);
static Instruction* decodeInstruction(File* f, Arena* allocator);
static MovInstruction* decodeMov(DecodeContext* ctx, OpCode oc);
static void decodeMovRM(DecodeContext* ctx, MovInstruction* instr, error* err);
static void decodeMovIR(DecodeContext* ctx, MovInstruction* instr, error* err);
static MemoryLoc* decodeMemoryLoc(DecodeContext* ctx, error* err);
static ImmediateLoc* decodeImmediateLoc(DecodeContext* ctx, bool w, usize idx,
                                        error* err);

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
	Arena allocator = ArenaCreate(1024);
	StringBuilder sb = StringBuilderCreate();
	while (true) {
		Instruction* instr = decodeInstruction(f, &allocator);
		if (instr == null) {
			break;
		}
		InstructionUnparse(instr, &sb);
		char* str = StringBuilderString(&sb);
		printf("%s\n", str);
		StringBuilderRewind(&sb);
	}
	return feof(f) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static const usize N_MOV_OPCODES = 7;
static const OpCode MOV_OPCODES[] = {
		// Need to order these highest to lowest
		OC_MOV_IRM, OC_MOV_IR, OC_MOV_AM, OC_MOV_MA, OC_MOV_RMS, OC_MOV_SRM, OC_MOV_RM,
};

static Instruction* decodeInstruction(File* f, Arena* allocator) {
	DecodeContext ctx;
	ctx.allocator = allocator;
	ctx.f = f;
	ctx.len = 0;

	if (read(&ctx, 1) != 0) {
		return null;
	}

	u8 b1 = ctx.bytes[0];

	for (usize i = 0; i < N_MOV_OPCODES; i++) {
		OpCode oc = MOV_OPCODES[i];
		if (OpCodeIs(b1, oc)) {
			return (Instruction*)decodeMov(&ctx, oc);
		}
	}

	fprintf(stderr, "Unhandled byte: %02x\n", b1);
	return null;
}

static MovInstruction* newMovInstruction(Arena* allocator, OpCode oc) {
	MovInstruction* instr = ArenaAlloc(allocator, sizeof(MovInstruction));
	instr->type = IT_MOV;
	instr->oc = oc;
	instr->dst = null;
	instr->src = null;

	return instr;
}

static MovInstruction* decodeMov(DecodeContext* ctx, OpCode oc) {
	MovInstruction* instr = newMovInstruction(ctx->allocator, oc);

	error err = 0;
	switch (oc) {
		case OC_MOV_RM:
			decodeMovRM(ctx, instr, &err);
			break;
		case OC_MOV_IR:
			decodeMovIR(ctx, instr, &err);
			break;
		default:
			fprintf(stderr, "Unhandled opcode: %02x\n", oc);
			assert(false);
	}

	if (err != 0) {
		return null;
	}

	return instr;
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

static RegisterLoc* newRegisterLoc(Arena* allocator, u8 b, bool w) {
	RegisterLoc* loc = ArenaAlloc(allocator, sizeof(RegisterLoc));
	loc->type = ML_Register;
	loc->reg = getRegister(b, w);
	return loc;
}

static void decodeMovRM(DecodeContext* ctx, MovInstruction* instr, error* err) {
	if (read(ctx, 1) != 0) {
		*err = 1;
		return;
	}

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	bool d = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;

	MovLoc* regLoc = (MovLoc*)newRegisterLoc(ctx->allocator, (b2 & 0x38) >> 3, w);
	MovLoc* rmLoc;

	if ((b2 & 0xC0) == 0xC0) {
		// Register mode
		rmLoc = (MovLoc*)newRegisterLoc(ctx->allocator, b2 & 0x07, w);
	} else {
		// Memory mode
		rmLoc = (MovLoc*)decodeMemoryLoc(ctx, err);
	}

	if (d) {
		instr->dst = regLoc;
		instr->src = rmLoc;
	} else {
		instr->dst = rmLoc;
		instr->src = regLoc;
	}
}

static void decodeMovIR(DecodeContext* ctx, MovInstruction* instr, error* err) {
	u8 b1 = ctx->bytes[0];
	bool w = (b1 & 0x08) == 0x08;

	instr->dst = (MovLoc*)newRegisterLoc(ctx->allocator, b1 & 0x07, w);
	instr->src = (MovLoc*)decodeImmediateLoc(ctx, w, 1, err);
}

static MemoryLoc* decodeMemoryLoc(DecodeContext* ctx, error* err) {
#define SET_EA(e)                                                                       \
	loc->ea = e;                                                                          \
	break;

	MemoryLoc* loc = ArenaAlloc(ctx->allocator, sizeof(MemoryLoc));
	loc->type = ML_Memory;
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
		return loc;
	} else if (mode == 0x40) {
		if(read(ctx, 1) != 0) {
			*err = 1;
			return null;
		}
		loc->disp = (u16)(ctx->bytes[2]);
	} else if (mode == 0x80) {
		if(read(ctx, 2) != 0) {
			*err = 1;
			return null;
		}
		loc->disp = ((u16)(ctx->bytes[2])) | ((u16)(ctx->bytes[3]) << 8);
	} else {
		// panic
		fprintf(stderr, "Unexpected mode value: %02x", mode);
		assert(false);
	}

	return loc;

#undef SET_EA
}

static ImmediateLoc* decodeImmediateLoc(DecodeContext* ctx, bool w, usize idx, error* err) {
	ImmediateLoc* loc = ArenaAlloc(ctx->allocator, sizeof(ImmediateLoc));
	loc->type = ML_Immediate;

	if (w) {
		if(read(ctx, 2) != 0) {
			*err = 1;
			return null;
		}
		loc->data = ((u16)(ctx->bytes[idx])) | ((u16)(ctx->bytes[idx + 1]) << 8);
	} else {
		if (read(ctx, 1) != 0) {
			*err = 1;
			return null;
		};
		loc->data = (u16)(ctx->bytes[idx]);
	}

	return loc;
}
