#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/types.h"

#include "sim8086.h"

#define INSTR_BYTES 2
#define MOV 0x88
#define D 0x02
#define W 0x01
#define REGMASK 0x38
#define RMMASK 0x07

// Offset of registers 1 and 2 in a mov instruction
#define R1 4
#define R2 8

static void printUsage(void);
static int decodeFile(File* f);

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

static inline bool isMov(u8 byte1) {
	return (byte1 & MOV) == MOV;
}

#define SET_REGISTER(r1, r2)                                                            \
	reg[0] = r1;                                                                          \
	reg[1] = r2;                                                                          \
	break;

void setRegister(char* reg, u8 enc) {
	// clang-format off
	switch (enc) {
		case 0x00: SET_REGISTER('a', 'l');
		case 0x01: SET_REGISTER('c', 'l');
		case 0x02: SET_REGISTER('d', 'l');
		case 0x03: SET_REGISTER('b', 'l');
		case 0x04: SET_REGISTER('a', 'h');
		case 0x05: SET_REGISTER('c', 'h');
		case 0x06: SET_REGISTER('d', 'h');
		case 0x07: SET_REGISTER('b', 'h');
		case 0x08: SET_REGISTER('a', 'x');
		case 0x09: SET_REGISTER('c', 'x');
		case 0x0A: SET_REGISTER('d', 'x');
		case 0x0B: SET_REGISTER('b', 'x');
		case 0x0C: SET_REGISTER('s', 'p');
		case 0x0D: SET_REGISTER('b', 'p');
		case 0x0E: SET_REGISTER('s', 'i');
		case 0x0F: SET_REGISTER('d', 'i');
		default:
			// panic
			fprintf(stderr, "Unexpected encoded instruction: %02x\n", enc);
			assert(false);
	}
	// clang-format on
}

static int decodeFile(File* f) {
	u8 instr[2];
	usize read;
	char buffer[] = "mov ??, ??";

	while ((read = fread(instr, sizeof(u8), INSTR_BYTES, f)) == INSTR_BYTES) {
		if (!isMov(instr[0])) {
			fprintf(stderr, "Error: unexpected instruction: %02X", instr[0] >> 2);
			return EXIT_FAILURE;
		}
		usize regOff, rmOff;
		if (instr[0] & D) {
			regOff = R1;
			rmOff = R2;
		} else {
			regOff = R2;
			rmOff = R1;
		}
		u8 w = (instr[0] & W) << 3;
		u8 regEnc = w | ((instr[1] & REGMASK) >> 3);
		u8 rmEnc = w | (instr[1] & RMMASK);
		setRegister(buffer + regOff, regEnc);
		setRegister(buffer + rmOff, rmEnc);

		printf("%s\n", buffer);
	}

	error err = errno;
	if (ferror(f)) {
		fprintf(stderr, "Error reading file: %s\n", strerror(err));
		return EXIT_FAILURE;
	} else if (read != 0) {
		fprintf(stderr, "Malformed file. File length should be a multiple of 16.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
