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
	i32 id;
	i32 GetUniform(const char *name)
	{
		return glGetUniformLocation(id, name);
	};
} gaiShader;

#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF i32  gaiOpenGLShaderCreate(gaiShader *shader);
GAI_DEF i32  gaiOpenGLShaderCreate(gaiShader *shader);
GAI_DEF void gaiOpenGLShaderFree(gaiShader *shader);
GAI_DEF i32  gaiOpenGLShaderCompileAttach(gaiShader *shader, const char *source, GLenum type);
GAI_DEF i32  gaiOpenGLShaderLoadCompileAttach(gaiShader *shader, const char *filename, GLenum type);
GAI_DEF b32  gaiOpenGLShaderLink(gaiShader *shader);
GAI_DEF i32  gaiOpenGLShaderGetUniform(gaiShader *shader, const char *name);

#ifdef __cplusplus
}
#endif

#ifdef GAI_OPENGL_SHADERS_IMPLEMENTATION
	#include "gai_opengl_shaders.cc"
#endif

#define _GAI_OPENGL_SHADERS_H_
#endif
