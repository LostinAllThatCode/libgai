#ifndef GAI_INCLUDE_XWINDOW_H
/*
	TODO: Make a documentation as soon as this api is somewhat useable!!!

	Linker dependencies per platform:
		Windows : user32.lib, gdi32.lib, winmm.lib, opengl32.lib (when using opengl)


	Do this:
		#define GAIXW_IMPLEMENTATION
   	before you include this file in *one* C or C++ file to create the implementation.

	All function prefixed with a underscore(_) are internally used functions.
	DO NOT use them if you are not 100% sure what they do.

	Example:

	#define GAIXW_IMPLEMENTATION
	#include "gai_xwindow.h"

	int main(int argc, char **argv)
	{
		gaixw_context window;
		if(!gaiXWindow(&window)) return -1;
		while( gaiXWindowUpdate(&window) );

		return 0;
	}
*/

#ifdef GAIXW_STATIC
	#define GAIXW_API static
#else
	#define GAIXW_API extern
#endif

#ifndef GAIXW_ASSERT
	#include <assert.h>
	#define GAIXW_ASSERT(x) assert(x)
#endif

#ifdef GAIXW_DEBUG
	#include <stdio.h>
	#ifndef GAIXW_PRINT
		#define GAIXW_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
	#endif
#else
	#define GAIXW_PRINT(fmt, ...)
#endif

#include <string.h>

#if _WIN32
	#ifndef GAIXW_DONT_INCL_WIN
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
	#pragma comment( lib, "user32.lib" )
	#pragma comment( lib, "gdi32.lib" )
	#pragma comment( lib, "winmm.lib" )
#elif __linux__
	#include <X11/Xlib.h>
#elif __APPLE__
#else
#endif

enum gaixw_renderer_enum
{
	gaixwRendererGDI, gaixwRendererDX10, gaixwRendererDX11, gaixwRendererDX12,
	gaixwRendererOpenGL, gaixwRendererVulcan,
	gaixwRendererMax
};

enum
{
	gaixwFlagsNone 			= 0x0,
	gaixwFlagsFullscreen    = 0x1,
	gaixwFlagsVSYNC			= 0x2
};

struct gaixw_context;
typedef void(*gaiXWindowDeInit) (gaixw_context *window, void *userdata);
struct gaixw_deinit_callback
{
	gaiXWindowDeInit callback;
	void			 *userdata;
};

struct gaixw_context
{
	struct
	{
		float seconds;
		float millis;
		float micros;
	} frametime;

	struct
	{
		const char *title;
		int width, height, x, y, fps;
	} info;

	struct
	{
		int x, y, dx, dy, wheel, dwheel;
		int mod_alt, mod_ctrl, mod_shift;
		unsigned char keys[256], keys_history[256];
		unsigned char mouse[3], mouse_history[3];
	} input;

	struct
	{
		#if _WIN32
		// windows specific handles and helper variables
		HWND            	hwnd;
		HDC             	hdc;
		HINSTANCE       	instance;
		WINDOWPLACEMENT     position;
		#elif __linux__
		// linux specific handles and helper variables
		Display *display;
		Visual  *visual;
		Window  *window;
		#else

		#endif
	} platform;

	struct
	{
		gaixw_renderer_enum type;
		int attributes;
		union
		{
			struct
			{
				int is_modern;
				char *vendor;
				char *version;
				char *renderer;
				char *shading_language_version;
			} opengl;
		} info;
	} renderer;

	gaixw_deinit_callback deinit[12];

	int is_running;
	int is_visible;
};
#ifdef __cplusplus
extern "C" {
#endif

GAIXW_API int  			gaiXWindow        					(gaixw_context *window, const char *title = "default window title", int width = -1, int height = -1, int x = -1, int y = -1, const char *classname = "crossplatform-window-framework");
GAIXW_API void 			gaiXWindowSetTitle					(gaixw_context *window, const char *title);
GAIXW_API float		 	gaiXWindowUpdate					(gaixw_context *window);
GAIXW_API void 			gaiXWindowSwapBuffers				(gaixw_context *window);
GAIXW_API int  			gaiXWindowSetVSYNC					(gaixw_context *window, unsigned int state);
GAIXW_API void* 		gaiXWindowGetProc					(gaixw_context *window, const char *name);
GAIXW_API void  		gaiXWindowToggleFullscreen  		(gaixw_context *window);
GAIXW_API void			gaiXWindowRegisterDeInitCallback	(gaixw_context *window, gaiXWindowDeInit *callback, void *userdata);
GAIXW_API int           gaixw_IsVSYNC						(gaixw_context *window);
GAIXW_API int           gaixw_IsFullscreen					(gaixw_context *window);

#ifdef __cplusplus
}
#endif

