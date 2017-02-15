#define GAI_OPENGL_WIN32_DEFAULT_CLASSNAME "5a11ddb6-bccc-49a2-a2ca-0cf1223d70a8"

typedef int   (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC)    (HDC hdc, const int *piAttribIList, const float *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef int   (WINAPI * PFNWGLSWAPINTERVALEXTPROC)         (int interval);
typedef int   (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC)      (void);


void APIENTRY
gaiOpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
    SYSTEMTIME time = {};
    GetLocalTime(&time);

    switch(source)
    {
        case GL_DEBUG_SOURCE_API: { printf("gl_debug_source_api"); } break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: { printf("gl_debug_source_window_system"); } break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: { printf("gl_debug_source_shader_compiler"); } break;
        case GL_DEBUG_SOURCE_APPLICATION: { printf("gl_debug_source_libgai"); } break;
        default: { printf("gl_debug_source_third_party"); } break;
    }

    printf(":%i,%i,%i ", type, id, severity);
    printf("[%.4i/%.2i/%.2i:%.2i:%.2i:%.2i.%.3i]", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    printf(" %s\n", message);
}

#ifdef GAI_OPENGL_DEBUG
static char _DEBUG_BUFFER[4096] = {};
#define gaiOpenGLDebugMessage(id, ...) \
    snprintf(_DEBUG_BUFFER, 4096, __VA_ARGS__); \
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, (id), GL_DEBUG_SEVERITY_HIGH, strlen(_DEBUG_BUFFER), _DEBUG_BUFFER)
#else
    #define gaiOpenGLDebugMessage
#endif

b32
gaiOpenGLIsSupported(const char *extension)
{
    const size_t extlen = strlen(extension);
    const char *supported = 0;

    // Try To Use wglGetExtensionStringARB On Current DC, If Possible
    PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
    if (wglGetExtString)
    {
        supported = ((char*(__stdcall*)(HDC)) wglGetExtString)(wglGetCurrentDC());
    }

    // If That Failed, Try Standard Opengl Extensions String
    if (!supported)
        supported = (char*) glGetString(GL_EXTENSIONS);

    // If That Failed Too, Must Be No Extensions Supported
    if (!supported) return false;

    // Begin Examination At Start Of String, Increment By 1 On False Match
    for (const char* p = supported; ; p++)
    {
        // Advance p Up To The Next Possible Match
        p = strstr(p, extension);
        if (!p) return false;
        if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' ')) return true;
    }
}

void
gaiOpenGLSwapBuffers(gaiWindow *window)
{
    GAI_ASSERT(window);
    SwapBuffers(window->platform.ctx);
}

i32
gaiOpenGLCheckMSAASupport(gaiWindow *window, PIXELFORMATDESCRIPTOR *pfd, i32 samples)
{
    if (gaiOpenGLIsSupported("WGL_ARB_multisample"))
    {
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
        if (!wglChoosePixelFormatARB) return 0;

        HDC     hDC = window->platform.ctx;
        i32     pixelFormat;
        i32     valid;
        u32     numFormats;
        r32     fAttributes[] = {0, 0};

        i32 iAttributes[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 24,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 16,
            WGL_STENCIL_BITS_ARB, 0,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, samples,
            0, 0
        };

        valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
        if (valid && numFormats >= 1)
        {
            return pixelFormat;
        }
    }
    return 0;
}


b32
gaiOpenGLGetSwapInterval()
{
    if (gaiOpenGLIsSupported("WGL_EXT_swap_control"))
    {
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC) wglGetProcAddress("wglGetSwapIntervalEXT");
        if (wglGetSwapInterval)
        {
            return wglGetSwapInterval();
        }
    }
    return false;
}

void
gaiOpenGLSetSwapInterval(b32 vsync)
{
    if (gaiOpenGLIsSupported("WGL_EXT_swap_control"))
    {
        PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapInterval)
        {
            int swap_interval = gaiOpenGLGetSwapInterval();
            if (swap_interval != vsync) wglSwapInterval(vsync);
        }
    }
}

