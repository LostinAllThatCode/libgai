#define GAI_DEBUG
#include <gai_core.h>

i32 window_ext[] =
{
    GAI_OPENGL_VSYNC, true,
    GAI_OPENGL_MSAA, 0,
    GAI_OPENGL_DEBUG, true,
};

int main(int argc, char **argv)
{
    gaiTimer timer;
    gaiTimerInit(&timer);

    gaiWindow wnd;
    if (gaiWindowCreate(&wnd, "title", -1, -1, -1, -1, GAI_WINDOW_UUID, gaiWindowFlagsOpenGL, window_ext, gai_array_length(window_ext)))
    {
        char window_title_buffer[1024];
        char *ogl_version = (char *)glGetString(GL_VERSION);
        r32 frametime = 0.f;
        i32 fps = 0;
        r32 time = 0.f;

        r32 aspect = ((r32) wnd.width / (r32) wnd.height);

        gaiRenderer renderer = {};
        if (!gaiRendererCreate(&renderer, wnd.width, wnd.height)) return -1;

        gaiRendererMesh quad[11];
        gai_fei(quad)
        {
            quad[i] = gaiRendererPushQuad(&renderer, V3i(5 - i, 0, 0), 1.f, 1.f, V4(1, 0, 1, 1));
        }

        gaiRendererMesh grid = gaiRendererPushGrid(&renderer, 1.f, 128, V4(.2f,.2f,.2f,.8f));

        r32 yaw     = -10;
        r32 pitch   = -30;
        v3  cam_pos = V3(0, 5, 5);
        r32 rot     = 0;
        
        b32 render = true;
        b32 vsync  = gaiOpenGLGetSwapInterval();
        while (gaiWindowUpdate(&wnd, 0))
        {
            r32 dt = gaiTimerGetTicksSeconds(&timer);
            frametime += dt;
            fps++;
            if (frametime >= 1.f)
            {
                snprintf(window_title_buffer, 1024, "%ix%i %ifps %ims(%f), %s, %s", wnd.width, wnd.height, fps, (i32)(dt * 1000), dt, ogl_version, gai_getosname());
                gaiWindowSetTitle(&wnd, window_title_buffer);
                fps = 0;
                frametime = 0;
            }

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if(wnd.input.keys['R'].ended_down) render = !render;
            if(wnd.input.keys['V'].ended_down) {
                gaiOpenGLSetSwapInterval(!vsync);
                vsync  = gaiOpenGLGetSwapInterval();
            }
            if (render)
            {
                if (wnd.input.buttons[0].count > 0)
                {
                    yaw   += wnd.input.dtx * 0.07f;
                    pitch += wnd.input.dty * 0.07f;
                }

                m4x4 proj       = Perspective(aspect, 60, 0.1f, 100.f);
                m4x4 view_front = CameraOrbit(cam_pos, DEG2RAD(pitch), DEG2RAD(yaw));
                gaiRendererSetProjection(&renderer, &proj, &view_front);

                time += dt;

                glUniformMatrix4fv(renderer.mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
                gaiRendererDraw(&renderer, GL_LINES, grid.start, grid.count);

                gai_fei(quad)
                {
                    m4x4 model = Translate(Identity4(), V3(0, _sin(time * 2.f + (i * 0.1f)), 0));
                    glUniformMatrix4fv(renderer.mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model.E);
                    gaiRendererDraw(&renderer, GL_TRIANGLES, quad[i].start, quad[i].count);
                }
            }
            gaiOpenGLSwapBuffers(&wnd);
        }
    }
    wglDeleteContext(wglGetCurrentContext());

    return 0;
}