#ifdef GAIXW_IMPLEMENTATION

#ifdef _WIN32

#ifdef GAIXW_OPENGL
#pragma comment( lib, "opengl32.lib" )
// ##########################################################################################################
//                                              BEGIN OF
//                              OPENGL WINDOWS PLATFORM IMPLEMENTATION CODE
// ##########################################################################################################
//
// 		Windows specific platform layer for managing the opengl driver
//		- Gives you access to all opengl function specified with prefix [gl]
//			- Visit http://www.docs.gl to find the documentation for all gl functions.
//      - Allows you to create a opengl context which fits your interest
//      	- Following macros can be defined before including this file:
//				GAIXW_OPENGL_MAJOR_VERSION, GAIXW_OPENGL_MINOR_VERSION
//          - Note:
//				If the macro GAIXW_DEBUG is speficied the opengl context becomes a debug context
//				and all opengl debug messages will be outputted to either the console console.
//
// ##########################################################################################################


#include <gl\gl.h>
typedef char GLchar;
typedef size_t GLsizeiptr;
typedef size_t GLintptr;
typedef void (APIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#define gaiXWindowFNWrapper \
	gaiXWindowFN(, attach_shader_fn,  				void, 	 	AttachShader, GLuint, GLuint) \
	gaiXWindowFN(, compile_shader_fn, 				void, 	 	CompileShader, GLuint) \
	gaiXWindowFN(, create_program_fn, 				GLuint,  	CreateProgram, void) \
	gaiXWindowFN(, create_shader_fn,  				GLuint,  	CreateShader, GLenum) \
	gaiXWindowFN(, delete_program_fn, 				void,    	DeleteProgram, GLuint) \
	gaiXWindowFN(, delete_shader_fn,  				void,    	DeleteShader, GLuint) \
	gaiXWindowFN(, detach_shader_fn,  			    void,    	DetachShader, GLuint, GLuint) \
	gaiXWindowFN(, disable_vertex_attrib_array_fn,  void,    	DisableVertexAttribArray, GLuint) \
	gaiXWindowFN(, enable_vertex_attrib_array_fn,   void,    	EnableVertexAttribArray, GLuint) \
	gaiXWindowFN(, get_program_iv_fn,			    void,    	GetProgramiv, GLuint, GLenum, GLint*) \
	gaiXWindowFN(, get_program_info_log_fn,			void,    	GetProgramInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	gaiXWindowFN(, get_shader_info_log_fn,			void,    	GetShaderInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	gaiXWindowFN(, get_shader_source_fn, 			void,    	GetShaderSource, GLuint, GLsizei, GLsizei*, GLchar *) \
	gaiXWindowFN(, get_uniform_location_fn,         GLint,   	GetUniformLocation, GLuint, const GLchar*) \
	gaiXWindowFN(, link_program_fn,					void,    	LinkProgram, GLuint) \
	gaiXWindowFN(, shader_source_fn,                void,    	ShaderSource, GLuint, GLsizei, GLchar**, GLint*) \
	gaiXWindowFN(, use_program, 					void,    	UseProgram, GLuint) \
	gaiXWindowFN(, uniform_1f_fn,					void,    	Uniform1f, GLint, GLfloat) \
	gaiXWindowFN(, uniform_2f_fn,					void,    	Uniform2f, GLint, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_3f_fn,					void,    	Uniform3f, GLint, GLfloat, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_4f_fn,					void,    	Uniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat) \
	gaiXWindowFN(, uniform_1i_fn,					void,    	Uniform1i, GLint, GLint) \
	gaiXWindowFN(, uniform_2i_fn,					void,    	Uniform2i, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_3i_fn, 					void,    	Uniform3i, GLint, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_4i_fn, 					void,    	Uniform4i, GLint, GLint, GLint, GLint, GLint) \
	gaiXWindowFN(, uniform_1fv_fn,					void,    	Uniform1fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_2fv_fn,					void,    	Uniform2fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_3fv_fn,					void,    	Uniform3fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_4fv_fn,					void,    	Uniform4fv, GLint, GLsizei, const GLfloat*) \
	gaiXWindowFN(, uniform_1iv_fn,					void,    	Uniform1iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_2iv_fn,					void,    	Uniform2iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_3iv_fn,					void,    	Uniform3iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_4iv_fn,					void,    	Uniform4iv, GLint, GLsizei, const GLint*) \
	gaiXWindowFN(, uniform_matrix_2fv_fn, 			void,    	UniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, uniform_matrix_3fv_fn,			void,    	UniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, uniform_matrix_4fv_fn,			void,    	UniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	gaiXWindowFN(, validate_program_fn, 			void,	 	ValidateProgram ,GLuint) \
	gaiXWindowFN(, vertex_attrib_pointer_fn,   		void,    	VertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *) \
	gaiXWindowFN(, debug_message_callback_fn, 		void,    	DebugMessageCallback, DEBUGPROC, void *) \
	gaiXWindowFN(, get_string_i_fn,					GLubyte*,	GetStringi, GLenum, GLuint ) \
	gaiXWindowFN(, bind_buffer_fn,					void,	 	BindBuffer, GLenum, GLuint buffer) \
	gaiXWindowFN(, delete_buffers_fn,				void, 	 	DeleteBuffers, GLsizei, const GLuint*) \
	gaiXWindowFN(, gen_buffers_fn, 					void,    	GenBuffers, GLsizei, GLuint*) \
	gaiXWindowFN(, buffer_data_fn, 					void,	 	BufferData, GLenum, GLsizeiptr, const void*, GLenum) \
	gaiXWindowFN(, buffer_sub_data_fn,				void, 	 	BufferSubData, GLenum, GLintptr, GLsizeiptr, const void *) \
	gaiXWindowFN(, map_buffer_fn, 					GLvoid*, 	MapBuffer, GLenum, GLenum) \
	gaiXWindowFN(, unmap_buffer_fn, 				void, 	 	UnmapBuffer, GLenum) \
	gaiXWindowFN(, bind_vertex_array_fn,			void, 		BindVertexArray, GLuint) \
	gaiXWindowFN(, delete_vertex_arrays_fn,			void, 		DeleteVertexArrays, GLsizei, const GLuint*) \
	gaiXWindowFN(, gen_vertex_arrays_fn,			void, 		GenVertexArrays, GLsizei, GLuint*) \
	gaiXWindowFN(, map_buffer_range_fn,				GLvoid*,	MapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) \
	gaiXWindowFN(, buffer_storage_fn,				void,  		BufferStorage, GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) \
	gaiXWindowFN(, blend_equation_fn,				void, 		Blend_Equation, GLenum) \
	gaiXWindowFN(, blend_func_seperate_fn,			void,		BlendFuncSeparate, GLenum, GLenum, GLenum, GLenum)

#define gaiXWindowFN(ext, def, a, b, ...) typedef a (gl_##def) (__VA_ARGS__);
gaiXWindowFNWrapper
#undef gaiXWindowFN

#define gaiXWindowFN(ext, def, a, b, ...) gl_##def *gl##b;;
gaiXWindowFNWrapper
#undef gaiXWindowFN

#define gaiXWindowFN(ext, def, a, b, ...) gl##b = (gl_##def *) wglGetProcAddress("gl"#b#ext); GAIXW_ASSERT( gl##b );
GAIXW_API void
gaiWGLInitFunctions()
{
	gaiXWindowFNWrapper
}
#undef gaiXWindowFN
#undef gaiXWindowFNWrapper

GAIXW_API int
gaiWGLIsSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = 0;
	void* wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtString)
	{
		supported = ((char*(__stdcall*)(HDC)) wglGetExtString)(wglGetCurrentDC());
	}

	if (supported)
	{
		for (const char* p = supported; ; p++)
		{
			p = strstr(p, extension);
			if (!p) return false;
			if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' ')) return true;
		}
	}
	return false;
} // This is just copy & paste from nehe prod. IIRC

