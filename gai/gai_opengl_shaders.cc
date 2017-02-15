i32
gaiOpenGLProgramCreate()
{
    return glCreateProgram();
}

void
gaiOpenGLProgramFree(i32 id)
{
    glDeleteProgram(id);
}

i32
gaiOpenGLShaderLoad(i32 id, const char *source, GLenum type)
{
    b32 result = false;
    i32 sid = glCreateShader(type);
    if(glGetError() != GL_INVALID_ENUM)
    {
        glShaderSource(sid, 1, &source, 0);
        glCompileShader(sid);

        i32 status;
        glGetShaderStatus(sid, &status);
        if(status == GL_TRUE)
        {
            glAttachShader(id, sid);
            result = true;
        }
        glDeleteShader(sid);
    }
    else
    {
        GAI_ASSERT(!"Shader compile error");
    }
    return result;
}


i32
gaiOpenGLShaderLoadFromFile(i32 id, const char *filename, GLenum type)
{
    i32 result;
    char *source;
    FILE *fp = fopen(filename, "rb");
    if(fp) {
        i32 size = 0;
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        source = (char *) GAI_MALLOC(sizeof(char) * (size+1));
        fread(source, sizeof(char), size, fp);
        source[size] = '\0';
        fclose(fp);
        result = gaiOpenGLShaderLoad(id, source, type);
        GAI_FREE(source);
    }
    return result;
}

b32
gaiOpenGLProgramLink(i32 id)
{
    b32 result = false;
    glLinkProgram(id);
    if(glGetError() != GL_INVALID_VALUE && glGetError() != GL_INVALID_OPERATION)
    {
        i32 status;
        glGetProgramStatus(id, &status);
        if(status == GL_TRUE)
        {
            u32 shaders[12] = {};
            i32 attached = 0;
            glGetAttachedShaders(id, sizeof(shaders) / sizeof(shaders[0]), &attached, shaders);
            for(i32 i=0; i < attached; i++)
            {
                glDetachShader(id, shaders[i]);
            }
            glUseProgram(id);
            result = true;
        }
    } else GAI_ASSERT(!"Shader link error");
    return result;
}

i32
gaiOpenGLProgramGetUniform(i32 id, const char *name)
{
    return glGetUniformLocation(id, name);
}
