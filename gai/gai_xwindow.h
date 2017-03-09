#ifndef _GAI_XWINDOW_H_

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

	struct
	{
		gai_xwnd_renderer_enum type;
		union
		{
			struct
			{
				char *vendor;
				char *version;
				char *renderer;
				char *shading_language_version;
			} opengl;
		} info;

		struct
		{
			stbtt_packedchar char_data[255];
			i32 texture_size;
			i32 font_height;
			u32 program;
			u32 uniform_transform;
			u32 texture;
			u32 vao;
			u32 vbo;
		} debug_font_shader;
	} renderer;

	b32 is_running;
	b32 is_fullscreen;
	b32 is_visible;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int  			gaiXWindow        			(gai_xwnd *window, const char *title = "default window title", int width = -1, int height = -1, int x = -1, int y = -1, gai_xwnd_renderer_enum renderer = XWND_RENDERER_GDI, const char *classname = "crossplatform_xwindow_framework");
extern void 			gaiXWindowSetTitle			(gai_xwnd *window, const char *title);
inline extern float  	gaiXWindowUpdate			(gai_xwnd *window);
extern void 			gaiXWindowRender			(gai_xwnd *window);
extern int  			gaiXWindowSetVSYNC			(gai_xwnd *window, b32 state);
extern void* 			gaiXWindowGetProc			(gai_xwnd *window, const char *name);

#ifdef __cplusplus
}
#endif

#ifdef XWND_IMPLEMENTATION

//	[GENERAL PLATFORM INDEPENDEND LAYER]
//
//
//
//

#include <gl\gl.h>
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_BUFFER_ACCESS                  0x88BB
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA

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
	gaiXWindowFN(, debug_message_callback_fn, 		void,    	DebugMessageCallback, GLDEBUGPROC, void *) \
	gaiXWindowFN(, get_string_i_fn,					GLubyte*,	GetStringi, GLenum, GLuint ) \
	gaiXWindowFN(, bind_buffer_fn,					void,	 	BindBuffer, GLenum, GLuint buffer) \
	gaiXWindowFN(, delete_buffers_fn,				void, 	 	DeleteBuffers, GLsizei, const GLuint*) \
	gaiXWindowFN(, gen_buffers_fn, 					void,    	GenBuffers, GLsizei, GLuint*) \
	gaiXWindowFN(, buffer_data_fn, 					void,	 	BufferData, GLenum, GLsizeiptr, const void*, GLenum) \
	gaiXWindowFN(, buffer_sub_data_fn,				void, 	 	BufferSubData, GLenum, GLintptr, GLsizeiptr, const void *) \
	gaiXWindowFN(, map_buffer_fn, 					void, 	 	MapBuffer, GLenum, GLenum) \
	gaiXWindowFN(, unmap_buffer_fn, 				void, 	 	UnmapBuffer, GLenum) \
	gaiXWindowFN(, bind_vertex_array_fn,			void, 		BindVertexArray, GLuint) \
	gaiXWindowFN(, delete_vertex_arrays_fn,			void, 		DeleteVertexArrays, GLsizei, const GLuint*) \
	gaiXWindowFN(, gen_vertex_arrays_fn,			void, 		GenVertexArrays, GLsizei, GLuint*)

#define gaiXWindowFN(ext, def, a, b, ...) typedef a (gl_##def) (__VA_ARGS__);
gaiXWindowFNWrapper

#undef gaiXWindowFN
#define gaiXWindowFN(ext, def, a, b, ...) gl_##def *gl##b;
gaiXWindowFNWrapper

#undef gaiXWindowFN

