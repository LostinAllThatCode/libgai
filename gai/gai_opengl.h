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
#include <glext.h>

#define GAI_OPENGL_DBG_SHADER_FILENOTFOUND 109
#define GAI_OPENGL_DBG_SHADER_BINDLOCATION 110

enum
{
	GAI_OPENGL_NONE, GAI_OPENGL_MINOR, GAI_OPENGL_MAJOR, GAI_OPENGL_VSYNC, 	GAI_OPENGL_MSAA,
	GAI_OPENGL_DEBUG, GAI_OPENGL_COLOR_BITS, GAI_OPENGL_DEPTH_BITS, GAI_OPENGL_STENCIL_BITS,
};

/* Use GAI_OPENGL_EXTENSIONS_USE_ARB to use all available ARB functions. */
#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF b32   gaiOpenGLLoadFunctions   (void);
GAI_DEF i32   gaiOpenGLCreateContext   (gaiWindow * window, const char *title, i32 width = 640, i32 height = 480, i32 x = 0, i32 y = 0,
                                        i32 major = 0, i32 minor = 0, b32 vsync = false, i32 multisample = 0, b32 debug = false,
                                        u8 color_bits = 32, u8 depth_bits = 24, u8 stencil_bits = 8);
GAI_DEF void  gaiOpenGLSwapBuffers     (gaiWindow * window);
GAI_DEF void* gaiOpenGLGetProcAddress  (const char *proc);
GAI_DEF b32   gaiOpenGLIsSupported     (const char *extension);
GAI_DEF b32   gaiOpenGLGetSwapInterval (void);
GAI_DEF void  gaiOpenGLSetSwapInterval (b32 vsync);


#ifdef __cplusplus
}
#endif

static char __gaiOpenGLDebugTextBuffer[4096] = {};
#define gaiOpenGLDebugInfo(id, format, ...) \
	snprintf(__gaiOpenGLDebugTextBuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, (id), GL_DEBUG_SEVERITY_LOW, strlen(__gaiOpenGLDebugTextBuffer), __gaiOpenGLDebugTextBuffer)
#define gaiOpenGLDebugMessage(id, format, ...) \
	snprintf(__gaiOpenGLDebugTextBuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, (id), GL_DEBUG_SEVERITY_MEDIUM, strlen(__gaiOpenGLDebugTextBuffer), __gaiOpenGLDebugTextBuffer)
#define gaiOpenGLDebugError(id, format, ...) \
	snprintf(__gaiOpenGLDebugTextBuffer, 4096, "%s(%i): %s()\n -> "format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, (id), GL_DEBUG_SEVERITY_HIGH, strlen(__gaiOpenGLDebugTextBuffer), __gaiOpenGLDebugTextBuffer)

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
