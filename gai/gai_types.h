#ifndef _GAI_TYPES_H_

#if _MSC_VER
	#include <intrin.h>
#elif __clang__
	#include <x86intrin.h>
#else
	#error SEE/NEON optimizations are not available for this compiler yet!!!!
#endif

#ifndef GAI_USE_NO_STD
	#include <stdint.h>
	#include <stddef.h>
	#include <limits.h>
	#include <float.h>
	#include <stdlib.h>
	#define _USE_MATH_DEFINES
	#include <math.h>
	#include <stdio.h>
	#include <string.h>
#endif

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int32_t  b32;
typedef int64_t  i64;

typedef float    r32;
typedef double   r64;

#define _GAI_TYPES_H_
#endif
