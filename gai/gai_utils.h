#ifndef _GAI_UTILS_H

#ifdef GAI_DEBUG
#if _MSC_VER
#   define GAI_ASSERT(exp) ( (exp) ? (void) 0 : *(int *)0 = 0)
#endif
#if __clang__
#   undef GAI_ASSERT
#   include "assert.h"
#   define GAI_ASSERT(exp)  assert(exp)
#endif
#else
#   define GAI_ASSERT(exp)
#endif

#ifndef GAI_MALLOC
#   define GAI_MALLOC(size)             malloc(size)
#   define GAI_REALLOC(pointer,newsize) realloc(pointer, newsize)
#   define GAI_FREE(pointer)            free(pointer);
#endif

#ifndef GAI_STATIC
#   define GAI_DEF static
#else
#   define GAI_DEF extern
#endif

#define gai_length(array) (sizeof(array)/sizeof(array[0]))     // Only works with STATIC arrays like int a[12];
#define gai_fiz(n) for(u32 i = 0; i < n; i++)                  // Loop until: i < n 
#define gai_fei(v) for(u32 i = 0; i < GAI_ARRAYCOUNT(v); i++)  // Loop until: i < arraysize of v

#define _GAI_UTILS_H
#endif
