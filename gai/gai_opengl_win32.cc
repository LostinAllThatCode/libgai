
GAI_DEF void*
gaiOpenGLGetProcAddress(const char *proc)
{
    return wglGetProcAddress(proc);
}

void APIENTRY
gaiOpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
    SYSTEMTIME time = {};
    GetLocalTime(&time);
    printf("[%.4i/%.2i/%.2i:%.2i:%.2i:%.2i.%.3i ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               { printf("source: gl_debug_source_api, "); } break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     { printf("source: gl_debug_source_window_system, "); } break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   { printf("source: gl_debug_source_shader_compiler, "); } break;
        case GL_DEBUG_SOURCE_APPLICATION:       { printf("source: gl_debug_source_libgai, "); } break;
        default:                                { printf("source: gl_debug_source_third_party, "); } break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:                { printf("type: error, "); } break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  { printf("type: deprecated behaviour, "); } break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   { printf("type: undefined behaviour, "); } break;
        case GL_DEBUG_TYPE_PORTABILITY:          { printf("type: portability, "); } break;
        case GL_DEBUG_TYPE_PERFORMANCE:          { printf("type: performance, "); } break;
        case GL_DEBUG_TYPE_OTHER:                { printf("type: other, "); } break;
        case GL_DEBUG_TYPE_MARKER:               { printf("type: marker, "); } break;
        case GL_DEBUG_TYPE_PUSH_GROUP:           { printf("type: push group, "); } break;
        case GL_DEBUG_TYPE_POP_GROUP:            { printf("type: pop group, "); } break;
        default:                                 { printf("type: n/a, "); } break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            { printf("severity: high, "); } break;
        case GL_DEBUG_SEVERITY_MEDIUM:          { printf("severity: medium, "); } break;
        case GL_DEBUG_SEVERITY_LOW:             { printf("severity: low, "); } break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    { printf("severity: notification, "); } break;
        default:                                { printf("severity: n/a, "); } break;
    }

    printf("id: %i ]\n - %s\n", id, message);
}

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
    gai_assert(window);
    SwapBuffers(window->platform.hDC);
}

i32
gaiOpenGLCheckMSAASupport(gaiWindow *window, i32 samples)
{
    if (gaiOpenGLIsSupported("WGL_ARB_multisample"))
    {
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
        if (!wglChoosePixelFormatARB) return 0;

        HDC     hDC = window->platform.hDC;
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
            wglSwapInterval(vsync);
            /*
            int swap_interval = gaiOpenGLGetSwapInterval();
            if (swap_interval != vsync) 
            */
        }
    }
}

i32
gaiOpenGLCreateDummy(gaiWindow *window, PIXELFORMATDESCRIPTOR *pfd, const char *title = "gaiOpenGLDummyCtx", const char *wndclass = "gaiOpenGLDummyContextCreate",
                     i32 width = -1, i32 height = -1, i32 x = -1, i32 y = -1, u32 pf = 0)
{
    gai_assert(window);

    i32 ext[] = { gaiWindowFlagsStyle, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN };
    if (!gaiWindowCreate(window, title, width, height, x, y, wndclass, gaiWindowTypeDefault, ext, gai_array_length(ext)))
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
        pf = ChoosePixelFormat(hdc, pfd);
        if (!pf)
        {
            gaiWindowQuit(window);
            return 0;
        }
    }
    SetPixelFormat(hdc, pf, pfd);

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
                         PIXELFORMATDESCRIPTOR *pfd, int *attribs, b32 vsync, i32 multisample, b32 debug,
                         const char *wndclass)
{
    gai_assert(window);

    if (!gaiOpenGLCreateDummy(window, pfd)) return 0;
    if (!gaiOpenGLIsSupported("WGL_ARB_create_context"))
    {
        HGLRC old_glctx = wglGetCurrentContext();
        wglDeleteContext(old_glctx);
        gaiWindowQuit(window);
        gaiOpenGLCreateDummy(window, pfd, title, wndclass, width, height, x, y, 0);
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
        if (multisample > 0) pf = gaiOpenGLCheckMSAASupport(window, multisample);

        HGLRC old_glctx     = wglGetCurrentContext();
        wglDeleteContext(old_glctx);
        gaiWindowQuit(window);
        gaiOpenGLCreateDummy(window, pfd, title, wndclass, width, height, x, y, pf);

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

    if (debug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback( (GLDEBUGPROC) gaiOpenGLDebugCallback, 0);
    }

    window->flags = gaiWindowTypeOpenGL;
    gaiWindowShow(window, SW_SHOW);
    return 1;
}

i32
gaiOpenGLCreateContext(gaiWindow *window, const char *title, const char *wndclass, i32 width, i32 height, i32 x, i32 y, i32 *ext, i32 count)
{
    gai_assert(window);

    b32 vsync         = false;
    BYTE minor        = 0;
    BYTE major        = 0;
    BYTE msaa         = 0;
    BYTE color_bits   = 32;
    BYTE depth_bits   = 24;
    BYTE stencil_bits = 8;
    b32 fullscreen    = false;
    b32 debug         = false;

    if (ext)
    {
        for (i32 i = 0; i < count; i += 2)
        {
            switch (ext[i])
            {
                case gaiOpenGLFlagsVSYNC:           { vsync         = ext[i+1]; } break;
                case gaiOpenGLFlagsMajor:           { major         = ext[i+1]; } break;
                case gaiOpenGLFlagsMinor:           { minor         = ext[i+1]; } break;
                case gaiOpenGLFlagsMSAA:            { msaa          = ext[i+1]; } break;
                case gaiOpenGLFlagsDebug:           { debug         = ext[i+1]; } break;
                case gaiOpenGLFlagsFullscreen:      { fullscreen    = ext[i+1]; } break;
                case gaiOpenGLFlagsColorBits:       { color_bits    = ext[i+1]; } break;
                case gaiOpenGLFlagsDepthBits:       { depth_bits    = ext[i+1]; } break;
                case gaiOpenGLFlagsStencilBits:     { stencil_bits  = ext[i+1]; } break;
            }
        }
    }
    if (!wndclass) wndclass = GAI_OPENGL_UUID;
    u8 debug_flag = (debug == 1 ? WGL_CONTEXT_DEBUG_BIT_ARB : 0);

    i32 attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB,  major,
        WGL_CONTEXT_MINOR_VERSION_ARB,  minor,
        WGL_CONTEXT_FLAGS_ARB,          debug_flag,
        WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, color_bits, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, depth_bits, stencil_bits,
        0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    if(gaiOpenGLCreateContextEx(window, title, width, height, x, y, &pfd, attribs, vsync, msaa, debug, wndclass))
    {
        if(fullscreen) gaiWindowToggleFullscreen(window);
        return 1;
    }
    return 0;
}

i32
gaiOpenGLDestroyContext(void)
{
    HGLRC context = wglGetCurrentContext();
    if(context) return wglDeleteContext(context);
    return -1;
}
