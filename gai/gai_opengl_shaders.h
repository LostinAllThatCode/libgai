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

#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF i32  gaiOpenGLProgramCreate(void);
GAI_DEF void gaiOpenGLProgramFree(i32 id);
GAI_DEF i32  gaiOpenGLShaderLoad(i32 id, const char *source, GLenum type);
GAI_DEF i32  gaiOpenGLShaderLoadFromFile(i32 id, const char *filename, GLenum type);
GAI_DEF b32  gaiOpenGLProgramLink(i32 id);
GAI_DEF i32  gaiOpenGLProgramGetUniform(i32 id, const char *name);

#ifdef __cplusplus
}
#endif

#ifdef GAI_OPENGL_SHADERS_IMPLEMENTATION
	#include "gai_opengl_shaders.cc"
#endif

#define _GAI_OPENGL_SHADERS_H_
#endif
