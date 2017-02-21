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
	u32 start;
	u32 count;
	/*
	gaiRendererData *data;
	gaiRendererMesh *prev;
	gaiRendererMesh *next;
	*/
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

	u32 data_size;
	u32 vbo_offset;
	u32 max_size_vbo;
	u32 vao;
	u32 vbo;
	u32 ibo;
};

void
gaiRendererSetViewport(gaiRenderer *renderer, i32 width, i32 height)
{
	glViewport(0, 0, width, height);	
}

i32
gaiRendererCreate(gaiRenderer *renderer, i32 width, i32 height)
{
	renderer->last_view = Identity4();
	renderer->last_projection = Identity4();

	//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE_ARB);

	gaiRendererSetViewport(renderer, width, height);

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
	u32 data_size    = sizeof(gaiRendererData);
	u32 max_size     = data_size * max_elements;

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, max_size, 0, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 8);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 24);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 36);

	glBindVertexArray(0);

	renderer->data_size  = data_size;
	renderer->max_size_vbo = max_size;
	renderer->vbo_offset = 0;
	return 1;
}

void
gaiRendererDraw(gaiRenderer *renderer)
{
	glDrawArrays(GL_TRIANGLES, 0, renderer->vbo_offset / renderer->data_size);
}

void
gaiRendererDraw(gaiRenderer *renderer, GLenum mode, i32 first, i32 count)
{
	glDrawArrays(mode, first, count);
}


void
gaiRendererSetProjection(gaiRenderer *renderer, r32 fov, r32 aspect, m4x4 *view, r32 znear = 0.1f, r32 zfar = 100.f)
{

	renderer->last_projection = Perspective(aspect, fov, znear, zfar);
	renderer->last_view       = *view;
	//glUseProgram(renderer->mesh_shader.program);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);	
}

void
gaiRendererSetProjection(gaiRenderer *renderer, m4x4 *projection, m4x4 *view)
{
	renderer->last_projection = *projection;
	renderer->last_view       = *view;
	glUseProgram(renderer->mesh_shader.program);
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);
}

inline gaiRendererMesh
gaiRendererPushGrid(gaiRenderer *renderer, r32 cell_size, i32 size, v4 color = V4(1,1,1,1))
{
	gaiRendererMesh mesh = {};
	r32 s = (r32) size * cell_size;
	mesh.start = renderer->vbo_offset / renderer->data_size;
	mesh.count = ((size*2)+1)*4;
	gaiRendererData data[] = {
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, 0, 0, (r32)  s, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, 0, 0, (r32) -s, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, (r32)  s, 0, 0, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, (r32) -s, 0, 0, 0.f, 0.f, 0.f, 0.f,
	};
	glBufferSubData(GL_ARRAY_BUFFER, renderer->vbo_offset, sizeof(data), data);
	renderer->vbo_offset += sizeof(data);
	for(i32 i=1; i <= s; i++)
	{
		r32 x = i * cell_size;
		r32 z = i * cell_size;

		gaiRendererData data[] = {
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  x, 0,  s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  x, 0, -s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -x, 0,  s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -x, 0, -s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -s, 0,  z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  s, 0,  z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -s, 0, -z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  s, 0, -z, 0.f, 0.f, 0.f, 0.f,
		};
		glBufferSubData(GL_ARRAY_BUFFER, renderer->vbo_offset, sizeof(data), data);
		renderer->vbo_offset += sizeof(data);	
	}
	return mesh;
}

void
gaiRendererPushTriangle(gaiRenderer *renderer, v3 offset = V3(0,0,0), v4 color = V4(1, 1, 1, 1))
{
	gaiRendererData data[] =
	{
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -1.f + offset.x, -1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  1.f + offset.x, -1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  0.f + offset.x,  1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
	};
	i32 size = sizeof(data);
	glBufferSubData(GL_ARRAY_BUFFER, renderer->vbo_offset, size, data);
	renderer->vbo_offset += size;
}