#define gaiXWindowFN(ext, def, a, b, ...) \
	gl##b = (gl_##def *) gaiXWindowGetProc(window, "gl"#b#ext); \
	if (!gl##b) { \
		gai_debug("[  ERROR  ] gl%s%s (no address with this name found)\n",  #b, #ext); \
		gai_assert(!"gaiXWindowGetProc failed."); \
	} else { gai_debug("[ SUCCESS ] gl%s%s@0x%p\n",  #b, #ext, gl##b); }

GAI_DEF void
gaiGLGetFunctions(gai_xwnd *window)
{
	gai_debug("Initializing OpenGL functions...\n");
	gaiXWindowFNWrapper
	gai_debug("Done.\n");
}

#undef gaiXWindowFN
#undef gaiXWindowFNWrapper

GAI_DEF GLuint
gaiGLCreateProgram(char *version, char *vertex_code, char *fragment_code)
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

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return (program_id);
}

GAI_DEF void
gaiGLDebugFontShader(gai_xwnd *window)
{
	static b32 initialized;
	if (!initialized)
	{

		int   	debug_font_oversample   = 4;
		int 	debug_font_texture_size = 1024;
		char   	*debug_font             = "C:/windows/fonts/segoeuib.ttf";

		window->renderer.debug_font_shader.font_height = 14;
		stbtt_pack_context debug_font_ctx;

		// Load file into temporary buffer
		unsigned char *font_buffer;
		long font_buffer_size = 0;
		FILE *File = fopen(debug_font, "rb");
		fseek(File, 0, SEEK_END);
		font_buffer_size = ftell(File);
		rewind(File);

		font_buffer = (unsigned char *) malloc(font_buffer_size);
		fread(font_buffer, 1, font_buffer_size, File);
		fclose(File);

		// Create temporary bitmap buffer for the texture
		unsigned char *debug_font_texture_buffer = (unsigned char *) malloc( debug_font_texture_size * debug_font_texture_size );

		stbtt_PackBegin(&debug_font_ctx, debug_font_texture_buffer, debug_font_texture_size, debug_font_texture_size, 0, 1, 0);
		stbtt_PackSetOversampling(&debug_font_ctx, debug_font_oversample, debug_font_oversample);
		stbtt_PackFontRange(&debug_font_ctx, font_buffer, 0, window->renderer.debug_font_shader.font_height, 0, 255, window->renderer.debug_font_shader.char_data);
		stbtt_PackEnd(&debug_font_ctx);

		glGenTextures(1, &window->renderer.debug_font_shader.texture);
		glBindTexture(GL_TEXTURE_2D, window->renderer.debug_font_shader.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, debug_font_texture_size, debug_font_texture_size, 0, GL_RED, GL_UNSIGNED_BYTE, debug_font_texture_buffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, debug_font_texture_size, debug_font_texture_size, 0, GL_RED, GL_UNSIGNED_BYTE, debug_font_texture_buffer);
		window->renderer.debug_font_shader.texture_size = 1024;
		//glBindTexture(GL_TEXTURE_2D, 0);

		// GL_ALPHA is deprecated since opengl 4. So this method of texture rendering would not work. Replace it with GL_RED and
		// use a shader to render the text with the red channel.

		free(debug_font_texture_buffer);
		free(font_buffer);

		char *version = R"GLSL(#version 440)GLSL";
		char *vertex = R"GLSL(
			layout (location = 0) in vec4 color;
			layout (location = 1) in vec2 vertex;
			layout (location = 2) in vec2 uv;
			uniform mat4 transform;
			out vec4 _color;
			out vec2 _uv;
			void main()
			{
				gl_Position = transform * vec4(vertex.x, vertex.y, 0, 1);
				_color = vec4(color);
				_uv    = uv;
			})GLSL";

		char *frag = R"GLSL(
	        in vec4 _color;
	        in vec2 _uv;
	        out vec4 color;
	        uniform sampler2D font_texture;
	        void main() {
	            color = texture(font_texture, _uv);
	            color = vec4(color.r * _color.r, color.r * _color.g, color.r * _color.b, color.a * _color.a);
	            //color = _color;
	        })GLSL";

		window->renderer.debug_font_shader.program           = gaiGLCreateProgram(version, vertex, frag);
		window->renderer.debug_font_shader.uniform_transform = glGetUniformLocation(window->renderer.debug_font_shader.program, "transform");

		//		glUseProgram(window->renderer.debug_font_shader.program);


		glGenVertexArrays(1, &window->renderer.debug_font_shader.vao);
		glBindVertexArray(window->renderer.debug_font_shader.vao);

		glGenBuffers(1, &window->renderer.debug_font_shader.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, window->renderer.debug_font_shader.vbo);
		glBufferData(GL_ARRAY_BUFFER, gai_bytes_kb(256), 0, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 32, (void *) (i64) 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void *) (i64) 16);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, (void *) (i64) 24);

		glBindVertexArray(0);
	}
}