void
gaiGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
	if (severity == 0x9146 /* HIGH */ )
	{
		GAIXW_PRINT("Source:   0x%04x\nType:     0x%04x\nMessage:  %s", source, type, message);
		GAIXW_ASSERT(false);
	}
	#if 0
	switch (source)
	{
		case 0x8246: { GAIXW_PRINT("Source: GL_DEBUG_SOURCE_API, "); } break;
		case 0x8247: { GAIXW_PRINT("Source: GL_DEBUG_SOURCE_WINDOW_SYSTEM, "); } break;
		case 0x8248: { GAIXW_PRINT("Source: GL_DEBUG_SOURCE_SHADER_COMPILER, "); } break;
		case 0x8249: { GAIXW_PRINT("Source: GL_DEBUG_SOURCE_LIBGAI, "); } break;
		default:     { GAIXW_PRINT("Source: GL_DEBUG_SOURCE_THIRD_PARTY, "); } break;
	}
	switch (type)
	{
		case 0x824C: { GAIXW_PRINT("Type: ERROR, "); } break;
		case 0x824D: { GAIXW_PRINT("Type: DEPRECATED_BEHAVIOUR, "); } break;
		case 0x824E: { GAIXW_PRINT("Type: UNDEFINED_BEHAVIOUR, "); } break;
		case 0x824F: { GAIXW_PRINT("Type: PORTABILITY, "); } break;
		case 0x8250: { GAIXW_PRINT("Type: PERFORMANCE, "); } break;
		case 0x8251: { GAIXW_PRINT("Type: OTHER, "); } break;
		case 0x8268: { GAIXW_PRINT("Type: MARKER, "); } break;
		case 0x8269: { GAIXW_PRINT("Type: PUSH_GROUP, "); } break;
		case 0x826A: { GAIXW_PRINT("Type: POP_GROUP, "); } break;
		default:     { GAIXW_PRINT("Type: N/A, "); } break;
	}
	switch (severity)
	{
		case 0x9146: { GAIXW_PRINT("Severity: HIGH, "); } break;
		case 0x9147: { GAIXW_PRINT("Severity: MEDIUM, "); } break;
		case 0x9148: { GAIXW_PRINT("Severity: LOW, "); } break;
		case 0x826B: { GAIXW_PRINT("Severity: NOTIFICATION, "); } break;
		default:     { GAIXW_PRINT("Severity: N/A, "); } break;
	}
	GAIXW_PRINT("ID: %i\n%s", id, message);

	#endif
}

