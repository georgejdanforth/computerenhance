#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <stdbool.h>
#include <string.h>

static inline bool StrEquals(char* lhs, const char* rhs) {
	return strcmp(lhs, rhs) == 0;
}

#endif // __STRINGS_H__
