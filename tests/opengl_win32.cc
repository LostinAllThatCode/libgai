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

#define XWND_OPENGL_DEBUG
#define XWND_IMPLEMENTATION
#define GAI_DEBUG
#include "gai_xwindow.h"

int main(int argc, char **argv)
{
    gai_xwnd window;
    gaiXWindow(&window, "yolo", -1, -1, -1, -1, XWND_RENDERER_OPENGL);    
    glClearColor(1, 0, 1, 1);

    char *version = R"GLSL( 
        #version 440
    )GLSL";

    char *vs = R"GLSL(
        layout (location = 0) in vec2 uv;
        layout (location = 1) in vec4 color;
        layout (location = 2) in vec3 normal;
        layout (location = 3) in vec3 vertex;
        uniform mat4 model = mat4(1);
        uniform mat4 view  = mat4(1);
        uniform mat4 proj  = mat4(1);
        out vec4 _color;
        void main() {
           gl_Position = proj * view * model * vec4(vertex, 1);
           _color = vec4(color);
        }
    )GLSL";

    char *fs = R"GLSL(
        in vec4 _color;
        out vec4 color;
        void main() {
            color = _color;
        }
    )GLSL";

    u32 a = gaiOpenGLCreateProgram(version, vs, fs);

    char title[4096] = { 0 };
    i32 running = 1;
    while (running)
    {

        glClear(GL_COLOR_BUFFER_BIT);

        gai_snprintf(title, 4096, "%f, %s\n", window.dt.seconds, (char*) glGetString(GL_VERSION));
        gaiXWindowSetTitle(&window, title);

        running = gaiXWindowUpdate(&window);

        gaiXWindowRender(&window);
    }
    return 0;
}
#endif

