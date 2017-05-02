#ifndef _GAI_INCLUDE_RENDERER_OPENGL_H
#include "gai_renderbuffer.h"

#ifdef GAIRGL_STATIC
	#define GAIRGL_API static
#else
	#define GAIRGL_API extern
#endif

#ifndef GAIRGL_ASSERT
	#include <assert.h>
	#define GAIRGL_ASSERT(x) assert(x)
#endif

#define GL_ARRAY_BUFFER 								0x8892

/* GL_ARB_buffer_storage */
#define GL_MAP_READ_BIT                                	0x0001
#define GL_MAP_WRITE_BIT                               	0x0002
#define GL_MAP_PERSISTENT_BIT                          	0x0040
#define GL_MAP_COHERENT_BIT                            	0x0080
#define GL_DYNAMIC_STORAGE_BIT                         	0x0100
#define GL_CLIENT_STORAGE_BIT                          	0x0200
#define GL_BUFFER_IMMUTABLE_STORAGE                    	0x821F
#define GL_BUFFER_STORAGE_FLAGS                        	0x8220
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT            	0x00004000
/* GL_ARB_buffer_storage */

/* GL_ARB_texture_storage */
#define GL_TEXTURE_IMMUTABLE_FORMAT       				0x912F
/* GL_ARB_texture_storage */

/* GL_ARB_map_buffer_range */
#define GL_MAP_INVALIDATE_RANGE_BIT    					0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT   					0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT      					0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT      					0x0020
/* GL_ARB_map_buffer_range */

/* GL_ARB_sync */
#define GL_MAX_SERVER_WAIT_TIMEOUT         				0x9111
#define GL_OBJECT_TYPE                     				0x9112
#define GL_SYNC_CONDITION                  				0x9113
#define GL_SYNC_STATUS                     				0x9114
#define GL_SYNC_FLAGS                      				0x9115
#define GL_SYNC_FENCE                      				0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE      				0x9117
#define GL_UNSIGNALED                      				0x9118
#define GL_SIGNALED                        				0x9119
#define GL_SYNC_FLUSH_COMMANDS_BIT         				0x00000001
#define GL_TIMEOUT_IGNORED                 				0xFFFFFFFFFFFFFFFFull
#define GL_ALREADY_SIGNALED                				0x911A
#define GL_TIMEOUT_EXPIRED                 				0x911B
#define GL_CONDITION_SATISFIED             				0x911C
#define GL_WAIT_FAILED                     				0x911D
/* GL_ARB_sync */

struct gairgl
{
	unsigned int  	GL_ARB_buffer_storage;
	unsigned int 	GL_ARB_texture_storage;
	unsigned int 	GL_ARB_map_buffer_range;
	unsigned int 	GL_ARB_sync;

	u32 shader;
	u32 transform;

	u32 vao;
	u32 vbo;
};

GAIRGL_API void 	gairgl_Initialize(gairgl *opengl);
GAIRGL_API GLuint 	gairgl_LoadShader(char *version, char *vertex, char *fragment);

#ifdef GAIRGL_IMPLEMENTATION

#define USE_BUFFER_STORAGE 1



#if _WIN32

void* _gairgl_IsOpenGLContextAvailable()
{
	HGLRC opengl_context = wglGetCurrentContext();
	GAIRGL_ASSERT(opengl_context);
	return opengl_context;
}

#endif

GAIRGL_API void
gairgl_Initialize(gairgl *opengl)
{
	_gairgl_IsOpenGLContextAvailable();
	GAIRGL_ASSERT(opengl);


#define IS_EXTENSION_SUPPORTED(ext) opengl->ext = gaixw_GLIsSupported(#ext)
	IS_EXTENSION_SUPPORTED(GL_ARB_buffer_storage);
	IS_EXTENSION_SUPPORTED(GL_ARB_texture_storage);
	IS_EXTENSION_SUPPORTED(GL_ARB_map_buffer_range);
	IS_EXTENSION_SUPPORTED(GL_ARB_sync);

#undef IS_EXTENSION_SUPPORTED


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
	        uniform sampler2D tex;
	        void main() {
	        	#if 0
				gl_FragColor = out_color;
				#else
				vec4 tex = texture2D(tex, out_uv);
				gl_FragColor = tex * out_color;
				#endif
	        })GLSL";

	GLuint shader = gairgl_LoadShader(version, vertex, frag);
	GAIRGL_ASSERT(shader != 0);

	opengl->shader 		= shader;
	opengl->transform 	= glGetUniformLocation(shader, "transform");

	glGenBuffers(1, &opengl->vbo);
	glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, opengl->vbo);
	#if USE_BUFFER_STORAGE
	glBufferStorage(0x8892, 1024 * 1024 * 100, 0, GL_MAP_WRITE_BIT);
	#else
	glBufferData(0x8892, 0, 0,  0x88E8); //0x88E0);
	#endif

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
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	//glBlendFunc(GL_ONE, GL_ONE);

	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
}

GAIRGL_API void
gairgl_Destroy(gairgl *opengl)
{
	glDeleteBuffers(1, &opengl->vbo);
	glDeleteVertexArrays(1, &opengl->vao);
	glDeleteProgram(opengl->shader);
}

