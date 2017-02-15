#define GAI_DEBUG
#define GAI_OPENGL_DEBUG
#include <gai_core.h>
#include <gai_renderer.h>

int main(int argc, char **argv)
{
    gaiTimer timer;
    gaiTimerInit(&timer);

    gaiWindow wnd;
    if (gaiOpenGLCreateContext(&wnd, "asd", 1024, 768, 0, 0, 0, 0, 0, 0))
    {        
        gaiOpenGLSetSwapInterval(-1);
        char window_title_buffer[1024];
        char *ogl_version = (char *)glGetString(GL_VERSION);

        r32 frametime = 0.f;
        i32 fps = 0;
        r32 time = 0.f;

        gaiRenderer renderer = {};
        if(!gaiRendererCreate(&renderer, wnd.width, wnd.height)) return -1;

        r32 rot = 0;
        while (gaiWindowUpdate(&wnd, 0))
        {
            r32 dt = gaiTimerGetTicksSeconds(&timer);
            time += dt;

            rot += 1;;
            if (rot > 360) rot -= rot;

            #if 1
            frametime += dt;
            fps++;
            if (frametime >= .5f)
            {
                snprintf(window_title_buffer, 1024, "%ix%i %ifps %ims(%f), %s", wnd.width, wnd.height, fps * 2, (i32)(500.0 / double(fps)), dt, ogl_version);
                gaiWindowSetTitle(&wnd, window_title_buffer);
                fps = 0;
                frametime = 0;
            }
            #endif

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            r32 aspect = ((r32) wnd.width / (r32) wnd.height);
            m4x4 ident = Identity4();
            m4x4 model = ZRotation(DEG2RAD(-rot)) * YRotation(DEG2RAD(rot)) * XRotation(DEG2RAD(rot));

            m4x4 proj = Perspective(aspect, 90, 0.1f, 100.f);

            glViewport(0, 0, wnd.width, wnd.height);
            m4x4 view_front = CameraTrans(V3(0,0,10),V3(0,0,0),V3(0,1,0));
            gaiRendererSetProjection(&renderer, &proj, &view_front);
            gaiRendererDrawTriangleImmediate(&renderer, &model, V4(1, 0, 1, 1));            
    
            glViewport(wnd.width/2, 0, wnd.width / 2, wnd.height / 2);
            m4x4 view_side = CameraTrans(V3(10,0,0),V3(0,0,0),V3(0,1,0));
            gaiRendererSetProjection(&renderer, &proj, &view_side);
            gaiRendererDrawTriangleImmediate(&renderer, &model, V4(1, 0, 1, 1));

            glViewport(wnd.width / 2, wnd.height / 2, wnd.width / 2, wnd.height / 2);
            m4x4 view_top = CameraTrans(V3(0,10,.1f),V3(0,0,0),V3(0,1,0));
            gaiRendererSetProjection(&renderer, &proj, &view_top);
            gaiRendererDrawTriangleImmediate(&renderer, &model, V4(1, 0, 1, 1));

            gaiOpenGLSwapBuffers(&wnd);
        }
    }
    wglDeleteContext(wglGetCurrentContext());

    return 0;
}


