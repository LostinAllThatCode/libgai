/*
==========================================================================================
$Name: gai_opengl.h$
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
$Dependencies: win32(user32.lib), linux(X11)$
==========================================================================================
*/
#ifndef _GAI_WINDOW_H_

#include <gai_types.h>
#include <gai_utils.h>

#if _WIN32
#include <windows.h>
/* WIN32 platform structure which helds the window handle and the instance handle */
typedef struct gaiWindowPlatform
{
    HWND        hWnd;
    HDC         ctx;
    HINSTANCE   instance;
    const char  *classname;
} gaiWindowPlatform;

#elif __linux__
#include <X11/Xlib.h>
typedef struct gaiWindowPlatform
{
    Display *display;
    Visual  *visual;
    Window  *window;
} gaiWindowPlatform;
#elif __APPLE__
#else
struct gaiWindowPlatform;
#endif

enum gaiWindowFlagsEnum
{
    gaiWindowFlagsNone       = 0,
    gaiWindowFlagsOpenGL     = 1,
    gaiWindowFlagsDirectX    = 2,
    gaiWindowFlagsFullscreen = 3,
};

enum gaiWindowExtFlagsEnum
{
    GAI_WINDOW_EXT_STYLE,
};

typedef struct
{
    i32 count;
    b32 ended_down;
} gaiKeyState;

enum { GAI_ENUM_MOUSE_LEFT = 0, GAI_ENUM_MOUSE_RIGHT = 1, GAI_ENUM_MOUSE_MIDDLE = 2 };
typedef struct
{
    gaiKeyState keys[256], buttons[3];
    i32 x, y, dtx, dty, dtwheel;
    i32 scrx, scry;
} gaiInput;

typedef struct
{
    gaiWindowFlagsEnum      flags;
    r32                     dt;
    r32                     step;
    i32                     width, height;
    gaiInput                input;
    gaiWindowPlatform       platform;
    void*                   userdata;

} gaiWindow;

#define GAI_WINDOW_UUID "49186d43-91c8-4fbd-9192-2e30297c9838"

#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF i32  gaiWindowCreate        (gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y, const char *classname = GAI_WINDOW_UUID, gaiWindowFlagsEnum flags = gaiWindowFlagsNone, i32 *ext = 0, i32 count = 0);
GAI_DEF i32  gaiWindowUpdate        (gaiWindow *window, i32 block);
GAI_DEF void gaiWindowSetSize       (gaiWindow *window, i32 width, i32 height);
GAI_DEF void gaiWindowShow          (gaiWindow *window, u32 flag);
GAI_DEF void gaiWindowDestroy       (gaiWindow *window);
GAI_DEF void gaiWindowQuit          (gaiWindow *window);
GAI_DEF void gaiWindowSetTitle      (gaiWindow *window, const char *title);

#ifdef __cplusplus
}
#endif

#include <gai_opengl.h>

#ifdef GAI_WINDOW_IMPLEMENTATION
    #define GAI_MAIN int main(int argc, char **argv)
    #if _WIN32
        #ifdef GAI_NOCONSOLE
            #undef  GAI_MAIN
            #define GAI_MAIN int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
        #endif
        #pragma comment(lib, "user32.lib")
        #include <gai_window_win32.cc>        
    #elif __linux__
        #include <gai_window_linux.cc>
    #elif __APPLE__
        #error "not implemented"
    #endif
#endif

#define _GAI_WINDOW_H_
#endif
