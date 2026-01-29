#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	Help,
} CommandType;

void printUsage(void) {
	printf("Usage: computerenhance <command>\n\n"
	       "Run the given computerenhance course program.\n\n"
	       "Available commands:\n"
	       "\thelp\n");
}

static inline bool strEquals(char* lhs, const char* rhs) {
	return strcmp(lhs, rhs) == 0;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		printUsage();
		return EXIT_FAILURE;
	}

	if (strEquals(argv[1], "help")) {
		printUsage();
		return EXIT_SUCCESS;
	}

	printf("Unknown command: %s\n", argv[1]);
	printUsage();
	return EXIT_FAILURE;
}
