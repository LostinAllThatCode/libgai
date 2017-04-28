
#if _DEBUG
	#define GAIXW_DEBUG
#else
	#define LOG_ASSERT(cond) 	if(cond) {} else WriteToLog(__FILE__, __LINE__, #cond);
	#define GAIXW_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIRB_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIHR_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIRGL_ASSERT(cond) LOG_ASSERT(cond)
#endif

#include <stdio.h>

void
WriteToLog(char *source, unsigned int linenumber, char *line)
{
	FILE *fp = fopen("log.txt", "a+");
	if (fp)
	{
		fprintf(fp, "Assertion failed! %s(%i): %s\n", source, linenumber, line );
		fclose(fp);
	}
}

#include "renderer_shared.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#define GAIRGL_IMPLEMENTATION
#include "gai_renderer_opengl.h"

#define GAIHR_IMPLEMENTATION
#include "gai_hotreload.h"


PLATFORM_API_LOAD_BITMAP(win32_LoadBitmapFromFile)
{
	if (!bitmap) assert(!"error");
	int bpp;
	stbi_set_flip_vertically_on_load(0);
	bitmap->memory 		= stbi_load(filename, &bitmap->width, &bitmap->height, &bpp, 4);
	bitmap->is_loaded 	= 0;
	bitmap->handle 		= 0;
}

void hotreloadtexture(gaihr_file *file)
{
	loaded_bitmap *bitmap = (loaded_bitmap*) file->userdata;
	if (bitmap)
	{
		if (bitmap->memory) stbi_image_free(bitmap->memory);
		win32_LoadBitmapFromFile(bitmap, (char *)file->filename);
	}
}

HMODULE reloadable_dll;
update_and_render_fn *UpdateAndRender;
GAIHR_CALLBACK(reloadDLL)
{
	char NewFilename[4096] = {};
	snprintf(NewFilename, 4096, "~%s", file->filename);
	if (reloadable_dll) FreeLibrary(reloadable_dll);
	CopyFileA(file->filename, NewFilename, false);
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
CreateWhiteBitmap(int width, int height)
{
	loaded_bitmap result = {0, 0, width, height, 0};
	int texture_size = width * height * 4;
	result.memory = (u8*) VirtualAlloc(0, texture_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (result.memory)
	{
		memset(result.memory, 255, texture_size);
	}
	return result;
}

int main(int argc, char **argv)
{
	platform_api platform = {};
	platform.load_bitmap_fn = win32_LoadBitmapFromFile;

	gaixw_context window;
	if (!gaixw_Init(&window, "yolo")) return -1;
	gaixw_SetVerticalSync(&window, 1);

	gairgl opengl = {};
	gairgl_Initialize(&opengl);

	gaihr_file reloadable_file = {};
	gaihr_Track(&reloadable_file, "renderer.dll", reloadDLL, 0, gaihr_FlagsDontHandleEvent);

	u32 pbuffersize = 1024 * 1024 * 2;
	u8 *pbuffer = (u8*) VirtualAlloc(0, pbuffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	u32 vertex_count_max = (1 << 16) * 2;
	gairb_textured_vertex *vbuffer = (gairb_textured_vertex*) VirtualAlloc(0, sizeof(gairb_textured_vertex) * vertex_count_max, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	u32 quad_texture_count = vertex_count_max >> 2;;
	loaded_bitmap **quad_textures = (loaded_bitmap **) VirtualAlloc(0, sizeof(loaded_bitmap*) * quad_texture_count, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	loaded_bitmap assets[64] = {};
	assets[0] = CreateWhiteBitmap(128, 128);

	gaihr_file texture;
	gaihr_Track(&texture, "test.jpg", hotreloadtexture, &assets[1]);

	platform.assets = assets;

	for (;;)
	{
		gaixw_Update(&window);
		if (!window.is_running) break;

		gairb_renderbuffer render_commands = gairb_RenderBuffer(V4(.0f, .0f, .0f, 1.0f), pbuffersize, pbuffer, vertex_count_max, vbuffer, quad_textures, &assets[0]);

		if (gaixw_KeyPressed(&window, 'K')) gaihr_Untrack(&texture);

		gaihr_WaitForEvent(&reloadable_file);
		if (UpdateAndRender) UpdateAndRender(&window, &render_commands, &platform);

		gairgl_Render(&opengl, &render_commands, V2i(window.info.width, window.info.height));
		gaixw_SwapBuffers(&window);
	}

	gairgl_Destroy(&opengl);
	gaixw_Deinit(&window);

	if (reloadable_dll) FreeLibrary(reloadable_dll);

	VirtualFree(pbuffer, 0, MEM_RELEASE);
	VirtualFree(vbuffer, 0, MEM_RELEASE);
	VirtualFree(assets[0].memory, 0, MEM_RELEASE);
	stbi_image_free(assets[1].memory);


	return 0;
}