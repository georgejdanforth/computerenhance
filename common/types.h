#ifndef __TYPES_H__
#define __TYPES_H__

#include <stddef.h>
#include <stdint.h>
// MacOS SDK does not include the uchar header
#ifndef __APPLE__
#include <uchar.h>
#endif

typedef uint8_t u8;

// MacOS SDK does not include the uchar header
#ifndef __APPLE__
typedef char16_t c16;
#endif

typedef int32_t b32;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef uintptr_t uintptr;
typedef intptr_t isize;
typedef size_t usize;

#endif // __TYPES_H__
