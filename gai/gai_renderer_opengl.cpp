#define DBG_OPENGL(name) do{ GLuint error = glGetError(); if (error) printf("gl error at %s: %i\n", name, error); } while(0)

GAI_DEF void
__gai_glrenderer_internal_shader_callback(gai_glshader *shader, gai_glshader_callback_type type)
{
	switch (type)
	{
		case gaiglBind:
		{
			glBindBuffer(GL_ARRAY_BUFFER, shader->buffers[0]);
			glBindVertexArray(shader->buffers[1]);
			if (shader->attribs[0] == -1)
			{
				shader->attribs[0] = gai_glshader_get_attrib_location(shader, "in_position");
			}
			else
			{
				glEnableVertexAttribArray(shader->attribs[0] );
				glVertexAttribPointer(shader->attribs[0], 4, GL_FLOAT, GL_FALSE, sizeof(gai_textured_vertex), (const GLvoid*) gai_offset_of(gai_textured_vertex, position) );
			}

			if (shader->attribs[1] == -1)
			{
				shader->attribs[1] = gai_glshader_get_attrib_location(shader, "in_normal");
			}
			else
			{
				glEnableVertexAttribArray(shader->attribs[1]);
				glVertexAttribPointer(shader->attribs[1], 3, GL_FLOAT, GL_FALSE, sizeof(gai_textured_vertex), (const GLvoid*) gai_offset_of(gai_textured_vertex, normal) );
			}

			if (shader->attribs[2] == -1)
			{
				shader->attribs[2]  = gai_glshader_get_attrib_location(shader, "in_uv");
			}
			else
			{
				glEnableVertexAttribArray(shader->attribs[2]);
				glVertexAttribPointer(shader->attribs[2],  2, GL_FLOAT, GL_FALSE, sizeof(gai_textured_vertex), (const GLvoid*) gai_offset_of(gai_textured_vertex, uv) );

			}
			if (shader->attribs[3] == -1)
			{
				shader->attribs[3] = gai_glshader_get_attrib_location(shader, "in_color");
			}
			else
			{
				glEnableVertexAttribArray(shader->attribs[3]);
				glVertexAttribPointer(shader->attribs[3], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(gai_textured_vertex), (const GLvoid*) gai_offset_of(gai_textured_vertex, color) );
			}

			if (shader->attribs[4] == -1)
			{
				shader->attribs[4] = gai_glshader_get_uniform_location(shader, "texture");
			}
			else
			{
				gai_glshader_set_uniform1i(shader, shader->attribs[4], 0);
			}
		} break;
		case gaiglUnbind:
		{

			if (shader->attribs[0] == -1)
			{
				glDisableVertexAttribArray(shader->attribs[0]);
			}

			if (shader->attribs[1] == -1)
			{
				glDisableVertexAttribArray(shader->attribs[1]);
			}

			if (shader->attribs[2] == -1)
			{
				glDisableVertexAttribArray(shader->attribs[2]);
			}

			if (shader->attribs[3] == -1)
			{
				glDisableVertexAttribArray(shader->attribs[3]);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		} break;
	}
}

GAI_DEF i32
gai_glrenderer_function_wrapper(gai_glrenderer *renderer)
{
	i32 mapped_functions = 0, unmapped_functions = 0;
#define GAI_ENGINE_PROC(ext, def, a, b, ...) gl##b = (gl_##def *) gai_glrenderer_getprocaddress("gl"#b); if(gl##b) mapped_functions++; else unmapped_functions++;
	GAI_ENGINE_GEN_PROC
#undef GAI_ENGINE_PROC
	if (unmapped_functions > 0)
	{
		printf("Mapped functions: %i/%i\n", mapped_functions, unmapped_functions);
	}
	return mapped_functions;
}

GAI_DEF void*
gai_glrenderer_getprocaddress(const char *name)
{
	#if defined(GAI_OS_WINDOWS)
	return wglGetProcAddress(name);
	#else
	return (void *)glXGetProcAddress((GLubyte*) name);
	#endif
}

GAI_DEF void
gai_glrenderer_init(gai_glrenderer *renderer)
{
	gai_glrenderer_function_wrapper(renderer);

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	u32 num_samples = 8;
	glGenTextures( 1, &renderer->multisample_tex );
	glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, renderer->multisample_tex );
	glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, 1280, 720, false );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures( 1, &renderer->depth_tex );
	glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, renderer->depth_tex );
	glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_DEPTH_COMPONENT, 1280, 720, false );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers( 1, &renderer->multisample_fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, renderer->multisample_fbo );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderer->multisample_tex, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderer->depth_tex, 0 );

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	GAI_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);

	char *version 	= R"GLSL(#version 150)GLSL";
	char *vertex 	= R"GLSL(
		in vec4 in_position;
		//in vec3 in_normal;
		in vec2 in_uv;
		in vec4 in_color;
		uniform mat4 projection;
		out vec2 pass_uv;
		out vec4 pass_color;
		void main()
		{
			gl_Position = projection * in_position;
			pass_uv 	= in_uv;
			pass_color  = in_color;
      	}
	)GLSL";
	char *fragment 	= R"GLSL(
		in vec2 pass_uv;
		in vec4 pass_color;
		uniform sampler2D texture;
		void main()
		{
			vec4 uv = texture2D(texture, pass_uv);
			gl_FragColor = vec4(pass_color.rgb, uv.r * pass_color.a);
      	}
	)GLSL";


	renderer->font_shader = gai_glshader_create_program(renderer, version, vertex, fragment);
	gai_glshader_register_callback(&renderer->font_shader, __gai_glrenderer_internal_shader_callback);
	glGenBuffers(1, &renderer->font_shader.buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->font_shader.buffers[0]);
	glGenVertexArrays(1, &renderer->font_shader.buffers[1]);
	glBindVertexArray(renderer->font_shader.buffers[1]);
	gai_glshader_bind(&renderer->font_shader);

	renderer->default_shader = gai_glshader_create_program_from_file(renderer, "glsl.vert", "glsl.frag");
	gai_glshader_register_callback(&renderer->default_shader, __gai_glrenderer_internal_shader_callback);
	glGenBuffers(1, &renderer->default_shader.buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->default_shader.buffers[0]);
	glGenVertexArrays(1, &renderer->default_shader.buffers[1]);
	glBindVertexArray(renderer->default_shader.buffers[1]);
	gai_glshader_set_debug(&renderer->default_shader);
	gai_glshader_bind(&renderer->default_shader);

	glActiveTexture(GL_TEXTURE0);
}

