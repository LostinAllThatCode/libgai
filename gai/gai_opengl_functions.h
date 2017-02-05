/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_OPENGL_FUNCTIONS_H_

#include <gai_types.h>
#include <gai_utils.h>

#define GAI_OPENGL_EXTENSIONS_CORE                                      \
    GAI_OPENGL_FUNC_DEF(GENERATEMIPMAP           , GenerateMipmap      ) \
    GAI_OPENGL_FUNC_DEF(GENVERTEXARRAYS          , GenVertexArrays     ) \
    GAI_OPENGL_FUNC_DEF(DELETEVERTEXARRAYS       , DeleteVertexArrays  ) \
    GAI_OPENGL_FUNC_DEF(BINDVERTEXARRAY          , BindVertexArray     ) \
    GAI_OPENGL_FUNC_DEF(GENFRAMEBUFFERS          , GenFramebuffers     ) \
    GAI_OPENGL_FUNC_DEF(DELETEFRAMEBUFFERS       , DeleteFramebuffers  ) \
    GAI_OPENGL_FUNC_DEF(BINDFRAMEBUFFER          , BindFramebuffer     ) \
    GAI_OPENGL_FUNC_DEF(FRAMEBUFFERTEXTURE2D     , FramebufferTexture2D)

#define GAI_OPENGL_EXTENSIONS_DEF_OR_ARB                                \
    GAI_OPENGL_FUNC_DEF(BINDATTRIBLOCATION       , BindAttribLocation  ) \
    GAI_OPENGL_FUNC_DEF(COMPILESHADER            , CompileShader       ) \
    GAI_OPENGL_FUNC_DEF(SHADERSOURCE             , ShaderSource        ) \
    GAI_OPENGL_FUNC_DEF(LINKPROGRAM              , LinkProgram         ) \
    GAI_OPENGL_FUNC_DEF(GETUNIFORMLOCATION       , GetUniformLocation  ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM1F                , Uniform1f           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM2F                , Uniform2f           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM3F                , Uniform3f           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM4F                , Uniform4f           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM1I                , Uniform1i           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM2I                , Uniform2i           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM3I                , Uniform3i           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM4I                , Uniform4i           ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM1FV               , Uniform1fv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM2FV               , Uniform2fv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM3FV               , Uniform3fv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM4FV               , Uniform4fv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM1IV               , Uniform1iv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM2IV               , Uniform2iv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM3IV               , Uniform3iv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORM4IV               , Uniform4iv          ) \
    GAI_OPENGL_FUNC_DEF(UNIFORMMATRIX2FV         , UniformMatrix2fv    ) \
    GAI_OPENGL_FUNC_DEF(UNIFORMMATRIX3FV         , UniformMatrix3fv    ) \
    GAI_OPENGL_FUNC_DEF(UNIFORMMATRIX3FV         , UniformMatrix4fv    ) \
    GAI_OPENGL_FUNC_DEF(ENABLEVERTEXATTRIBARRAY  , EnableVertexAttribArray ) \
    GAI_OPENGL_FUNC_DEF(DISABLEVERTEXATTRIBARRAY , DisableVertexAttribArray ) \
    GAI_OPENGL_FUNC_DEF(VERTEXATTRIBPOINTER      , VertexAttribPointer ) \
    GAI_OPENGL_FUNC_DEF(GENBUFFERS               , GenBuffers          ) \
    GAI_OPENGL_FUNC_DEF(DELETEBUFFERS            , DeleteBuffers       ) \
    GAI_OPENGL_FUNC_DEF(BINDBUFFER               , BindBuffer          ) \
    GAI_OPENGL_FUNC_DEF(BUFFERDATA               , BufferData          ) \
    GAI_OPENGL_FUNC_DEF(BUFFERSUBDATA            , BufferSubData       ) \
    GAI_OPENGL_FUNC_DEF(MAPBUFFER                , MapBuffer           ) \
    GAI_OPENGL_FUNC_DEF(UNMAPBUFFER              , UnmapBuffer         ) \
    GAI_OPENGL_FUNC_DEF(ACTIVETEXTURE            , ActiveTexture       )

#ifdef GAI_OPENGL_EXTENSIONS_USE_ARB
#   define GAI_OPENGL_EXTENSIONS_DYNAMIC                                \
    GAI_OPENGL_FUNC_DEF(ATTACHOBJECT        , AttachObject        )     \
    GAI_OPENGL_FUNC_DEF(CREATEPROGRAMOBJECT , CreateProgramObject )     \
    GAI_OPENGL_FUNC_DEF(CREATESHADEROBJECT  , CreateShaderObject  )     \
    GAI_OPENGL_FUNC_DEF(DELETEOBJECT        , DeleteObject        )     \
    GAI_OPENGL_FUNC_DEF(DETACHOBJECT        , DetachObject        )     \
    GAI_OPENGL_FUNC_DEF(USEPROGRAMOBJECT    , UseProgramObject    )     \
    GAI_OPENGL_FUNC_DEF(GETATTACHEDOBJECTS  , GetAttachedObjects  )     \
    GAI_OPENGL_FUNC_DEF(GETOBJECTPARAMETERIV, GetObjectParameteriv)     \
    GAI_OPENGL_FUNC_DEF(GETINFOLOG          , GetInfoLog          )
