/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_RENDERER_H_

#include <gai_types.h>
#include <gai_utils.h>
#include <gai_math.h>
#include <gai_window.h>

struct gaiRendererData
{
	r32 u, v;
	r32 r, g, b, a;
	r32 nx, ny, nz;
	r32 vx, vy, vz;
	r32 __reserved__[4];
}; // 64 byte length

struct gaiRendererMesh
{
	u32 id;
	size_t data_size;
	gaiRendererData *data;
	gaiRendererMesh *prev;
	gaiRendererMesh *next;
};

struct gaiRendererTriangle
{
	u32 material;
	gaiRendererMesh mesh;
	v3 position;
};

struct gaiRenderMeshShader
{
	u32 program;
	u32 projection;
	u32 view;
	u32 model;

	const char *vs =
	{
		"#version 430\n"
		"layout (location = 0) in vec2 uv;\n"
		"layout (location = 1) in vec4 color;\n"
		"layout (location = 2) in vec3 normal;\n"
		"layout (location = 3) in vec3 vertex;\n"
		"uniform mat4 model = mat4(1);\n"
		"uniform mat4 view  = mat4(1);\n"
		"uniform mat4 proj  = mat4(1);\n"
		"out vec4 _color;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(vertex, 1);\n"
		"   _color = vec4(color);\n"
		"}\n"
	};

	const char *fs =
	{
		"#version 430\n"
		"in vec4 _color;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"    color = _color;\n"
		"}\n"
	};
};

struct gaiRenderDebugFontShader
{
	u32 program;
	u32 projection;
	const char *vs =
	{
		"#version 430\n"
		"layout (location = 0) in vec4 vertex;\n"
		"uniform mat4 proj = mat4(1);\n"
		"void main() {\n"
		"   gl_Position = proj * vec4(vertex.x, vertex.y, 1, 1);\n"
		"}\n"
	};
	const char *fs =
	{
		"#version 430\n"
		"out vec4 color;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(1,1,1,1);\n"
		"}\n"
	};
};

struct gaiRenderer
{
	gaiRenderMeshShader mesh_shader;
	gaiRenderDebugFontShader debug_font_shader;
	u32 debug_font_vao;
	u32 debug_font_vbo;

	m4x4 last_projection;
	m4x4 last_view;

	u32 max_size_vbo;
	u32 vao;
	u32 vbo;
	u32 ibo;
};

i32
gaiRendererCreate(gaiRenderer *renderer, i32 width, i32 height)
{
	renderer->last_view = Identity4();
	renderer->last_projection = Identity4();

	u32 program = gaiOpenGLProgramCreate();
	gaiOpenGLShaderLoad(program, renderer->debug_font_shader.vs, GL_VERTEX_SHADER);
	gaiOpenGLShaderLoad(program, renderer->debug_font_shader.fs, GL_FRAGMENT_SHADER);
	if (!gaiOpenGLProgramLink(program)) return -1;
	renderer->debug_font_shader.projection = gaiOpenGLProgramGetUniform(program, "proj");
	renderer->debug_font_shader.program    = program;

	m4x4 ortho = Orthographic(0.f, (r32) width, (r32) height, 0.f, -1.f, 1.f);
	glUniformMatrix4fv(renderer->debug_font_shader.projection, 1, GL_FALSE,  (const GLfloat*) &ortho);

	glGenVertexArrays(1, &renderer->debug_font_vao);
	glBindVertexArray(renderer->debug_font_vao);

	glGenBuffers(1, &renderer->debug_font_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->debug_font_vbo);
	glBufferData(GL_ARRAY_BUFFER, 99999, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) (i64) 0);

	glBindVertexArray(0);

	program = gaiOpenGLProgramCreate();
	gaiOpenGLShaderLoad(program, renderer->mesh_shader.vs, GL_VERTEX_SHADER);
	gaiOpenGLShaderLoad(program, renderer->mesh_shader.fs, GL_FRAGMENT_SHADER);
	if (!gaiOpenGLProgramLink(program)) return -2;
	renderer->mesh_shader.projection = gaiOpenGLProgramGetUniform(program, "proj");
	renderer->mesh_shader.view       = gaiOpenGLProgramGetUniform(program, "view");
	renderer->mesh_shader.model      = gaiOpenGLProgramGetUniform(program, "model");
	renderer->mesh_shader.program    = program;

	i32 max_elements = 4096;
	u32 max_size     = sizeof(gaiRendererData) * max_elements;

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, max_size, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 8);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 24);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 36);

	glBindVertexArray(0);

	renderer->max_size_vbo = max_size;
	return 1;
}

void
gaiRendererSetProjection(gaiRenderer *renderer, r32 fov, r32 aspect, m4x4 *view, r32 znear = 0.1f, r32 zfar = 100.f)
{

	renderer->last_projection = Perspective(aspect, fov, znear, zfar);
	renderer->last_view       = *view;
	glUseProgram(renderer->mesh_shader.program);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);	
}

void
gaiRendererSetProjection(gaiRenderer *renderer, m4x4 *projection, m4x4 *view)
{
	renderer->last_projection = *projection;
	renderer->last_view       = *view;
	glUseProgram(renderer->mesh_shader.program);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);
}

// Immediate mode drawing! Pretty slow.
void
gaiRendererDrawTriangleImmediate(gaiRenderer *renderer, m4x4 *model, v4 color = V4(1, 1, 1, 1))
{
	glUseProgram(renderer->mesh_shader.program);
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	gaiRendererData data[] =
	{
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  0.f,  1.f, 0.f},
	};

	#if 0
	gaiRendererData *memory = (gaiRendererData *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!memory) return;
	memcpy(memory, data, sizeof(data));
	glUnmapBuffer(GL_ARRAY_BUFFER);
	#else
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	#endif
	glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void
gaiRendererDrawMeshImmediate(gaiRenderer *renderer, m4x4 *model, gaiRendererData *data, size_t size)
{
	glUseProgram(renderer->mesh_shader.program);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	GAI_ASSERT(size < renderer->max_size_vbo);

	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	if (model) glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);
	else glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
	glDrawArrays(GL_TRIANGLES, 0, size / sizeof(gaiRendererData));
}

#define _GAI_RENDERER_H_
#endif
