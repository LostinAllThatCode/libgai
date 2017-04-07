
#if _DEBUG
	#define GAIXW_DEBUG
#endif

#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#include "gai_renderer.h"

#define gai_array_length(array)      						(sizeof(array)/sizeof((array)[0]))
#define gai_offsetof(st, m) 								((size_t)&(((st *)0)->m))

u32
LoadShader(char *version, char *vertex, char *fragment)
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

		GAIRB_ASSERT(!"Shader validation failed");
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

struct render_layout
{
	m4x4 projection;
};

int main(int argc, char **argv)
{
	gaixw_context window;
	if (!gaiXWindow(&window, "yolo")) return -1;
	gaiXWindowSetVSYNC(&window, 1);

	u32 pbuffersize = 1024 * 1024 * 2;
	u8 *pbuffer = (u8*) VirtualAlloc(0, pbuffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	u32 vertex_count_max = 512;
	gairb_textured_vertex *vbuffer = (gairb_textured_vertex*) VirtualAlloc(0, sizeof(gairb_textured_vertex) * vertex_count_max, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	printf("Pushbuffer Size: %.2f mb\n", pbuffersize / 1024.f / 1024.f);
	printf("VertexArray Size: %.2f mb\n", (sizeof(gairb_textured_vertex) * vertex_count_max) / 1024.f / 1024.f);

	char *version = R"GLSL(#version 330)GLSL";
	char *vertex  = R"GLSL(
			layout (location = 0) in vec4 in_vertex;
			layout (location = 1) in vec2 in_uv;
			uniform vec4 in_color = vec4(1.0, 1.0, 1.0, 1.0);
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

	u32 shader = LoadShader(version, vertex, frag);
	if (!shader) return -1;

	u32 color = glGetUniformLocation(shader, "in_color");
	u32 proj = glGetUniformLocation(shader, "transform");

	u32 vao;
	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(0x8892 /*  GL_ARRAY_BUFFER */, vbo);
	glBufferData(0x8892, 0, 0, 0x88E0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(gairb_textured_vertex), (const GLvoid*) gai_offsetof(gairb_textured_vertex, p) );
	glEnableVertexAttribArray(1); glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gairb_textured_vertex), (const GLvoid*) gai_offsetof(gairb_textured_vertex, uv) );
	glBindVertexArray(0);

	r32 time = 0.f;

	for (;;)
	{
		gaiXWindowUpdate(&window);
		if (!window.is_running) break;
		time += window.frametime.seconds;
		#if 1
		gairb_renderbuffer render_commands = gairb_RenderBuffer(V4(1.0f, 0.0f, 1.0f, 1.0f), pbuffersize, pbuffer, vertex_count_max, vbuffer, 0, 0);

		render_layout layout = { OrthographicProjection(window.info.width, window.info.height, 1, -1) };

		gairb_group g1 = gairb_BeginGroup(V2(window.info.width, window.info.height), &render_commands, &layout, 0);

		gairb_PushRect(&g1, V4(1.0f, 0.0f, 0.0f, 1.0f), V4(0.f, 0.f, 0.f, 1.f), 100.f, 100.f, gairb_AlignToTopLeft);
		//gairb_PushRect(&g1, V4(1.0f, 1.0f, 1.0f, 1.0f), V4(-0.5f, -0.5f, 0.f, 1.f), V2(0.f, 0.f), V4(-0.5f, 0.5f, 0.f, 1.f), V2(0.f, 1.f), V4(0.5f, -0.5f, 0.f, 1.f), V2(1.f, 0.f), V4(0.5f, 0.5f, 0.f, 1.f), V2(1.f, 1.f) );
		gairb_PushRect(&g1, V4(1.0f, 1.0f, 1.0f, 1.0f), V4(100.f, 100.f, 0.f, 1.f), 50.f, 25.f, (int)time % 5 );

		gairb_renderbuffer *commands = g1.commands;
		if (commands)
		{
			glClearColor( commands->clear_color.x, commands->clear_color.y, commands->clear_color.z, commands->clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (u8 *header_at = commands->pushbuffer_base; header_at < commands->pushbuffer_at; header_at += sizeof(gairb_entry_header))
			{
				gairb_entry_header *header = (gairb_entry_header *) header_at;
				switch (header->type)
				{
					case gairb_EntryType_gairb_entry_textured_quads:
					{
						header_at += sizeof(gairb_entry_textured_quads);
						gairb_entry_textured_quads *entry = (gairb_entry_textured_quads *) header;

						render_layout *lay = (render_layout *) g1.setup;

						glUseProgram(shader);
						glViewport(0, 0, window.info.width, window.info.height);
						glUniformMatrix4fv(proj, 1, GL_TRUE, lay->projection.E[0]);


						glBindBuffer(0x8892, vbo);
						glBufferData(0x8892, commands->vertex_count * sizeof(gairb_textured_vertex), commands->vertex_array, 0x88E0);

						glBindVertexArray(vao);

						for (u32 vertex_index = entry->vertex_array_offset; vertex_index < (entry->vertex_array_offset + 4 * entry->quad_count); vertex_index += 4)
						{
							glUniform4fv(color, 1, &commands->vertex_array[vertex_index].color.r);
							glDrawArrays(GL_TRIANGLE_STRIP, vertex_index, 4);
						}
					} break;
				}
			}

			gairb_EndGroup(&g1);
		}
		#endif


		gaiXWindowSwapBuffers(&window);
	}

	VirtualFree(pbuffer, 0, MEM_RELEASE);
	VirtualFree(vbuffer, 0, MEM_RELEASE);

	return 0;
}