i32
gaiOpenGLCreateDummy(gaiWindow *window, const char *title = "gaiOpenGLDummyCtx", const char *wndclass = "gaiOpenGLDummyContextCreate",
                     i32 width = 1, i32 height = 1, i32 x = 0, i32 y = 0, u32 pf = 0)
{
    GAI_ASSERT(window);

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 24, 8,
        0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    if (!gaiWindowCreateEx(window, title, wndclass, dwStyle, x, y, width, height, 0, 0, GetModuleHandle(0), 0, 0))
    {
        return 0;
    }

    HDC hdc = GetDC(window->platform.hWnd);
    if (!hdc)
    {
        gaiWindowQuit(window);
        return 0;
    }


    if (!pf)
    {
        pf = ChoosePixelFormat(hdc, &pfd);
        if (!pf)
        {
            gaiWindowQuit(window);
            return 0;
        }
    }
    SetPixelFormat(hdc, pf, &pfd);

    HGLRC oglrc = wglCreateContext(hdc);
    if (!oglrc)
    {
        gaiWindowQuit(window);
        return 0;
    }

    if (!wglMakeCurrent(hdc, oglrc))
    {
        wglDeleteContext(oglrc);
        gaiWindowQuit(window);
        return 0;
    }
    else
    {
        return 1;
    }
}

i32
gaiOpenGLCreateContextEx(gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y,
                         PIXELFORMATDESCRIPTOR *pfd, int *attribs, b32 vsync, i32 multisample,
                         const char *wndclass = GAI_OPENGL_WIN32_DEFAULT_CLASSNAME)
{
    GAI_ASSERT(window);

    if (!gaiOpenGLCreateDummy(window)) return 0;
    if (!gaiOpenGLIsSupported("WGL_ARB_create_context"))
    {
        HGLRC old_glctx = wglGetCurrentContext();
        wglDeleteContext(old_glctx);
        gaiWindowQuit(window);
        gaiOpenGLCreateDummy(window, title, wndclass, width, height, x, y, 0);
        return 1;
    }

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB)
    {
        if (!attribs[1] && !attribs[3])
        {
            /* Get major and minor supported opengl version, if not specified */
            glGetIntegerv(GL_MAJOR_VERSION, attribs + 1);
            glGetIntegerv(GL_MINOR_VERSION, attribs + 3);
        }

        i32 pf = 0;
        if (multisample > 0) pf = gaiOpenGLCheckMSAASupport(window, pfd, multisample);

        HGLRC old_glctx     = wglGetCurrentContext();
        wglDeleteContext(old_glctx);
        gaiWindowQuit(window);
        gaiOpenGLCreateDummy(window, title, wndclass, width, height, x, y, pf);

        old_glctx        = wglGetCurrentContext();
        HDC   hdc        = wglGetCurrentDC();
        HGLRC new_oglctx = wglCreateContextAttribsARB(hdc, 0, attribs);
        if (new_oglctx)
        {
            wglMakeCurrent(0, 0);
            wglDeleteContext(old_glctx);
            wglMakeCurrent(hdc, new_oglctx);
            gaiOpenGLSetSwapInterval(vsync);
        }
    }

    gaiOpenGLInitialzeFunctions();

    #ifdef GAI_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback((GLDEBUGPROC)gaiOpenGLDebugCallback, 0);
    #endif

    gaiWindowShow(window, SW_SHOW);
    return 1;
}

i32
gaiOpenGLCreateContext(gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y, i32 major, i32 minor, b32 vsync, i32 multisample, u8 color_bits, u8 depth_bits, u8 stencil_bits)
{
    GAI_ASSERT(window);

    i32 attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor,
        WGL_CONTEXT_FLAGS_ARB, 0
        #ifdef GAI_OPENGL_DEBUG
        |WGL_CONTEXT_DEBUG_BIT_ARB
        #endif
        ,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, color_bits, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, depth_bits, stencil_bits,
        0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    return gaiOpenGLCreateContextEx(window, title, width, height, x, y, &pfd, attribs, vsync, multisample);
}
