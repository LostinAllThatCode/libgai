
#if _DEBUG
	#define GAIXW_DEBUG
#endif

#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#define GAIRGL_IMPLEMENTATION
#include "gai_renderer_opengl.h"

int main(int argc, char **argv)
{
	gaixw_context window;
	if (!gaiXWindow(&window, "yolo")) return -1;
	gaiXWindowSetVSYNC(&window, 0);

	gairgl ogl = {};
	gairgl_Initialize(&ogl);

	u32 pbuffersize = 1024 * 1024 * 2;
	u8 *pbuffer = (u8*) VirtualAlloc(0, pbuffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	u32 vertex_count_max = 512;
	gairb_textured_vertex *vbuffer = (gairb_textured_vertex*) VirtualAlloc(0, sizeof(gairb_textured_vertex) * vertex_count_max, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	r32 time = 0.f;
	for (;;)
	{
		gaiXWindowUpdate(&window);
		if (!window.is_running) break;
		time += window.frametime.seconds;

		gairb_renderbuffer render_commands = gairb_RenderBuffer(V4(0.0f, 0.0f, 0.0f, 1.0f), pbuffersize, pbuffer, vertex_count_max, vbuffer, 0, 0);
		gairb_group g1 = gairb_BeginGroup(V2(window.info.width, window.info.height), &render_commands, 0);
		{
			gairb_setup setup = { OrthographicProjection(window.info.width, window.info.height, 1, -1) };
			//m4x4 transform = PerspectiveProjection((float) window.info.width / (float) window.info.height, 90.f, 0.1f, 100.f) * CameraOrbit(V3(0,0,15), GAIRB_DEG2RAD(-45.f), GAIRB_DEG2RAD(0.f));
			gairb_PushSetup(&g1, &setup);
			//gairb_PushRect(&g1, V4(1.0f), V4(0.f, 1.f, 0.f, 1.f), V2(0.f), V4(0.f, 0.f, 0.f, 1.f), V2(0.f), V4(1.f, 1.f, 0.f, 1.f), V2(0.f), V4(1.f, 0.f, 0.f, 1.f), V2(0.f));
			gairb_PushRect(&g1, V4(1.0f), V3(10.f, 10.f, 0.f), 150.f, 150.f, 0);
			gairb_EndGroup(&g1);
		}


		gairgl_Render(&ogl, &render_commands, V2(window.info.width, window.info.height));

		gaiXWindowSwapBuffers(&window);
		Sleep(32);

	}

	VirtualFree(pbuffer, 0, MEM_RELEASE);
	VirtualFree(vbuffer, 0, MEM_RELEASE);

	return 0;
}