GAI_DEF void
gai_glrenderer_render(gai_glrenderer *renderer, gai_render_commands *commands, i32 pixel_width, i32 pixel_height)
{
	static gai_loaded_texture *active_texture;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer->multisample_fbo);

	glViewport(0, 0, pixel_width, pixel_height);
	glClearColor(commands->settings.clear_color.r, commands->settings.clear_color.g, commands->settings.clear_color.b, commands->settings.clear_color.a);
	//glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gai_render_commands *cmds = commands;
	for (u8 *header_at = cmds->pushbuffer_base; header_at < cmds->pushbuffer_at; header_at += sizeof(gai_render_entry_header))
	{
		gai_render_entry_header *header = (gai_render_entry_header *) header_at;
		void *data 						= header_at + sizeof(*header);
		switch (header->type)
		{
			case gaiRenderEntryType_gai_render_entry_textured_quads:
			{
				header_at += sizeof(gai_render_entry_textured_quads);
				gai_render_entry_textured_quads *entry = (gai_render_entry_textured_quads*)data;
				if (entry->texture && (!active_texture || (active_texture != entry->texture)))
				{
					glBindTexture(GL_TEXTURE_2D, gai_handle_to_u32(entry->texture->handle));
					active_texture = entry->texture;
				}

				if (entry->setup.shaderid == renderer->font_shader.program)
				{
					gai_glshader_bind(&renderer->font_shader);
				}
				else
				{
					gai_glshader_bind(&renderer->default_shader);
				}

				glBufferData(GL_ARRAY_BUFFER, cmds->vertices_count * sizeof(gai_textured_vertex), cmds->vertices, GL_STATIC_DRAW);
				gai_glshader_set_uniform_matrix_4fv(renderer->active_shader, "projection", 1, true, entry->setup.projection);

				#if 1
				for (u32 vertex_index = entry->vertex_offset;
				        vertex_index < (entry->vertex_offset + 4 * entry->quad_count);
				        vertex_index += 4)
				{
					glDrawArrays(GL_TRIANGLE_STRIP, vertex_index, 4);
				}
				#else
				glDrawArrays(GL_TRIANGLE_STRIP, 0, cmds->vertices_count);
				#endif
			}
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer->multisample_fbo); // Make sure your multisampled FBO is the read framebuffer
	glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
	glBlitFramebuffer(0, 0, pixel_width, pixel_height, 0, 0, pixel_width, pixel_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

GAI_INL void*gai_glrenderer_create_texture2d(gai_glrenderer * renderer, void *data, i32 width, i32 height, GLenum filter, GLenum src_format, GLenum dest_format)
{
	void *result = 0;
	if (data)
	{
		u32 texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//TODO:(aga) switch for filter enum in future
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexImage2D(GL_TEXTURE_2D, 0, src_format, width, height, 0, dest_format, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);

		result = gai_u32_to_handle(texture);
	}
	return result;
}

GAI_INL void
_gai_glshader_set_debug(gai_glshader * shader)
{
	#if GAI_ENGINE_DEBUG_MODE
	shader->last_vertex_shader_info 	= gai_file_getinfo(shader->vertex_shader_file);
	shader->last_fragment_shader_info 	= gai_file_getinfo(shader->fragment_shader_file);
	gai_growable_append(shader->renderer->debug_reloadable_shaders, shader);
	#endif
}

//TODO:(aga) CompareFileTime comes from WIN32 layer right now, make this a crossplatform thingy!!!
GAI_INL void
_gai_glshader_reload_program(gai_glshader *shader)
{
	#if GAI_ENGINE_DEBUG_MODE
	if (!gai_file_exists(shader->vertex_shader_file) || !gai_file_exists(shader->fragment_shader_file))
	{
		shader->link_error = -1;
		return;
	}

	gai_fileinfo now_vertex = gai_file_getinfo(shader->vertex_shader_file);
	gai_fileinfo now_frag = gai_file_getinfo(shader->fragment_shader_file);

	i32 vertex_shader_cmp = (CompareFileTime((FILETIME*)&shader->last_vertex_shader_info.modification_time, (FILETIME*)&now_vertex.modification_time) == -1);
	i32 fragment_shader_cmp = (CompareFileTime((FILETIME*)&shader->last_fragment_shader_info.modification_time, (FILETIME*)&now_frag.modification_time) == -1);

	if ( vertex_shader_cmp || fragment_shader_cmp )
	{
		printf("%s shader changed. Reloading shaders.\n", (vertex_shader_cmp ? shader->vertex_shader_file : shader->fragment_shader_file));

		if (shader->program)
		{
			gai_glshader_unbind(shader);
			gai_glshader_delete_program(shader);
		}

		gai_glshader result = gai_glshader_create_program_from_file(shader->renderer, shader->vertex_shader_file, shader->fragment_shader_file);
		GAI_ASSERT(result.link_error == 0);

		shader->program = result.program;
		gai_glshader_bind(shader);

		shader->last_vertex_shader_info 	= now_vertex;
		shader->last_fragment_shader_info 	= now_frag;
	}
	#endif
}

GAI_INL void
gai_glshader_register_callback(gai_glshader *shader, gai_shader_callback_fn *callback, void *userdata)
{
	GAI_ASSERT(shader);
	GAI_ASSERT(callback);
	shader->userdata = userdata;
	shader->callback = callback;
}

GAI_DEF gai_glshader
gai_glshader_create_program_from_file(gai_glrenderer * renderer, const char *vertex_shader_file, const char *fragment_shader_file)
{
	gai_glshader result = {0};
	result.renderer = renderer;

	gai_loaded_file vertex_source = gai_engine_file_read_content(vertex_shader_file);
	gai_loaded_file fragment_source = gai_engine_file_read_content(fragment_shader_file);

	printf("Loading shader from file(s): vertex_shader_file %s(%i bytes) | fragment_shader_file  %s(%i bytes)\n", vertex_shader_file, vertex_source.size, fragment_shader_file, fragment_source.size);
	if (!vertex_source.data || !fragment_source.data)
	{
		result.link_error = -1;
		return result;
	}

	result = gai_glshader_create_program(renderer, "",  (char *) vertex_source.data, (char *) fragment_source.data);
	if (result.link_error == 0)
	{
		#if GAI_ENGINE_DEBUG_MODE
		result.vertex_shader_file = vertex_shader_file;
		result.fragment_shader_file = fragment_shader_file;
		//gai_growable_append(renderer->debug_reloadable_shaders, result);
		#endif
	}
	else GAI_ASSERT(!"shader load from file failed");

	return result;
}

GAI_DEF gai_glshader
gai_glshader_create_program(gai_glrenderer * renderer, char *shared, char *vertex_shader, char *fragment_shader)
{
	gai_glshader result = {0};
	result.renderer = renderer;
	for (i32 i = 0; i <= gai_array_size(result.attribs); i++)
	{
		result.attribs[i] = -1;
	}


	GLchar *vertex_shader_source[] =
	{
		shared,
		vertex_shader,
	};
	u32 vertex_shader_program = glCreateShader(0x8B31 /* GL_VERTEX_SHADER */);
	glShaderSource(vertex_shader_program, gai_array_size(vertex_shader_source), vertex_shader_source, 0);
	glCompileShader(vertex_shader_program);

	GLchar *fragment_shader_source[] =
	{
		shared,
		fragment_shader,
	};
	u32 fragment_shader_program = glCreateShader(0x8B30 /* GL_FRAGMENT_SHADER */);
	glShaderSource(fragment_shader_program, gai_array_size(fragment_shader_source), fragment_shader_source, 0);
	glCompileShader(fragment_shader_program);

	result.program = glCreateProgram();
	glAttachShader(result.program, vertex_shader_program);
	glAttachShader(result.program, fragment_shader_program);
	glLinkProgram(result.program);

	glValidateProgram(result.program);

	GLint link_success = false;
	glGetProgramiv(result.program, 0x8B82 /* GL_LINK_STATUS */, &link_success);
	if (!link_success)
	{
		GLsizei Ignored;
		char VertexErrors[4096];
		char FragmentErrors[4096];
		char ProgramErrors[4096];
		glGetShaderInfoLog(vertex_shader_program, sizeof(VertexErrors), &Ignored, VertexErrors);
		glGetShaderInfoLog(fragment_shader_program, sizeof(FragmentErrors), &Ignored, FragmentErrors);
		glGetProgramInfoLog(result.program, sizeof(ProgramErrors), &Ignored, ProgramErrors);

		GAI_ASSERT(!"Shader validation failed");
	}

	glDeleteShader(vertex_shader_program);
	glDeleteShader(fragment_shader_program);

	result.link_error = glGetError();
	return result;
}

GAI_DEF b32
gai_glshader_delete_program(gai_glshader * shader)
{
	glDeleteProgram(shader->program);
	shader->program = 0;
	return true;
}

GAI_INL void
gai_glshader_bind(gai_glshader * shader)
{
	if (shader->renderer->active_shader != shader)
	{
		glUseProgram(shader->program);
		shader->renderer->active_shader = shader;
		if (shader->callback) shader->callback(shader, gaiglBind);
	}
}

GAI_INL void
gai_glshader_unbind(gai_glshader * shader)
{
	if (shader->callback) shader->callback(shader, gaiglUnbind);
	shader->renderer->active_shader = 0;
	glUseProgram(0);
}

GAI_INL i32
gai_glshader_get_uniform_location(gai_glshader * shader, const char *name)
{
	return glGetUniformLocation(shader->program, name);
}

GAI_INL i32
gai_glshader_get_attrib_location(gai_glshader * shader, const char *name)
{
	return glGetAttribLocation(shader->program, name);
}

GAI_INL void
gai_glshader_set_uniform1i(gai_glshader * shader, u32 location, i32 value)
{
	glUniform1i(location, value);
}

GAI_INL void
gai_glshader_set_uniform2i(gai_glshader * shader, u32 location, i32 value_a, i32 value_b)
{
	glUniform2i(location, value_a, value_b);
}

GAI_INL void
gai_glshader_set_uniform3i(gai_glshader * shader, u32 location, i32 value_a, i32 value_b, i32 value_c)
{
	glUniform3i(location, value_a, value_b, value_c);
}

GAI_INL void
gai_glshader_set_uniform4i(gai_glshader * shader, u32 location, i32 value_a, i32 value_b, i32 value_c, i32 value_d)
{
	glUniform4i(location, value_a, value_b, value_c, value_d);
}

GAI_INL void
gai_glshader_set_uniformiv(gai_glshader * shader, u32 location, i32 count, i32 * array, i32 elements)
{
	switch (elements)
	{
		case 1: { glUniform1iv(location, count, array); } break;
		case 2: { glUniform2iv(location, count, array); } break;
		case 3: { glUniform3iv(location, count, array); } break;
		case 4: { glUniform4iv(location, count, array); } break;
	}
}

GAI_INL void
gai_glshader_set_uniform1f(gai_glshader * shader, u32 location, r32 value)
{
	glUniform1f(location, value);
}

GAI_INL void
gai_glshader_set_uniform2f(gai_glshader * shader, u32 location, r32 value_a, r32 value_b)
{
	glUniform2f(location, value_a, value_b);
}

GAI_INL void
gai_glshader_set_uniform3f(gai_glshader * shader, u32 location, r32 value_a, r32 value_b, r32 value_c)
{
	glUniform3f(location, value_a, value_b, value_c);
}

GAI_INL void
gai_glshader_set_uniform4f(gai_glshader * shader, u32 location, r32 value_a, r32 value_b, r32 value_c, r32 value_d)
{
	glUniform4f(location, value_a, value_b, value_c, value_d);
}

GAI_INL void
gai_glshader_set_uniformfv(gai_glshader * shader, u32 location, i32 count, r32 * array, i32 elements)
{
	switch (elements)
	{
		case 1: { glUniform1fv(location, count, array); } break;
		case 2: { glUniform2fv(location, count, array); } break;
		case 3: { glUniform3fv(location, count, array); } break;
		case 4: { glUniform4fv(location, count, array); } break;
	}
}

GAI_INL void
gai_glshader_set_uniform_matrix_4fv(gai_glshader * shader, u32 location, i32 count, b32 transpose, mx4 matrix)
{
	glUniformMatrix4fv(location, count, transpose, matrix.T);
}

GAI_INL void
gai_glshader_set_uniform1i(gai_glshader * shader, const char *uniform, i32 value)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform1i(location, value);
}

GAI_INL void
gai_glshader_set_uniform2i(gai_glshader * shader, const char *uniform, i32 value_a, i32 value_b)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform2i(location, value_a, value_b);
}

