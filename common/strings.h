#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stdbool.h>
#include <string.h>

#include "types.h"

static inline bool StrEquals(char* lhs, const char* rhs) {
	return strcmp(lhs, rhs) == 0;
}

typedef struct {
	usize len;
	usize cap;
	char* s;
} StringBuilder;

StringBuilder StringBuilderCreate();
void StringBuilderAppend(StringBuilder* sb, const char* s);
void StringBuilderAppendEx(StringBuilder* sb, const char* s, usize slen);
void StringBuilderRewind(StringBuilder* sb);
char* StringBuilderString(StringBuilder* sb);

#endif // __STRINGS_H__
