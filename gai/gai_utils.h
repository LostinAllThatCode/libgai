#ifndef _GAI_UTILS_H

#ifdef GAI_DEBUG
	#if _MSC_VER
		#define GAI_ASSERT(exp) ( (exp) ? (void) 0 : *(int *)0 = 0)
	#endif
	#if __clang__
		#include "assert.h"
		#undef GAI_ASSERT
		#define GAI_ASSERT(exp)  assert(exp)
	#endif
#endif

#ifndef GAI_ASSERT
	#define GAI_ASSERT(exp)
#endif

#if _WIN32
	#include <windows.h>
	#define GAI_MALLOC(size)              VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define GAI_REALLOC(pointer, newsize) VirtualAlloc(pointer, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define GAI_FREE(pointer)             VirtualFree(pointer, 0, MEM_RELEASE)
#endif

#ifndef GAI_MALLOC
	#define GAI_MALLOC(size)              malloc(size)
	#define GAI_REALLOC(pointer,newsize)  realloc(pointer, newsize)
	#define GAI_FREE(pointer)             free(pointer);
#endif

#define gai_array_reset(array) (memset(array, 0, sizeof(array)))
#define gai_length(array) (sizeof(array)/sizeof(array[0]))
#define gai_fiz(n)        for(u32 i = 0; i < n; i++)
#define gai_fei(v)        for(u32 i = 0; i < GAI_ARRAYCOUNT(v); i++)

#define _GAI_UTILS_H
#endif
