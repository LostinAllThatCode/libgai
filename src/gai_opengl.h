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
GAI_DEF i32  gaiOpenGLCreateContext   (gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y,
                                       i32 major, i32 minor, b32 vsync, i32 multisample,
                                       u8 color_bits, u8 depth_bits, u8 stencil_bits);
GAI_DEF void gaiOpenGLSwapBuffers     (gaiWindow *window);
GAI_DEF b32  gaiOpenGLIsSupported     (const char *extension);
GAI_DEF b32  gaiOpenGLGetSwapInterval (void);
GAI_DEF void gaiOpenGLSetSwapInterval (b32 vsync);
GAI_DEF b32  gaiOpenGLLoadFunctions   (void);


/* Use GAI_OPENGL_EXTENSIONS_USE_ARB to use all available ARB functions. */
#include "gai_opengl_functions.h"
    
#ifdef GAI_OPENGL_IMPLEMENTATION
#   if _WIN32
#      pragma comment(lib, "opengl32.lib")
#      pragma comment(lib, "gdi32.lib")
#      include "wglext.h"
#      include "gai_opengl_win32.cc"
#   elif __linux__
#      error "Linux is currently not support by this api"
#   elif __APPLE__
#      error "Apple is currently not support by this api"
#   endif
#endif

#ifdef __cplusplus
}
#endif

#define _GAI_OPENGL_H_
#endif 
