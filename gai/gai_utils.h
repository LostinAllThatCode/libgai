#ifndef _GAI_UTILS_H

#ifdef GAI_STATIC
	#define GAI_DEF static
#else
	#define GAI_DEF extern
#endif

#ifdef _DEBUG
#if _MSC_VER
	#define gai_assert(exp) if (!(exp)) { gai_debug("ASSERTION ERROR: "#exp"\n"); *(int *)0 = 0; }
	static char _xwnd_global_dbg_buffer[4096];
#define gai_debug(fmt, ...) gai_snprintf(_xwnd_global_dbg_buffer, 4096, "\"%s\" %s(%i): "fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); OutputDebugStringA(_xwnd_global_dbg_buffer)
#endif
#if __clang__
	#include "assert.h"
	#undef gai_assert
	#define gai_assert(exp)  				assert(exp)
#endif

#ifdef GAI_DBGCONSOLE
	#undef gai_debug
	#define gai_debug(fmt, ...) gai_printf("\"%s\" %s(%i): "fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#endif
#else
#define gai_assert(exp)
#ifdef GAI_DBGCONSOLE
	#undef gai_debug
	#define gai_debug(fmt, ...) gai_printf("%s: "fmt, __FUNCTION__, __VA_ARGS__)
#else
	#define gai_debug(...)
#endif
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

#define gai_memset(ptr, value, num)  					memset( (ptr), (value) , (num) )
#define gai_memcpy(dest, source, num)  					memcpy( (dest) , (source) , (num) )
#define gai_strlen(s)                      				strlen( (s) )
#define gai_printf(fmt, ...)          					printf( fmt , __VA_ARGS__ )
#define gai_snprintf(buffer, buflen, fmt, ...)       	snprintf( (buffer), (buflen) , (fmt) , __VA_ARGS__ )

#define gai_array_reset(array)       					(gai_memset(array, 0, sizeof(array)))
#define gai_array_length(array)      					(sizeof(array)/sizeof((array)[0]))
#define gai_fiz(n)                   					for(u32 i = 0; i < n; i++)
#define gai_fei(v)                   					for(u32 i = 0; i < gai_array_length(v); i++)

#define gai_bitset(a, pos)                              ((a) & (1>>(pos)))
#define gai_bitget(a, pos)                              ((a) & (1<<(pos)))
#define gai_bitxor(a, b)                                ((a) ^ (b))
#define gai_bitand(a, b)                                ((a) & (b))
#define gai_bitor(a, b)                                 ((a) | (b))

#define gai_bytes_kb(val)								((val) * 1024)
#define gai_bytes_mb(val)								(gai_bytes_kb((val)) * 1024)
#define gai_bytes_gb(val)								(gai_bytes_mb((val)) * 1024)

#define _GAI_UTILS_H
#endif
