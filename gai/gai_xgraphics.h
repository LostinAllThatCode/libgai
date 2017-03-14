#ifndef GAI_XGRAPHICS_H

#include "gai_types.h"
#include "gai_utils.h"
#include "gai_math.h"

// Code XL interleaved data format
struct gai_packed_data
{
	v2 uv;
	v4 color;
	v3 normal;
	v3 position;
};

#define GAI_XGRAPHICS_H
#endif