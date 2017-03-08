#ifndef GAI_XWINDOW_H

#include "gai_types.h"
#include "gai_utils.h"

#if _WIN32
	#include <windows.h>
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
#elif __linux__
	#include <X11/Xlib.h>
#elif __APPLE__
#else
#endif

struct gai_input_key
{
	b32 is_pressed;
	b32 was_pressed;
	b32 was_released;
};

enum gai_xwnd_renderer_enum
{
	XWND_RENDERER_GDI, XWND_RENDERER_DX10, XWND_RENDERER_DX11, XWND_RENDERER_DX12,
	XWND_RENDERER_OPENGL, XWND_RENDERER_VULCAN,
	XWND_RENDERER_MAX
};

enum gai_xwnd_renderer_attrib_enum
{
	XWND_ATTRIB_FULLSCREEN,
	XWND_ATTRIB_VSYNC,

	XWND_ATTRIB_OPENGL_MINOR,
	XWND_ATTRIB_OPENGL_MAJOR,
	XWND_ATTRIB_OPENGL_DEBUG,

	XWND_ATTRIB_MAX,
};

struct gai_xwnd
{
	const char *title;

	struct
	{
		r32 seconds;
		r32 millis;
		r32 nanos;
	} dt;

	struct
	{
		i32 width;
		i32 height;
		i32 x;
		i32 y;
	} info;

	struct
	{
		gai_input_key keys[256];
		i32 x, y, dx, dy, wheel, dwheel;
	} input;

	struct
	{
		#if _WIN32
		HWND            	hwnd;
		HDC             	hdc;
		HINSTANCE       	instance;
		#elif __linux__
		Display *display;
		Visual  *visual;
		Window  *window;
		#else

		#endif
	} platform;

	gai_xwnd_renderer_enum renderer;
};

#ifdef __cplusplus
extern "C" {
#endif

extern i32  	gaiXWindow        			(gai_xwnd *window, const char *title = "default window title", i32 width = -1, i32 height = -1, i32 x = -1, i32 y = -1, gai_xwnd_renderer_enum renderer = XWND_RENDERER_GDI, const char *classname = "crossplatform_xwindow_framework");
extern void 	gaiXWindowSetTitle			(gai_xwnd *window, const char *title);
extern i32  	gaiXWindowUpdate			(gai_xwnd *window);
extern void 	gaiXWindowRender			(gai_xwnd *window);
extern i32  	gaiXWindowSetVSYNC			(gai_xwnd *window, b32 state);
extern void* 	gaiXWindowGetProc			(gai_xwnd *window, const char *name);

#ifdef __cplusplus
}
#endif

#ifdef XWND_IMPLEMENTATION

#ifdef XWND_VERBOSE_DEBUG
	#define XWND_MSG(fmt, ...) printf("%s: "fmt, __FUNCTION__, __VA_ARGS__)
#else
	#define XWND_MSG
#endif


//	[GENERAL PLATFORM INDEPENDEND LAYER]
//
//
//
//

#include <gl\gl.h>
typedef char GLchar;
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83

