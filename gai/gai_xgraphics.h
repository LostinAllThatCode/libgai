#ifndef GAI_XGRAPHICS_H

#include "gai_types.h"
#include "gai_utils.h"
#include "gai_math.h"

struct gai_xgp_layout36 //  36 bytes, 9 floats
{
	float uv[2];
	float color[4];
	float position[3];
};

struct gai_xgp_layout48 //  48 bytes, 12 floats
{
	float uv[2];
	float color[4];
	float normal[3];
	float position[3];
}; // NOTE: Code XL interleaved data format T2F_C4F_N3F_V3F (0 bytes offset, 0 bytes stride)

struct gai_xgp_layout64 //  64 bytes, 16 floats
{
	float uv[2];
	float color[4];
	float normal[3];
	float position[3];
	float reserved[4];
}; // NOTE: Code XL interleaved data format T2F_C4F_N3F_V3F (0 bytes offset, 16 bytes stride) ** 16 bytes stride are reserved for possible later use

#define gaiXGraphicsSetLayoutOpenGL( layout ) \
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(layout), (const GLvoid*) gai_offsetof(layout, uv) ); \
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(layout), (const GLvoid*) gai_offsetof(layout, color) ); \
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(layout), (const GLvoid*) gai_offsetof(layout, normal) ); \
	glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(layout), (const GLvoid*) gai_offsetof(layout, position) ); \
	glEnableVertexAttribArray(4); glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(layout), (const GLvoid*) gai_offsetof(layout, reserved) )

#define gaiXGraphicsClearLayoutOpenGL( layout ) \
	glEnableVertexAttribArray(0); \
	glEnableVertexAttribArray(1); \
	glEnableVertexAttribArray(2); \
	glEnableVertexAttribArray(3); \
	glEnableVertexAttribArray(4);

enum gai_xgp_buffer_flags_enum
{
	XGP_BUFFER_STATIC,
	XGP_BUFFER_DYNAMIC,
	XGP_BUFFER_STREAM,
	XGP_BUFFER_PERSISTENT_MEMORY,

	XGP_BUFFER_READ,
	XGP_BUFFER_WRITE,
};
struct gai_xgp_buffer
{
	gai_xgp_buffer_flags_enum 	flags;
	u32 						handle;
	size_t  					size;
	size_t  					used;
	size_t              		size_per_segment;
	void			 			*address;
};


struct gai_xgp_font
{
	u32               	texture;
	i32 			 	texture_size;
	i32					height;
	stbtt_packedchar 	characters[255];
	gai_xgp_buffer      *buffer;
};

#define GAI_XGP_DYN_STR_DEF_BUFSIZE gai_megabytes(8);
struct gai_xgp_drawcall_dynamic_string
{
	u32					start;
	u32					length;
	gai_xgp_font        *font;
};

GAI_DYNAMIC( gai_xgp_font_array,    gai_xgp_font );
GAI_DYNAMIC( gai_xgp_buffer_array,  gai_xgp_buffer );
GAI_DYNAMIC( gai_xgp_opengl_handles, u32 );

struct gai_xgp_context
{
	gai_xwnd 				*window;
	gai_xgp_font 			*default_font;

	gai_xgp_font_array 		fonts;
	gai_xgp_buffer_array    buffers;

	gai_xgp_opengl_handles 	textures;
	gai_xgp_opengl_handles 	programs;

	struct
	{
		u32 			program;
		u32 			transform;
	} builtin_font_shader;
};


extern void 			gaiXGraphicsCreateContext			(gai_xgp_context *context, gai_xwnd *window);
extern void 			gaiXGraphicsDrawDynamicText2D		(gai_xgp_context *context, float x, float y, char *text, v4 color = V4(1, 1, 1, 1), gai_xgp_font *font = 0);

extern void 			gaiXGraphicsOpenGLReleaseHandles	(gai_xgp_context *context);

extern gai_xgp_font* 	gaiXGraphicsLoadFont				(gai_xgp_context *context, char *filename, int size, bool dynamic = false);
extern gai_xgp_font*    gaiXGraphicsLoadDynamicFont 		(gai_xgp_context *context, char *filename, int size);
extern gai_xgp_font*    gaiXGraphicsLoadStaticFont 			(gai_xgp_context *context, char *filename, int size);

