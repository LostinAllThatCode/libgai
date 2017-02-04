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

#include "gai_types.h"
#include "gai_utils.h"

#define GAI_WINDOW_IMPLEMENTATION
#include "gai_window.h"

#include <gl\gl.h>
#include "glext.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Every platform has a specific CreateContextEx function which can be used when you need more control */
GAI_DEF i32  gaiOpenGLCreateContext(gaiWindow * window, const char *title, i32 width = 640, i32 height = 480, i32 x = 0, i32 y = 0,
                                    i32 major = 0, i32 minor = 0, b32 vsync = true, i32 multisample = 0,
                                    u8 color_bits = 32, u8 depth_bits = 24, u8 stencil_bits = 8);
GAI_DEF void gaiOpenGLSwapBuffers(gaiWindow * window);
GAI_DEF b32  gaiOpenGLIsSupported(const char *extension);
GAI_DEF b32  gaiOpenGLGetSwapInterval(void);
GAI_DEF void gaiOpenGLSetSwapInterval(b32 vsync);
GAI_DEF b32  gaiOpenGLLoadFunctions(void);

#ifdef __cplusplus
}
#endif

/* Use GAI_OPENGL_EXTENSIONS_USE_ARB to use all available ARB functions. */
#include "gai_opengl_functions.h"

#ifdef GAI_OPENGL_IMPLEMENTATION
	#if _WIN32
		#pragma comment(lib, "opengl32.lib")
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
