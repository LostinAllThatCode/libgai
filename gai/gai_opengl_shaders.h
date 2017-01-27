/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_OPENGL_SHADERS_H_

#include "gai_types.h"
#include "gai_utils.h"

typedef struct
{
    u32 type;
    i32 id;
    i32 *buffers;
    i32 *uniforms;
} gaiShader;

#define GAI_OPENGL_SHADERS_DEFAULT_VBOS 8
#define GAI_OPENGL_SHADERS_DEFAULT_UNIFORMS 16


#ifdef GAI_OPENGL_SHADERS_IMPLEMENTATION
#   include "gai_opengl_shaders.cc"
#endif

#define _GAI_OPENGL_SHADERS_H_
#endif
