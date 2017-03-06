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
	#ifndef gai_malloc
		#define gai_malloc(size)              	malloc(size)
		#define gai_realloc(pointer,newsize)  	realloc(pointer, newsize)
		#define gai_free(pointer)             	free(pointer)
	#endif
#endif

enum gaiPlatformOSEnum
{
	gaiPlatformOSUnknown 				= 0,
	gaiPlatformOSWindows 				= 1,
	gaiPlatformOSLinux   				= 2,
	gaiPlatformOSApple   				= 3,	
	gaiPlatformOSAndroid 				= 4,
};

#define gai_memset(ptr, value, num)  					memset( (ptr), (value) , (num) )
#define gai_memcpy(dest, source, num)  					memcpy( (dest) , (source) , (num) )
#define gai_strlen(s)                      				strlen( (s) )
#define gai_printf(fmt, ...)          					printf( fmt , __VA_ARGS__ )
#define gai_snprintf(buffer, buflen, fmt, ...)       	snprintf( (buffer), (buflen) , (fmt) , __VA_ARGS__ )

#define gai_array_reset(array)       					(gai_memset(array, 0, sizeof(array)))
#define gai_array_length(array)      					(sizeof(array)/sizeof(array[0]))
#define gai_fiz(n)                   					for(u32 i = 0; i < n; i++)
#define gai_fei(v)                   					for(u32 i = 0; i < gai_array_length(v); i++)

#define gai_bitset(a, pos)                              ((a) & (1>>(pos)))
#define gai_bitget(a, pos)                              ((a) & (1<<(pos)))
#define gai_bitxor(a, b)                                ((a) ^ (b))
#define gai_bitand(a, b)                                ((a) & (b)) 
#define gai_bitor(a, b)                                 ((a) | (b)) 


#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF gaiPlatformOSEnum 						gai_getos();
GAI_DEF char* 									gai_getosname();

#ifdef __cplusplus
}
#endif

#if defined GAI_SOURCE || defined GAI_EXPORT
inline gaiPlatformOSEnum
gai_getos()
{
	gaiPlatformOSEnum result = gaiPlatformOSUnknown;
	#if _WIN32
	result = gaiPlatformOSWindows;
	#elif __linux__
	result = gaiPlatformOSLinux;
	#elif __APPLE__
	result = gaiPlatformOSApple;
	#endif
	return result;
}

inline char *
gai_getosname()
{
	char *result = "unknown";
	switch (gai_getos())
	{
		case gaiPlatformOSWindows: { result = "windows"; } break;
		case gaiPlatformOSLinux:   { result = "linux"; } break;
		case gaiPlatformOSApple:   { result = "apple"; } break;
	}
	return result;
}
#endif


#define _GAI_UTILS_H
#endif
