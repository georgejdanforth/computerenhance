#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/cmd.h"
#include "common/strings.h"
#include "common/types.h"

#include "debug.h"
#include "instruction.h"
#include "sim.h"
#include "sim8086.h"

typedef struct {
	bool decodeOnly;
	char* filePath;
} Opts;

static void printUsage(void) {
	fprintf(stderr,
	        "Usage: computerenhance sim8086 [options]\n\n"
	        "Execute and/or disassemble the given file.\n\n"
	        "  -f FILE             Binary file path\n"
	        "  -d, --decode-only   Only decode without executing the binary file\n");
}

int RunSim8086(int argc, char** argv) {
	if (argc < 3) {
		printUsage();
		return EXIT_FAILURE;
	}

	Opts opts;
	opts.decodeOnly = false;
	opts.filePath = null;

	int i = 0;
	while (i < argc) {
		OPT_SHORT(i, opts, filePath, "-f");
		FLAG_LONG(i, opts, decodeOnly, "-d", "--decode-only");
		i++;
	}

	File* in = fopen(opts.filePath, "r");
	if (in == null) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		printUsage();
		return EXIT_FAILURE;
	}

	CPU cpu;
	CPUInit(&cpu);

	StringBuilder sb = StringBuilderCreate();
	while (true) {
		DecodeResult result = InstructionDecodeFromFile(in);
		if (result.eof) {
			break;
		}
		if (opts.decodeOnly) {
			UnparseInstruction(&result.instr, null, &sb);
		} else {
			UnparseInstruction(&result.instr, &cpu, &sb);
			CPUExec(&cpu, &result.instr);
		}
		char* str = StringBuilderString(&sb);
		printf("%s\n", str);
		StringBuilderRewind(&sb);
	}

	if (!opts.decodeOnly) {
		printf("\n");
		DumpCPURegisters(&cpu);
	}

	fclose(in);
	return EXIT_SUCCESS;
}
