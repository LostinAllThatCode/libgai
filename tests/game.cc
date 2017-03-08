#include <gai_core.h>

#include "game.h"
#define external extern "C" __declspec (dllexport)

r32 frametime;
i32 fps;
b32 render = true;
b32 msaa   = true;
b32 vsync;

r32 fov     = 90.f;
r32 yaw     = 0;
r32 pitch   = -45;
v3  cam_pos = V3(0, 5, 5);
r32 rot     = 0;

m4x4 proj;
m4x4 view;
r32 time;
r32 aspect;

external void
InitGame(GameMemory *memory)
{
    gaiOpenGLInitialzeFunctions();
    aspect  = ((r32) memory->window.width / (r32) memory->window.height);
    vsync       = gaiOpenGLGetSwapInterval();
    proj        = Perspective(aspect, 90, 0.1f, 100.f);
}

external void 
UpdateAndRender(GameMemory *memory)
{

    frametime += memory->window.dt;
    fps++;
    if (frametime >= 1.f)
    {

        gai_snprintf(__libgai_global_textbuffer, 4096, "%ix%i %ifps %ims(%f) fov(%i), %s, %s", memory->window.width, memory->window.height, fps, (i32)(memory->window.dt * 1000), memory->window.dt, (i32)fov, (char *) glGetString(GL_VERSION), gai_getosname());
        gaiWindowSetTitle(&memory->window, __libgai_global_textbuffer);
        fps = 0;
        frametime = 0;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (memory->window.input.keys[VK_ESCAPE].ended_down) gaiWindowDestroy(&memory->window);
    if (memory->window.input.keys['R'].ended_down) render = !render;
    if (memory->window.input.keys['M'].ended_down)
    {
        if (msaa) glDisable(GL_MULTISAMPLE);
        else glEnable(GL_MULTISAMPLE);
        msaa = !msaa;
    }
    if (memory->window.input.keys['V'].ended_down)
    {
        gaiOpenGLSetSwapInterval(!vsync);
        vsync  = gaiOpenGLGetSwapInterval();
    }

    if (render)
    {
        if (memory->window.input.buttons[0].count > 0)
        {
            yaw   += memory->window.input.dtx * 0.07f;
            pitch += memory->window.input.dty * 0.07f;
        }

        if (memory->window.input.dtwheel != 0)
        {
            fov = gaiMathClamp(fov - (memory->window.input.dtwheel / WHEEL_DELTA), 45, 90);
            printf("%i\n", memory->window.input.dtwheel);
            aspect  = ((r32) memory->window.width / (r32) memory->window.height);
            proj = Perspective(aspect, fov, 0.1f, 100.f);
        }

        glViewport(0, 0, memory->window.width, memory->window.height);
        m4x4 view_front = CameraOrbit(cam_pos, DEG2RAD(pitch), DEG2RAD(yaw));
        gaiRendererSetProjection(&memory->renderer, &proj, &view_front);

        glUniformMatrix4fv(memory->renderer.mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
        gaiRendererDraw(&memory->renderer, GL_LINES, memory->grid.start, memory->grid.count);

        time += memory->window.dt * 2;
        gai_fei(memory->quad)
        {
            m4x4 model = Translate(Identity4(), V3(0, _sin(time * 2.f + (i * 0.1f)), 0));
            glUniformMatrix4fv(memory->renderer.mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model.E);
            gaiRendererDraw(&memory->renderer, GL_TRIANGLES, memory->quad[i].start, memory->quad[i].count);
        }
    }
}