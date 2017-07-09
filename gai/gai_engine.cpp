static gai_engine *_gai_engine_context;

GAI_INL
GAI_ENGINE_ALLOC_PROC(gai_engine_platform_alloc_x)
{
	#if defined(GAI_OS_WINDOWS)
	return VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	#else
	return malloc(size);
	#endif
}

GAI_INL
GAI_ENGINE_FREE_PROC(gai_engine_platform_free_x)
{
	#if defined(GAI_OS_WINDOWS)
	VirtualFree(ptr, 0, MEM_RELEASE);
	#else
	free(ptr);
	#endif
}

GAI_INL
GAI_ENGINE_READ_FILE_CONTENT_PROC(gai_engine_platform_read_file_content_x)
{
	gai_loaded_file result;
	gai_file f = gai_file_open(filename, "rb");
	{
		result.size = f.size;
		result.data = (void *) gai_engine_platform_alloc_x(f.size + 1);
		u8 *buffer = (u8*) result.data;
		i32 read_bytes = gai_file_read_all(&f, buffer);
		buffer[read_bytes] = 0;
		gai_file_close(&f);
	}
	return result;
}

GAI_INL
GAI_ENGINE_FREE_FILE_MEMORY_PROC(gai_engine_platform_free_file_memory_x)
{
	gai_engine_platform_free_x(file->data);
}

GAI_INL gai_render_commands
gai_render_commands_default(gai_allocator allocator,
                            u32 pushbuffer_max, u8 *pushbuffer_base, u8* pushbuffer_at,
                            gai_textured_vertex *vertices, u32 vertices_max,
                            u32 width, u32 height)
{
	gai_render_commands result = {0};
	result.settings.width  	= width;
	result.settings.height 	= height;
	result.pushbuffer_max  	= pushbuffer_max;
	result.pushbuffer_base 	= pushbuffer_base;
	result.pushbuffer_at   	= pushbuffer_at;
	result.vertices 		= vertices;
	result.vertices_max 	= vertices_max;
	return result;
}

GAI_INL gai_engine*
gai_engine_get_context()
{
	return _gai_engine_context;
}

GAI_INL void
gai_engine_set_context(gai_engine *engine)
{
	_gai_engine_context = engine;
}


GAI_INL void*
gai_engine_mem_alloc(size_t size)
{
	void *result = 0;
	if (_gai_engine_context)
	{
		result = _gai_engine_context->platform_api.alloc(size);
		GAI_ASSERT(result);
	}
	return result;
}

GAI_INL void
gai_engine_mem_free(void *address)
{
	GAI_ASSERT(address);
	if (_gai_engine_context)
	{
		_gai_engine_context->platform_api.free(address);
	}
}

GAI_INL gai_loaded_file
gai_engine_file_read_content(const char *filename)
{
	gai_loaded_file result = {0};
	if (_gai_engine_context) result = _gai_engine_context->platform_api.read_file_content(filename);
	return result;
}

GAI_INL void
gai_engine_file_free(gai_loaded_file *file)
{
	GAI_ASSERT(file);
	if (_gai_engine_context) _gai_engine_context->platform_api.free_file_memory(file);
}


GAI_INL i32
gai_engine_init(gai_engine *engine, const char *title, u16 flags, i32 width, i32 height, i32 x, i32 y, gai_engine_setup setup)
{
	i32 success = 0;

	engine->platform_api.alloc             = gai_engine_platform_alloc_x;
	engine->platform_api.free              = gai_engine_platform_free_x;
	engine->platform_api.read_file_content = gai_engine_platform_read_file_content_x;
	engine->platform_api.free_file_memory  = gai_engine_platform_free_file_memory_x;
	gai_allocator engine_allocator 		   = gai_allocator_init(engine->platform_api.alloc, engine->platform_api.free);

	gai_engine_set_context(engine);

	switch (setup.type)
	{
		case gaiWindowDriverDefault:
		{
			success = gai_window_init(&_gai_engine_context->window, title, width, height, flags);
		} break;
		case gaiWindowDriverOpenGL:
		{
			success = gai_window_init_gl(&_gai_engine_context->window, title, width, height, flags,
			                             setup.opengl.major, setup.opengl.minor, setup.opengl.core, setup.opengl.debug);
			if (success)
			{
				#if GAI_ENGINE_DEBUG_MODE
				gai_growable_init(_gai_engine_context->renderer.opengl.debug_reloadable_shaders, engine_allocator);
				#endif
				gai_window_vsync(&_gai_engine_context->window, setup.swap_interval);
				gai_glrenderer_init(&_gai_engine_context->renderer.opengl);
			}
		} break;
		default:
		{
			GAI_ASSERT(!"Invalid codepath");
		} break;
	}
	if (success == 1)
	{
		gai_window_set_position(&_gai_engine_context->window, x, y);
		gai_window_show(&_gai_engine_context->window);
		if (setup.render_buffer_size_mb <= 0) setup.render_buffer_size_mb = GAI_ENGINE_DEFAULTS_RENDERBUFFER_SIZE;

		u32 vertex_max 					= 1 << 20;
		u32 vertex_array_size			= sizeof(gai_textured_vertex) * vertex_max;
		gai_textured_vertex *vertices 	= (gai_textured_vertex *) gai_engine_mem_alloc(vertex_array_size);

		u32 pushbuffer_size 			= gai_megabytes(setup.render_buffer_size_mb);
		u8 *pushbuffer_memory			= (u8*) gai_engine_mem_alloc(pushbuffer_size);
		_gai_engine_context->commands 	= gai_render_commands_default(engine_allocator,
		                                                              pushbuffer_size, pushbuffer_memory, pushbuffer_memory,
		                                                              vertices, vertex_max,
		                                                              width, height);

		gai_loaded_texture white_tex = {};
		white_tex.width 	= 128;
		white_tex.height 	= 128;
		white_tex.data 		= (u8*)gai_engine_mem_alloc(white_tex.width * white_tex.height * 4);
		memset(white_tex.data, 255, white_tex.width * white_tex.height * 4);
		white_tex.handle  	= gai_glrenderer_create_texture2d(&_gai_engine_context->renderer.opengl, white_tex.data,
		                                                      128, 128, GL_LINEAR, GL_RGBA,  GL_RGBA);
		gai_engine_mem_free(white_tex.data);
		_gai_engine_context->commands.white_texture = white_tex;

		printf("gai engine initialized. (rtype: %u, rbufsize: %.2f mb, rviewport: %ix%i)\n",
		       _gai_engine_context->window.type, (pushbuffer_size / 1024.f / 1024.f),
		       _gai_engine_context->window.w, _gai_engine_context->window.h);
	}
	GAI_ASSERT(success == 1);
	return success;
}

