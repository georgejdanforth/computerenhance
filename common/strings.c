#include <assert.h>
#include <stdlib.h>

#include "strings.h"

#define STRING_BUILDER_DEFAULT_CAPACITY 256
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

StringBuilder StringBuilderCreate() {
	char* s = (char*)malloc(STRING_BUILDER_DEFAULT_CAPACITY);
	assert(s != null);
	return (StringBuilder){
			0,
			STRING_BUILDER_DEFAULT_CAPACITY,
			s,
	};
}

void StringBuilderAppend(StringBuilder* sb, const char* s) {
	usize slen = strlen(s);
	StringBuilderAppendEx(sb, s, slen);
}

void StringBuilderAppendEx(StringBuilder* sb, const char* s, usize slen) {
	usize newLen = sb->len + slen;
	if (newLen >= sb->cap) {
		usize newCap = sb->cap * 2;
		while (newLen >= newCap) {
			newCap *= 2;
		}
		sb->s = realloc(sb->s, newCap);
		sb->cap = newCap;
		assert(sb->s != null);
	}
	memcpy(sb->s + sb->len, s, slen);
	sb->len = newLen;
}

void StringBuilderRewind(StringBuilder* sb) {
	memset(sb->s, '\0', sb->len);
	sb->len = 0;
}

char* StringBuilderString(StringBuilder* sb) {
	sb->s[sb->len] = '\0';
	return sb->s;
}
