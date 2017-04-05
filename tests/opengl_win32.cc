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

#if _DEBUG
    #define GAIXW_DEBUG
#endif

#define GAIXW_OPENGL_MAJOR_VERSION 3
#define GAIXW_OPENGL_MINOR_VERSION 3
#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#include "gai_xgraphics.h"

#if 1
gaixg_context *_dbg_ctx = 0;
char _dbg_txt_buffer[4096]; float _dbg_txt_xoff = 0, _dbg_txt_yoff = 12, _dbg_txt_z = 9;
#define gai_dbg_begin(ctx) _dbg_ctx = ctx
#define gai_dbg_end() _dbg_ctx = 0; _dbg_txt_xoff = 0; _dbg_txt_yoff = 12; _dbg_txt_z = 9;
#define gai_dbg_sprint(prefix, fmt, ...) \
    { \
        gai_snprintf( (_dbg_txt_buffer), 4095, "[ %-16s ] "fmt, prefix, __VA_ARGS__); \
        gaiXGraphicsDrawDynamicText2D( (_dbg_ctx), &(_dbg_txt_xoff), &(_dbg_txt_yoff), &(_dbg_txt_z), (_dbg_txt_buffer) , {1,1,0,1}); \
    }
#else
#define gai_dbg_begin(...)
#define gai_dbg_end(...)
#define gai_dbg_sprint(...)
#endif

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
int main(int argc, char **argv)
{
    gaixw_context window;
    if (!gaiXWindow(&window, "yoloä", -1, -1, -1, -1)) return -1;
    gaiXWindowSetTitle(&window, "testö123");

    gaixg_context graphics;
    gaiXGraphicsCreateContext(&graphics, &window);

    gaixg_font *f = gaiXGraphicsLoadFont(&graphics, "C:/windows/fonts/consolab.ttf", 12, false);
    gaixg_font *f2 = gaiXGraphicsLoadFont(&graphics, "C:/windows/fonts/consolab.ttf", 24, false);
    glClearColor(0, 0, 0, 1);

    srand(0);
    int min = 0;//48;
    int max = 254;//122;

    wchar_t matrix[220][100] = { 0 };
    for (int i = 0; i < 220; i++)
    {
        for (int j = 0; j < 99; j++)
        {
            int c = rand() % (max - min + 1) + min;
            if ( c >= 50 && c < 62) c = '\0';
            matrix[i][j] = c;
        }
    }

    char *name = getenv("username");

    float text_timer = 0.f;
    int   textid = 0;
    float time = 0.f;
    float time2 = 0.f;

    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time += gaiXWindowUpdate(&window);
        if (!window.is_running) break;

        #if 1
        gai_dbg_begin(&graphics);
        gai_dbg_sprint("Viewport", "%ix%i x:%i, y:%i, fps:%i, frametime:%f, vsync(f2):%i, fullscreen(f1):%i\n", window.info.width, window.info.height, window.info.x, window.info.y, window.info.fps, window.frametime.seconds, (window.renderer.attributes & gaixwFlagsVSYNC ? 1 : 0), (window.renderer.attributes & gaixwFlagsFullscreen ? 1 : 0));
        gai_dbg_sprint("Mouse", "x:%i, y:%i, dx:%i, dy:%i, wheel:%i, dwheel:%i, left:%i, middle:%i, right:%i\n", window.input.x, window.input.y, window.input.dx, window.input.dy, window.input.wheel, window.input.dwheel, gai_input_mouse[0], gai_input_mouse[1], gai_input_mouse[2]);
        gai_dbg_sprint("Renderer", "OpenGL, %s, %s, %s, %s\n", window.renderer.info.opengl.version, window.renderer.info.opengl.vendor, window.renderer.info.opengl.renderer, window.renderer.info.opengl.shading_language_version);
        gai_dbg_end();
        #endif

        #if 0
        if (time >= 0.048f)
        {
            time = 0.f;
            for ( int i = 0; i < (size / 2) - 1; i++)
            {
                if ( (i != 0) && ((i % 300) == 0))
                {
                    matrix[i] = '\n';
                }
                else
                {
                    int c = rand() % (max - min + 1) + min;
                    if ( c >= 0 && c < 31) c = 32;
                    matrix[i] = c;
                }
            }
        }
        #endif
        if (time >= .064f)
        {
            for (int i = 0; i < 220; i++)
            {
                for (int j = 0; j < 99; j++)
                {
                    int c = rand() % (max - min + 1) + min;
                    if ( c >= 55 && c < 62) c = '\0';
                    matrix[i][j] = c;
                }
            }
            time = 0.f;
        }

        time2 += window.frametime.seconds;
        float x = 10.f, y = window.info.height - 12, z = -1.f;        
        gaiXGraphicsDrawDynamicText2D(&graphics, &x, &y, &z, ">", V4(0, cosf(sinf(time2)), 0, 1), f2);
        gaiXGraphicsDrawDynamicText2D(&graphics, &x, &y, &z, name, V4(0, cosf(sinf(time2)), 0, 1), f2);
        gaiXGraphicsDrawDynamicText2D(&graphics, &x, &y, &z, "_", V4(0, sinf(time2 * 6.f), 0, 1), f2);

        gai_fiz(220)
        {
            float x = i * 9.f, y = 0, z = 0.f;
            gaiXGraphicsDrawDynamicText2D(&graphics, &x, &y, &z, matrix[i], V4(0, 1, 0, 1), f, 2);
        }

        gaiXGraphicsRenderDynamicText2D(&graphics);
        gaiXWindowRender(&window);
    }

    return 0;
}
#endif