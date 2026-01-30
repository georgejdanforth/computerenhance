#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/strings.h"
#include "sim8086/sim8086.h"

#define REGISTER_CMD(name, fn)                                                          \
	if (StrEquals(argv[1], name))                                                         \
		return fn(argc, argv);

static void printUsage(void) {
	printf("Usage: computerenhance <command>\n\n"
	       "Run the given computerenhance course program.\n\n"
	       "Available commands:\n"
	       "\thelp\n");
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printUsage();
		return EXIT_FAILURE;
	}

	if (StrEquals(argv[1], "help")) {
		printUsage();
		return EXIT_SUCCESS;
	}

	REGISTER_CMD("sim8086", RunSim8086);

	printf("Unknown command: %s\n", argv[1]);
	printUsage();
	return EXIT_FAILURE;
}
