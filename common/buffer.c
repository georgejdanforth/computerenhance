#include <assert.h>

#include "buffer.h"

Buffer BufferSlice(Buffer b, usize start, usize end) {
	assert(start < b.size);
	assert(end <= b.size);
	assert(start < end);
	return (Buffer){
			.size = end - start,
			.data = b.data + start,
	};
}
