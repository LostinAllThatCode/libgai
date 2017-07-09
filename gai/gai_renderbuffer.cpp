GAI_INL void *
__gai_renderbuffer_push(gai_render_buffer *buffer, u32 size)
{
	gai_render_commands *cmds = buffer->commands;

	void *result = 0;
	u8 *push_buffer_end = cmds->pushbuffer_base + cmds->pushbuffer_max;
	if ( (cmds->pushbuffer_at + size) <= push_buffer_end)
	{
		result = cmds->pushbuffer_at;
		cmds->pushbuffer_at += size;
	}
	else
	{
		GAI_ASSERT(!"pushbuffer overflow");
	}
	return result;
}

#define __gai_renderbuffer_push_element(buffer, type) (type *) __gai_renderbuffer_push_element_((buffer), sizeof(type), gaiRenderEntryType_##type)
GAI_INL void *
__gai_renderbuffer_push_element_(gai_render_buffer *buffer, u32 size, gai_render_entry_type type)
{
	gai_render_commands *cmds = buffer->commands;
	u8 *result = 0;

	size 	+= sizeof(gai_render_entry_header);
	result 	= (u8*) __gai_renderbuffer_push(buffer, size);
	if (result)
	{
		gai_render_entry_header *header = (gai_render_entry_header*) result;
		header->type = type;
		result += sizeof(*header);
	}

	buffer->current_quads = 0;
	return result;
}

GAI_INL gai_render_entry_textured_quads *
__gai_renderbuffer_get_current_quads(gai_render_buffer *buffer, u32 quad_count)
{
	gai_render_commands *cmds = buffer->commands;

	if (!buffer->current_quads)
	{
		buffer->current_quads                = __gai_renderbuffer_push_element(buffer, gai_render_entry_textured_quads);
		buffer->current_quads->vertex_offset = cmds->vertices_count;
		buffer->current_quads->setup         = buffer->last_setup;
		buffer->current_quads->quad_count    = 0;
	}

	if ((cmds->vertices_count + 4 * quad_count) > cmds->vertices_max)
	{
		return 0;
	}
	return (buffer->current_quads);
}

GAI_INL void
__gai_renderbuffer_push_textured_quad(gai_render_buffer *buffer,
                                      v4 p0, v2 uv0, u32 c0,
                                      v4 p1, v2 uv1, u32 c1,
                                      v4 p2, v2 uv2, u32 c2,
                                      v4 p3, v2 uv3, u32 c3)
{
	gai_render_entry_textured_quads *entry = buffer->current_quads;
	GAI_ASSERT(entry);
	++entry->quad_count;

	gai_render_commands *cmds = buffer->commands;
	gai_textured_vertex *v 	  = cmds->vertices + cmds->vertices_count;
	cmds->vertices_count += 4;

	v[0].position = p0; v[0].normal = vec3(0, 0, 0); v[0].uv = uv0; v[0].color = c0;
	v[1].position = p1; v[1].normal = vec3(0, 0, 0); v[1].uv = uv1; v[1].color = c1;
	v[2].position = p2; v[2].normal = vec3(0, 0, 0); v[2].uv = uv2; v[2].color = c2;
	v[3].position = p3; v[3].normal = vec3(0, 0, 0); v[3].uv = uv3; v[3].color = c3;
}

GAI_INL void
__gai_renderbuffer_push_textured_quad_array(gai_render_buffer *buffer, u32 quad_count, gai_textured_vertex *array)
{
	gai_render_entry_textured_quads *entry = buffer->current_quads;
	GAI_ASSERT(entry);
	entry->quad_count += quad_count;

	gai_render_commands *cmds = buffer->commands;
	gai_textured_vertex *v 	  = cmds->vertices + cmds->vertices_count;
	cmds->vertices_count += quad_count * 4;

	memcpy(v, array, sizeof(gai_textured_vertex) * quad_count * 4);
}

GAI_INL void
gai_renderbuffer_push_rect2d(gai_render_buffer *buffer, gai_rect2d *rectangle)
{
	gai_render_entry_textured_quads *entry = __gai_renderbuffer_get_current_quads(buffer, 1);
	if (entry)
	{
		entry->texture = &buffer->commands->white_texture;

		u32 c0, c1, c2, c3;
		c0 = c1 = c2 = c3 = rectangle->color;

		v4 p0 	= vec4(rectangle->position.x, rectangle->position.y + rectangle->size.y, rectangle->position.z, 1.0f);
		v2 uv0	= vec2(0.0f, 1.0f);

		v4 p1 	= vec4(rectangle->position.x + rectangle->size.x, rectangle->position.y + rectangle->size.y, rectangle->position.z, 1.0f);
		v2 uv1	= vec2(1.0f, 1.0f);

		v4 p2 	= vec4(rectangle->position.x, rectangle->position.y, rectangle->position.z, 1.0f);
		v2 uv2	= vec2(0.0f, 0.0f);

		v4 p3 	= vec4(rectangle->position.x + rectangle->size.x, rectangle->position.y, rectangle->position.z, 1.0f);
		v2 uv3	= vec2(1.0f, 0.0f);

		__gai_renderbuffer_push_textured_quad(buffer, p0, uv0, c0, p1, uv1, c1, p2, uv2, c2, p3, uv3, c3);
	}
}

