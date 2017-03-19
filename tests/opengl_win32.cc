#if 0
#define GAI_DEBUG
#include <gai_core.h>

#define GAME_DLL_IMPORT
#include "game.h"

void reloadGameDLL(gaiHotReloadFile *dll)
{
    GameMemory *mem = (GameMemory *)dll->userdata;
    char NewFilename[GAI_HOTRELOAD_MAX_TEXT_LEN] = {};
    gai_snprintf(NewFilename, GAI_HOTRELOAD_MAX_TEXT_LEN, "~%s", dll->filename);
    if (mem->gameDLL) FreeLibrary(mem->gameDLL);
    CopyFileA(dll->filename, NewFilename, false);
    mem->gameDLL = LoadLibrary(NewFilename);
    if (mem->gameDLL)
    {
        InitGame = (init_game_fn *) GetProcAddress(mem->gameDLL, "InitGame");
        if (InitGame) InitGame((GameMemory *)dll->userdata);
        UpdateAndRender = (update_and_render_fn *) GetProcAddress(mem->gameDLL, "UpdateAndRender");
    }
}


int main(int argc, char **argv)
{
    i32 flags[] =
    {
        gaiOpenGLFlagsVSYNC,        true,
        gaiOpenGLFlagsDebug,        true,
        gaiOpenGLFlagsFullscreen,   true,
        gaiOpenGLFlagsMSAA,         8,
    };

    GameMemory mem = {};

    if (gaiWindowCreate2(&mem.window, gaiWindowTypeOpenGL, flags, gai_array_length(flags)))
    {
        if (!gaiRendererCreate(&mem.renderer, mem.window.width, mem.window.height)) return -1;

        gaiHotReloadFile dll;
        gaiHotReloadAddFile(&dll, "game.dll", 2000, reloadGameDLL, &mem, gaiHotReloadDontHandleEvent);

        gai_fei(mem.quad) mem.quad[i] = gaiRendererPushQuad(&mem.renderer, V3i(5 - i, 0, 0), 1.f, 1.f, V4(0, 1, 1, 1));
        mem.grid  = gaiRendererPushGrid(&mem.renderer, 1.f, 128, V4(.2f, .2f, .2f, .8f));

        while (gaiWindowUpdate(&mem.window, 0))
        {
            if (gaiHotReloadBeginMutex(&dll, 0))
            {
                gaiHotReloadEventCallback(&dll);
                if (UpdateAndRender) UpdateAndRender(&mem);
                gaiHotReloadEndMutex(&dll);
            }
            gaiOpenGLSwapBuffers(&mem.window);
        }
    }
    gaiOpenGLDestroyContext();
    return 0;
}
#else

#if 0
    #define XWND_OPENGL_MAJOR 3
    #define XWND_OPENGL_MINOR 1
#endif

#include "gai_core.h"

#if 1
gai_xgp_context *_dbg_ctx = 0;
char _dbg_txt_buffer[4096]; float _dbg_txt_xoff, _dbg_txt_yoff;
#define gai_dbg_begin(ctx) _dbg_ctx = ctx
#define gai_dbg_end() _dbg_ctx = 0; _dbg_txt_xoff = 0; _dbg_txt_yoff = 0
#define gai_dbg_sprint(prefix, fmt, ...) \
    { \
        gai_snprintf( (_dbg_txt_buffer), 4095, "[ %-16s ] "fmt, prefix, __VA_ARGS__); \
        gaiXGraphicsDrawDynamicText2D( (_dbg_ctx), &(_dbg_txt_xoff), &(_dbg_txt_yoff), (_dbg_txt_buffer) , {1,1,1,1}); \
    }
#else
#define gai_dbg_begin(...)
#define gai_dbg_end(...)
#define gai_dbg_sprint(...)
#endif

void reload()
{

}

int main(int argc, char **argv)
{
    gai_xwnd window;
    if (!gaiXWindow(&window, "yolo", -1, -1, -1, -1, XWND_RENDERER_OPENGL)) return -1;

    gai_xgp_context graphics;
    gaiXGraphicsCreateContext(&graphics, &window);

    gai_xgp_font *f = gaiXGraphicsLoadFont(&graphics, "C:/windows/fonts/impact.ttf", 48);
    glClearColor(0, 0, 0, 1);

    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        gaiXWindowUpdate(&window);
        if (!window.is_running) break;

        #if 1
        gai_dbg_begin(&graphics);
        gai_dbg_sprint("Viewport", "%ix%i x:%i, y:%i, fps:%i, frametime:%f, vsync(f2):%i, fullscreen(f1):%i\n", window.info.width, window.info.height, window.info.x, window.info.y, window.info.fps, window.frametime.seconds, (window.renderer.attributes & XWND_VSYNC ? 1 : 0), (window.renderer.attributes & XWND_FULLSCREEN ? 1 : 0));
        gai_dbg_sprint("Mouse", "x:%i, y:%i, dx:%i, dy:%i, wheel:%i, dwheel:%i, left:%i, middle:%i, right:%i\n", window.input.x, window.input.y, window.input.dx, window.input.dy, window.input.wheel, window.input.dwheel, gai_input_mouse[0], gai_input_mouse[1], gai_input_mouse[2]);
        gai_dbg_sprint("Renderer", "OpenGL, %s, %s, %s, %s\n", window.renderer.info.opengl.version, window.renderer.info.opengl.vendor, window.renderer.info.opengl.renderer, window.renderer.info.opengl.shading_language_version);
        gai_dbg_end();
        #endif

        gaiXGraphicsDrawDynamicText2D(&graphics, 100, 600, ";)", V4(1, 1, 0, 1), f);
        gaiXGraphicsRenderDynamicText2D(&graphics);
        gaiXWindowRender(&window);
    }
    return 0;
}
#endif