#ifndef INCLUDE_RENDERER_SHARED_H

struct loaded_bitmap
{
	unsigned char *memory;
	int is_loaded;
	int width;
	int height;
	void *handle;
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

struct gaixw_context;
struct gairb_renderbuffer;
#define update_and_render_(name) void name(gaixw_context *window, gairb_renderbuffer *renderbuffer, platform_api *platform)
typedef update_and_render_(update_and_render_fn);
#define INCLUDE_RENDERER_SHARED_H
#endif