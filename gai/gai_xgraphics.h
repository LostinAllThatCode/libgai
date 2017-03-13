#ifndef GAI_XGRAPHICS_H

#include "gai_types.h"
#include "gai_utils.h"

#define GAI_DYN_ARRAY_DEFAULT_ELEMENTS_MAX 64
struct gai_dynamic_array
{
	u8 		*data;
	size_t  used;
	size_t 	size;
};

#define gaiDynArrayInit(array, type, ...) _gaiDynArrayInit( (array), sizeof(type), __VA_ARGS__ )
void    _gaiDynArrayInit(gai_dynamic_array *array, size_t element_size, int elements_initial = GAI_DYN_ARRAY_DEFAULT_ELEMENTS_MAX)
{
	if (!array)
	{
		gai_assert(array);
		return;
	}

	size_t memory_needed = element_size * elements_initial;
	array->data = (u8*) gai_malloc(memory_needed);
	if (array->data == 0)
	{
		gai_assert(!"Memory allocation failed.");
		return;
	}
	array->size = memory_needed;
	array->used = 0;
}

int _gaiDynArrayReAlloc(gai_dynamic_array *array, size_t new_size)
{
	gai_realloc( array->data, new_size );
	if (array->data == 0)
	{
		gai_assert(!"Memory reallocation failed.");
		return 0;
	}
	array->size = new_size;
	return 1;
}

void gaiDynArrayClear(gai_dynamic_array *array)
{
	gai_assert(array);
	memset(array->data, 0, array->size);
	array->used = 0;
}

void gaiDynArrayFree(gai_dynamic_array *array)
{
	gai_assert(array);
	gai_free(array->data);
}

#define gaiDynArrayInsert(array, el) _gaiDynArrayInsert( (array), &(el), sizeof(el) )
void    _gaiDynArrayInsert(gai_dynamic_array *array, void *data, size_t size)
{
	gai_assert(array);
	if ( (array->used + size) < array->size )
	{
		memcpy(array->data + array->used, data, size);
		array->used += size;
	}
	else
	{
		if (_gaiDynArrayReAlloc(array, array->size * 2) == 1)
		{
			_gaiDynArrayInsert(array, data, size);
		}
		else
		{
			gai_assert("Dynamic array size exceeded.")
		}

	}
}

#define gaiDynArrayGet(array, idx, type) ((type *) _gaiDynArrayGet( (array),(idx), sizeof(type) ))
void*   _gaiDynArrayGet(gai_dynamic_array *array, int index, size_t element_size)
{
	gai_assert(array);
	void *el = array->data + (index * element_size);
	if (el < (array->data + array->size - element_size) && el != 0)
	{
		return el;
	}
	return 0;
}

#define gaiDynArraySet(array, idx, el) _gaiDynArraySet( (array),  (idx), &(el), sizeof(el) )
void*   _gaiDynArraySet(gai_dynamic_array *array, int index, void *data, size_t element_size)
{
	gai_assert(array);
	void *el = array->data + (index * element_size);
	if (el != 0)
	{
		memcpy(el, data, element_size);
		return el;
	}
	return 0;
}

#define gaiDynArraySize(array, type) _gaiDynArraySize( (array), sizeof(type) )
int _gaiDynArraySize(gai_dynamic_array *array, size_t element_size)
{
	gai_assert(array);
	return (array->used / element_size);
}


#define GAI_XGRAPHICS_H
#endif