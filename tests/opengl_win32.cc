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
        gaiOpenGLFlagsFullscreen,   false,
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