GAI_INL void
gai_glshader_set_uniform3i(gai_glshader * shader, const char *uniform, i32 value_a, i32 value_b, i32 value_c)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform3i(location, value_a, value_b, value_c);
}

GAI_INL void
gai_glshader_set_uniform4i(gai_glshader * shader, const char *uniform, i32 value_a, i32 value_b, i32 value_c, i32 value_d)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform4i(location, value_a, value_b, value_c, value_d);
}

GAI_INL void
gai_glshader_set_uniformiv(gai_glshader * shader, const char *uniform, i32 count, i32 * array, i32 elements)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	switch (elements)
	{
		case 1: { glUniform1iv(location, count, array); } break;
		case 2: { glUniform2iv(location, count, array); } break;
		case 3: { glUniform3iv(location, count, array); } break;
		case 4: { glUniform4iv(location, count, array); } break;
	}
}

GAI_INL void
gai_glshader_set_uniform1f(gai_glshader * shader, const char *uniform, r32 value)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform1f(location, value);
}

GAI_INL void
gai_glshader_set_uniform2f(gai_glshader * shader, const char *uniform, r32 value_a, r32 value_b)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform2f(location, value_a, value_b);
}

GAI_INL void
gai_glshader_set_uniform3f(gai_glshader * shader, const char *uniform, r32 value_a, r32 value_b, r32 value_c)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform3f(location, value_a, value_b, value_c);
}

GAI_INL void
gai_glshader_set_uniform4f(gai_glshader * shader, const char *uniform, r32 value_a, r32 value_b, r32 value_c, r32 value_d)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniform4f(location, value_a, value_b, value_c, value_d);
}

GAI_INL void
gai_glshader_set_uniformfv(gai_glshader * shader, const char *uniform, i32 count, r32 * array, i32 elements)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	switch (elements)
	{
		case 1: { glUniform1fv(location, count, array); } break;
		case 2: { glUniform2fv(location, count, array); } break;
		case 3: { glUniform3fv(location, count, array); } break;
		case 4: { glUniform4fv(location, count, array); } break;
	}
}

GAI_INL void
gai_glshader_set_uniform_matrix_4fv(gai_glshader * shader, const char *uniform, i32 count, b32 transpose, mx4 matrix)
{
	u32 location = gai_glshader_get_uniform_location(shader, uniform);
	glUniformMatrix4fv(location, count, transpose, matrix.T);
}


#undef GAI_ENGINE_GEN_PROC