#define U32FromHandle(handle) ((u32)   (size_t) (handle))
#define HandleFromU32(value)  ((void*) (size_t) (value))
u32
gairgl_LoadTexture(gairgl *opengl, gairb_renderbuffer *commands, int quad_id)
{
	if (!commands->quad_textures) return -1;
	loaded_bitmap *b = commands->quad_textures[quad_id];
	GAIRGL_ASSERT(b);
	if (!b) return -1;
	if (b->handle) return U32FromHandle(b->handle);
	else
	{
		if (!b->memory) return U32FromHandle(commands->default_texture->handle);
		
		GLuint handle;
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (!b->is_font_texture)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, b->width, b->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, b->memory);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, b->width, b->height, 0, GL_RED, GL_UNSIGNED_BYTE, b->memory);
		}
		b->is_loaded = 1;
		b->handle = HandleFromU32(handle);
	}
	return -1;
}

inline GAIRGL_API void
gairgl_UnloadTexture(void *handle)
{
	if (handle) glDeleteTextures(1, (GLuint *) handle);
}

inline GAIRGL_API void
gairgl_ManageOpenGLTextures()
{

}



inline GAIRGL_API unsigned int
gairgl_Render(gairgl *opengl, gairb_renderbuffer *commands, v2 draw_region)
{
	int texture_switches_per_frame = 0;
	int draw_calls = 0;
	static GLsync fence = 0;
	if (commands)
	{
		size_t offset 	 = 0;
		size_t data_size = commands->vertex_count * sizeof(gairb_textured_vertex);
		#if USE_BUFFER_STORAGE

		if (data_size)
		{
			glUseProgram(opengl->shader);
			glBindBuffer(GL_ARRAY_BUFFER, opengl->vbo);

			glClearColor( commands->clear_color.x, commands->clear_color.y, commands->clear_color.z, commands->clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if ( fence ) glClientWaitSync(fence, 1 /* GL_SYNC_FLUSH_COMMANDS_BIT */, 0xFFFFFFFFFFFFFFFFull /* GL_TIMEOUT_IGNORED */);

			void *old_data = glMapBufferRange(GL_ARRAY_BUFFER, offset, data_size, GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
			// Modify buffer, flush, and unmap.
			memcpy(old_data, commands->vertex_array, data_size);
			glFlushMappedBufferRange(GL_ARRAY_BUFFER, offset, data_size);
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindVertexArray(opengl->vao);
		}
		#else

		glUseProgram(opengl->shader);
		glBindBuffer(GL_ARRAY_BUFFER, opengl->vbo);

		glClearColor( commands->clear_color.x, commands->clear_color.y, commands->clear_color.z, commands->clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBufferSubData(0x8892, 0, commands->vertex_count * sizeof(gairb_textured_vertex), commands->vertex_array);
		glBufferData(0x8892,  data_size, commands->vertex_array, 0x88E8);
		glBindVertexArray(opengl->vao);
		#endif

		static u32 active_texture;
		for (u8 *header_at = commands->pushbuffer_base; header_at < commands->pushbuffer_at; )//header_at += sizeof(gairb_entry_header))
		{
			gairb_entry_header *header = (gairb_entry_header *) header_at;
			switch (header->type)
			{
				case gairb_EntryType_gairb_entry_textured_quads:
				{
					header_at += sizeof(gairb_entry_textured_quads);

					gairb_entry_textured_quads *entry = (gairb_entry_textured_quads *) header;

					glUniformMatrix4fv(opengl->transform, 1, GL_TRUE, entry->setup.transform.E[0]);

					#if 0
					for (u32 vertex_index = entry->vertex_array_offset; vertex_index < (entry->vertex_array_offset + 6 * entry->quad_count); vertex_index += 6)
					{

						u32 textureid = gairgl_LoadTexture(opengl, commands, vertex_index >> 2);
						if (textureid != -1)
						{
							if (active_texture != textureid)
							{
								glBindTexture(GL_TEXTURE_2D, textureid);
								active_texture = textureid;
							}
						}

						glDrawArrays(GL_TRIANGLE_STRIP, vertex_index, 6);
					}
					#else
					int vertex_index = (entry->vertex_array_offset != 0 ? entry->vertex_array_offset / 6 : entry->vertex_array_offset);
					u32 textureid = gairgl_LoadTexture(opengl, commands, vertex_index);
					if (textureid != -1 && active_texture != textureid)
					{
						glBindTexture(GL_TEXTURE_2D, textureid);
						texture_switches_per_frame++;
						active_texture = textureid;
					}
					glDrawArrays(0x0005 /* GL_TRIANGLE_STRIP */, entry->vertex_array_offset, (6 * entry->quad_count));
					draw_calls++;
					#endif
				} break;
				default: { GAIRGL_ASSERT(!"Invalid code path") ;}
			}
		}

		if (data_size)
		{
			glViewport(0, 0, (int)draw_region.x, (int)draw_region.y);
			//fence = glFenceSync(0x9117 /* GL_SYNC_GPU_COMMANDS_COMPLETE */, 0);
		}
	}
	return draw_calls;
}

GAIRGL_API GLuint
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