GAI_DEF i32
gaiOpenGLShaderCreate(gaiShader *shader)
{
    GAI_ASSERT(shader);
    shader->id = glCreateProgram();
    return (shader->id != 0);
}

GAI_DEF void
gaiOpenGLShaderFree(gaiShader *shader)
{
    GAI_ASSERT(shader);
    glDeleteProgram(shader->id);
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
        glDeleteShader(id);
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

            u32 shaders[12] = {};
            i32 attached = 0;
            glGetAttachedShaders(shader->id, sizeof(shaders) / sizeof(shaders[0]), &attached, shaders);
            for(i32 i=0; i < attached; i++)
            {
                glDetachShader(shader->id, shaders[i]);
            }
            
            glUseProgram(programid);
            result = true;
        }
    } else GAI_ASSERT(!"Shader link error");
    return result;
}

GAI_DEF i32
gaiOpenGLShaderGetUniform(gaiShader *shader, const char *name)
{
    return glGetUniformLocation(shader->id, name);
}
