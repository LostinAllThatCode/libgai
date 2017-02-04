/*
#define GAI_MALLOC(size)             VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
#define GAI_REALLOC(pointer,newsize) VirtualAlloc(pointer, newsize, MEM_RESET, PAGE_EXECUTE_READWRITE)
#define GAI_FREE(pointer)            VirtualFree(pointer, 0, MEM_RELEASE);
*/

#include <gai_core.h>
#include <stdio.h>
#include <math.h>

static const GLfloat g_vertex_buffer_data[] =
{
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

typedef struct
{
    gaiShader shader;
    i32       uniform_color;
} DefaultShader;

int main(int argc, char **argv)
{
    mat3x3 whatever = Mat3(1);
    gaiTimer timer = {};
    DefaultShader def;
    gaiWindow wnd = {0};
    gaiTimerInit(&timer);
    if (gaiOpenGLCreateContext(&wnd, "asd", 1024, 768, 0, 0, 0, 0, true, 8))
    {
        gaiWindowSetTitle(&wnd, (const char *)glGetString(GL_VERSION));

        if (gaiOpenGLShaderCreate(&def.shader))
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

            gaiOpenGLShaderCompileAttach(&def.shader, vs, GL_VERTEX_SHADER);
            gaiOpenGLShaderCompileAttach(&def.shader, fs, GL_FRAGMENT_SHADER);
            if (!gaiOpenGLShaderLink(&def.shader)) return 1;

            def.uniform_color = gaiOpenGLShaderGetUniform(&def.shader, "somecolor");
        }

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);

        glClearColor(1, 0, 1, 0);

        r32 color = 0.f;
        r32 time = 0;
        glBindVertexArray(1);

        while (gaiWindowUpdate(&wnd, 0))
        {
            r32 dt = (gaiTimerGetTicks(&timer) / 1000000.f);
            time += dt;
            color = gaiMathLerp(0.0f, 1.0f, (cosf(sinf(time))) );

            glViewport(0, 0, wnd.width, wnd.height);

            glUniform3f(def.uniform_color, color, color, color);
            //r32 *data = (r32 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            //glUnmapBuffer(GL_ARRAY_BUFFER);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            gaiOpenGLSwapBuffers(&wnd);
        }
        glDeleteBuffers(1, &vertexbuffer);
    }

    gaiOpenGLShaderFree(&def.shader);
    wglDeleteContext(wglGetCurrentContext());

    return 0;
}


