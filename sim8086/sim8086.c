#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/strings.h"
#include "common/types.h"

#include "instruction.h"
#include "sim8086.h"

static void printUsage(void);

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

	StringBuilder sb = StringBuilderCreate();
	while (true) {
		DecodeResult result = InstructionDecodeFromFile(f);
		if (result.eof) {
			break;
		}
		InstructionUnparse(&result.instr, &sb);
		char* str = StringBuilderString(&sb);
		printf("%s\n", str);
		StringBuilderRewind(&sb);
	}

	fclose(f);
	return EXIT_SUCCESS;
}

static void printUsage(void) {
	fprintf(stderr, "Usage: computerenhance sim8086 <file>\n\n"
	                "Execute and/or disassemble the given file.\n");
}
