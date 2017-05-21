#ifndef _INCLUDE_GAI_ENGINE_H

/**
 * Temporary stuff for testing
 */

#include <gai_platform.h>

struct loaded_bitmap
{
	u08* 	memory;
	b32  	is_loaded;
	i32  	width;
	i32  	height;
	void*   handle;
	b32     is_font_texture;
};

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GAI_ENGINE_MAX_CHARACTERS 4096
struct loaded_font
{
	stbtt_pack_context 	stbtt_context;
	stbtt_packedchar   	stbtt_chars[GAI_ENGINE_MAX_CHARACTERS];
	loaded_bitmap      	bitmap;
	int 				size;
} GAIPF_FONT_NULL;

struct gaipf_file
{
	void 	*data;
	size_t 	size;
};
#define PLATFORM_API_READ_FILE(name) int name(gaipf_file *file, const char *filename)
typedef PLATFORM_API_READ_FILE(platform_read_file_fn);

#define PLATFORM_API_LOAD_BITMAP(name) void name(loaded_bitmap *bitmap, char *filename)
typedef PLATFORM_API_LOAD_BITMAP(platform_load_bitmap_fn);

#define PLATFORM_API_LOAD_FONT(name) void name(loaded_font *font, const char* filename, int size)
typedef PLATFORM_API_LOAD_FONT(platform_load_font_fn);

struct gaipf_api
{
	loaded_bitmap *assets;

	platform_read_file_fn	*ReadFile;
	platform_load_bitmap_fn *LoadBitmapFromFile;
	platform_load_font_fn 	*LoadFontFromFile;
} PLATFORM;

#if _WIN32
	#include <gai_platform_win.h>
#elif __linux__
	#error No platform layer for linux yet!
#endif

gaipf_api *
gaipf_GetPlatform(void)
{
	static int is_initialized;
	if(!is_initialized)
	{
		#if _WIN32
			PLATFORM.ReadFile 			= gaipfwin_ReadFile;
			PLATFORM.LoadBitmapFromFile = gaipfwin_LoadBitmapFromFile;
			PLATFORM.LoadFontFromFile 	= gaipfwin_LoadFontFromFile;
			is_initialized  			= 1;
		#endif
	}
	return (is_initialized == 1 ? &PLATFORM : 0);
}

#define GAIXW_IMPLEMENTATION
#include <gai_xwindow.h>

#define GAIRB_IMPLEMENTATION
#include <gai_renderbuffer.h>

#ifdef GAIXW_OPENGL
	#define GAIRGL_IMPLEMENTATION
	#include <gai_renderer_opengl.h>
#endif

#define update_and_render_(name) void name(gaixw_context *window, gairb_renderbuffer *renderbuffer, gaipf_api *platform)
typedef update_and_render_(update_and_render_fn);


#define GAIHR_IMPLEMENTATION
#include "gai_hotreload.h"

#define _INCLUDE_GAI_ENGINE_H
#endif