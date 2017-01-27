GAI_DEF i32
gaiOpenGLShaderCreate(gaiShader *shader,
                      i32 buffer_count = GAI_OPENGL_SHADERS_DEFAULT_VBOS,
                      i32 uniform_count = GAI_OPENGL_SHADERS_DEFAULT_UNIFORMS,
                      u32 type = 0)
{
    GAI_ASSERT(shader);

    shader->id = glCreateProgram();
    if(!shader->id) return -1;
    
    shader->type = type;
    shader->buffers  = (i32 *) GAI_MALLOC(sizeof(i32) * buffer_count);
    memset(shader->buffers, 0, (sizeof(i32) * buffer_count));
    shader->uniforms = (i32 *) GAI_MALLOC(sizeof(i32) * uniform_count);
    memset(shader->uniforms, 0, (sizeof(i32) * buffer_count));
    
    if(!shader->buffers || !shader->uniforms) {
        if(shader->buffers) GAI_FREE(shader->buffers);
        if(shader->uniforms) GAI_FREE(shader->uniforms);
        return -2;
    }

    return 1;
}

GAI_DEF void
gaiOpenGLShaderFree(gaiShader *shader)
{
    GAI_ASSERT(shader);

    u32 shaders[12] = {};
    i32 attached = 0;
    glGetAttachedShaders(shader->id, sizeof(shaders) / sizeof(shaders[0]), &attached, shaders);
    for(i32 i=0; i < attached; i++)
    {
        glDeleteShader(shaders[i]);
        glDetachShader(shader->id, shaders[i]);
    }

    glDeleteProgram(shader->id);

    GAI_FREE(shader->buffers);
    GAI_FREE(shader->uniforms);
}

GAI_DEF i32
gaiOpenGLShaderCompileAttach(gaiShader *shader, const char *source, GLenum type)
{
    b32 result = false;
    i32 id = glCreateShader(type);
    i32 programid = shader->id;
    
    if(glGetError() != GL_INVALID_ENUM)
    {
        glShaderSource(id, 1, &source, 0);
        glCompileShader(id);

        i32 status;
        glGetShaderStatus(id, &status);
        if(status == GL_FALSE)
        {
            i32 length = 0;
            char error[1024];
            glGetShaderInfoLog(id, 1024, &length, error);
            printf("error: %s\n %s", error, source);
            GAI_ASSERT(!"Shader compile error");
            result = false;
        } else {
            glAttachShader(programid, id);
            result = true;
        }
    }
    else
    {
        GAI_ASSERT(!"Shader compile error");
    }
    return result;
}


GAI_DEF i32
gaiOpenGLShaderLoadCompileAttach(gaiShader *shader, const char *filename, GLenum type)
{
    i32 result;
    char *source;
    FILE *fp = fopen(filename, "rb");
    if(fp) {
        i64 size = 0;
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        source = (char *) GAI_MALLOC(sizeof(char) * (size+1));
        fread(source, sizeof(char), size, fp);
        source[size] = '\0';
        fclose(fp);
        
        result = gaiOpenGLShaderCompileAttach(shader, source, type);
        GAI_FREE(source);
    }
    
    return result;
}

GAI_DEF b32
gaiOpenGLShaderLink(gaiShader *shader)
{
    b32 result = false;
    i32 programid = shader->id;
    
    glLinkProgram(programid);
    if(glGetError() != GL_INVALID_VALUE && glGetError() != GL_INVALID_OPERATION)
    {
        i32 status;
        glGetProgramStatus(programid, &status);
        if(status == GL_FALSE)
        {
            i32 length = 0;
            char error[1024];
            glGetProgramInfoLog(programid, 1024, &length, error);
            printf("error: %s\n", error);
            GAI_ASSERT(!"Shader link error");
            result = false;
        } else {
            glUseProgram(programid);
            result = true;
        }
    } else GAI_ASSERT(!"Shader link error");
    return result;
}

GAI_DEF i32
gaiOpenGLShaderUniformInit(gaiShader *shader, const char *name, i32 id = 0)
{
    GAI_ASSERT(shader);

    i32 uniform = glGetUniformLocation(shader->id, name);
    if(uniform == -1) return 0;
    shader->uniforms[id] = uniform;
    return 1;
}
