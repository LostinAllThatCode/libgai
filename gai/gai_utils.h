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

	#ifdef GAI_DEBUG_TO_CONSOLE
		#undef gai_debug
		#define gai_debug(fmt, ...) gai_printf("\"%s\" %s(%i): "fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
	#endif
#else
	#define gai_assert(exp)
	#ifdef GAI_DEBUG_TO_CONSOLE
		#undef gai_debug
		#define gai_debug(fmt, ...) gai_printf("%s: "fmt, __FUNCTION__, __VA_ARGS__)
	#else
		#define gai_debug(...)
	#endif
#endif

#if _WIN32
	#include <windows.h>
	#define gai_malloc(size)              		VirtualAlloc(0, (size), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define gai_realloc(pointer, newsize) 		VirtualAlloc((pointer), (newsize), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
	#define gai_free(pointer)             		VirtualFree((pointer), 0, MEM_RELEASE)
#else
	#ifndef gai_malloc
		#define gai_malloc(size)              	malloc((size))
		#define gai_realloc(pointer,newsize)  	realloc((pointer), (newsize))
		#define gai_free(pointer)             	free((pointer))
	#endif
#endif

#ifndef gai_memset
	#define gai_memset(ptr, value, num)  					memset( (ptr), (value) , (num) )
#endif
#ifndef gai_memcpy
	#define gai_memcpy(dest, source, num)  					memcpy( (dest) , (source) , (num) )
#endif
#ifndef gai_strlen
	#define gai_strlen(s)                      				strlen( (s) )
#endif
#ifndef gai_printf
	#define gai_printf(fmt, ...)          					printf( fmt , __VA_ARGS__ )
#endif
#ifndef gai_snprintf
	#define gai_snprintf(buffer, buflen, fmt, ...)       	snprintf( (buffer), (buflen) , (fmt) , __VA_ARGS__ )
#endif

#define gai_array_reset(array)       						(gai_memset(array, 0, sizeof(array)))
#define gai_array_length(array)      						(sizeof(array)/sizeof((array)[0]))
#define gai_fiz(n)                   						for(u32 i = 0; i < n; i++)
#define gai_fjz(n)		                   					for(u32 j = 0; j < n; j++)
#define gai_fkz(n)		                   					for(u32 k = 0; k < n; k++)
#define gai_fei(v)                   						for(u32 i = 0; i < gai_array_length(v); i++)

#define gai_bitset(a, pos)                              	((a) & (1>>(pos)))
#define gai_bitget(a, pos)                              	((a) & (1<<(pos)))
#define gai_bitxor(a, b)                                	((a) ^ (b))
#define gai_bitand(a, b)                                	((a) & (b))
#define gai_bitor(a, b)                                 	((a) | (b))

#define gai_kilobytes(val)									((val) * 1024LL)
#define gai_megabytes(val)									(gai_kilobytes((val)) * 1024LL)
#define gai_gigabytes(val)									(gai_megabytes((val)) * 1024LL)

#define gai_offsetof(st, m) 								((size_t)&(((st *)0)->m))



// Really plain straight forward dynamic array implementation.
// It simply doubles the inital size whenever the maximum element size is exceeded.
// To use this with you wanted structure or type just generate the type with the
// GAI_GENDYNAMIC macro.
//
// Ex.:
// 		struct myCustomStruct { int a; int b; };
// 		GAI_DYNAMIC( myDynCustomStruct, myCustomStruct );
//
//      int main(void) {
// 			myDynCustomStruct test;
// 			test.append( { 2, 3} );
// 			printf("%i, %i\n, test.get(0)->a, test.get(0)->b));
// 		}
//