#else
#   define GAI_OPENGL_EXTENSIONS_DYNAMIC                                \
    GAI_OPENGL_FUNC_DEF(ATTACHSHADER        , AttachShader        )     \
    GAI_OPENGL_FUNC_DEF(CREATEPROGRAM       , CreateProgram       )     \
    GAI_OPENGL_FUNC_DEF(DELETEPROGRAM       , DeleteProgram       )     \
    GAI_OPENGL_FUNC_DEF(CREATESHADER        , CreateShader        )     \
    GAI_OPENGL_FUNC_DEF(DELETESHADER        , DeleteShader        )     \
    GAI_OPENGL_FUNC_DEF(DETACHSHADER        , DetachShader        )     \
    GAI_OPENGL_FUNC_DEF(USEPROGRAM          , UseProgram          )     \
    GAI_OPENGL_FUNC_DEF(GETATTACHEDSHADERS  , GetAttachedShaders  )     \
    GAI_OPENGL_FUNC_DEF(GETSHADERIV         , GetShaderiv         )     \
    GAI_OPENGL_FUNC_DEF(GETSHADERINFOLOG    , GetShaderInfoLog    )     \
    GAI_OPENGL_FUNC_DEF(GETPROGRAMIV        , GetProgramiv        )     \
    GAI_OPENGL_FUNC_DEF(GETPROGRAMINFOLOG   , GetProgramInfoLog   )
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define GAI_OPENGL_FUNC_DEF(x,y) GAI_API PFNGL##x##PROC __gl##y;
GAI_OPENGL_EXTENSIONS_CORE

#ifdef GAI_OPENGL_EXTENSIONS_USE_ARB
    #undef  GAI_OPENGL_FUNC_DEF
    #define GAI_OPENGL_FUNC_DEF(x,y) GAI_API PFNGL##x##ARBPROC __gl##y##ARB;
#endif

GAI_OPENGL_EXTENSIONS_DEF_OR_ARB
GAI_OPENGL_EXTENSIONS_DYNAMIC
#undef GAI_OPENGL_FUNC_DEF

#ifdef __cplusplus
}
#endif

void
gaiOpenGLInitialzeFunctions()
{
    static b32 initialized = false;
    if (initialized) return;
    initialized = true;

#define GAI_OPENGL_FUNC_DEF(x,y) __gl##y = (PFNGL##x##PROC) wglGetProcAddress("gl" #y); if (!__gl##y) printf("error mapping %s\n", "gl" #y);
    GAI_OPENGL_EXTENSIONS_CORE

    #ifdef GAI_OPENGL_EXTENSIONS_USE_ARB
#undef  GAI_OPENGL_FUNC_DEF
#define GAI_OPENGL_FUNC_DEF(x,y) __gl##y##ARB = (PFNGL##x##ARBPROC) wglGetProcAddress("gl" #y "ARB"); if (!__gl##y) printf("error mapping %s\n", "gl" #y "ARB");
    #endif

    GAI_OPENGL_EXTENSIONS_DEF_OR_ARB
    GAI_OPENGL_EXTENSIONS_DYNAMIC
#undef GAI_OPENGL_FUNC_DEF
}
#undef GAI_OPENGL_EXTENSIONS_DYNAMIC

// Remapping glXXX commands since there are different versions (ARB/EXT)
// Only the new functions for shader support and vbo/ibo/vao and some others
#define glGenerateMipmap             __glGenerateMipmap
#define glGenVertexArrays            __glGenVertexArrays
#define glDeleteVertexArrays         __glDeleteVertexArrays
#define glBindVertexArray            __glBindVertexArray
#define glGenFramebuffers            __glGenFramebuffers
#define glDeleteFramebuffers         __glDeleteFramebuffers
#define glBindFramebuffer            __glBindFramebuffer
#define glFramebufferTexture2D       __glFramebufferTexture2D