extern int				gaiXGraphicsCompileShader			(gai_xgp_context *context, char *version, char *vertex, char *fragment);

extern gai_xgp_buffer*	gaiXGraphicsBufferCreate			(gai_xgp_context *context, size_t buffer_size, size_t size_per_segment, gai_xgp_buffer_flags_enum flags = XGP_BUFFER_STATIC);
extern void 			gaiXGraphicsBufferMap 				(gai_xgp_context *context, gai_xgp_buffer *buffer);
extern void 			gaiXGraphicsBufferUnmap 			(gai_xgp_context *context, gai_xgp_buffer *buffer);
extern void 			gaiXGraphicsBufferPushData			(gai_xgp_context *context, gai_xgp_buffer *buffer, void *data, size_t size);
extern void 			gaiXGraphicsBufferPushSubData 		(gai_xgp_context *context, gai_xgp_buffer *buffer, void *data, size_t size);
extern void 			gaiXGraphicsBufferReset				(gai_xgp_context *context, gai_xgp_buffer *buffer);

extern void gaiXGraphicsSetLayout64x(gai_xgp_context *context);

void gaiXGraphicsContextRelease(void *context)
{
	gai_xgp_context *c = (gai_xgp_context *) context;
	gaiXGraphicsOpenGLReleaseHandles(c);
}

GAI_DEF void
gaiXGraphicsCreateContext(gai_xgp_context *context, gai_xwnd *window)
{
	context->window 				= window;
	context->default_font			= gaiXGraphicsLoadDynamicFont(context, "c:/windows/fonts/consolab.ttf", 10);

	char *version = R"GLSL(#version 440)GLSL";
	char *vertex  = R"GLSL(
			layout (location = 0) in vec2 in_uv;
			layout (location = 1) in vec4 in_color;
			layout (location = 2) in vec3 in_normal;
			layout (location = 3) in vec3 in_vertex;
			uniform mat4 transform;
			out vec4 out_color;
			out vec2 out_uv;
			void main()
			{
				gl_Position = transform * vec4(in_vertex.x, in_vertex.y, 0, 1);
				out_color 	= in_color;
				out_uv    	= in_uv;
			})GLSL";

	#if 1
	char *frag = R"GLSL(
	        in vec4 out_color;
	        in vec2 out_uv;
	        uniform sampler2D font_texture;
	        void main() {
				gl_FragColor = vec4(1.0, 1.0, 1.0, texture(font_texture, out_uv).r) * out_color;
				//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	        })GLSL";
	#else
	char *frag = R"GLSL(
	        in vec4 out_color;
	        in vec2 out_uv;
	        uniform vec4 u_outlineColor = vec4(1,0,0,1);
	        uniform sampler2D font_texture;
			const float smoothing = 3.0/16.0;
			const float outlineWidth = 3.0/16.0;
			const float outerEdgeCenter = 0.5 - outlineWidth;
	        void main() {
    			float distance = texture2D(font_texture, out_uv).r;
    			float alpha = smoothstep(outerEdgeCenter - smoothing, outerEdgeCenter + smoothing, distance);
    			float border = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    			gl_FragColor = vec4( mix(u_outlineColor.rgb, out_color.rgb, border), alpha );
	        })GLSL";
	#endif
	context->builtin_font_shader.program    = gaiXGraphicsCompileShader(context, version, vertex, frag);
	context->builtin_font_shader.transform 	= glGetUniformLocation(context->builtin_font_shader.program, "transform");
	gaiXWindowRegisterDeInitCallback(window, gaiXGraphicsContextRelease, context);
}