#define GAI_DYNAMIC( name, type ) \
	struct name \
	{ \
		/* General gai_dynarray structure for internal use. Do not change any of these! */ \
		type	*elements; \
		size_t  used; \
		size_t 	size; \
		size_t  type_size = sizeof(type); \
		/* Functions to actually use. */ \
		name()  							{ gaiDynArrayInit(this, this->type_size); }; \
		~name() 							{ gaiDynArrayFree(this); }; \
		type* append()				 		{ return (type *) gaiDynArrayInsert(this, 0, this->type_size); }; \
		type* append( type  element ) 		{ return (type *) gaiDynArrayInsert(this, &element, sizeof(element) ); }; \
		type* get( int index ) 				{ return (type *) gaiDynArrayGet(this, index, this->type_size ); }; \
		void  set( int index, type element) { gaiDynArraySet(this, index, &element, this->type_size ); }; \
		void  clear()	 					{ gaiDynArrayClear(this); }; \
		int   length()						{ return gaiDynArraySize(this, this->type_size); }; \
	}

//void  append( type* element ) 		{ gaiDynArrayInsert(this, element, this->type_size ); };

#define GAI_DYN_ARRAY_DEFAULT_ELEMENTS_MAX 64
struct gai_dynarray
{
	void	*elements;
	size_t  used;
	size_t 	size;
	size_t  type_size;
};

void gaiDynArrayInit(void *dynamic, size_t element_size, int elements_initial = GAI_DYN_ARRAY_DEFAULT_ELEMENTS_MAX)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	if (!array)
	{
		gai_assert(array);
		return;
	}

	size_t memory_needed = element_size * elements_initial;
	array->elements = (u8*) malloc(memory_needed);
	if (array->elements == 0)
	{
		gai_assert(!"Memory allocation failed.");
		return;
	}
	array->size = memory_needed;
	array->used = 0;
}

int gaiDynArrayReAlloc(void *dynamic, size_t new_size)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	#if 0
	void *ptr = realloc( array->elements, new_size );
	if (ptr) array->elements = (u8*) ptr;
	#else
	void *new_address = malloc(new_size);
	if (new_address)
	{
		memcpy(new_address, array->elements, array->used);
		free(array->elements);
		array->elements = (u8*) new_address;
		array->size = new_size;
	}
	else
	{
		gai_assert(!"Memory reallocation failed.");
		return 0;
	}
	#endif
	return 1;
}

void gaiDynArrayClear(void *dynamic)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);
	memset(array->elements, 0, array->size);
	array->used = 0;
}

void gaiDynArrayFree(void *dynamic)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);
	free(array->elements);
}

void *gaiDynArrayInsert(void *dynamic, void *element, size_t element_size)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);
	if ( (array->used + element_size) < array->size )
	{
		void *dest = ((u8*) array->elements) + array->used;
		if (element == 0)
		{
			memset(dest, 0, element_size);
		}
		else
		{
			memcpy(dest, element, element_size);
		}
		array->used += element_size;
		return dest;
	}
	else
	{
		void *new_address = realloc(array->elements, array->size * 2);
		if (new_address)
		{
			array->elements = (u8*)new_address;
			array->size = array->size * 2;
			return gaiDynArrayInsert(array, element, element_size);
		}
		else
		{
			gai_assert(!"Memory reallocation failed.");
			return 0;
		}
	}
}

void* gaiDynArrayGet(void *dynamic, int index, size_t element_size)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);
	if ( (index * element_size) > array->used ) return 0;
	void *address = ((u8*) array->elements) + (index * element_size);
	if (address != 0)
	{
		return address;
	}
	return 0;
}

void* gaiDynArraySet(void *dynamic, int index, void *element, size_t element_size)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);

	if ( (index * element_size) >= array->size ) return 0;
	void *address = ((u8*) array->elements) + (index + element_size);
	if (address != 0)
	{
		memcpy(address, element, element_size);
		return address;
	}
	return 0;
}

int gaiDynArraySize(void *dynamic, size_t element_size)
{
	gai_dynarray *array = (gai_dynarray *) dynamic;
	gai_assert(array);
	return (array->used / element_size);
}


GAI_DEF int
gai_file_exists(char *filename)
{
	struct stat buffer;
	int result = stat(filename, &buffer);
	return (result == 0);
}

#define _GAI_UTILS_H
#endif
