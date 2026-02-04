#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

Arena ArenaCreate(usize size) {
	void* data = malloc(size);
	assert(data != null);
	return (Arena){0, size, data};
}

void ArenaDestroy(Arena* a) {
	free(a->data);
	a->data = null;
	a->cap = 0;
	a->offset = 0;
}

void* ArenaAlloc(Arena* a, usize size) {
	assert(a->offset + size < a->cap);
	void* ptr = a->data + a->offset;
	a->offset += size;
	return ptr;
}

void ArenaClear(Arena* a) {
	a->offset = 0;
}
