
#if _DEBUG
	#define GAIXW_DEBUG
#endif
 
struct loaded_bitmap
{
	unsigned char *memory;
	unsigned int id;
	int is_loaded;
	int width;
	int height;
};

#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#define GAIRGL_IMPLEMENTATION
#include "gai_renderer_opengl.h"

#define GAIHR_IMPLEMENTATION
#include "gai_hotreload.h"

#include <stdio.h>

HMODULE reloadable_dll;
typedef void update_and_render_fn(gaixw_context *window, gairb_renderbuffer *renderbuffer);
update_and_render_fn *UpdateAndRender;
void reloadDLL(gaihr_file *dll)
{
	char NewFilename[GAIHR_TEXTLENGTH_MAX] = {};
	snprintf(NewFilename, GAIHR_TEXTLENGTH_MAX, "~%s", dll->filename);
	if (reloadable_dll) FreeLibrary(reloadable_dll);
	CopyFileA(dll->filename, NewFilename, false);
	reloadable_dll = LoadLibrary(NewFilename);
	assert(reloadable_dll);
	if (reloadable_dll)
	{
		UpdateAndRender = (update_and_render_fn *) GetProcAddress(reloadable_dll, "UpdateAndRender");
	}
	else
	{
		UpdateAndRender = 0;
	}
}

inline loaded_bitmap
CreateWhiteTexture(int width, int height)
{
	loaded_bitmap result = {0, 0, 0, width, height};
	int texture_size = width * height * 4;
	result.memory = (u8*) VirtualAlloc(0, texture_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (result.memory)
	{
		memset(result.memory, 255, texture_size);
	}
	return result;
}

void DeinitWindow(gaixw_context *window, void *userdata)
{
	gaihr_file *reloadable_file = (gaihr_file *) userdata;
	if (gaihr_BeginTicketMutex(reloadable_file, 0))
	{
		if (reloadable_dll)
		{
			FreeLibrary(reloadable_dll);
			UpdateAndRender = 0;
		}
		gaihr_EndTicketMutex(reloadable_file);
	}
}

int main(int argc, char **argv)
{
	gaixw_context window;
	if (!gaiXWindow(&window, "yolo")) return -1;

	
	gaihr_file reloadable_file;
	gaihr_AddFile(&reloadable_file, "renderer.dll", 750, reloadDLL, 0, gaihr_FlagsDontHandleEvent);
	gaiXWindowRegisterDeInitCallback(&window, DeinitWindow, &reloadable_file);

	u32 pbuffersize = 1024 * 1024 * 2;
	u8 *pbuffer = (u8*) VirtualAlloc(0, pbuffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	u32 vertex_count_max = (1 << 16) * 2;
	gairb_textured_vertex *vbuffer = (gairb_textured_vertex*) VirtualAlloc(0, sizeof(gairb_textured_vertex) * vertex_count_max, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	u32 quad_texture_count = (vertex_count_max >> 2);
	loaded_bitmap **quad_textures = (loaded_bitmap**) VirtualAlloc(0, sizeof(loaded_bitmap) * quad_texture_count, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	loaded_bitmap whitebitmap = CreateWhiteTexture(128, 128);

	for (;;)
	{
		gaiXWindowUpdate(&window);
		if (!window.is_running) break;

		gairb_renderbuffer render_commands = gairb_RenderBuffer(V4(0.0f, 0.0f, 0.0f, 1.0f), pbuffersize, pbuffer, vertex_count_max, vbuffer, quad_textures, &whitebitmap);
		if (gaihr_BeginTicketMutex(&reloadable_file, 0))
		{
			gaihr_WaitForEvent(&reloadable_file);
			if (UpdateAndRender) UpdateAndRender(&window, &render_commands);
			gaihr_EndTicketMutex(&reloadable_file);
		}

		gaiXWindowSwapBuffers(&window);
	}


	VirtualFree(pbuffer, 0, MEM_RELEASE);
	VirtualFree(vbuffer, 0, MEM_RELEASE);
	VirtualFree(whitebitmap.memory, 0, MEM_RELEASE);

	return 0;
}