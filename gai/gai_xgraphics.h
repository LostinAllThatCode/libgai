#ifndef GAI_XGRAPHICS_H

// ##########   ** THIS LIBRARY HAS CORE DEPENDENCIES ** ###############
#include "gai_types.h"
#include "gai_utils.h"
#include "gai_math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"        /* image loading support library */
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"     /* font loading support library */

#ifdef GAIXG_STATIC
	#define GAIXG_API static
#else
	#define GAIXG_API extern
#endif

#ifndef GAIXG_ASSERT
	#include <assert.h>
	#define GAIXG_ASSERT(x) assert(x)
#endif

#ifdef GAIXG_DEBUG
	#include <stdio.h>
	#define GAIXG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
#else
	#define GAIXG_PRINT(fmt, ...)
#endif

// #####################################################################

struct gaixg_layout32 //  32 bytes, 8 floats
{
	float uv[2];
	float color[3];
	float position[3];
};

struct gaixg_layout48 //  48 bytes, 12 floats
{
	float uv[2];
	float color[4];
	float normal[3];
	float position[3];
}; // NOTE: Code XL interleaved data format T2F_C4F_N3F_V3F (0 bytes offset, 0 bytes stride)

struct gaixg_layout64 //  64 bytes, 16 floats
{
	float uv[2];
	float color[4];
	float normal[3];
	float position[3];
	float reserved[4];
}; // NOTE: Code XL interleaved data format T2F_C4F_N3F_V3F (0 bytes offset, 16 bytes stride) ** 16 bytes stride are reserved for possible later use

enum gaixg_buffer_flags_enum
{
	XGP_BUFFER_STATIC,
	XGP_BUFFER_DYNAMIC,
	XGP_BUFFER_STREAM,
	XGP_BUFFER_PERSISTENT_MEMORY,

	XGP_BUFFER_READ,
	XGP_BUFFER_WRITE,
};
struct gaixg_buffer
{
	gaixg_buffer_flags_enum 	flags;
	u32							vao;
	u32 						handle;
	size_t  					size;
	size_t  					used;
	size_t              		size_per_segment;
	void			 			*address;
};

struct gaixg_font
{
	u32               	texture;
	i32 			 	texture_size;
	i32					height;
	stbtt_packedchar 	characters[255];
	gaixg_buffer      	*buffer;
};

#define gaixg_DYN_STR_DEF_BUFSIZE gai_megabytes(8);
struct gaixg_drawcall_dynamic_string
{
	u32					start;
	u32					length;
	gaixg_font        	*font;
};

GAI_DYNAMIC( gaixg_font_array,    gaixg_font );
GAI_DYNAMIC( gaixg_buffer_array,  gaixg_buffer );
GAI_DYNAMIC( gaixg_opengl_handles, u32 );

struct gaixg_context
{
	gaixw_context 			*window;

	gaixg_font_array 		fonts;
	gaixg_buffer_array    	buffers;

	gaixg_opengl_handles 	textures;
	gaixg_opengl_handles 	programs;

	struct
	{
		u32 			program;
		u32 			transform;
		void            *memory;
		u32				vao;
		gaixg_font 	*default_font;
	} builtin_font_shader;
};

#define external extern
struct gaixg_text_rect
{
	int x, y, w, h;
};


external int 				gaiXGraphicsCreateContext			(gaixg_context *context, gaixw_context *window);
external gaixg_text_rect 	gaiXGraphicsGetTextRect				(gaixg_context *context, float  x, float  y, char *text, gaixg_font *font = 0);
GAI_DEF  void 				gaiXGraphicsDrawDynamicText2D		(gaixg_context *context, float *x, float *y, float *z, wchar_t *text, v4 color, gaixg_font * font, int random = 0);
GAI_DEF  void 				gaiXGraphicsDrawDynamicText2D		(gaixg_context *context, float *x, float *y, float *z, char *text, v4 color = V4(1, 1, 1, 1), gaixg_font *font = 0);
GAI_DEF  void 				gaiXGraphicsDrawDynamicText2D		(gaixg_context *context, float  x, float  y, float  z, char *text, v4 color = V4(1, 1, 1, 1), gaixg_font *font = 0);

