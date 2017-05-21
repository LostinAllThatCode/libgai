/**
 * @file 				gai_platform.h
 *
 * @brief      			Template
 *
 * @author Andreas Gaida
 * @date   03.05.2017
 * @see    https://github.com/LostinAllThatCode/libgai
 */
#ifndef _INCLUDE_GAI_PLATFORM_H_

#if _MSC_VER
	#undef COMPILER_MSVC
	#define COMPILER_MSVC 1
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

typedef int8_t  i08;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int32_t b32;

typedef uint8_t  u08;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float   f32;
typedef double  f64;

union v2 { struct { f32 x, y; }; struct { f32 u, v; }; f32 E[2]; };
union v3 { struct { v2 xy; f32 _ignored0_; }; struct { f32 _ignored1_; v2 yz; }; struct { v2 uv; f32 _ignored2_; }; struct { f32 _ignored3_; v2 vw; }; struct { f32 x, y, z; }; struct { f32 r, g, b; }; struct { f32 u, v, w; }; f32 E[3]; };
union v4 { struct { union { v3 xyz; struct { f32 x, y, z; }; }; f32 w; }; struct { union { v3 rgb; struct { f32 r, g, b; }; }; f32 a; }; struct { v2 xy; f32 _ignored0_; f32 _ignored1_; }; struct { f32 _ignored2_; v2 yz; f32 _ignored3_; }; struct { f32 _ignored4_; f32 _ignored5_; v2 zw; }; f32 E[4]; };

#if COMPILER_MSVC

#include <intrin.h>
#define gaipf_CompletePreviousReadsBeforeFutureReads _ReadBarrier()
#define gaipf_CompletePreviousWritesBeforeFutureWrites _WriteBarrier()

inline u32 gaipf_AtomicCompareExchangeU32(u32 volatile *Value, u32 New, u32 Expected)
{
	u32 Result = _InterlockedCompareExchange((long volatile *)Value, New, Expected);

	return (Result);
}

inline u64 gaipf_AtomicExchangeU64(u64 volatile *Value, u64 New)
{
	u64 Result = _InterlockedExchange64((__int64 volatile *)Value, New);

	return (Result);
}
inline u64 gaipf_AtomicAddU64(u64 volatile *Value, u64 Addend)
{
	// NOTE(casey): Returns the original value _prior_ to adding
	u64 Result = _InterlockedExchangeAdd64((__int64 volatile *)Value, Addend);

	return (Result);
}
inline u32 gaipf_GetThreadID(void)
{
	u08 *ThreadLocalStorage = (u08 *)__readgsqword(0x30);
	u32 ThreadID = *(u32 *)(ThreadLocalStorage + 0x48);

	return (ThreadID);
}

#elif COMPILER_LLVM
// Does LLVM have real read-specific barriers yet?
#define gaipf_CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define gaipf_CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")
inline u32 gaipf_AtomicCompareExchangeU32(u32 volatile *Value, u32 New, u32 Expected)
{
	u32 Result = __sync_val_compare_and_swap(Value, Expected, New);

	return (Result);
}
inline u64 gaipf_AtomicExchangeU64(u64 volatile *Value, u64 New)
{
	u64 Result = __sync_lock_test_and_set(Value, New);

	return (Result);
}
inline u64 gaipf_AtomicAddU64(u64 volatile *Value, u64 Addend)
{
	// NOTE(casey): Returns the original value _prior_ to adding
	u64 Result = __sync_fetch_and_add(Value, Addend);

	return (Result);
}
inline u32 gaipf_GetThreadID(void)
{
	u32 ThreadID;
	#if defined(__APPLE__) && defined(__x86_64__)
	asm("mov %%gs:0x00,%0" : "=r"(ThreadID));
	#elif defined(__i386__)
	asm("mov %%gs:0x08,%0" : "=r"(ThreadID));
	#elif defined(__x86_64__)
	asm("mov %%fs:0x10,%0" : "=r"(ThreadID));
	#else
#error Unsupported architecture
	#endif
#endif

#define GAIPF_EXTERNAL							extern "C" __declspec (dllexport)

#define GAIPF_ValueFromPointer( type, ptr )   	((type)(size_t)(ptr))
#define GAIPF_PointerFromValue( type, value ) 	(type *)((size_t)value)

#define GAIPF_Kilobytes( value ) 				((value)*1024LL)
#define GAIPF_Megabytes( value ) 				(GAIPF_Kilobytes(value)*1024LL)
#define GAIPF_Gigabytes( value ) 				(GAIPF_Megabytes(value)*1024LL)
#define GAIPF_Terabytes( value ) 				(GAIPF_Gigabytes(value)*1024LL)

#define GAIPF_ArrayCount( array ) 				(sizeof(array)/sizeof(array[0]))

struct gaipf_mutex
{
	u64 volatile mutex;
	u64 volatile serving;
};

struct gaipf_textured_vertex
{
	v2  uv;
	v3  position;
	u32 color;	
};


#define _INCLUDE_GAI_PLATFORM_H
#endif