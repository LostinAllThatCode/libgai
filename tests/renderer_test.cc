
#if 1
	#define LOG_ASSERT(cond) 	if(cond) {} else WriteToLog(__FILE__, __LINE__, #cond);
	#define GAIXW_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIRB_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIHR_ASSERT(cond) 	LOG_ASSERT(cond)
	#define GAIRGL_ASSERT(cond) LOG_ASSERT(cond)
#endif

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void
WriteToLog(const char *source, unsigned int linenumber, const char *line)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	FILE *f;
	const char *filename = "log.txt";
	const char *mode	 = "a+";
	#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&f, filename, mode))
		f = 0;
	#else
	f = fopen(filename, mode);
	#endif

	if (f)
	{
		fprintf(f, "[%s(%i) %04i.%02i.%02i.%02i.%02i.%02i.%04i] ", source, linenumber, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		fprintf(f, "Assertion failed! >> (%s)\n", line);
		fclose(f);
	}
}

#define GAIXW_DEBUG
#define GAIXW_OPENGL
#define GAIXW_OPENGL_CORE_PROFILE
#include <gai_engine.h>

struct hr_texture
{
	gaipf_api *platform;
	loaded_bitmap *bitmap;
};

void HotreloadTexture(gaihr_file *file)
{
	hr_texture *texture 	= (hr_texture*) file->userdata; 
	if (texture->bitmap)
	{
		if (texture->bitmap->memory) stbi_image_free(texture->bitmap->memory);
		texture->platform->LoadBitmapFromFile(texture->bitmap, (char *)file->filename);
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

int mb_to_unicode(int mb)
{
	int result = 0;
	int convert = mb;
	unsigned char *l = (unsigned char*) &convert;
	if( convert >= 0xF0000000 )
	{
		result += (l[3] & 7) << 18; 
		result += ((l[2] & 0x3F) << 12); 
		result += (l[1] & 0x3F) << 6;
		result += (l[0] & 0x3F);
	} 
	else if( convert >= 0xE00000 ) 
	{
		result += ((l[2] & 0xF) << 12); 
		result += (l[1] & 0x3F) << 6;
		result += (l[0] & 0x3F);
	}
	else if( convert >= 0xC000 ) 
	{
		result += (l[1] & 0x1F) << 6;
		result += (l[0] & 0x3F);
	}
	else
	{
		result += (l[0] & 0x7F);
	}
	return result;
}

int main(int argc, char **argv)
{
	gaipf_api *platform = gaipf_GetPlatform();
	if(!platform) return -1;

	gaixw_context window;
	if (!gaixw_Init(&window, "yolo")) return -1;
	gaixw_SetVerticalSync(&window, 1);

	gairgl opengl = {};
	gairgl_Initialize(&opengl);

	gaihr_file reloadable_file = {};
	gaihr_Track(&reloadable_file, "renderer.dll", reloadDLL, 0, gaihr_FlagsDontHandleEvent);

	u32 pbuffersize = GAIPF_Megabytes(8);
	u8 *pbuffer = (u8*) VirtualAlloc(0, pbuffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	u32 vertex_count_max = (1 << 16) * 32;
	gairb_textured_vertex *vbuffer = (gairb_textured_vertex*) VirtualAlloc(0, sizeof(gairb_textured_vertex) * vertex_count_max, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	u32 quad_texture_count = vertex_count_max >> 2;
	loaded_bitmap **quad_textures = (loaded_bitmap **) VirtualAlloc(0, sizeof(loaded_bitmap*) * quad_texture_count, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	loaded_font consolab = GAIPF_FONT_NULL;
	platform->LoadFontFromFile(&consolab, "C:/windows/fonts/consolab.ttf", 64);

	loaded_bitmap assets[64] = {};
	assets[0] = CreateWhiteBitmap(128, 128);
	assets[2] = consolab.bitmap;

	hr_texture tex = { platform, &assets[1] };
	gaihr_file texture;
	gaihr_Track(&texture, "test.jpg", HotreloadTexture, &tex);

	platform->assets = assets;

	for (;;)
	{
		gaixw_Update(&window);
		if (!window.is_running) break;

		gairb_renderbuffer render_commands = gairb_RenderBuffer(V4(.0f, .0f, .0f, 1.0f), pbuffersize, pbuffer, vertex_count_max, vbuffer, quad_textures, &assets[0]);

		if (gaixw_KeyPressed(&window, 'K')) gaihr_Untrack(&texture);

		gaihr_WaitForEvent(&reloadable_file);
		if (UpdateAndRender) UpdateAndRender(&window, &render_commands, platform);

		int draw_calls = gairgl_Render(&opengl, &render_commands, V2i(window.width, window.height));
		gaixw_SwapBuffers(&window);
	}

	gairgl_Destroy(&opengl);
	gaixw_Deinit(&window);

	if (reloadable_dll) FreeLibrary(reloadable_dll);

	VirtualFree(pbuffer, 0, MEM_RELEASE);
	VirtualFree(vbuffer, 0, MEM_RELEASE);
	VirtualFree(assets[0].memory, 0, MEM_RELEASE);
	stbi_image_free(assets[1].memory);
	VirtualFree(assets[2].memory, 0, MEM_RELEASE);

	return 0;
}