#ifndef _INCLUDE_GAI_RENDERBUFFER_H

//NOTE(aga): These macros automatically generate the render entry type and its corresponding enums

#define GAI_RENDER_ENTRY_GEN \
	GAI_RENDER_ENTRY_TYPE(gai_render_entry_textured_quads, u32 quad_count; u32 vertex_offset; gai_loaded_texture *texture;) 

#define GAI_RENDER_ENTRY_TYPE(T, _) gaiRenderEntryType_##T,
enum gai_render_entry_type
{
	gaiRenderEntryType_Unknown = 0,
	GAI_RENDER_ENTRY_GEN
	gaiRenderEntryType_Count
};

struct gai_render_entry_header
{
	gai_render_entry_type type;
};

struct gai_render_entry_setup
{
	mx4 projection;
	mx4 xtransform;
	v3 	cam_pos;
	i32	shaderid;
};

#undef GAI_RENDER_ENTRY_TYPE
#define GAI_RENDER_ENTRY_TYPE(T, BODY) \
	struct T \
	{ \
		gai_render_entry_setup 		setup; \
		BODY \
	};
GAI_RENDER_ENTRY_GEN

#undef GAI_RENDER_ENTRY_GEN
#undef GAI_RENDER_ENTRY_TYPE

// Begin of actual render buffer code

struct gai_render_buffer
{
	v2 								screen_dim;
	u32 							missing_resource_count;
	gai_render_entry_setup 			last_setup;
	gai_render_commands 			*commands;
	gai_render_entry_textured_quads *current_quads;
};

struct gai_rect2d
{
	v3 position;
	v2 size;
	u32 color;
};

void gai_renderbuffer_set_shader(gai_render_buffer *buffer, u32 program_id);
void gai_renderbuffer_push_rect2d(gai_render_buffer *buffer, gai_rect2d rectangle);
void gai_renderbuffer_push_text2d(gai_render_buffer *buffer, gai_loaded_font *font, char *string, u32 length, v3 position, u32 color, b32 shadow, u32 shadow_color = 0xff000000);
void gai_renderbuffer_push_text2d(gai_render_buffer *buffer, gai_loaded_font *font, char *string, u32 length, v3 position, u32 color);

#define _INCLUDE_GAI_RENDERBUFFER_H
#endif