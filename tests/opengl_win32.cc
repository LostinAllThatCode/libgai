/*
#define GAI_MALLOC(size)             VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
#define GAI_REALLOC(pointer,newsize) VirtualAlloc(pointer, newsize, MEM_RESET, PAGE_EXECUTE_READWRITE)
#define GAI_FREE(pointer)            VirtualFree(pointer, 0, MEM_RELEASE);
*/
#define GAI_DEBUG

#define GAI_OPENGL_EXTENSIONS_USE_ARB
#define GAI_OPENGL
#include <gai_core.h>

#include <stdio.h>

#include <math.h>

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

//int main(int argc, char **argv)
GAI_MAIN
{
    gaiShader test = {0};
    gaiWindow wnd = {0};
    if(gaiOpenGLCreateContext(&wnd, "test", 640, 480, 0, 0, 4, 1, 1, 8))
    {
        gaiWindowSetTitle(&wnd, (const char *)glGetString(GL_VERSION));

        if(gaiOpenGLShaderCreate(&test))
        {
            const char vs[] = {
                "#version 430\n"
                "layout (location = 0) in vec3 position;\n"
                "uniform vec3 somecolor;\n"
                "out vec4 _color;\n"
                "void main() {\n"
                "    gl_Position = vec4(position, 1);\n"
                "    _color = vec4(somecolor,1);\n"
                "}\n"
            };
            const char fs[] = {
                "#version 430\n"
                "in vec4 _color;\n"
                "out vec4 color;\n"
                "void main() {\n"
                "    color = _color;\n"
                "}\n"
            };
            gaiOpenGLShaderCompileAttach(&test, vs, GL_VERTEX_SHADER);
            gaiOpenGLShaderCompileAttach(&test, fs, GL_FRAGMENT_SHADER);
            if(!gaiOpenGLShaderLink(&test)) return 1;
            
            gaiOpenGLShaderUniformInit(&test, "somecolor", 0);
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
        
        glClearColor(0,0,0,0);

        r32 clr_x, clr_y, clr_z = 0.f;
        r32 dt = 0;
        glBindVertexArray(1);
        while(gaiWindowUpdate(&wnd, 0)) {
            glViewport(0, 0, wnd.width, wnd.height);
            clr_x = (wnd.input.x / ((r32) wnd.width));
            clr_y = (wnd.input.y / ((r32) wnd.height));
            clr_z = clr_x - clr_y;
            dt += .0016f;

            
            glUniform3f(test.uniforms[0], clr_x, clr_y, clr_z);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            

            r32 *data = (r32 *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

            int a = 1;

            glUnmapBuffer(GL_ARRAY_BUFFER);
            
            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
                       
            gaiOpenGLSwapBuffers(&wnd);
        }
        
        glDeleteBuffers(1, &vertexbuffer);
    }


    gaiOpenGLShaderFree(&test);
    wglDeleteContext(wglGetCurrentContext());
    
    return 0;
}
 