#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_DEBUG_LOGGED_MESSAGES          0x9145
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
typedef void (APIENTRY  *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#define gaiXWindowFNWrapper \
	gaiXWindowFN(, attach_shader_fn,  				void, 	 AttachShader, GLuint, GLuint) \
	gaiXWindowFN(, compile_shader_fn, 				void, 	 CompileShader, GLuint) \
	gaiXWindowFN(, create_program_fn, 				GLuint,  CreateProgram, void) \
	gaiXWindowFN(, create_shader_fn,  				GLuint,  CreateShader, GLenum) \
	gaiXWindowFN(, delete_program_fn, 				void,    DeleteProgram, GLuint) \
	gaiXWindowFN(, delete_shader_fn,  				void,    DeleteShader, GLuint) \
	gaiXWindowFN(, detach_shader_fn,  			    void,    DetachShader, GLuint, GLuint) \
	gaiXWindowFN(, disable_vertex_attrib_array_fn,  void,    DisableVertexAttribArray, GLuint) \
	gaiXWindowFN(, enable_vertex_attrib_array_fn,   void,    EnableVertexAttribArray, GLuint) \
	gaiXWindowFN(, get_program_iv_fn,			    void,    GetProgramiv, GLuint, GLenum, GLint*) \
	gaiXWindowFN(, get_program_info_log_fn,			void,    GetProgramInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	gaiXWindowFN(, get_shader_info_log_fn,			void,    GetShaderInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	gaiXWindowFN(, get_shader_source_fn, 			void,    GetShaderSource, GLuint, GLsizei, GLsizei*, GLchar *) \
	gaiXWindowFN(, get_uniform_location_fn,         GLint,   GetUniformLocation, GLuint, const GLchar*) \
	gaiXWindowFN(, link_program_fn,					void,    LinkProgram, GLuint) \
	gaiXWindowFN(, shader_source_fn,                void,    ShaderSource, GLuint, GLsizei, GLchar**, GLint*) \
	gaiXWindowFN(, use_program, 					void,    UseProgram, GLuint) \
	gaiXWindowFN(, uniform_1f_fn,					void,    Uniform1f, GLint, GLfloat) \
	gaiXWindowFN(, uniform_2f_fn,					void,    Uniform2f, GLint, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_3f_fn,					void,    Uniform3f, GLint, GLfloat, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_4f_fn,					void,    Uniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_1i_fn,					void,    Uniform1i, GLint, GLint) \
	gaiXWindowFN(, uniform_2i_fn,					void,    Uniform2i, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_3i_fn, 					void,    Uniform3i, GLint, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_4i_fn, 					void,    Uniform4i, GLint, GLint, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_1fv_fn,					void,    Uniform1fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_2fv_fn,					void,    Uniform2fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_3fv_fn,					void,    Uniform3fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_4fv_fn,					void,    Uniform4fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_1iv_fn,					void,    Uniform1iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_2iv_fn,					void,    Uniform2iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_3iv_fn,					void,    Uniform3iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_4iv_fn,					void,    Uniform4iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_matrix_2fv_fn, 			void,    UniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, uniform_matrix_3fv_fn,			void,    UniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, uniform_matrix_4fv_fn,			void,    UniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, validate_program_fn, 			void,	 ValidateProgram ,GLuint) \
	gaiXWindowFN(, vertex_attrib_pointer_fn,   		void,    VertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *) \
	gaiXWindowFN(, debug_message_callback_fn, 		void,    DebugMessageCallback, GLDEBUGPROC, void *)

#define gaiXWindowFN(ext, def, a, b, ...) typedef a (gl_##def) (__VA_ARGS__);
gaiXWindowFNWrapper

#undef gaiXWindowFN
#define gaiXWindowFN(ext, def, a, b, ...) gl_##def *gl##b;
gaiXWindowFNWrapper

#undef gaiXWindowFN

#define gaiXWindowFN(ext, def, a, b, ...) \
	gl##b = (gl_##def *) gaiXWindowGetProc(window, "gl"#b#ext); \
	if (!gl##b) { \
		XWND_MSG("failed get proc address for \"gl"#b#ext"\"\nExiting. At least OpenGL 3.1 needs to be supported."); \
		gai_assert(!"gaiXWindowGetProc failed."); \
	} \
	else { XWND_MSG("gl%s%s@0x%p\n",  #b, #ext, gl##b); }

GAI_DEF void
gaiOpenGLGetFunctions(gai_xwnd *window)
{
	gaiXWindowFNWrapper
}

#undef gaiXWindowFN

GLuint
gaiOpenGLCreateProgram(char *version, char *vertex_code, char *fragment_code)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLchar *vertex_shader_code[] =
	{
		version,
		vertex_code,
	};
	glShaderSource(vertex_shader_id, gai_array_length(vertex_shader_code), vertex_shader_code, 0);
	glCompileShader(vertex_shader_id);

	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar *fragment_shader_code[] =
	{
		version,
		fragment_code,
	};
	glShaderSource(fragment_shader_id, gai_array_length(fragment_shader_code), fragment_shader_code, 0);
	glCompileShader(fragment_shader_id);

	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	glValidateProgram(program_id);
	GLint linked = false;
	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei ignored;
		char vertex_code_errors[4096];
		char fragment_code_errors[4096];
		char program_errors[4096];
		glGetShaderInfoLog(vertex_shader_id, sizeof(vertex_code_errors), &ignored, vertex_code_errors);
		glGetShaderInfoLog(fragment_shader_id, sizeof(fragment_code_errors), &ignored, fragment_code_errors);
		glGetProgramInfoLog(program_id, sizeof(program_errors), &ignored, program_errors);

		gai_assert(!"Shader validation failed");
	}

	return (program_id);
}


#ifdef _WIN32  // WIN32 LAYER FOR WINDOW MANAGEMENT

//#include <wglext.h>

//#include <glcorearb.h>
// OPENGL WIN32 LAYER

#define WGL_CONTEXT_DEBUG_BIT_ARB         			0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 		0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     			0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     			0x2092
#define WGL_CONTEXT_FLAGS_ARB             			0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB      			0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  			0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 	0x00000002

typedef HGLRC 	(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL 	(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int 	(WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);

void
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

GAI_DEF b32
gaiOpenGLIsSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = 0;
	
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtString)
	{
		supported = ((char*(__stdcall*)(HDC)) wglGetExtString)(wglGetCurrentDC());
	}

	if (!supported) supported = (char*) glGetString(GL_EXTENSIONS);
	if (!supported) return false;

	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);
		if (!p) return false;
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' ')) return true;
	}
} // This is just copy & paste from nehe prod. IIRC

