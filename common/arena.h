#ifndef __ARENA_H__
#define __ARENA_H__

#include "types.h"

typedef struct {
	usize offset;
	usize cap;
	void* data;
} Arena;

Arena ArenaCreate(usize size);
void ArenaDestroy(Arena* a);
void* ArenaAlloc(Arena* a, usize size);
void ArenaClear(Arena* a);

#endif // __ARENA_H__
