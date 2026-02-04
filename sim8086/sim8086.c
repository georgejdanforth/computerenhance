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
static const OpCode MOV_OPCODES[] = {OC_MOV_RM, OC_MOV_IRM, OC_MOV_IR, OC_MOV_MA,
                                     OC_MOV_AM, OC_MOV_RMS, OC_MOV_SRM};

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

static RegisterLoc* newRegisterLoc(Arena* allocator) {
	RegisterLoc* loc = ArenaAlloc(allocator, sizeof(RegisterLoc));
	loc->type = ML_Register;
	return loc;
}

static void decodeMovRM(DecodeContext* ctx, MovInstruction* instr, error* err) {
	if (read(ctx, 1) != 0) {
		*err = 1;
		return;
	}

	u8 b1 = ctx->bytes[0];
	u8 b2 = ctx->bytes[1];
	// TODO: This asserts that we're doing a register to regiser MOV, so we will
	// need to remove this before handling anything else.
	assert((b2 & 0xC0) == 0xC0);

	RegisterLoc* dst = newRegisterLoc(ctx->allocator);
	RegisterLoc* src = newRegisterLoc(ctx->allocator);

	bool d = (b1 & 0x02) == 0x02;
	bool w = (b1 & 0x01) == 0x01;
	Register reg = getRegister((b2 & 0x38) >> 3, w);
	Register rm = getRegister(b2 & 0x07, w);

	if (d) {
		dst->reg = reg;
		src->reg = rm;
	} else {
		dst->reg = rm;
		src->reg = reg;
	}

	instr->dst = (MovLoc*)dst;
	instr->src = (MovLoc*)src;
}
