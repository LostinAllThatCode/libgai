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

#define PLATFORM_API_LOAD_BITMAP(name) void name(loaded_bitmap *bitmap, char *filename)
typedef PLATFORM_API_LOAD_BITMAP(platform_load_bitmap_fn);

struct platform
{
	platform_load_bitmap_fn *LoadBitmap;
};

struct platform_api
{
	loaded_bitmap *assets;

	platform_load_bitmap_fn *load_bitmap_fn;
};

// =====================================================================

#ifndef GAI_NOSTBLIB
	#define STB_TRUETYPE_IMPLEMENTATION
	#include "stb_truetype.h"
	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"
#endif

#define GAIXW_IMPLEMENTATION
#include <gai_xwindow.h>

#define GAIRB_IMPLEMENTATION
#include <gai_renderbuffer.h>

#ifdef GAIXW_OPENGL
	#define GAIRGL_IMPLEMENTATION
	#include <gai_renderer_opengl.h>
#endif

#define update_and_render_(name) void name(gaixw_context *window, gairb_renderbuffer *renderbuffer, platform_api *platform)
typedef update_and_render_(update_and_render_fn);


#define GAIHR_IMPLEMENTATION
#include "gai_hotreload.h"

#define _INCLUDE_GAI_ENGINE_H
#endif