GAI_DEF b32
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

GAI_DEF void
gaiOpenGLSetSwapInterval(b32 vsync)
{
	if (gaiOpenGLIsSupported("WGL_EXT_swap_control"))
	{
		PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapInterval)
		{
			wglSwapInterval(vsync);
		}
	}
}

GAI_DEF void *
gaiXWindowGetProc(gai_xwnd *window, const char *name)
{
	return wglGetProcAddress(name);
}

LRESULT
gaiXWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gai_xwnd *window = (gai_xwnd *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (window)
	{
		switch (uMsg)
		{
			case WM_SIZE:
			{
				window->info.width  = LOWORD(lParam);
				window->info.height = HIWORD(lParam);
				return 0;
			} break;
			case WM_MOVE:
			{
				window->info.x = (int)(short) LOWORD(lParam);
				window->info.y = (int)(short) HIWORD(lParam);
				return 0;
			} break;
			/*
			case WM_MOUSEMOVE:
			{
				i32 x = ((i16) lParam), y = ((i16)((lParam >> 16) & 0xFFFF));
			} break;
			*/
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GAI_DEF void
gaiXWindowSetTitle(gai_xwnd *window, const char *title)
{
	SetWindowText(window->platform.hwnd, title);
}

GAI_DEF void
gaiXWindowRender(gai_xwnd *window)
{
	SwapBuffers(window->platform.hdc);
}

GAI_DEF i32
gaiXWindowSetVSYNC(gai_xwnd *window, b32 state)
{
	gaiOpenGLSetSwapInterval(state);
	return gaiOpenGLGetSwapInterval();
}

GAI_DEF i32
gaiXWindow(gai_xwnd *window, const char *title, i32 width, i32 height, i32 x, i32 y, gai_xwnd_renderer_enum renderer, const char *classname)
{
	if (!ConvertThreadToFiber(0) ) return -1;

	gai_xwnd init = { title, {0}, {width, height, x, y}, {0}, {0, 0, GetModuleHandle(0)}, renderer};
	*window = init;

	WNDCLASSEXA wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = sizeof(gai_xwnd*);
	wnd_class.lpfnWndProc   = gaiXWindowProc;
	wnd_class.hInstance     = window->platform.instance;
	wnd_class.lpszClassName = classname;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassExA(&wnd_class))
	{
		gai_assert(!"Can't register class.");
		return -1;
	}

	DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	HWND hwnd 	= CreateWindowA(classname, title, style,
	                            (x == -1 ? CW_USEDEFAULT : x),
	                            (y == -1 ? CW_USEDEFAULT : y),
	                            (width == -1 ? CW_USEDEFAULT : width),
	                            (height == -1 ? CW_USEDEFAULT : height),
	                            0, 0, window->platform.instance, window);
	if (!hwnd)
	{
		gai_assert(!"Can't create window.");
		return -2;
	}

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window);
	HDC hdc = GetDC(hwnd);
	window->platform.hwnd = hwnd;
	window->platform.hdc  = hdc;

	if (renderer == XWND_RENDERER_OPENGL)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 24,
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 32, 0, 0,
			PFD_MAIN_PLANE,
			0, 0, 0, 0
		};

		i32 pf = ChoosePixelFormat(hdc, &pfd);
		if (!pf)
		{
			gai_assert(!"No valid pxiel format found.");
			DestroyWindow(hwnd);
			return -3;
		}

		if (!SetPixelFormat(hdc, pf, &pfd))
		{
			gai_assert(!"Set pixel format failed.");
			return -4;
		}

		HGLRC old_glctx = wglCreateContext(hdc);
		if (!old_glctx)
		{
			gai_assert(!"Can't create opengl context.");
			DestroyWindow(hwnd);
			return -5;
		}

		if (!wglMakeCurrent(hdc, old_glctx))
		{
			gai_assert(!"Activate opengl context failed..");
			wglDeleteContext(old_glctx);
			DestroyWindow(hwnd);
			return -6;
		}

		if (gaiOpenGLIsSupported("WGL_ARB_create_context"))
		{
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB)
			{
				i32 attribs[] =
				{
					#ifdef XWND_OPENGL_MAJOR
					WGL_CONTEXT_MAJOR_VERSION_ARB,  XWND_OPENGL_MAJOR,
					#else
					WGL_CONTEXT_MAJOR_VERSION_ARB,  0,
					#endif
					#ifdef XWND_OPENGL_MINOR
					WGL_CONTEXT_MINOR_VERSION_ARB,  XWND_OPENGL_MINOR,
					#else
					WGL_CONTEXT_MINOR_VERSION_ARB,  0,
					#endif
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
					#ifdef XWND_OPENGL_DEBUG
					| WGL_CONTEXT_DEBUG_BIT_ARB
					#endif
					,
					WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0
				};

				if (attribs[1] == 0) glGetIntegerv(GL_MAJOR_VERSION, attribs + 1);
				if (attribs[3] == 0) glGetIntegerv(GL_MINOR_VERSION, attribs + 3);

				HGLRC new_oglctx = wglCreateContextAttribsARB(hdc, 0, attribs);
				if (new_oglctx)
				{
					//XWND_MSG("OpenGL context created: %s, %s, %s, %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
					//XWND_MSG("OpenGL extensions:\n %s\n", glGetString(GL_EXTENSIONS));
					wglMakeCurrent(0, 0);
					wglDeleteContext(old_glctx);
					wglMakeCurrent(hdc, new_oglctx);
					gaiXWindowSetVSYNC(window, 1);
					gaiOpenGLGetFunctions(window);
					#ifdef XWND_OPENGL_DEBUG
					//glEnable(GL_DEBUG_OUTPUT);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback( (GLDEBUGPROC) gaiOpenGLDebugCallback, 0);
					#endif
				}
				else
				{
					XWND_MSG("Error: wglCreateContextAttribsARB failed to create modern context.");
				}
			}
			else
			{
				XWND_MSG("Error: wglGetProcAddress failed for wglCreateContextAttribsARB.");
			}
		}
		else
		{
			XWND_MSG("Warning: This graphics card does not support \"WGL_ARB_create_context\".");
		}
	}
	ShowWindow(hwnd, SW_SHOW);
	return 1;
}