GAI_INL void
gai_engine_deinit()
{
	gai_window_destroy(&_gai_engine_context->window);

	if (_gai_engine_context->commands.pushbuffer_base) gai_engine_mem_free(_gai_engine_context->commands.pushbuffer_base);
	gai_engine_mem_free(_gai_engine_context->commands.vertices);
}

GAI_INL r64
gai_engine_update()
{
	switch (_gai_engine_context->window.type)
	{
		case gaiWindowDriverOpenGL:
		{
			#if GAI_ENGINE_DEBUG_MODE
			static r32 check_timer;
			if (check_timer > GAI_ENGINE_DEBUG_SHADER_INTERVAL)
			{

				i32 debug_shader_count = gai_growable_count(_gai_engine_context->renderer.opengl.debug_reloadable_shaders);
				if (debug_shader_count > 0)
				{
					for (i32 i = 0; i < debug_shader_count; i++)
					{
						gai_glshader *shader = _gai_engine_context->renderer.opengl.debug_reloadable_shaders[i];
						gai_glshader_reload_program(shader);
					}
				}
				check_timer = 0.0f;
			}
			else check_timer += _gai_engine_context->window.dt;

			#endif
			gai_glrenderer_render(&_gai_engine_context->renderer.opengl, &_gai_engine_context->commands,
			                      _gai_engine_context->window.w, _gai_engine_context->window.h);

		} break;
		default:
		{
			GAI_ASSERT(!"Invalid codepath");
		} break;
	}

	_gai_engine_context->commands.vertices_count 	= 0;
	_gai_engine_context->commands.pushbuffer_at 	= _gai_engine_context->commands.pushbuffer_base;
	gai_window_display(&_gai_engine_context->window);

	// NOTE:(aga) Blocking for message is not implemented yet and may never be? Don't know if i want an event driven engine?!?!?!
	gai_window_handle_messages(&_gai_engine_context->window, false);

	return _gai_engine_context->window.dt;
}

/*
GAI_INL void
gai_engine_display()
{
	if (!_gai_engine_context) return;

	switch (_gai_engine_context->window.type)
	{
		case gaiWindowDriverOpenGL:
		{
			gai_glrenderer_display_opengl(&_gai_engine_context->renderer.opengl, &_gai_engine_context->commands, _gai_engine_context->window.w, _gai_engine_context->window.h);
			gai_window_display(&_gai_engine_context->window);
		} break;
		default:
		{
			GAI_ASSERT(!"Invalid codepath");
		} break;
	}
};
*/

GAI_INL b32
gai_engine_running()
{
	return !_gai_engine_context->window.was_closed;
}

GAI_INL void
gai_engine_stop()
{
	_gai_engine_context->window.was_closed 		= true;
	_gai_engine_context->window.request_close 	= true;
}

GAI_INL b32
gai_engine_closed()
{
	return _gai_engine_context->window.request_close;
}

GAI_INL b32
gai_engine_active()
{
	return gai_window_is_foreground(&_gai_engine_context->window);
}

GAI_INL b32
gai_engine_inactive()
{
	return !gai_window_is_foreground(&_gai_engine_context->window);
}

//GAI_INL b32			gai_engine_minimized();
//GAI_INL b32			gai_engine_maximized();