void
gaiRendererPushTriangle(gaiRenderer *renderer, v3 p1, v2 uv1, v4 c1, v3 p2, v2 uv2, v4 c2, v3 p3, v2 uv3, v4 c3)
{
	gaiRendererData data[] =
	{
		{uv1.u, uv1.v, c1.r, c1.g, c1.b, c1.a, 0.f, 0.f, 0.f, p1.x, p1.y, p1.z},
		{uv2.u, uv2.v, c2.r, c2.g, c2.b, c2.a, 0.f, 0.f, 0.f, p2.x, p2.y, p2.z},
		{uv3.u, uv3.v, c3.r, c3.g, c3.b, c3.a, 0.f, 0.f, 0.f, p3.x, p3.y, p3.z},
	};
	i32 size = sizeof(data);
	glBufferSubData(GL_ARRAY_BUFFER, renderer->vbo_offset, size, data);
	renderer->vbo_offset += size;
}

void
gaiRendererPushRectangle(gaiRenderer *renderer, v3 p1, v2 uv1, v4 c1, v3 p2, v2 uv2, v4 c2, v3 p3, v2 uv3, v4 c3, v3 p4, v2 uv4, v4 c4)
{
	gaiRendererPushTriangle(renderer, p1, uv1, c1, p2, uv2, c2, p3, uv3, c3);
	gaiRendererPushTriangle(renderer, p1, uv1, c1, p3, uv3, c3, p4, uv4, c4);
}

inline gaiRendererMesh
gaiRendererPushQuad(gaiRenderer *renderer, v3 center = V3(0,0,0), r32 width = 1.f, r32 height = 1.f, v4 color = V4(1,1,1,1))
{
	gaiRendererMesh mesh = {};
	r32 half_width = .5f * width;
	r32 half_heigth = .5f * height;

	v3 p1 = center + V3(-half_width, -half_heigth,  half_width);
	v3 p2 = center + V3( half_width, -half_heigth,  half_width);
	v3 p3 = center + V3( half_width,  half_heigth,  half_width);
	v3 p4 = center + V3(-half_width,  half_heigth,  half_width);
	v3 p5 = center + V3(-half_width, -half_heigth, -half_width);
	v3 p6 = center + V3( half_width, -half_heigth, -half_width);
	v3 p7 = center + V3( half_width,  half_heigth, -half_width);
	v3 p8 = center + V3(-half_width,  half_heigth, -half_width);

	v2 uv     = V2(0);
	v4 top    = .75f * color;
	v4 bottom = .25f * color;
	v4 c1     = V4(top.r, top.g, top.b, color.a);
	v4 c2     = V4(bottom.r, bottom.g, bottom.b, color.a);

	mesh.start = renderer->vbo_offset / renderer->data_size;
	gaiRendererPushRectangle(renderer, p1, uv, c2, p2, uv, c2, p3, uv, c1, p4, uv, c1);
	gaiRendererPushRectangle(renderer, p5, uv, c2, p8, uv, c1, p7, uv, c1, p6, uv, c2);
	gaiRendererPushRectangle(renderer, p1, uv, c2, p2, uv, c2, p6, uv, c2, p5, uv, c2);
	gaiRendererPushRectangle(renderer, p4, uv, color, p8, uv, color, p7, uv, color, p3, uv, color);
	gaiRendererPushRectangle(renderer, p2, uv, c2, p6, uv, c2, p7, uv, c1, p3, uv, c1);
	gaiRendererPushRectangle(renderer, p1, uv, c2, p4, uv, c1, p8, uv, c1, p5, uv, c2);
	mesh.count = 36;
	return mesh;
}


// Immediate mode drawing! Pretty slow.
void
gaiRendererDrawTriangleImmediate(gaiRenderer *renderer, m4x4 *model, v4 color = V4(1, 1, 1, 1))
{
	glUseProgram(renderer->mesh_shader.program);
	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

	gaiRendererData data[] =
	{
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  0.f,  1.f, 0.f},
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	if (model) glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);
	else glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
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

	gai_assert(size < renderer->max_size_vbo);

	glBindVertexArray(renderer->vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	if (model) glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);
	else glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
	glDrawArrays(GL_TRIANGLES, 0, size / sizeof(gaiRendererData));
}

#define _GAI_RENDERER_H_
#endif