GAI_DEF void
gaiXGraphicsDrawDynamicText2D(gai_xgp_context *context, float x, float y, wchar_t *text, v4 color, gai_xgp_font *font)
{
	if ( font == 0 && context->default_font != 0) font = context->default_font;
	y += font->height;

	glBindBuffer(GL_ARRAY_BUFFER, font->buffer->handle);
	while (*text)
	{
		stbtt_aligned_quad q;

		stbtt_GetPackedQuad(font->characters, font->texture_size, font->texture_size, *text, &x, &y, &q, 0);
		*text++;
		#if 0
		gai_xgp_layout64 data[] =
		{
			{q.s1, q.t1, color.r, color.g, color.b, color.a, 0, 0, 0, q.x1, q.y1, 0, 0, 0, 0, 0},
			{q.s0, q.t1, color.r, color.g, color.b, color.a, 0, 0, 0, q.x0, q.y1, 0, 0, 0, 0, 0},
			{q.s0, q.t0, color.r, color.g, color.b, color.a, 0, 0, 0, q.x0, q.y0, 0, 0, 0, 0, 0},
			{q.s1, q.t0, color.r, color.g, color.b, color.a, 0, 0, 0, q.x1, q.y0, 0, 0, 0, 0, 0},
			{q.s0, q.t0, color.r, color.g, color.b, color.a, 0, 0, 0, q.x0, q.y0, 0, 0, 0, 0, 0},
			{q.s1, q.t1, color.r, color.g, color.b, color.a, 0, 0, 0, q.x1, q.y1, 0, 0, 0, 0, 0},
		};
		#else
		gai_xgp_layout64 data[] =
		{
			{q.s1, q.t1, color.r, color.g, color.b, color.a, 0, 0, 0, q.x1, q.y1, 0, 0, 0, 0, 0},
			{q.s0, q.t1, color.r, color.g, color.b, color.a, 0, 0, 0, q.x0, q.y1, 0, 0, 0, 0, 0},
			{q.s0, q.t0, color.r, color.g, color.b, color.a, 0, 0, 0, q.x0, q.y0, 0, 0, 0, 0, 0},
			//{q.s1, q.t0, color.r, color.g, color.b, color.a, 0, 0, 0, q.x1, q.y0, 0, 0, 0, 0, 0},
		};
		#endif
		gaiXGraphicsBufferPushSubData(context, font->buffer, data, sizeof(data));
	}
}


GAI_DEF void
gaiXGraphicsDrawDynamicText2D(gai_xgp_context *context, float x, float y, char *text, v4 color, gai_xgp_font *font)
{
	wchar_t utf8_text[4096] = {};
	MultiByteToWideChar(CP_UTF8, 0, text, strlen(text) * sizeof(char), utf8_text, 4095);
    gaiXGraphicsDrawDynamicText2D(context, x, y, utf8_text, color, font);
}

GAI_DEF void
gaiXGraphicsRenderDynamicText2D(gai_xgp_context *context)
{
	glUseProgram(context->builtin_font_shader.program);
	glViewport(0, 0, context->window->info.width, context->window->info.height);
	m4x4 transform = Orthographic(0, context->window->info.width, 0, context->window->info.height, 1, -1);
	glUniformMatrix4fv(context->builtin_font_shader.transform, 1, GL_TRUE, (const GLfloat*) &transform);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gai_fiz(context->fonts.length())
	{
		gai_xgp_font* font = context->fonts.get(i);
		if (font)
		{
			glBindBuffer(GL_ARRAY_BUFFER, font->buffer->handle);
			glBindTexture(GL_TEXTURE_2D, font->texture);

			gaiXGraphicsSetLayoutOpenGL(gai_xgp_layout64);

			glDrawArrays(GL_TRIANGLES, 0, font->buffer->used / font->buffer->size_per_segment);

			gaiXGraphicsClearLayoutOpenGL(gai_xgp_layout64);
			gaiXGraphicsBufferReset(context, font->buffer);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

GAI_DEF gai_xgp_buffer *
gaiXGraphicsBufferCreate(gai_xgp_context *context, size_t buffer_size, size_t size_per_segment, gai_xgp_buffer_flags_enum flags)
{
	gai_xgp_buffer *buffer   = context->buffers.append();
	buffer->size             = buffer_size;
	buffer->size_per_segment = size_per_segment;
	buffer->used             = 0;
	buffer->flags 			 = flags;

	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			glGenBuffers(1, &buffer->handle);
			glBindBuffer(GL_ARRAY_BUFFER, buffer->handle);

			switch (flags)
			{
				case XGP_BUFFER_STATIC:
				{
					glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, GL_STATIC_DRAW);
				} break;
				case XGP_BUFFER_STREAM:
				{
					glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, GL_STREAM_DRAW);
				} break;
				case XGP_BUFFER_DYNAMIC:
				{
					glBufferStorage(GL_ARRAY_BUFFER, buffer_size, 0, GL_DYNAMIC_STORAGE_BIT);
				} break;
				case XGP_BUFFER_PERSISTENT_MEMORY:
				{
					if (flags & XGP_BUFFER_WRITE)
					{
						glBufferStorage(GL_ARRAY_BUFFER, buffer_size, 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
						buffer->address  = (gai_xgp_layout64 *) glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer_size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
					}
					else if (flags & XGP_BUFFER_READ)
					{
						glBufferStorage(GL_ARRAY_BUFFER, buffer_size, 0, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT);
						buffer->address  = (gai_xgp_layout64 *) glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer_size, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT);
					}
				} break;
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} break;
	}
	return buffer;
}

