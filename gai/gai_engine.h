#ifndef _INCLUDE_GAI_ENGINE_H

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#ifdef GAI_ENGINE_DEBUG
	#define GAI_ENGINE_DEBUG_MODE 1
#else
	#define GAI_ENGINE_DEBUG_MODE 0
#endif

#ifdef GAI_ENGINE_IMPLEMENTATION
	#define GAI_IMPLEMENTATION
#endif
#include "gai.h"  // single file crossplatform api [window management, quick sort, dynamic array(gai_growable), networking(tcp/udp sockets), general macros]

struct gai_loaded_file
{
	u32 	size;
	void	*data;
};

#define GAI_ENGINE_ALLOC_PROC(name) void *name(size_t size)
typedef GAI_ENGINE_ALLOC_PROC(impl_platform_alloc);

#define GAI_ENGINE_FREE_PROC(name) void name(void *ptr)
typedef GAI_ENGINE_FREE_PROC(impl_platform_free);

#define GAI_ENGINE_READ_FILE_CONTENT_PROC(name) gai_loaded_file name(const char *filename)
typedef GAI_ENGINE_READ_FILE_CONTENT_PROC(impl_platform_read_file_content);

#define GAI_ENGINE_FREE_FILE_MEMORY_PROC(name) void name(gai_loaded_file *file)
typedef GAI_ENGINE_FREE_FILE_MEMORY_PROC(impl_platform_free_file_memory);

struct gai_engine_platform_api
{
	impl_platform_alloc *alloc;
	impl_platform_free  *free;

	impl_platform_read_file_content *read_file_content;
	impl_platform_free_file_memory  *free_file_memory;
};

#include "gai_math.h"

#define gai_handle_to_u32(handle) 		(u32)((size_t)(handle))
#define gai_u32_to_handle(value) 		(void*)((size_t)(value))

// Loaded in terms of loaded to ram not the gpu. This should be an extra step before drawing!
struct gai_loaded_texture
{
	void *data;
	i32 width;
	i32 height;
	void *handle;
};

struct gai_textured_vertex
{
	v4 	position;
	v3 	normal;
	v2 	uv;
	u32 color;
};

struct gai_loaded_font
{
	stbtt_packedchar 	glyph_data[1024];
	gai_loaded_texture 	font_texture;
	i32 size;
};

struct gai_render_settings
{
	v4 clear_color;
	u32 width, height;
};


#define _gai_vertex_percent(cmds) (r32)((cmds)->vertices_count)/(r32)((cmds)->vertices_max)
struct gai_render_commands
{
	//TODO:(aga) Move the textures into something of a resource file thingy!
	gai_loaded_texture 				textures[128];

	gai_render_settings 				settings;
	u32 								pushbuffer_max;
	u8  								*pushbuffer_base;
	u8  								*pushbuffer_at;

	gai_textured_vertex					*vertices;
	u32 								vertices_max;
	u32 								vertices_count;

	gai_loaded_texture					white_texture;
};

#include "gai_renderbuffer.h"

#include "gai_renderer_opengl.h"
union gai_renderer
{
	gai_glrenderer 	opengl;
};

struct gai_engine_setup
{
	u32 render_buffer_size_mb;
	i32 swap_interval;
	gai_window_driver_type type;
	union
	{
		struct
		{
			i32 major;
			i32 minor;
			i32 core;
			i32 debug;
		} opengl;
	};
};
#define GAI_ENGINE_DEFAULTS_SWAP_INTERVAL 1
#define GAI_ENGINE_DEFAULTS_DRIVER gaiWindowDriverOpenGL
#define GAI_ENGINE_DEFAULTS_DRIVER_OPENGL_SETTINGS {1, 0, 0, 0}
#define GAI_ENGINE_DEFAULTS_RENDERBUFFER_SIZE 4
#define GAI_ENGINE_DEFAULT_SETUP { \
		GAI_ENGINE_DEFAULTS_RENDERBUFFER_SIZE, \
		GAI_ENGINE_DEFAULTS_SWAP_INTERVAL, \
		GAI_ENGINE_DEFAULTS_DRIVER, \
		GAI_ENGINE_DEFAULTS_DRIVER_OPENGL_SETTINGS \
	}

struct gai_engine
{
	/* Platform API provides memory-management, file-handling, ... */
	gai_engine_platform_api platform_api;

	/* Actual engine data */
	gai_window   			window;
	gai_render_commands     commands;
	gai_renderer 			renderer;
};
GAI_INL gai_engine*	gai_engine_get_context();
GAI_INL void		gai_engine_set_context(gai_engine *engine);



// Platform stuff
GAI_INL void* 			gai_engine_mem_alloc(size_t size);
GAI_INL void 			gai_engine_mem_free(void *address);
GAI_INL gai_loaded_file gai_engine_file_read_content(const char *filename);
GAI_INL void			gai_engine_file_free(gai_loaded_file *file);


// Engine stuff
GAI_DEF i32		 	gai_engine_init(gai_engine* engine, const char *title, u16 flags, i32 width, i32 height, i32 x, i32 y, gai_engine_setup setup = GAI_ENGINE_DEFAULT_SETUP);
GAI_DEF void 		gai_engine_deinit();

GAI_INL r64 		gai_engine_update();
GAI_INL void 		gai_engine_display();
GAI_INL b32 		gai_engine_running();
GAI_INL void		gai_engine_stop();
GAI_INL b32			gai_engine_closed();
GAI_INL b32			gai_engine_active();
GAI_INL b32			gai_engine_inactive();
GAI_INL b32			gai_engine_minimized();
GAI_INL b32			gai_engine_maximized();
GAI_INL b32			gai_engine_vsync(b32 state);
GAI_INL b32			gai_engine_vsync_enabled();
GAI_INL void		gai_engine_borderless(b32 state);
GAI_INL b32			gai_engine_borderless_enabled();

GAI_INL r64			gai_engine_delta_time();
GAI_INL i32			gai_engine_monitor_vrefresh();

GAI_INL void		gai_engine_register_callback(gai_callback_type_enum type, void *callback, void *userdata = 0);
GAI_INL void		gai_engine_unregister_callback(void *callback); //NOTE:(aga) Callback won't get called anymore but still kindof exist. See gai.h note!
GAI_INL u8 			gai_engine_key_state(i32 key);
GAI_INL b32 		gai_engine_key_down(i32 key);
GAI_INL b32 		gai_engine_key_pressed(i32 key);
GAI_INL b32			gai_engine_key_released(i32 key);
GAI_INL u8 			gai_engine_mouse_state(i32 btn);
GAI_INL b32 		gai_engine_mouse_down(i32 btn);
GAI_INL b32 		gai_engine_mouse_pressed(i32 btn);
GAI_INL b32			gai_engine_mouse_released(i32 btn);
GAI_INL v2			gai_engine_mouse_position();

GAI_INL void 		gai_engine_font_load_from_file(gai_loaded_font *font, char *filename, i32 size);


#ifdef GAI_ENGINE_IMPLEMENTATION
	#include "gai_renderbuffer.cpp"
	#include "gai_renderer_opengl.cpp"
	#include "gai_engine.cpp"
#endif
#define _INCLUDE_GAI_ENGINE_H
#endif