external void			 	gaiXGraphicsOpenGLReleaseHandles	(gaixg_context *context);

external gaixg_font* 		gaiXGraphicsLoadFont				(gaixg_context *context, char *filename, int size, bool dynamic = false);
external gaixg_font*    	gaiXGraphicsLoadDynamicFont 		(gaixg_context *context, char *filename, int size);
external gaixg_font*   		gaiXGraphicsLoadStaticFont 			(gaixg_context *context, char *filename, int size);

external int				gaiXGraphicsCompileShader			(gaixg_context *context, char *version, char *vertex, char *fragment);

external gaixg_buffer*		gaiXGraphicsBufferCreate			(gaixg_context *context, size_t buffer_size, size_t size_per_segment, gaixg_buffer_flags_enum flags = XGP_BUFFER_STATIC);
external void 				gaiXGraphicsBufferMap 				(gaixg_context *context, gaixg_buffer *buffer);
external void 				gaiXGraphicsBufferUnmap 			(gaixg_context *context, gaixg_buffer *buffer);
external void 				gaiXGraphicsBufferPushData			(gaixg_context *context, gaixg_buffer *buffer, void *data, size_t size);
external void 				gaiXGraphicsBufferPushSubData 		(gaixg_context *context, gaixg_buffer *buffer, void *data, size_t size);
external void 				gaiXGraphicsBufferReset				(gaixg_context *context, gaixg_buffer *buffer);


GAI_DEF void
gaiXGraphicsContextRelease(gaixw_context *window, void *context)
{
	gaixg_context *c = (gaixg_context *) context;
	free(c->builtin_font_shader.memory);

	gaiXGraphicsOpenGLReleaseHandles(c);
}

GAI_DEF void
gaiXGraphicsOpenGLReleaseHandles(gaixg_context * context)
{
	glDeleteTextures(context->textures.length(), context->textures.elements);

	gai_fiz( context->programs.length() ) 	glDeleteProgram(context->programs.elements[i]);
	gai_fiz( context->buffers.length() ) 	glDeleteBuffers(1, &context->buffers.elements[i].handle);
}

GAI_DEF int
gaiXGraphicsCreateContext(gaixg_context *context, gaixw_context *window)
{
	context->window 							= window;
	context->builtin_font_shader.default_font	= gaiXGraphicsLoadFont(context, "c:/windows/fonts/consolab.ttf", 12, false);
	if (!context->builtin_font_shader.default_font) return 0;

	context->builtin_font_shader.memory 		= malloc(gai_megabytes(16));
	if (!context->builtin_font_shader.memory) 		return 0;

	char *version = R"GLSL(#version 330)GLSL";
	char *vertex  = R"GLSL(
			layout (location = 0) in vec2 in_uv;
			layout (location = 1) in vec3 in_color;
			layout (location = 2) in vec3 in_vertex;
			uniform mat4 transform;
			out vec4 out_color;
			out vec2 out_uv;
			void main()
			{
				gl_Position = transform * vec4(in_vertex.x, in_vertex.y, in_vertex.z, 1);
				out_color 	= vec4(in_color, 1.0);
				out_uv    	= in_uv;
			})GLSL";
	char *frag = R"GLSL(
	        in vec4 out_color;
	        in vec2 out_uv;
	        uniform sampler2D font;
	        void main() {
	        	#if 1
				gl_FragColor = vec4(1.0, 1.0, 1.0, texture(font, out_uv).r) * out_color;
				#else
				float tex = texture2D(font, out_uv).r;
				float a = clamp(tex.r * 2.0, 0.0, 1.0);
				gl_FragColor = vec4(1.0, 1.0, 1.0, a) * out_color;
				#endif
	        })GLSL";

	context->builtin_font_shader.program    = gaiXGraphicsCompileShader(context, version, vertex, frag);
	context->builtin_font_shader.transform 	= glGetUniformLocation(context->builtin_font_shader.program, "transform");

	gaiXWindowRegisterDeInitCallback(window, gaiXGraphicsContextRelease, (void*) context);

	return 1;
}