void
gaiDrawDbgText(gai_xwnd *window, float x, float y, char *text, v4 color)
{
	GLsizei offset = 0;
	GLint eot      = strlen(text);
	GLint count    = 0;

	r32 data[4096];
	r32 *cdata = data;

	stbtt_packedchar *char_data = window->renderer.debug_font_shader.char_data;
	int texture_size            = window->renderer.debug_font_shader.texture_size;
	int font_height             = window->renderer.debug_font_shader.font_height;

	y += font_height;
	while (*text && (count < eot))
	{
		stbtt_aligned_quad q;
		int c = *text;
		c = toupper(c);
		switch (*text)
		{
			case 'Ä': { c = 196; } break;
			case 'ä': { c = 228; } break;
			case 'Ö': { c = 214; } break;
			case 'ö': { c = 246; } break;
			case 'Ü': { c = 220; } break;
			case 'ü': { c = 252; } break;
		}

		stbtt_GetPackedQuad(char_data, texture_size, texture_size, c, &x, &y, &q, 0);

		r32 buffer[48] =
		{
			color.r, color.g, color.b, color.a, q.x0, q.y0, q.s0, q.t0,
			color.r, color.g, color.b, color.a, q.x1, q.y1, q.s1, q.t1,
			color.r, color.g, color.b, color.a, q.x0, q.y1, q.s0, q.t1,
			color.r, color.g, color.b, color.a, q.x0, q.y0, q.s0, q.t0,
			color.r, color.g, color.b, color.a, q.x1, q.y0, q.s1, q.t0,
			color.r, color.g, color.b, color.a, q.x1, q.y1, q.s1, q.t1,
		};

		memcpy(cdata, buffer, 192);
		cdata += 48;
		*text++;
		count++;
	}

	GLsizei bytes = count * 6 * 32;
	glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, data);
	glDrawArrays(GL_TRIANGLES, 0, count * 6);
}

#ifdef _WIN32  // WIN32 LAYER FOR WINDOW MANAGEMENT
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
gaiGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
	static char _buffer[4096];
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:               { gai_snprintf(_buffer, 4096, "Source: GL_DEBUG_SOURCE_API, "); } break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     { gai_snprintf(_buffer, 4096, "Source: GL_DEBUG_SOURCE_WINDOW_SYSTEM, "); } break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:   { gai_snprintf(_buffer, 4096, "Source: GL_DEBUG_SOURCE_SHADER_COMPILER, "); } break;
		case GL_DEBUG_SOURCE_APPLICATION:       { gai_snprintf(_buffer, 4096, "Source: GL_DEBUG_SOURCE_LIBGAI, "); } break;
		default:                                { gai_snprintf(_buffer, 4096, "Source: GL_DEBUG_SOURCE_THIRD_PARTY, "); } break;
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:                { gai_snprintf(_buffer, 4096, "%sType: ERROR, ", _buffer); } break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  { gai_snprintf(_buffer, 4096, "%sType: DEPRECATED_BEHAVIOUR, ", _buffer); } break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   { gai_snprintf(_buffer, 4096, "%sType: UNDEFINED_BEHAVIOUR, ", _buffer); } break;
		case GL_DEBUG_TYPE_PORTABILITY:          { gai_snprintf(_buffer, 4096, "%sType: PORTABILITY, ", _buffer); } break;
		case GL_DEBUG_TYPE_PERFORMANCE:          { gai_snprintf(_buffer, 4096, "%sType: PERFORMANCE, ", _buffer); } break;
		case GL_DEBUG_TYPE_OTHER:                { gai_snprintf(_buffer, 4096, "%sType: OTHER, ", _buffer); } break;
		case GL_DEBUG_TYPE_MARKER:               { gai_snprintf(_buffer, 4096, "%sType: MARKER, ", _buffer); } break;
		case GL_DEBUG_TYPE_PUSH_GROUP:           { gai_snprintf(_buffer, 4096, "%sType: PUSH_GROUP, ", _buffer); } break;
		case GL_DEBUG_TYPE_POP_GROUP:            { gai_snprintf(_buffer, 4096, "%sType: POP_GROUP, ", _buffer); } break;
		default:                                 { gai_snprintf(_buffer, 4096, "%sType: N/A, ", _buffer); } break;
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:            { gai_snprintf(_buffer, 4096, "%sSeverity: HIGH, ", _buffer); } break;
		case GL_DEBUG_SEVERITY_MEDIUM:          { gai_snprintf(_buffer, 4096, "%sSeverity: MEDIUM, ", _buffer); } break;
		case GL_DEBUG_SEVERITY_LOW:             { gai_snprintf(_buffer, 4096, "%sSeverity: LOW, ", _buffer); } break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:    { gai_snprintf(_buffer, 4096, "%sSeverity: NOTIFICATION, ", _buffer); } break;
		default:                                { gai_snprintf(_buffer, 4096, "%sSeverity: N/A, ", _buffer); } break;
	}

	gai_debug("%sID: %i\n%s", _buffer, id, message);
}

