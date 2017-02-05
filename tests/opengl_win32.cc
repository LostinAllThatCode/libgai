
#include <gai_core.h>

static const mat3x3 g_vertex_buffer_data =
{
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

typedef struct
{
    i32 id;
    i32 uniform_color;
} DefaultShader;

int main(int argc, char **argv)
{
    DefaultShader def;

    gaiTimer timer;
    gaiTimerInit(&timer);

    gaiWindow wnd;
    if (gaiOpenGLCreateContext(&wnd, "asd", 1024, 768))
    {
        gaiOpenGLSetSwapInterval(1);
        gaiWindowSetTitle(&wnd, (const char *)glGetString(GL_VERSION));
        def.id = gaiOpenGLProgramCreate();
        if (def.id)
        {
            const char vs[] =
            {
                "#version 430\n"
                "layout (location = 0) in vec3 position;\n"
                "uniform vec3 somecolor;\n"
                "out vec4 _color;\n"
                "void main() {\n"
                "    gl_Position = vec4(position, 1);\n"
                "    _color = vec4(somecolor,1);\n"
                "}\n"
            };

            const char fs[] =
            {
                "#version 430\n"
                "in vec4 _color;\n"
                "out vec4 color;\n"
                "void main() {\n"
                "    color = _color;\n"
                "}\n"
            };

            gaiOpenGLShaderLoad(def.id, vs, GL_VERTEX_SHADER);
            gaiOpenGLShaderLoad(def.id, fs, GL_FRAGMENT_SHADER);
            if (!gaiOpenGLProgramLink(def.id)) return 1;

            def.uniform_color = gaiOpenGLProgramGetUniform(def.id, "somecolor");
        }

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        glBindVertexArray(1);

        glClearColor(1, 0, 1, 0);
        r32 color = 0.f;
        r32 time = 0;

        r32 frametime = 0.f;
        i32 fps = 0;
        while (gaiWindowUpdate(&wnd, 0))
        {
            r32 dt = gaiTimerGetTicksSeconds(&timer);
            frametime += dt;

            fps++;
            if(frametime >= 1.0f) {
                printf("%f ms per frame, %i fps, %f dt\n", (1000.0 / double(fps)), fps, dt);
                fps = 0;
                frametime = 0;
            }
            
            color = gaiMathClamp(sinf(frametime), 0.2f, 1.0f);
            glViewport(0, 0, wnd.width, wnd.height);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(def.id);
            glEnableVertexAttribArray(0);
            glUniform3f(def.uniform_color, color, color, color);
            //r32 *data = (r32 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            //glUnmapBuffer(GL_ARRAY_BUFFER);
            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glDisableVertexAttribArray(0);
            gaiOpenGLSwapBuffers(&wnd);
        }
        glDeleteBuffers(1, &vertexbuffer);
    }

    gaiOpenGLProgramFree(def.id);
    wglDeleteContext(wglGetCurrentContext());

    return 0;
}


