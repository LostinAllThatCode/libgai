#define GAIXW_DEBUG
#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"
#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#include <stdlib.h>

void render(gairb_renderbuffer *commands, v2 draw_region)
{
	glClearColor( commands->clear_color.x, commands->clear_color.y, commands->clear_color.z, commands->clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	for (u8 *header_at = commands->pushbuffer_base; header_at < commands->pushbuffer_at; )//header_at += sizeof(gairb_entry_header))
	{
		gairb_entry_header *header = (gairb_entry_header *) header_at;
		switch (header->type)
		{
			case gairb_EntryType_gairb_entry_textured_quads:
			{
				header_at += sizeof(gairb_entry_textured_quads);

				gairb_entry_textured_quads *entry = (gairb_entry_textured_quads *) header;
				glLoadMatrixf(entry->setup.transform.E[0]);

				for (u32 vertex_index = entry->vertex_array_offset; vertex_index < (entry->vertex_array_offset + 4 * entry->quad_count); vertex_index += 4)
				{
					glBegin(GL_TRIANGLE_STRIP);
					for (int i = 0; i < 4; i++)
					{
						v4 color = commands->vertex_array[vertex_index + i].color;
						v4 vertex = commands->vertex_array[vertex_index + i].p;
						glColor4f(color.r, color.g, color.b, color.a);
						glVertex3f(vertex.x, vertex.y, vertex.z);
					}
					glEnd();
				}

			} break;
		}
	}
	glViewport(0, 0, (int)draw_region.x, (int)draw_region.y);
}

int main(int argc, char **argv)
{

	unsigned int renderbuffer_size = 1024 * 1024 * 4;  // 4 megabytes
	unsigned int vertex_count_max  = 120000;
	unsigned int vertexbuffer_size = sizeof(gairb_textured_vertex) * vertex_count_max;
	unsigned char *renderbuffer = (unsigned char *) malloc( renderbuffer_size );
	unsigned char *vertexbuffer = (unsigned char *) malloc( vertexbuffer_size );
	if (!renderbuffer || !vertexbuffer) return 1;

	gaixw_context window;
	if (!gaixw_Init(&window, "lol", -1, -1, -1, -1, "asdas")) return -1;
	gaixw_SetVerticalSync(&window, 1);
	r32 time = 0.f;
	for (;;)
	{
		gaixw_Update(&window);
		if (!window.is_running) break;

		time += window.dt.seconds * 15.f;
		gairb_renderbuffer MyRenderBuffer = gairb_RenderBuffer( V4(0.f), renderbuffer_size, renderbuffer, vertex_count_max, vertexbuffer, 0, 0);

		gairb_group MyRenderGroup = gairb_BeginGroup(V2i(window.info.width, window.info.height), &MyRenderBuffer, 0);
		gairb_setup OrthoProject = { Transpose(OrthographicProjection((float)window.info.width, (float)window.info.height, 1, -1)) };
		gairb_PushSetup(&MyRenderGroup, &OrthoProject);

		for ( int i = 0; i < 300; i++)
		{
			gairb_PushRect(&MyRenderGroup, V4(1.0f, 0.0f, 1.f, 1.0f), V3(10.f + (i * 3.f), 10.f + sinf(time + i), 0.f), 1.f, 1.f, gairb_AlignToTopLeft);
		}

		gairb_EndGroup(&MyRenderGroup);

		render(&MyRenderBuffer, V2i(window.info.width, window.info.height));
		gaixw_SwapBuffers(&window);
	}

	free(renderbuffer);
	free(vertexbuffer);

	return 0;
}