int
_gaiXGraphicsParseEscapeSequence(float *x, float *y, wchar_t *text, float init_x, float init_y, gaixg_font *font, v4 *c)
{
	switch (*text)
	{
		case '\n': { *y += font->height; *x = init_x; return 1; } break;
		case '\t': { stbtt_packedchar *space = font->characters + ' '; float xadv = space->xadvance; *x += xadv * 4; return 1; } break;
	}
	return 0;
}

inline gaixg_text_rect
gaiXGraphicsGetTextRect(gaixg_context *context, float x, float y, wchar_t *text, gaixg_font *font)
{
	gaixg_text_rect result = { (int)x, (int)y, 0, 0};
	if ( font == 0 ) font = context->builtin_font_shader.default_font;
	float width = 0, height = font->height;
	while (*text)
	{
		stbtt_packedchar *c = font->characters + (int)(*text);
		if (*text == '\t') 		width += c->xadvance * 4;
		else if (*text == '\n') 	height += font->height;
		else 					width += c->xadvance;
		*text++;
	}
	result.w = (int)width;
	result.h = (int)height;
	return result;
}

inline gaixg_text_rect
gaiXGraphicsGetTextRect(gaixg_context *context, float x, float y, char *text, gaixg_font *font)
{
	wchar_t utf8_text[4096] = {};
	MultiByteToWideChar(CP_UTF8, 0, text, -1, utf8_text, 4095);
	return gaiXGraphicsGetTextRect(context, x, y, utf8_text, font);
}

inline GAI_DEF void
gaiXGraphicsDrawDynamicText2D(gaixg_context * context, float *x, float *y, float *z, wchar_t *text, v4 color, gaixg_font * font, int random)
{
	if ( font == 0 ) font = context->builtin_font_shader.default_font;

	gaixg_layout32 *start 	= (gaixg_layout32*) context->builtin_font_shader.memory;
	gaixg_layout32 *current = start;

	GAIXG_ASSERT(start);
	if (start == 0) return;

	float initial_x = *x;
	float initial_y = *y;

	// Add the font height to given y-coord so that the topleft of the text is where [x, y] is actually specifed
	while (*text)
	{
		int advance = _gaiXGraphicsParseEscapeSequence(x, y, text, initial_x, initial_y, font, &color);
		if (advance > 0)
		{
			text += advance;
			continue;
		}

		if (random == 1)
		{
			color.g = (rand() % 255) / 255.f;
		} else if (random == 2)
		{
			color.g = (rand() % 255) / 255.f;
			*x = initial_x;
			*y += font->height;
		}

		stbtt_aligned_quad q;
		stbtt_GetPackedQuad(font->characters, font->texture_size, font->texture_size, *text, x, y, &q, 0);
		*current++ = {q.s1, q.t1, color.r, color.g, color.b, q.x1, q.y1, *z};
		*current++ = {q.s0, q.t1, color.r, color.g, color.b, q.x0, q.y1, *z};
		*current++ = {q.s0, q.t0, color.r, color.g, color.b, q.x0, q.y0, *z};
		*current++ = {q.s1, q.t0, color.r, color.g, color.b, q.x1, q.y0, *z};
		*current++ = {q.s0, q.t0, color.r, color.g, color.b, q.x0, q.y0, *z};
		*current++ = {q.s1, q.t1, color.r, color.g, color.b, q.x1, q.y1, *z};
		*text++;
	}
	gaiXGraphicsBufferPushSubData(context, font->buffer, start, (current - start) * sizeof(gaixg_layout32) );
}

