#ifndef _GAI_INCLUDE_RENDERER_OPENGL_H
#include "gai_renderbuffer.h"

#ifdef GAIRGL_STATIC
	#define GAIRGL_DEF static
#else
	#define GAIRGL_DEF extern
#endif

#ifndef GAIRGL_ASSERT
	#include <assert.h>
	#define GAIRGL_ASSERT(x) assert(x)
#endif

struct gairgl
{
	u32 shader;
	u32 transform;

	u32 vao;
	u32 vbo;
};

GAIRGL_DEF void 	gairgl_Initialize(gairgl *opengl);
GAIRGL_DEF GLuint 	gairgl_LoadShader(char *version, char *vertex, char *fragment);

#ifdef GAIRGL_IMPLEMENTATION

GAIRGL_DEF void
gairgl_Initialize(gairgl *opengl)
{
	GAIRGL_ASSERT(opengl);

	char *version = R"GLSL(#version 330)GLSL";
	char *vertex  = R"GLSL(
			layout (location = 0) in vec4 in_vertex;
			layout (location = 1) in vec2 in_uv;
			layout (location = 2) in vec4 in_color;
			uniform mat4 transform = mat4(1);
			out vec4 out_color;
			out vec2 out_uv;
			void main()
			{
				gl_Position = transform * vec4(in_vertex.x, in_vertex.y, in_vertex.z, in_vertex.w);
				out_color 	= in_color;
				out_uv    	= in_uv;
			})GLSL";
	char *frag = R"GLSL(
	        in vec4 out_color;
	        in vec2 out_uv;
	        uniform sampler2D font;
	        void main() {
	        	#if 1
				gl_FragColor = out_color;
				#else
				float tex = texture2D(font, out_uv).r;
				float a = clamp(tex.r * 2.0, 0.0, 1.0);
				gl_FragColor = vec4(1.0, 1.0, 1.0, a) * out_color;
				#endif
	        })GLSL";

	GLuint shader = gairgl_LoadShader(version, vertex, frag);
	GAIRGL_ASSERT(shader != 0);

	opengl->shader 		= shader;
	opengl->transform 	= glGetUniformLocation(shader, "transform");

	glGenBuffers(1, &opengl->vbo);
	glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, opengl->vbo);
	glBufferData(0x8892, 0, 0, 0x88E0);


	glGenVertexArrays(1, &opengl->vao);
	glBindVertexArray(opengl->vao);
#define GAIRGL_OFFSETOF(st, m) 		((size_t)&(((st *)0)->m))
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(gairb_textured_vertex), (const GLvoid*) GAIRGL_OFFSETOF(gairb_textured_vertex, p) );
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gairb_textured_vertex), (const GLvoid*) GAIRGL_OFFSETOF(gairb_textured_vertex, uv) );
	glEnableVertexAttribArray(2); glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(gairb_textured_vertex), (const GLvoid*) GAIRGL_OFFSETOF(gairb_textured_vertex, color) );
#undef GAIRGL_OFFSETOF
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
}

GAIRGL_DEF void
gairgl_Destroy(gairgl *opengl)
{
	glDeleteBuffers(1, &opengl->vbo);
	glDeleteVertexArrays(1, &opengl->vao);
	glDeleteProgram(opengl->shader);
}

inline GAIRGL_DEF void
gairgl_Render(gairgl *opengl, gairb_renderbuffer *commands, v2 draw_region)
{
	if (commands)
	{
		glClearColor( commands->clear_color.x, commands->clear_color.y, commands->clear_color.z, commands->clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (u8 *header_at = commands->pushbuffer_base; header_at < commands->pushbuffer_at; )//header_at += sizeof(gairb_entry_header))
		{
			gairb_entry_header *header = (gairb_entry_header *) header_at;
			switch (header->type)
			{
				case gairb_EntryType_gairb_entry_textured_quads:
				{
					header_at += sizeof(gairb_entry_textured_quads);

					gairb_entry_textured_quads *entry = (gairb_entry_textured_quads *) header;

					glUseProgram(opengl->shader);
					glUniformMatrix4fv(opengl->transform, 1, GL_TRUE, entry->setup.transform.E[0]);

					glBindBuffer(0x8892, opengl->vbo);
					glBufferData(0x8892, commands->vertex_count * sizeof(gairb_textured_vertex), commands->vertex_array, 0x88E0);

					glBindVertexArray(opengl->vao);

					for (u32 vertex_index = entry->vertex_array_offset; vertex_index < (entry->vertex_array_offset + 4 * entry->quad_count); vertex_index += 4)
					{
						glDrawArrays(GL_TRIANGLE_STRIP, vertex_index, 4);
					}
				} break;
				default: { GAIRGL_ASSERT(!"Invalid code path") ;}
			}
		}
		glViewport(0, 0, (int)draw_region.x, (int)draw_region.y);
	}
}

GAIRGL_DEF GLuint
gairgl_LoadShader(char *version, char *vertex, char *fragment)
{
	GLuint vertex_shader_id = glCreateShader(0x8B31 /* GL_VERTEX_SHADER */);
	GLchar *vertex_shader_code[] =
	{
		version,
		vertex,
	};
	glShaderSource(vertex_shader_id, (sizeof(vertex_shader_code) / sizeof((vertex_shader_code)[0])), vertex_shader_code, 0);
	glCompileShader(vertex_shader_id);

	GLuint fragment_shader_id = glCreateShader(0x8B30 /* GL_FRAGMENT_SHADER */);
	GLchar *fragment_shader_code[] =
	{
		version,
		fragment,
	};
	glShaderSource(fragment_shader_id,  (sizeof(fragment_shader_code) / sizeof((fragment_shader_code)[0])), fragment_shader_code, 0);
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

		GAIRB_ASSERT(!"Shader validation failed");
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

#endif

#define _GAI_INCLUDE_RENDERER_OPENGL_H
#endif