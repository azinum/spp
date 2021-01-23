// common.h

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;
typedef float r32;
typedef double r64;

#define PI32 3.14159265359f
#define ARR_SIZE(ARR) ((sizeof(ARR)) / (sizeof(ARR[0])))

#endif