inline GAI_DEF void
gaiXGraphicsDrawDynamicText2D(gaixg_context * context, float *x, float *y, float *z, char *text, v4 color, gaixg_font * font)
{
	wchar_t utf8_text[4096] = {};
	MultiByteToWideChar(CP_UTF8, 0, text, -1, utf8_text, 4095);
	return gaiXGraphicsDrawDynamicText2D(context, x, y, z, utf8_text, color, font);
}

inline GAI_DEF void
gaiXGraphicsDrawDynamicText2D(gaixg_context * context, float x, float y, float z, char *text, v4 color, gaixg_font * font)
{
	wchar_t utf8_text[4096] = {};
	MultiByteToWideChar(CP_UTF8, 0, text, -1, utf8_text, 4095);
	return gaiXGraphicsDrawDynamicText2D(context, &x, &y, &z, utf8_text, color, font);
}

GAI_DEF void
gaiXGraphicsRenderDynamicText2D(gaixg_context * context)
{
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			glUseProgram(context->builtin_font_shader.program);
			glViewport(0, 0, context->window->info.width, context->window->info.height);
			m4x4 transform = Orthographic(0, context->window->info.width, 0, context->window->info.height, 10, -10);
			glUniformMatrix4fv(context->builtin_font_shader.transform, 1, GL_TRUE, (const GLfloat*) &transform);

			glEnable(GL_DEPTH_TEST);
			glDepthRange(10, -10);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//gai_fiz(context->fonts.length())
			for (int i = context->fonts.length() - 1; i >= 0; i--)
			{
				gaixg_font* font = context->fonts.get(i);
				if (font)
				{
					glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, font->buffer->handle);
					glBindTexture(GL_TEXTURE_2D, font->texture);
					glBindVertexArray(font->buffer->vao);
					#if 0
					glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, uv) );
					glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, color) );
					glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, position) );

					glDrawArrays(GL_TRIANGLES, 0, font->buffer->used / font->buffer->size_per_segment);

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);
					#endif
					glDrawArrays(GL_TRIANGLES, 0, font->buffer->used / font->buffer->size_per_segment);
					gaiXGraphicsBufferReset(context, font->buffer);
				}
			}

			glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, 0);
			glUseProgram(0);
		} break;
	}

}

GAI_DEF gaixg_buffer *
gaiXGraphicsBufferCreate(gaixg_context * context, size_t buffer_size, size_t size_per_segment, gaixg_buffer_flags_enum flags)
{
	gaixg_buffer *buffer      = context->buffers.append();
	buffer->size                = buffer_size;
	buffer->size_per_segment    = size_per_segment;
	buffer->used                = 0;
	buffer->flags               = flags;

	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			glGenBuffers(1, &buffer->handle);
			glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, buffer->handle);

			glGenVertexArrays(1, &buffer->vao);
			glBindVertexArray(buffer->vao);
			glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, uv) );
			glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, color) );
			glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout32), (const GLvoid*) gai_offsetof(gaixg_layout32, position) );
			glBindVertexArray(0);

			switch (flags)
			{
				case XGP_BUFFER_STATIC:
				{
					glBufferData(0x8892 /*  GL_ARRAY_BUFFER */, buffer_size, 0, 0x88E4 /* GL_STATIC_DRAW */);
				} break;
				case XGP_BUFFER_STREAM:
				{
					glBufferData(0x8892 /*  GL_ARRAY_BUFFER */, buffer_size, 0, 0x88E0 /* GL_STREAM_DRAW */);
				} break;
				case XGP_BUFFER_DYNAMIC:
				{
					glBufferStorage(0x8892 /*  GL_ARRAY_BUFFER */, buffer_size, 0, 0x0100 /* GL_DYNAMIC_STORAGE_BIT */);
				} break;
				case XGP_BUFFER_PERSISTENT_MEMORY:
				{
					if (flags & XGP_BUFFER_WRITE)
					{
						glBufferStorage(0x8892 /*  GL_ARRAY_BUFFER */, buffer_size, 0, 0x0002 /* GL_MAP_WRITE_BIT */ | 0x0040 /* GL_MAP_PERSISTENT_BIT */);
						buffer->address  = (gaixg_layout64 *) glMapBufferRange(0x8892 /*  GL_ARRAY_BUFFER */, 0, buffer_size, 0x0002 /* GL_MAP_WRITE_BIT */ | 0x0040 /* GL_MAP_PERSISTENT_BIT */);
					}
					else if (flags & XGP_BUFFER_READ)
					{
						glBufferStorage(0x8892 /*  GL_ARRAY_BUFFER */, buffer_size, 0, 0x0001 /* GL_MAP_READ_BIT */ | 0x0040 /* GL_MAP_PERSISTENT_BIT */);
						buffer->address  = (gaixg_layout64 *) glMapBufferRange(0x8892 /*  GL_ARRAY_BUFFER */, 0, buffer_size, 0x0001 /* GL_MAP_READ_BIT */ | 0x0040 /* GL_MAP_PERSISTENT_BIT */);
					}
				} break;
			}
			glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, 0);
		} break;
	}
	return buffer;
}

