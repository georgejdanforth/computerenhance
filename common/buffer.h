#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "types.h"

typedef struct {
	usize size;
	u8* data;
} Buffer;

Buffer BufferSlice(Buffer b, usize start, usize end);

#endif // __BUFFER_H__
