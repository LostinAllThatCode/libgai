/*
==========================================================================================
$Name: gai_opengl.h$
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_OPENGL_H_

#include <gl\gl.h>
//#include <glext.h>
//#include "glcorearb.h"
#define GAI_OPENGL_UUID "5a11ddb6-bccc-49a2-a2ca-0cf1223d70a8"

#define GAI_OPENGL_DBG_SHADER_FILENOTFOUND 109
#define GAI_OPENGL_DBG_SHADER_BINDLOCATION 110

enum gaiOpenGLFlagsEnum
{
	gaiOpenGLFlagsNone, gaiOpenGLFlagsMinor, gaiOpenGLFlagsMajor, gaiOpenGLFlagsVSYNC, gaiOpenGLFlagsMSAA,
	gaiOpenGLFlagsDebug, gaiOpenGLFlagsColorBits, gaiOpenGLFlagsDepthBits, gaiOpenGLFlagsStencilBits,
	gaiOpenGLFlagsFullscreen,
};

/* Use GAI_OPENGL_EXTENSIONS_USE_ARB to use all available ARB functions. */
#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF b32   gaiOpenGLLoadFunctions   (void);
GAI_DEF i32   gaiOpenGLCreateContext   (gaiWindow * window, const char *title, const char *wndclass = GAI_OPENGL_UUID, i32 width = 640, i32 height = 480, i32 x = 0, i32 y = 0, i32 *ext = 0, i32 count = 0);
GAI_DEF i32   gaiOpenGLDestroyContext  (void);
GAI_DEF void  gaiOpenGLSwapBuffers     (gaiWindow * window);
GAI_DEF void* gaiOpenGLGetProcAddress  (const char *proc);
GAI_DEF b32   gaiOpenGLIsSupported     (const char *extension);
GAI_DEF b32   gaiOpenGLGetSwapInterval (void);
GAI_DEF void  gaiOpenGLSetSwapInterval (b32 vsync);

#ifdef __cplusplus
}
#endif

static char __libgai_global_opengldbgbuffer[4096] = {};
#define gaiOpenGLDebugInfo(id, format, ...) \
	snprintf(__libgai_global_opengldbgbuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, (id), GL_DEBUG_SEVERITY_LOW, strlen(__libgai_global_opengldbgbuffer), __libgai_global_opengldbgbuffer)
#define gaiOpenGLDebugMessage(id, format, ...) \
	snprintf(__libgai_global_opengldbgbuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, (id), GL_DEBUG_SEVERITY_MEDIUM, strlen(__libgai_global_opengldbgbuffer), __libgai_global_opengldbgbuffer)
#define gaiOpenGLDebugError(id, format, ...) \
	snprintf(__libgai_global_opengldbgbuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, (id), GL_DEBUG_SEVERITY_HIGH, strlen(__libgai_global_opengldbgbuffer), __libgai_global_opengldbgbuffer)

#include <gai_opengl_functions.h>
#include <gai_opengl_shaders.h>

#ifdef GAI_OPENGL_IMPLEMENTATION
	#if _WIN32
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "gdi32.lib")
		#include "wglext.h"
		#include "gai_opengl_win32.cc"
	#elif __linux__
		#error "Linux is currently not support by this api"
	#elif __APPLE__
		#error "Apple is currently not support by this api"
	#endif
#endif

#define _GAI_OPENGL_H_
#endif