GAI_DEF void
gaiXGraphicsBufferReset(gaixg_context * context, gaixg_buffer * buffer)
{
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			buffer->used = 0;
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferMap(gaixg_context * context, gaixg_buffer * buffer)
{
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			buffer->address = glMapBuffer(0x8892 /*  GL_ARRAY_BUFFER */, 0x88B9 /* GL_WRITE_ONLY */);
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferUnmap(gaixg_context * context, gaixg_buffer * buffer)
{
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			if (buffer->address) glUnmapBuffer(0x8892 /*  GL_ARRAY_BUFFER */);
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferPushSubData(gaixg_context * context, gaixg_buffer * buffer, void *data, size_t size)
{
	GAIXG_ASSERT( (buffer->used + size) < buffer->size );
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, buffer->handle);
			glBufferSubData(0x8892 /*  GL_ARRAY_BUFFER */, buffer->used, size, data);
			buffer->used += size;
			glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, 0);
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferPushData(gaixg_context * context, gaixg_buffer * buffer, void *data, size_t size)
{
	GAIXG_ASSERT( (buffer->used + size) < buffer->size );
	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			if (buffer->address)
			{
				void *dest = ((u8*) buffer->address) + buffer->used;
				memcpy(dest, data, size);
				buffer->used += size;
			}
		}
	}
}

GAI_DEF gaixg_font *
gaiXGraphicsLoadFont(gaixg_context * context, char *filename, int size, bool dynamic)
{
	if ( gai_file_exists(filename) == 0 ) return 0;

	int index       = context->fonts.length();
	int oversample  = 4;
	int texture_dim = 4096;
	stbtt_pack_context font_context;

	gaixg_font *font = context->fonts.append();
	font->texture_size = texture_dim;
	font->height       = size;

	// Load file into temporary buffer
	unsigned char *font_buffer;
	long font_buffer_size = 0;
	FILE *File = fopen(filename, "rb");
	fseek(File, 0, SEEK_END);
	font_buffer_size = ftell(File);
	rewind(File);

	font_buffer = (unsigned char *) malloc(font_buffer_size);
	fread(font_buffer, 1, font_buffer_size, File);
	fclose(File);

	// Create temporary bitmap buffer for the texture
	unsigned char *bitmap_buffer = (unsigned char *) malloc( texture_dim * texture_dim );

	stbtt_PackBegin(&font_context, bitmap_buffer, texture_dim, texture_dim, 0, 1, 0);
	stbtt_PackSetOversampling(&font_context, oversample, oversample);
	stbtt_PackFontRange(&font_context, font_buffer, 0, size, 0, 255, font->characters);
	stbtt_PackEnd(&font_context);

	gaixw_renderer_enum type = context->window->renderer.type;
	switch (type)
	{
		case gaixwRendererOpenGL:
		{
			#if 1
			glGenTextures(1, &font->texture);
			glBindTexture(GL_TEXTURE_2D, font->texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_dim, texture_dim, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap_buffer);
			glBindTexture(GL_TEXTURE_2D, 0);
			#endif
		}
	}

	free(bitmap_buffer);
	free(font_buffer);

	font->buffer = gaiXGraphicsBufferCreate(context, gai_megabytes(16), sizeof(gaixg_layout32), (dynamic ? XGP_BUFFER_DYNAMIC : XGP_BUFFER_STATIC));

	context->textures.append(font->texture);
	return font;
}

