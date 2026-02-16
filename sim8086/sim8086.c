#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/buffer.h"
#include "common/cmd.h"
#include "common/strings.h"
#include "common/types.h"

#include "debug.h"
#include "instruction.h"
#include "sim.h"
#include "sim8086.h"

static Buffer loadFile(const char* path);

static void printUsage(void) {
	fprintf(stderr,
	        "Usage: computerenhance sim8086 [options]\n\n"
	        "Execute and/or disassemble the given file.\n\n"
	        "  -f FILE             Binary file path\n"
	        "  -d, --decode-only   Only decode without executing the binary file\n");
}

typedef struct {
	bool decodeOnly;
	char* filePath;
} Opts;

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

	Buffer instrBuf = loadFile(opts.filePath);
	if (instrBuf.size == 0) {
		return EXIT_FAILURE;
	}

	CPUPair cpus;
	CPUInit(&cpus.curr);

	StringBuilder sb = StringBuilderCreate();
	while (cpus.curr.ip < instrBuf.size) {
		cpus.prev = cpus.curr;

		Buffer buf = BufferSlice(instrBuf, cpus.curr.ip, instrBuf.size);
		Instruction instr = InstructionDecode(buf);

		if (opts.decodeOnly) {
			UnparseInstruction(&instr, null, &sb);
			// Need to increment the instruction pointer manually in this case since we're not
			// executing the instruction.
			cpus.curr.ip += instr.base.size;
		} else {
			CPUExec(&cpus.curr, &instr);
			UnparseInstruction(&instr, &cpus, &sb);
		}

		char* str = StringBuilderString(&sb);
		printf("%s\n", str);
		StringBuilderRewind(&sb);
	}

	if (!opts.decodeOnly) {
		printf("\n");
		DumpCPURegisters(&cpus.curr);
	}

	free(instrBuf.data);

	return EXIT_SUCCESS;
}

static Buffer loadFile(const char* path) {
#define SEEK(n)                                                                         \
	if (fseek(f, 0, n) != 0) {                                                            \
		fprintf(stderr, "Error reading file: %s\n", strerror(errno));                       \
		return (Buffer){0, null};                                                           \
	}

	File* f = fopen(path, "r");
	if (f == null) {
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		printUsage();
		return (Buffer){0, null};
	}

	SEEK(SEEK_END);
	usize fileSize = ftell(f);
	SEEK(SEEK_SET);

	u8* buf = malloc(fileSize);
	if (buf == null) {
		fclose(f);
		fprintf(stderr, "Panic: failed to allocate instruction buffer.\n");
		assert(false);
	}

	usize bytesRead = fread(buf, 1, fileSize, f);
	fclose(f);
	if (bytesRead != fileSize) {
		fprintf(stderr, "Panic: failed to read entire file\n");
		assert(false);
	}

	return (Buffer){
			.size = bytesRead,
			.data = buf,
	};

#undef SEEK
}