GAIXW_API int
gaiGLIsSupported(char *extension)
{
	if (glGetStringi)
	{
		GLint n, i;
		glGetIntegerv(0x821D /* GL_NUM_EXTENSIONS */, &n);
		for (i = 0; i < n; i++)
		{
			char *gl_extension = (char *)glGetStringi(GL_EXTENSIONS, i);
			if (strcmp(gl_extension, extension) == 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void
gaiGLInitMultisampling(gaixw_context *window)
{
	GLint samples;
	glGetIntegerv(0x9110 /* GL_MAX_INTEGER_SAMPLES */, &samples);
}

GAIXW_API int
gaiGLGetSwapInterval()
{
	if (gaiWGLIsSupported("WGL_EXT_swap_control"))
	{
		void *wglGetSwapInterval = wglGetProcAddress("wglGetSwapIntervalEXT");
		if (wglGetSwapInterval) return ((int(__stdcall *)(void))wglGetSwapInterval)();
	}
	return false;
}

GAIXW_API void
gaiGLSetSwapInterval(unsigned int vsync)
{
	if (gaiWGLIsSupported("WGL_EXT_swap_control"))
	{
		void* wglSwapInterval = wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapInterval) ((GLboolean(__stdcall *)(int))wglSwapInterval)(vsync);
	}
}

// ##########################################################################################################
//                                               END OF
//                              OPENGL WINDOWS PLATFORM IMPLEMETATION CODE
// ##########################################################################################################

#endif

GAIXW_API int
gaixw_IsVSYNC(gaixw_context *window)
{
	return (window->renderer.attributes & gaixwFlagsVSYNC ? 1 : 0);
}

GAIXW_API int
gaixw_IsFullscreen(gaixw_context *window)
{
	return (window->renderer.attributes & gaixwFlagsFullscreen ? 1 : 0);
}

GAIXW_API void
_gaiXWindowDeInit(gaixw_context *window)
{
	gaixw_deinit_callback *deinit = 0;
	for (int i = 0; i < 12; i++)
	{
		deinit = &window->deinit[i];
		if (deinit->callback == 0) break;
		deinit->callback(window, deinit->userdata);
	}
}

GAIXW_API void
gaiXWindowRegisterDeInitCallback(gaixw_context *window, gaiXWindowDeInit callback, void *userdata)
{
	gaixw_deinit_callback *deinit = 0;
	for (int i = 0; i < 12; i++)
	{
		deinit = &window->deinit[i];
		if (deinit->callback == 0)
		{
			deinit->callback = callback;
			deinit->userdata = userdata;
			break;
		}
	}
}

GAIXW_API unsigned char
gaiKeyDown(gaixw_context *window, int key)
{
	return window->input.keys[key];
}

GAIXW_API unsigned char
gaiKeyPressed(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.keys_history[key];
	window->input.keys_history[key] = gaiKeyDown(window, key);
	return (window->input.keys_history[key] && !previousState);
}

GAIXW_API unsigned char
gaiKeyReleased(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.keys_history[key];
	window->input.keys_history[key] = gaiKeyDown(window, key);
	return (!window->input.keys_history[key] && previousState);
}

GAIXW_API unsigned char
gaiMouseDown(gaixw_context *window, int key)
{
	return window->input.mouse[key];
}

GAIXW_API unsigned char
gaiMousePressed(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.mouse_history[key];
	window->input.mouse_history[key] = gaiMouseDown(window, key);
	return (window->input.mouse_history[key] && !previousState);
}

GAIXW_API unsigned char
gaiMouseReleased(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.mouse_history[key];
	window->input.mouse_history[key] = gaiMouseDown(window, key);
	return (!window->input.mouse_history[key] && previousState);
}

LRESULT CALLBACK
gaiXWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gaixw_context *window = (gaixw_context *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (window)
	{
		switch (uMsg)
		{
			case WM_SIZE: 			{ window->info.width  = LOWORD(lParam); window->info.height = HIWORD(lParam); return 0; }
			case WM_MOVE: 			{ window->info.x = (int)(short) LOWORD(lParam); window->info.y = (int)(short) HIWORD(lParam); return 0; }
			case WM_KEYDOWN: 		{ window->input.keys_history[wParam] = window->input.keys[wParam]; window->input.keys[wParam] = 1; return 0; }
			case WM_KEYUP:			{ window->input.keys_history[wParam] = window->input.keys[wParam]; window->input.keys[wParam] = 0; return 0; }
			case WM_MOUSEMOVE:		{ POINTS p = MAKEPOINTS(lParam); window->input.x  = p.x; window->input.y  = p.y; return 0; }
			case WM_LBUTTONDOWN:	{ window->input.mouse_history[0] = window->input.mouse[0]; window->input.mouse[0] = 1; return 0; }
			case WM_LBUTTONUP:		{ window->input.mouse_history[0] = window->input.mouse[0]; window->input.mouse[0] = 0; return 0; }
			case WM_MBUTTONDOWN:	{ window->input.mouse_history[1] = window->input.mouse[1]; window->input.mouse[1] = 1; return 0; }
			case WM_MBUTTONUP:		{ window->input.mouse_history[1] = window->input.mouse[1]; window->input.mouse[1] = 0; return 0; }
			case WM_RBUTTONDOWN:	{ window->input.mouse_history[2] = window->input.mouse[2]; window->input.mouse[2] = 1; return 0; }
			case WM_RBUTTONUP:		{ window->input.mouse_history[2] = window->input.mouse[2]; window->input.mouse[2] = 0; return 0; }
			case WM_MOUSEWHEEL:		{ window->input.dwheel = GET_WHEEL_DELTA_WPARAM(wParam); window->input.wheel  += window->input.dwheel; return 0; }
			case WM_DESTROY:
			{
				_gaiXWindowDeInit(window);
				#ifdef GAIXW_OPENGL
				if (window->renderer.info.opengl.is_modern)
				{
					HGLRC ctx = wglGetCurrentContext();
					if (ctx)
					{
						wglMakeCurrent(0, 0);
						wglDeleteContext(ctx);
					}
				}
				#endif
				PostQuitMessage(0);
				return 0;
			} break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GAIXW_API void
gaiXWindowToggleFullscreen(gaixw_context *window)
{
	HWND hWnd = window->platform.hwnd;
	DWORD Style = GetWindowLong(hWnd, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
		if (GetWindowPlacement(hWnd, &window->platform.position) &&
		        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLong(hWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
			             MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
			             MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			             MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		window->renderer.attributes |= gaixwFlagsFullscreen;
	}
	else
	{
		SetWindowLong(hWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &window->platform.position);
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		window->renderer.attributes &= ~gaixwFlagsFullscreen;
	}
}

GAIXW_API void
gaiXWindowSetTitle(gaixw_context *window, const char *title)
{
	wchar_t buffer[4096];
	MultiByteToWideChar(CP_UTF8, 0, title, -1, buffer, 4095);
	SetWindowTextW(window->platform.hwnd, buffer);
	window->info.title = title;
}

GAIXW_API void
gaiXWindowSwapBuffers(gaixw_context *window)
{
	#ifdef GAIXW_OPENGL
	SwapBuffers(window->platform.hdc);
	#else
	#endif
}

GAIXW_API int
gaiXWindowSetVSYNC(gaixw_context *window, unsigned int state)
{
	unsigned int result = 0;
	#ifdef GAIXW_OPENGL
	gaiGLSetSwapInterval(state);
	result = gaiGLGetSwapInterval();
	window->renderer.attributes = (result == 1 ? window->renderer.attributes | gaixwFlagsVSYNC : window->renderer.attributes & ~gaixwFlagsVSYNC);
	#endif
	return result;
}

inline GAIXW_API float
gaiXWindowUpdate(gaixw_context *window)
{
	MSG msg;
	#ifdef GAIXW_OPENGL
	if (gaiKeyReleased(window, VK_F1)) gaiXWindowToggleFullscreen(window);
	if (gaiKeyReleased(window, VK_F2)) gaiXWindowSetVSYNC(window, !(window->renderer.attributes & gaixwFlagsVSYNC));

	static LARGE_INTEGER _frequency;
	static LARGE_INTEGER _ltframetime;
	static int fps;
	static int frames;
	static long double frametime;

	static unsigned int initialized;
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

	window->frametime.micros  = (float)elapsed.QuadPart;
	window->frametime.millis  = (float)elapsed.QuadPart / 1000.f;
	window->frametime.seconds = (float)elapsed.QuadPart / 1000000.f;
	_ltframetime = now;

	int old_x = window->input.x;
	int old_y = window->input.y;
	window->input.dwheel = 0;

	while ( PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			window->is_running = false;
			break;
		}
	}

	window->input.dx = window->input.x - old_x;
	window->input.dy = window->input.y - old_y;

	if (frametime >= 1.0f)
	{
		window->info.fps = frames;
		frames = 0;
		frametime = 0;
	}
	else
	{
		frametime += window->frametime.seconds;
		frames++;
	}
	#else
	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	window->is_running = false;
	#endif
	return (float) window->frametime.seconds;
}

GAIXW_API int
gaiXWindow(gaixw_context *window, const char *title, int width, int height, int x, int y, const char *classname)
{
	GAIXW_ASSERT(window);
	if (!ConvertThreadToFiber(0) ) return -1;

	gaixw_context init = { {0}, {title, width, height, x, y}, {0}, {0, 0, GetModuleHandle(0)}, { gaixwRendererGDI } };
	*window = init;

	WNDCLASSEXA wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = sizeof(gaixw_context*);
	wnd_class.lpfnWndProc   = gaiXWindowProc;
	wnd_class.hInstance     = window->platform.instance;
	wnd_class.lpszClassName = classname;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassExA(&wnd_class))
	{
		GAIXW_ASSERT(!"Can't register window class.");
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
		GAIXW_ASSERT(!"Can't create window.");
		return -2;
	}

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window);
	HDC hdc = GetDC(hwnd);
	window->platform.hwnd = hwnd;
	window->platform.hdc  = hdc;

	#ifdef GAIXW_OPENGL
	window->renderer.type = gaixwRendererOpenGL;
	//timeBeginPeriod(1);
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize                 = sizeof(pfd);
	pfd.nVersion              = 1;
	pfd.iPixelType            = PFD_TYPE_RGBA;
	pfd.dwFlags               = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cColorBits            = 32;
	pfd.cAlphaBits            = 8;
	pfd.cDepthBits            = 32;
	pfd.iLayerType            = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(hdc, &pfd);
	if (!pf)
	{
		GAIXW_ASSERT(!"No valid pxiel format found.");
		DestroyWindow(hwnd);
		return -3;
	}

	if (!SetPixelFormat(hdc, pf, &pfd))
	{
		GAIXW_ASSERT(!"Set pixel format failed.");
		return -4;
	}

	HGLRC old_glctx = wglCreateContext(hdc);
	if (!old_glctx)
	{
		GAIXW_ASSERT(!"Can't create opengl context.");
		DestroyWindow(hwnd);
		return -5;
	}

	if (!wglMakeCurrent(hdc, old_glctx))
	{
		GAIXW_ASSERT(!"Activate opengl context failed..");
		wglDeleteContext(old_glctx);
		DestroyWindow(hwnd);
		return -6;
	}

	window->renderer.info.opengl.vendor                   = (char *) glGetString(GL_VENDOR);
	window->renderer.info.opengl.version                  = (char *) glGetString(GL_VERSION);
	window->renderer.info.opengl.renderer                 = (char *) glGetString(GL_RENDERER);

	if (gaiWGLIsSupported("WGL_ARB_create_context"))
	{
		void *wglCreateContextAttribsARB = wglGetProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB)
		{
			int attribs[] =
			{
				#if GAIXW_OPENGL_MAJOR_VERSION
				0x2091 /* WGL_CONTEXT_MAJOR_VERSION_ARB */, GAIXW_OPENGL_MAJOR_VERSION,
				#endif
				#if GAIXW_OPENGL_MINOR_VERSION
				0x2092 /* WGL_CONTEXT_MINOR_VERSION_ARB */, GAIXW_OPENGL_MINOR_VERSION,
				#endif
				0x9126 /* WGL_CONTEXT_PROFILE_MASK_ARB */, 0x00000001 /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB */,
				0x2094 /* WGL_CONTEXT_FLAGS_ARB */, 0
				#ifdef _DEBUG
				| 0x00000001
				#endif
				,
				0
			};

			HGLRC new_oglctx = ((HGLRC(__stdcall *)(HDC, HGLRC, const int *))wglCreateContextAttribsARB)(hdc, 0, attribs);
			if (new_oglctx)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(old_glctx);
				wglMakeCurrent(hdc, new_oglctx);

				window->renderer.info.opengl.vendor                   = (char *) glGetString(GL_VENDOR);
				window->renderer.info.opengl.version                  = (char *) glGetString(GL_VERSION);
				window->renderer.info.opengl.renderer                 = (char *) glGetString(GL_RENDERER);
				window->renderer.info.opengl.shading_language_version = (char *) glGetString(0x8B8C);  // 0x8B8C GL_SHADING_LANGUAGE_VERSION
				window->renderer.info.opengl.is_modern = true;

				gaiXWindowSetVSYNC(window, 1);

				gaiWGLInitFunctions();

				gaiGLInitMultisampling(window);

				#ifdef _DEBUG
				glEnable(0x8242 /* GL_DEBUG_OUTPUT_SYNCHRONOUS */ ); // 0x92E0 /* GL_DEBUG_OUTPUT */
				glDebugMessageCallback( (DEBUGPROC) gaiGLDebugCallback, 0);
				#endif
			}
			else GAIXW_PRINT("Error: wglCreateContextAttribsARB failed to create modern context.");
		}
		else GAIXW_PRINT("Error: wglGetProcAddress failed for wglCreateContextAttribsARB.");
	}
	else GAIXW_PRINT("Warning : This graphics card does not support \"WGL_ARB_create_context\".");

	#else

	#endif

	window->is_running = true;
	window->is_visible = (ShowWindow(hwnd, SW_SHOW) == 0);
	return 1;
}



#elif __linux__

#else

#endif

#endif

#define GAI_INCLUDE_XWINDOW_H
#endif