GAI_DEF gaixg_font *
gaiXGraphicsLoadDynamicFont(gaixg_context * context, char *filename, int size)
{
	return gaiXGraphicsLoadFont(context, filename, size, true);
}

GAI_DEF void
gaiXGraphicsCreateTexture(gaixw_renderer_enum type)
{

}

GAI_DEF int
gaiXGraphicsCompileShader(gaixg_context * context, char *version, char *vertex, char *fragment)
{
	switch (context->window->renderer.type)
	{
		case gaixwRendererOpenGL:
		{
			GLuint vertex_shader_id = glCreateShader(0x8B31 /* GL_VERTEX_SHADER */);
			GLchar *vertex_shader_code[] =
			{
				version,
				vertex,
			};
			glShaderSource(vertex_shader_id, gai_array_length(vertex_shader_code), vertex_shader_code, 0);
			glCompileShader(vertex_shader_id);

			GLuint fragment_shader_id = glCreateShader(0x8B30 /* GL_FRAGMENT_SHADER */);
			GLchar *fragment_shader_code[] =
			{
				version,
				fragment,
			};
			glShaderSource(fragment_shader_id, gai_array_length(fragment_shader_code), fragment_shader_code, 0);
			glCompileShader(fragment_shader_id);

			GLuint program_id = glCreateProgram();
			glAttachShader(program_id, vertex_shader_id);
			glAttachShader(program_id, fragment_shader_id);
			glLinkProgram(program_id);

			glValidateProgram(program_id);
			GLint linked = false;
			glGetProgramiv(program_id, 0x8B82 /* GL_LINK_STATUS */, &linked);
			if (!linked)
			{
				GLsizei ignored;
				char vertex_code_errors[4096];
				char fragment_code_errors[4096];
				char program_errors[4096];
				glGetShaderInfoLog(vertex_shader_id, sizeof(vertex_code_errors), &ignored, vertex_code_errors);
				glGetShaderInfoLog(fragment_shader_id, sizeof(fragment_code_errors), &ignored, fragment_code_errors);
				glGetProgramInfoLog(program_id, sizeof(program_errors), &ignored, program_errors);

				GAIXG_ASSERT(!"Shader validation failed");
			}

			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);

			context->programs.append(program_id);

			return program_id;
		} break;
	}
	return -1;
}

/*
GAI_DEF void
gaiXGraphicsSetLayout(gaixg_context *context)
{
	switch (context->window->renderer.type)
	{
		case gaixwRendererOpenGL:
		{
			glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, uv) );
			glEnableVertexAttribArray(1); glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, color) );
			glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, normal) );
			glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, position) );
			glEnableVertexAttribArray(4); glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, reserved) );
		} break;
	}
}

GAI_DEF void
gaiXGraphicsClearLayout(gaixg_context *context)
{
	switch (context->window->renderer.type)
	{
		case gaixwRendererOpenGL:
		{
			glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, uv) );
			glEnableVertexAttribArray(1); glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, color) );
			glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, normal) );
			glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, position) );
			glEnableVertexAttribArray(4); glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(gaixg_layout64), (const GLvoid*) gai_offsetof(gaixg_layout64, reserved) );
		} break;
	}
}
*/

#define GAI_XGRAPHICS_H
#endif