GAI_INL void
gai_renderbuffer_push_text2d(gai_render_buffer *buffer, gai_loaded_font *font, wchar_t *unicode, u32 length, v3 position, u32 color,
                             b32 shadow, u32 shadow_color)
{
	gai_render_entry_textured_quads *entry = __gai_renderbuffer_get_current_quads(buffer, length);
	if (entry)
	{
		if (entry->texture && (&font->font_texture != entry->texture))
		{
			buffer->current_quads = 0;
			entry = __gai_renderbuffer_get_current_quads(buffer, length);
		}
		entry->texture = &font->font_texture;

		u32 c0, c1, c2, c3;
		c0 = c1 = c2 = c3 = color;
		r32 x = 0, y = 0;
		for (i32 i = 0; i < length; i++)
		{
			if (unicode[i] == '\n')
			{
				x = 0;
				y += font->size;
				continue;
			}

			stbtt_aligned_quad quad = {};
			stbtt_GetPackedQuad(font->glyph_data, font->font_texture.width, font->font_texture.height,
			                    unicode[i], &x, &y, &quad, 0);

			v4 p0 	= vec4(position.x + quad.x0, position.y - quad.y1, position.z, 1.0f);
			v2 uv0	= vec2(quad.s0, quad.t1);

			v4 p1 	= vec4(position.x + quad.x1, position.y - quad.y1, position.z, 1.0f);
			v2 uv1	= vec2(quad.s1, quad.t1);

			v4 p2 	= vec4(position.x + quad.x0, position.y - quad.y0, position.z, 1.0f);
			v2 uv2	= vec2(quad.s0, quad.t0);

			v4 p3 	= vec4(position.x + quad.x1, position.y - quad.y0, position.z, 1.0f);
			v2 uv3	= vec2(quad.s1, quad.t0);

			__gai_renderbuffer_push_textured_quad(buffer, p0, uv0, c0, p1, uv1, c1, p2, uv2, c2, p3, uv3, c3);

			if (shadow)
			{
				v4 s0 = vec4(p0.x + 1.0f, p0.y - 1.0f, p0.z - 1.0f, 1.0f);
				v4 s1 = vec4(p1.x + 1.0f, p1.y - 1.0f, p1.z - 1.0f, 1.0f);
				v4 s2 = vec4(p2.x + 1.0f, p2.y - 1.0f, p2.z - 1.0f, 1.0f);
				v4 s3 = vec4(p3.x + 1.0f, p3.y - 1.0f, p3.z - 1.0f, 1.0f);
				__gai_renderbuffer_push_textured_quad(buffer, s0, uv0, shadow_color, s1, uv1, shadow_color, s2, uv2, shadow_color, s3, uv3, shadow_color);
			}
		}
	}
}

GAI_INL void
gai_renderbuffer_push_text2d(gai_render_buffer *buffer, gai_loaded_font *font, char *string, u32 length, v3 position, u32 color,
                             b32 shadow, u32 shadow_color)
{
	wchar_t* unicode_buffer = (wchar_t*)gai_engine_mem_alloc(length * sizeof(wchar_t));
	GAI_ASSERT(unicode_buffer);

	
	//TODO:(aga) Bring this to the platform layer or just make it cross platform!
	MultiByteToWideChar(CP_UTF8, 0, string, length, unicode_buffer, length);
	i32 unicode_length = wcslen(unicode_buffer);
	gai_renderbuffer_push_text2d(buffer, font, unicode_buffer, unicode_length, position, color, shadow, shadow_color);
	gai_engine_mem_free(unicode_buffer);

}

GAI_INL void
gai_renderbuffer_push_text2d(gai_render_buffer *buffer, gai_loaded_font *font, char *string, u32 length, v3 position, u32 color)
{
	gai_renderbuffer_push_text2d(buffer, font, string, length, position, color, false);
}

GAI_INL gai_render_buffer
gai_renderbuffer_begin(gai_render_commands *commands, i32 pixel_width, i32 pixel_height)
{
	gai_render_buffer result;
	result.screen_dim 				= vec2(pixel_width, pixel_height);
	result.commands 				= commands;
	result.last_setup.projection 	= mat4(1.0f);
	result.last_setup.xtransform 	= mat4(1.0f);
	result.last_setup.cam_pos 		= vec3(0.0f, 0.0f, 0.0f);
	result.missing_resource_count	= 0;
	result.current_quads 			= 0;

	return result;
}

void
gai_renderbuffer_set_shader(gai_render_buffer *buffer, u32 program_id)
{
	buffer->last_setup.shaderid = program_id;
	buffer->current_quads 		= 0;
}


GAI_INL void
gai_renderbuffer_push_projection(gai_render_buffer *buffer, mx4 projection)
{
	buffer->last_setup.projection 	= projection;
	buffer->current_quads 			= 0;
}

GAI_INL void
gai_renderbuffer_push_setup(gai_render_buffer *buffer, gai_render_entry_setup setup)
{
	buffer->last_setup 		= setup;
	buffer->current_quads 	= 0;
}

GAI_INL void
gai_renderbuffer_end(gai_render_buffer *buffer)
{
}