GAI_DEF GLboolean
gaiGLIsSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = 0;
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
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
	if (glGetStringi)
	{
		GLint n, i;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n);
		for (i = 0; i < n; i++)
		{
			if (strcmp((char *)glGetStringi(GL_EXTENSIONS, i), extension) == 0)
			{
				return true;
			}
		}
	}
	return false;
} // This is just copy & paste from nehe prod. IIRC

GAI_DEF GLboolean
gaiGLGetSwapInterval()
{
	if (gaiGLIsSupported("WGL_EXT_swap_control"))
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
gaiGLSetSwapInterval(b32 vsync)
{
	if (gaiGLIsSupported("WGL_EXT_swap_control"))
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

				if (window->renderer.type == XWND_RENDERER_OPENGL)
				{
					glViewport(0, 0, window->info.width, window->info.height);

					glUseProgram(window->renderer.debug_font_shader.program);
					m4x4 transform = Orthographic(0, window->info.width, 0, window->info.height, 1, -1);
					glUniformMatrix4fv(window->renderer.debug_font_shader.uniform_transform, 1, GL_TRUE, (const GLfloat*) &transform);

				}

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
				if (window->renderer.type == XWND_RENDERER_OPENGL)
				{
					glDeleteProgram(window->renderer.debug_font_shader.program);
					glDeleteTextures(1, &window->renderer.debug_font_shader.texture);
					glDeleteVertexArrays(1, &window->renderer.debug_font_shader.vao);
					glDeleteBuffers(1, &window->renderer.debug_font_shader.vbo);

					HGLRC ctx = wglGetCurrentContext();
					if (ctx)
					{
						wglMakeCurrent(0, 0);
						wglDeleteContext(ctx);
					}
				}

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

GAI_DEF int
gaiXWindowSetVSYNC(gai_xwnd *window, b32 state)
{
	gaiGLSetSwapInterval(state);
	return gaiGLGetSwapInterval();
}

inline GAI_DEF float
gaiXWindowUpdate(gai_xwnd *window)
{
	static LARGE_INTEGER _frequency;
	static LARGE_INTEGER _ltframetime;
	static i32 fps;
	static i32 frames;
	static r32 frametime;

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

	window->dt.nanos = elapsed.QuadPart;
	window->dt.millis = elapsed.QuadPart / 1000.f;
	window->dt.seconds = elapsed.QuadPart / 1000000.f;


	MSG msg;
	switch (window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
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

			#ifdef _DEBUG
			glUseProgram(window->renderer.debug_font_shader.program);
			glBindTexture(GL_TEXTURE_2D, window->renderer.debug_font_shader.texture);
			glBindVertexArray(window->renderer.debug_font_shader.vao);
			glBindBuffer(GL_ARRAY_BUFFER, window->renderer.debug_font_shader.vbo);

			char buffer[4096];
			int  offset = window->renderer.debug_font_shader.font_height;
			int  yoff   = 12 - offset;
#define gai_dbg_print(fmt, ...) gai_snprintf( (buffer), 4096, (fmt), __VA_ARGS__); gaiDrawDbgText( (window), (12), (yoff+=offset), (buffer), V4(1,1,1,1) )
			gai_dbg_print("Viewport Info:");
			gai_dbg_print(" W: %i, H: %i X: %i, Y: %i, FPS: %i, DELTA: %f", window->info.width, window->info.height, window->info.x, window->info.y, fps, window->dt.seconds);
			yoff += offset;
			gai_dbg_print("OpenGL Info:", window->renderer.info.opengl.version);
			gai_dbg_print(" %s", window->renderer.info.opengl.version);
			gai_dbg_print(" %s", window->renderer.info.opengl.vendor);
			gai_dbg_print(" %s", window->renderer.info.opengl.renderer);
			gai_dbg_print(" %s", window->renderer.info.opengl.shading_language_version);
#undef gai_dbg_print
			#endif
		} break;
		case XWND_RENDERER_GDI:
		{
			while (GetMessage(&msg, 0, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		}
	}

	if (frametime >= 1.0f)
	{
		fps = frames;
		frames = 0;
		frametime = 0;
	}
	else
	{
		frametime += window->dt.seconds;
		frames++;
	}

	return window->dt.seconds;
}

GAI_DEF int
gaiXWindow(gai_xwnd *window, const char *title, int width, int height, int x, int y, gai_xwnd_renderer_enum renderer, const char *classname)
{
	if (!window)
	{
		gai_assert(!"This function does not allocate memory for the window!");
	}

	if (!ConvertThreadToFiber(0) ) return -1;

	gai_xwnd init = { title, {0}, {width, height, x, y}, {0}, {0, 0, GetModuleHandle(0)}, { renderer } };
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
		gai_assert(!"Can't register window class.");
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

		int pf = ChoosePixelFormat(hdc, &pfd);
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

		if (gaiGLIsSupported("WGL_ARB_create_context"))
		{
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB)
			{
				int attribs[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB,  0,
					WGL_CONTEXT_MINOR_VERSION_ARB,  0,
					WGL_CONTEXT_FLAGS_ARB, 0,
					WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					0
				};

				#ifndef XWND_OPENGL_MAJOR
				glGetIntegerv(GL_MAJOR_VERSION, attribs + 1);
				#else
				attribs[1] = XWND_OPENGL_MAJOR;
				#endif
				#ifndef XWND_OPENGL_MINOR
				glGetIntegerv(GL_MINOR_VERSION, attribs + 3);
				#else
				attribs[3] = XWND_OPENGL_MINOR;
				#endif
				#ifdef _DEBUG
				attribs[5] |= WGL_CONTEXT_DEBUG_BIT_ARB;
				#endif

				HGLRC new_oglctx = wglCreateContextAttribsARB(hdc, 0, attribs);
				if (new_oglctx)
				{
					wglMakeCurrent(0, 0);
					wglDeleteContext(old_glctx);
					wglMakeCurrent(hdc, new_oglctx);
					gaiXWindowSetVSYNC(window, 1);
					gaiGLGetFunctions(window);
					#ifdef _DEBUG
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback( (GLDEBUGPROC) gaiGLDebugCallback, 0);
					gaiGLDebugFontShader(window);
					#endif
				}
				else
				{
					gai_debug("Error: wglCreateContextAttribsARB failed to create modern context.");
				}
			}
			else
			{
				gai_debug("Error: wglGetProcAddress failed for wglCreateContextAttribsARB.");
			}
		}
		else
		{
			gai_debug("Warning: This graphics card does not support \"WGL_ARB_create_context\".");
		}

		window->renderer.info.opengl.vendor                   = (char *) glGetString(GL_VENDOR);
		window->renderer.info.opengl.version                  = (char *) glGetString(GL_VERSION);
		window->renderer.info.opengl.renderer                 = (char *) glGetString(GL_RENDERER);
		window->renderer.info.opengl.shading_language_version = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
		gai_debug("OpenGL context created: %s, %s, %s, %s\n", window->renderer.info.opengl.vendor, window->renderer.info.opengl.version, window->renderer.info.opengl.renderer, window->renderer.info.opengl.shading_language_version);

	}
	window->is_running = true;
	ShowWindow(hwnd, SW_SHOW);
	return 1;
}
#elif __linux__

#else

#endif

#endif

#define _GAI_XWINDOW_H_
#endif