GAI_DEF r32
gaiXWindowGetTime(gai_xwnd *window)
{
	static LARGE_INTEGER _frequency;
	static LARGE_INTEGER _ltframetime;

	static b32 initialized;
	if (!initialized)
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_ltframetime);
		initialized = true;
	}

	LARGE_INTEGER now, elapsed;
	QueryPerformanceCounter(&now);
	elapsed.QuadPart = now.QuadPart - _ltframetime.QuadPart;
	elapsed.QuadPart *= 1000000;
	elapsed.QuadPart /= _frequency.QuadPart;
	_ltframetime = now;
	return elapsed.QuadPart;
}

GAI_DEF i32
gaiXWindowUpdate(gai_xwnd *window)
{
	r32 dt = gaiXWindowGetTime(window) / 1000000.f;
	window->dt.seconds = dt;
	window->dt.millis  = dt * 1000.f;
	window->dt.nanos   = dt * 1000.f * 1000.f;

	MSG msg;
	switch (window->renderer)
	{
		case XWND_RENDERER_OPENGL:
		{
			while ( PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
				{
					return 0;
				}
			}

		} break;
		case XWND_RENDERER_GDI:
		{
			while (GetMessage(&msg, 0, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return 0;
		}
	}

	return 1;
}

#elif __linux__

#else

#endif

#endif

#define GAI_XWINDOW_H
#endif