#ifdef GAI_OPENGL_EXTENSIONS_USE_ARB
    #define glCreateShader               __glCreateShaderObjectARB
    #define glDeleteShader               __glDeleteObjectARB
    #define glAttachShader               __glAttachObjectARB
    #define glDetachShader               __glDetachObjectARB
    #define glShaderSource               __glShaderSourceARB
    #define glCompileShader              __glCompileShaderARB
    #define glGetShaderStatus(a,b)       __glGetObjectParameterivARB(a, GL_OBJECT_COMPILE_STATUS_ARB, b)
    #define glGetShaderInfoLog           __glGetInfoLogARB
    #define glCreateProgram              __glCreateProgramObjectARB
    #define glDeleteProgram              __glDeleteObjectARB
    #define glLinkProgram                __glLinkProgramARB
    #define glGetProgramStatus(a,b)      __glGetObjectParameterivARB(a, GL_OBJECT_LINK_STATUS_ARB, b)
    #define glGetProgramInfoLog          __glGetInfoLogARB
    #define glGetAttachedShaders         __glGetAttachedObjectsARB
    #define glBindAttribLocation         __glBindAttribLocationARB
    #define glGetUniformLocation         __glGetUniformLocationARB
    #define glUseProgram                 __glUseProgramObjectARB
    #define glUniform1f                  __glUniform1fARB
    #define glUniform2f                  __glUniform2fARB
    #define glUniform3f                  __glUniform3fARB
    #define glUniform4f                  __glUniform4fARB
    #define glUniform1i                  __glUniform1iARB
    #define glUniform2i                  __glUniform2iARB
    #define glUniform3i                  __glUniform3iARB
    #define glUniform4i                  __glUniform4iARB
    #define glUniform1fv                 __glUniform1fvARB
    #define glUniform2fv                 __glUniform2fvARB
    #define glUniform3fv                 __glUniform3fvARB
    #define glUniform4fv                 __glUniform4fvARB
    #define glUniform1iv                 __glUniform1ivARB
    #define glUniform2iv                 __glUniform2ivARB
    #define glUniform3iv                 __glUniform3ivARB
    #define glUniform4iv                 __glUniform4ivARB
    #define glUniformMatrix2fv           __glUniformMatrix2fvARB
    #define glUniformMatrix3fv           __glUniformMatrix3fvARB
    #define glUniformMatrix4fv           __glUniformMatrix4fvARB
    #define glGenBuffers                 __glGenBuffersARB
    #define glDeleteBuffers              __glDeleteBuffersARB
    #define glBufferData                 __glBufferDataARB
    #define glBufferSubData              __glBufferSubDataARB
    #define glBindBuffer                 __glBindBufferARB
    #define glMapBuffer                  __glMapBufferARB
    #define glUnmapBuffer                __glUnmapBufferARB
    #define glEnableVertexAttribArray    __glEnableVertexAttribArrayARB
    #define glDisableVertexAttribArray   __glDisableVertexAttribArrayARB
    #define glVertexAttribPointer        __glVertexAttribPointerARB
    #define glActiveTexture              __glActiveTextureARB
#else
    #define glCreateShader               __glCreateShader
    #define glDeleteShader               __glDeleteShader
    #define glAttachShader               __glAttachShader
    #define glDetachShader               __glDetachShader
    #define glShaderSource               __glShaderSource
    #define glCompileShader              __glCompileShader
    #define glGetShaderStatus(a,b)       __glGetShaderiv(a, GL_COMPILE_STATUS, b)
    #define glGetShaderInfoLog           __glGetShaderInfoLog
    #define glCreateProgram              __glCreateProgram
    #define glDeleteProgram              __glDeleteProgram
    #define glLinkProgram                __glLinkProgram
    #define glGetProgramStatus(a,b)      __glGetProgramiv(a, GL_LINK_STATUS, b)
    #define glGetProgramInfoLog          __glGetProgramInfoLog
    #define glGetAttachedShaders         __glGetAttachedShaders
    #define glBindAttribLocation         __glBindAttribLocation
    #define glUseProgram                 __glUseProgram
    #define glGetUniformLocation         __glGetUniformLocation
    #define glUniform1f                  __glUniform1f
    #define glUniform2f                  __glUniform2f
    #define glUniform3f                  __glUniform3f
    #define glUniform4f                  __glUniform4f
    #define glUniform1i                  __glUniform1i
    #define glUniform2i                  __glUniform2i
    #define glUniform3i                  __glUniform3i
    #define glUniform4i                  __glUniform4i
    #define glUniform1fv                 __glUniform1fv
    #define glUniform2fv                 __glUniform2fv
    #define glUniform3fv                 __glUniform3fv
    #define glUniform4fv                 __glUniform4fv
    #define glUniform1iv                 __glUniform1iv
    #define glUniform2iv                 __glUniform2iv
    #define glUniform3iv                 __glUniform3iv
    #define glUniform4iv                 __glUniform4iv
    #define glUniformMatrix2fv           __glUniformMatrix2fv
    #define glUniformMatrix3fv           __glUniformMatrix3fv
    #define glUniformMatrix4fv           __glUniformMatrix4fv
    #define glGenBuffers                 __glGenBuffers
    #define glDeleteBuffers              __glDeleteBuffers
    #define glBufferData                 __glBufferData
    #define glBufferSubData              __glBufferSubData
    #define glBindBuffer                 __glBindBuffer
    #define glMapBuffer                  __glMapBuffer
    #define glUnmapBuffer                __glUnmapBuffer
    #define glEnableVertexAttribArray    __glEnableVertexAttribArray
    #define glDisableVertexAttribArray   __glDisableVertexAttribArray
    #define glVertexAttribPointer        __glVertexAttribPointer
    #define glActiveTexture              __glActiveTexture
#endif

#define _GAI_OPENGL_FUNCTIONS_H_
#endif