#ifndef _GAI_UTILS_H

static char __libgai_global_textbuffer[4096];

#ifdef GAI_DEBUG
	#if _MSC_VER
		#define gai_assert(exp) 				( (exp) ? (void) 0 : *(int *)0 = 0)
	#endif
	#if __clang__
		#include "assert.h"
		#undef gai_assert
		#define gai_assert(exp)  				assert(exp)
	#endif
#else
	#define gai_assert(exp)
#endif

#if _WIN32
	#include <windows.h>
	#define gai_malloc(size)              		VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define gai_realloc(pointer, newsize) 		VirtualAlloc(pointer, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define gai_free(pointer)             		VirtualFree(pointer, 0, MEM_RELEASE)
#else
	#ifndef GAI_MALLOC
		#define gai_malloc(size)              	malloc(size)
		#define gai_realloc(pointer,newsize)  	realloc(pointer, newsize)
		#define gai_free(pointer)             	free(pointer)
	#endif
#endif

enum GAI_PLATFORM_OS_ENUM
{
	GAI_PLATFORM_OS_ENUM_UNKNOWN 				= 0,
	GAI_PLATFORM_OS_ENUM_WINDOWS 				= 1,
	GAI_PLATFORM_OS_ENUM_LINUX   				= 2,
	GAI_PLATFORM_OS_ENUM_APPLE   				= 3,
	GAI_PLATFORM_OS_ENUM_ANDROID 				= 4,
};

#define gai_memset(ptr, value, num)  					memset( (ptr), (value) , (num) )
#define gai_memcpy(dest, source, num)  					memcpy( (dest) , (source) , (num) )
#define gai_strlen(s)                      				strlen( (s) )
#define gai_printf(format, ...)          				printf( format , __VA_ARGS__)
#define gai_snprintf(buffer, buflen, format,...)        snprintf( (buffer), (buflen) , (format) , __VA_ARGS__)

#define gai_array_reset(array)       			(gai_memset(array, 0, sizeof(array)))
#define gai_array_length(array)      			(sizeof(array)/sizeof(array[0]))
#define gai_fiz(n)                   			for(u32 i = 0; i < n; i++)
#define gai_fei(v)                   			for(u32 i = 0; i < gai_array_length(v); i++)

#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF GAI_PLATFORM_OS_ENUM 					gai_getos();
GAI_DEF char* 									gai_getosname();

#ifdef __cplusplus
}
#endif

#if defined GAI_SOURCE || defined GAI_EXPORT
inline GAI_PLATFORM_OS_ENUM
gai_getos()
{
	GAI_PLATFORM_OS_ENUM result = GAI_PLATFORM_OS_ENUM_UNKNOWN;
	#if _WIN32
	result = GAI_PLATFORM_OS_ENUM_WINDOWS;
	#elif __linux__
	result = GAI_PLATFORM_OS_ENUM_LINUX;
	#elif __APPLE__
	result = GAI_PLATFORM_OS_ENUM_APPLE;
	#endif
	return result;
}

inline char *
gai_getosname()
{
	char *result = "unknown";
	switch (gai_getos())
	{
		case GAI_PLATFORM_OS_ENUM_WINDOWS: { result = "windows"; } break;
		case GAI_PLATFORM_OS_ENUM_LINUX:   { result = "linux"; } break;
		case GAI_PLATFORM_OS_ENUM_APPLE:   { result = "apple"; } break;
	}
	return result;
}
#endif


#define _GAI_UTILS_H
#endif