GAI_DEF void
gaiXGraphicsBufferReset(gai_xgp_context *context, gai_xgp_buffer * buffer)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			buffer->used = 0;
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferMap(gai_xgp_context *context, gai_xgp_buffer * buffer)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			buffer->address = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferUnmap(gai_xgp_context *context, gai_xgp_buffer * buffer)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			if (buffer->address) glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferPushSubData(gai_xgp_context *context, gai_xgp_buffer * buffer, void *data, size_t size)
{
	gai_assert( (buffer->used + size) < buffer->size );
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			glBufferSubData(GL_ARRAY_BUFFER, buffer->used, size, data);
			buffer->used += size;
		}
	}
}

GAI_DEF void
gaiXGraphicsBufferPushData(gai_xgp_context *context, gai_xgp_buffer * buffer, void *data, size_t size)
{
	gai_assert( (buffer->used + size) < buffer->size );
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
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

GAI_DEF void
gaiXGraphicsOpenGLReleaseHandles(gai_xgp_context * context)
{
	glDeleteTextures(context->textures.length(), context->textures.elements);

	gai_fiz( context->programs.length() ) 	glDeleteProgram(context->programs.elements[i]);
	gai_fiz( context->buffers.length() ) 	glDeleteBuffers(1, &context->buffers.elements[i].handle);
}

GAI_DEF gai_xgp_font *
gaiXGraphicsLoadFont(gai_xgp_context * context, char *filename, int size, bool dynamic)
{
	if ( gai_file_exists(filename) == 0 ) return 0;
	int index       = context->fonts.length();
	int oversample  = 4;
	int texture_dim = 4096;
	stbtt_pack_context font_context;

	gai_xgp_font *font = context->fonts.append();
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

	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
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

	font->buffer = gaiXGraphicsBufferCreate(context, gai_megabytes(2), sizeof(gai_xgp_layout64), (dynamic ? XGP_BUFFER_DYNAMIC : XGP_BUFFER_STATIC));

	context->textures.append(font->texture);
	return font;
}

GAI_DEF gai_xgp_font *
gaiXGraphicsLoadDynamicFont(gai_xgp_context * context, char *filename, int size)
{
	return gaiXGraphicsLoadFont(context, filename, size, true);
}

GAI_DEF void
gaiXGraphicsCreateTexture(gai_xwnd_renderer_enum type)
{

}

GAI_DEF int
gaiXGraphicsCompileShader(gai_xgp_context * context, char *version, char *vertex, char *fragment)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
			GLchar *vertex_shader_code[] =
			{
				version,
				vertex,
			};
			glShaderSource(vertex_shader_id, gai_array_length(vertex_shader_code), vertex_shader_code, 0);
			glCompileShader(vertex_shader_id);

			GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
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

			context->programs.append(program_id);

			return program_id;
		} break;
	}
	return -1;
}

/*
GAI_DEF void
gaiXGraphicsSetLayout(gai_xgp_context *context)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, uv) );
			glEnableVertexAttribArray(1); glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, color) );
			glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, normal) );
			glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, position) );
			glEnableVertexAttribArray(4); glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, reserved) );
		} break;
	}
}

GAI_DEF void
gaiXGraphicsClearLayout(gai_xgp_context *context)
{
	switch (context->window->renderer.type)
	{
		case XWND_RENDERER_OPENGL:
		{
			glEnableVertexAttribArray(0); glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, uv) );
			glEnableVertexAttribArray(1); glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, color) );
			glEnableVertexAttribArray(2); glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, normal) );
			glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, position) );
			glEnableVertexAttribArray(4); glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(gai_xgp_layout64), (const GLvoid*) gai_offsetof(gai_xgp_layout64, reserved) );
		} break;
	}
}
*/

#define GAI_XGRAPHICS_H
#endif