GAI_INL b32
gai_engine_vsync(b32 state)
{
	b32 result = false;
	switch (_gai_engine_context->window.type)
	{
		case gaiWindowDriverOpenGL:
		{
			gai_window_vsync(&_gai_engine_context->window, state);
			result = (gai_window_vsync_enabled(&_gai_engine_context->window) == state);
		} break;
		default:
		{
			GAI_ASSERT(!"Invalid codepath");
		} break;
	}
	return result;
}

GAI_INL b32
gai_engine_vsync_enabled()
{
	b32 result = false;
	switch (_gai_engine_context->window.type)
	{
		case gaiWindowDriverOpenGL:
		{
			result = gai_window_vsync_enabled(&_gai_engine_context->window);
		} break;
		default:
		{
			GAI_ASSERT(!"Invalid codepath");
		} break;
	}
	return result;
}


GAI_INL void
gai_engine_borderless(b32 state)
{
	gai_window_borderless(&_gai_engine_context->window, state);
}

GAI_INL b32
gai_engine_borderless_enabled()
{
	return ((_gai_engine_context->window.flags & gaiWindowFlagsBorderless) > 0);
}

GAI_INL r64
gai_engine_delta_time()
{
	return _gai_engine_context->window.dt;
}

GAI_INL i32
gai_engine_monitor_vrefresh()
{
	i32 result = 0;
	#if defined(GAI_OS_WINDOWS)
	HDC hdc = GetDC((HWND)_gai_engine_context->window.handle);
	result = GetDeviceCaps(hdc, VREFRESH);
	ReleaseDC(0, hdc);
	#else
#error not implemented for this platform!
	#endif
	return result;
}

GAI_INL void
gai_engine_register_callback(gai_callback_type_enum type, void *callback, void *userdata)
{
	gai_window_register_callback(&_gai_engine_context->window, type, callback, userdata);
}

GAI_INL void
gai_engine_unregister_callback(void *callback)
{
	gai_window_unregister_callback(&_gai_engine_context->window, callback);
}

GAI_INL u8
gai_engine_key_state(i32 key)
{
	return _gai_engine_context->window.input.keyboard_keys[key];
}

GAI_INL b32
gai_engine_key_down(i32 key)
{
	return gai_key_down(&_gai_engine_context->window, key);
}

GAI_INL b32
gai_engine_key_released(i32 key)
{
	return gai_key_released(&_gai_engine_context->window, key);
}

GAI_INL b32
gai_engine_key_pressed(i32 key)
{
	return  gai_key_pressed(&_gai_engine_context->window, key);
}


GAI_INL u8
gai_engine_mouse_state(i32 key)
{
	return _gai_engine_context->window.input.mouse_buttons[key];
}

GAI_INL b32
gai_engine_mouse_down(i32 btn)
{
	return gai_mouse_down(&_gai_engine_context->window, btn);
}

GAI_INL b32
gai_engine_mouse_released(i32 btn)
{
	return gai_mouse_released(&_gai_engine_context->window, btn);
}

GAI_INL b32
gai_engine_mouse_pressed(i32 btn)
{
	return gai_mouse_pressed(&_gai_engine_context->window, btn);
}

GAI_INL v2
gai_engine_mouse_position()
{
	v2 result = { (r32) _gai_engine_context->window.input.mouse_x, (r32) _gai_engine_context->window.input.mouse_y };
	return result;
}

GAI_INL void
gai_engine_font_load_from_file(gai_loaded_font *font, char *filename, i32 size)
{
	if (!font) return;

	gai_engine *engine = gai_engine_get_context();
	GAI_ASSERT(engine);

	gai_loaded_file file = gai_engine_file_read_content(filename);
	GAI_ASSERT(file.data);

	i32 texture_size = 1024;
	u8 *texture_buffer = (u8*) gai_engine_mem_alloc(texture_size * texture_size);
	memset(texture_buffer, 0, texture_size * texture_size);
	GAI_ASSERT(texture_buffer);

	stbtt_pack_context context = {};
	stbtt_PackBegin(&context, texture_buffer, texture_size, texture_size, 0, 1, 0);
	//stbtt_PackSetOversampling(&context, 2, 3);
	//stbtt_PackFontRanges()
	stbtt_PackFontRange(&context, (u8*)file.data, 0, size, 0, 255, font->glyph_data);
	stbtt_PackEnd(&context);

	switch (engine->window.type)
	{
		case gaiWindowDriverOpenGL:
		{
			font->font_texture.handle = gai_glrenderer_create_texture2d(&engine->renderer.opengl, texture_buffer, texture_size, texture_size, GL_LINEAR, GL_RED, GL_RED);
		} break;
		default: { GAI_ASSERT(!"UNKNOWN WINDOW DRIVER"); };
	}

	font->font_texture.width  = texture_size;
	font->font_texture.height = texture_size;
	font->size = size;
	gai_engine_mem_free(texture_buffer);

	gai_engine_file_free(&file);
}

u32
gai_engine_hash_string(char *str)
{
	u32 hash = 5381;
	i32 c;

	while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
