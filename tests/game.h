#ifndef _GAME_H
#include <gai_core.h>
struct GameMemory
{
    gaiWindow     window;
    gaiRenderer   renderer;

    gaiRendererMesh quad[11];
    gaiRendererMesh grid;

    HMODULE gameDLL;
};

typedef void init_game_fn(GameMemory *memory);
typedef void update_and_render_fn(GameMemory *memory);

#ifdef GAME_DLL_IMPORT
    update_and_render_fn *UpdateAndRender;
    init_game_fn *InitGame;
#endif

#define _GAME_H
#endif
