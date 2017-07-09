#ifndef _INCLUDE_GAI_RENDERER_OPENGL_H

#define GL_R8                             0x8229

#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE0                       0x84C0

#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4

#define GL_COLOR_ATTACHMENT0              0x8CE0

#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_FRAMEBUFFER                    0x8D40
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_MULTISAMPLE                    0x809D

#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E

typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef void (*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#define GAI_ENGINE_GEN_PROC \
	GAI_ENGINE_PROC(, attach_shader_fn, void, AttachShader, GLuint, GLuint) \
	GAI_ENGINE_PROC(, compile_shader_fn, void, CompileShader, GLuint) \
	GAI_ENGINE_PROC(, create_program_fn, GLuint, CreateProgram, void) \
	GAI_ENGINE_PROC(, create_shader_fn, GLuint, CreateShader, GLenum) \
	GAI_ENGINE_PROC(, delete_program_fn, void, DeleteProgram, GLuint) \
	GAI_ENGINE_PROC(, delete_shader_fn, void, DeleteShader, GLuint) \
	GAI_ENGINE_PROC(, detach_shader_fn, void, DetachShader, GLuint, GLuint) \
	GAI_ENGINE_PROC(, disable_vertex_attrib_array_fn, void, DisableVertexAttribArray, GLuint) \
	GAI_ENGINE_PROC(, enable_vertex_attrib_array_fn, void, EnableVertexAttribArray, GLuint) \
	GAI_ENGINE_PROC(, get_program_iv_fn, void, GetProgramiv, GLuint, GLenum, GLint*) \
	GAI_ENGINE_PROC(, get_program_info_log_fn, void, GetProgramInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	GAI_ENGINE_PROC(, get_shader_info_log_fn, void, GetShaderInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	GAI_ENGINE_PROC(, get_shader_source_fn, void, GetShaderSource, GLuint, GLsizei, GLsizei*, GLchar *) \
	GAI_ENGINE_PROC(, get_uniform_location_fn, GLint, GetUniformLocation, GLuint, const GLchar*) \
	GAI_ENGINE_PROC(, get_attrib_location_fn, GLint, GetAttribLocation, GLuint, const GLchar*) \
	GAI_ENGINE_PROC(, link_program_fn, void, LinkProgram, GLuint) \
	GAI_ENGINE_PROC(, shader_source_fn, void, ShaderSource, GLuint, GLsizei, GLchar**, GLint*) \
	GAI_ENGINE_PROC(, use_program_fn, void, UseProgram, GLuint) \
	GAI_ENGINE_PROC(, uniform_1f_fn, void, Uniform1f, GLint, GLfloat) \
	GAI_ENGINE_PROC(, uniform_2f_fn, void, Uniform2f, GLint, GLfloat, GLfloat) \
	GAI_ENGINE_PROC(, uniform_3f_fn, void, Uniform3f, GLint, GLfloat, GLfloat, GLfloat) \
	GAI_ENGINE_PROC(, uniform_4f_fn, void, Uniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat) \
	GAI_ENGINE_PROC(, uniform_1i_fn, void, Uniform1i, GLint, GLint) \
	GAI_ENGINE_PROC(, uniform_2i_fn, void, Uniform2i, GLint, GLint, GLint) \
	GAI_ENGINE_PROC(, uniform_3i_fn, void, Uniform3i, GLint, GLint, GLint, GLint) \
	GAI_ENGINE_PROC(, uniform_4i_fn, void, Uniform4i, GLint, GLint, GLint, GLint, GLint) \
	GAI_ENGINE_PROC(, uniform_1fv_fn, void, Uniform1fv, GLint, GLsizei, const GLfloat*) \
	GAI_ENGINE_PROC(, uniform_2fv_fn, void, Uniform2fv, GLint, GLsizei, const GLfloat*) \
	GAI_ENGINE_PROC(, uniform_3fv_fn, void, Uniform3fv, GLint, GLsizei, const GLfloat*) \
	GAI_ENGINE_PROC(, uniform_4fv_fn, void, Uniform4fv, GLint, GLsizei, const GLfloat*) \
	GAI_ENGINE_PROC(, uniform_1iv_fn, void, Uniform1iv, GLint, GLsizei, const GLint*) \
	GAI_ENGINE_PROC(, uniform_2iv_fn, void, Uniform2iv, GLint, GLsizei, const GLint*) \
	GAI_ENGINE_PROC(, uniform_3iv_fn, void, Uniform3iv, GLint, GLsizei, const GLint*) \
	GAI_ENGINE_PROC(, uniform_4iv_fn, void, Uniform4iv, GLint, GLsizei, const GLint*) \
	GAI_ENGINE_PROC(, uniform_matrix_2fv_fn, void, UniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAI_ENGINE_PROC(, uniform_matrix_3fv_fn,	void, UniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAI_ENGINE_PROC(, uniform_matrix_4fv_fn,	void, UniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAI_ENGINE_PROC(, validate_program_fn, void, ValidateProgram ,GLuint) \
	GAI_ENGINE_PROC(, vertex_attrib_pointer_fn, void, VertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *) \
	GAI_ENGINE_PROC(, debug_message_callback_fn, void, DebugMessageCallback, DEBUGPROC, void *) \
	GAI_ENGINE_PROC(, get_string_i_fn, GLubyte*,	GetStringi, GLenum, GLuint ) \
	GAI_ENGINE_PROC(, bind_buffer_fn, void, BindBuffer, GLenum, GLuint buffer) \
	GAI_ENGINE_PROC(, delete_buffers_fn, void, DeleteBuffers, GLsizei, const GLuint*) \
	GAI_ENGINE_PROC(, gen_buffers_fn, void, GenBuffers, GLsizei, GLuint*) \
	GAI_ENGINE_PROC(, buffer_data_fn, void, BufferData, GLenum, GLsizeiptr, const void*, GLenum) \
	GAI_ENGINE_PROC(, buffer_sub_data_fn, void, BufferSubData, GLenum, GLintptr, GLsizeiptr, const void *) \
	GAI_ENGINE_PROC(, map_buffer_fn, GLvoid*, MapBuffer, GLenum, GLenum) \
	GAI_ENGINE_PROC(, unmap_buffer_fn, void, UnmapBuffer, GLenum) \
	GAI_ENGINE_PROC(, bind_vertex_array_fn, void, BindVertexArray, GLuint) \
	GAI_ENGINE_PROC(, delete_vertex_arrays_fn, void, DeleteVertexArrays, GLsizei, const GLuint*) \
	GAI_ENGINE_PROC(, gen_vertex_arrays_fn, void, GenVertexArrays, GLsizei, GLuint*) \
	GAI_ENGINE_PROC(, active_texture_fn, void, ActiveTexture, GLenum) \
	/*GAI_ENGINE_PROC(, tex_image2d_fn, void, TexImage2D, GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*)*/ \
	GAI_ENGINE_PROC(, tex_image2d_multisample_fn, void, TexImage2DMultisample, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLboolean) \
	GAI_ENGINE_PROC(, bind_framebuffer_fn, void, BindFramebuffer, GLenum, GLuint) \
	GAI_ENGINE_PROC(, gen_framebuffers_fn, void, GenFramebuffers, GLsizei, GLuint *) \
	GAI_ENGINE_PROC(, framebuffer_texture2d_fn, void, FramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint) \
	GAI_ENGINE_PROC(, blit_framebuffer_fn, void, BlitFramebuffer, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum) \
	GAI_ENGINE_PROC(, check_framebuffer_status_fn, GLenum, CheckFramebufferStatus, GLenum) \
	GAI_ENGINE_PROC(, map_buffer_range_fn, GLvoid*, MapBufferRange, GLenum, GLintptr, GLsizeiptr, GLbitfield) /* GL_ARB_map_buffer_range */ \
	GAI_ENGINE_PROC(, flush_mapped_buffer_range_fn, void, FlushMappedBufferRange, GLenum, GLintptr, GLsizeiptr) /* GL_ARB_map_buffer_range */ \
	GAI_ENGINE_PROC(, buffer_storage_fn,	void, BufferStorage, GLenum, GLsizeiptr, const void *, GLbitfield ) /* GL_ARB_buffer_storage */ \
	GAI_ENGINE_PROC(, blend_equation_fn,	void, BlendEquation, GLenum) \
	GAI_ENGINE_PROC(, blend_func_seperate_fn, void, BlendFuncSeparate, GLenum, GLenum, GLenum, GLenum) \
	GAI_ENGINE_PROC(, fence_sync_fn, GLsync,	FenceSync, GLenum, GLbitfield) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, is_sync_fn, GLboolean,	IsSync, GLsync) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, delete_sync_fn, void, DeleteSync, GLsync) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, client_wait_sync_fn, GLenum, ClientWaitSync, GLsync, GLbitfield, GLuint64) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, wait_sync_fn, void, WaitSync, GLsync, GLbitfield, GLuint64) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, get_integer_64v_fn, void, GetInteger64v, GLenum, GLint64*) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, get_sync_iv_fn, void, GetSynciv, GLsync, GLenum, GLsizei, GLsizei*, GLint*) /* GL_ARB_sync */ \
	GAI_ENGINE_PROC(, tex_storage_1d_fn, void, TexStorage1D, GLenum, GLsizei, GLenum, GLsizei) /* GL_ARB_texture_storage */ \
	GAI_ENGINE_PROC(, tex_storage_2d_fn, void, TexStorage2D, GLenum, GLsizei, GLenum, GLsizei, GLsizei) /* GL_ARB_texture_storage */ \
	GAI_ENGINE_PROC(, tex_storage_3d_fn, void, TexStorage3D, GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei) /* GL_ARB_texture_storage */

#define GAI_ENGINE_PROC(ext, def, a, b, ...) typedef a (gl_##def) (__VA_ARGS__);
GAI_ENGINE_GEN_PROC
#undef GAI_ENGINE_PROC

#define GAI_ENGINE_PROC(ext, def, a, b, ...) gl_##def *gl##b;
//struct gai_glwrapper
//{
	GAI_ENGINE_GEN_PROC
//};
#undef GAI_ENGINE_PROC

struct gai_glshader;
struct gai_glrenderer;

enum gai_glshader_callback_type {
	gaiglBind, gaiglUnbind
};
#define GAI_SHADER_CALLBACK(name) void name(gai_glshader *shader, gai_glshader_callback_type type)
typedef GAI_SHADER_CALLBACK(gai_shader_callback_fn);

#define GAI_GL_SHADER_MAX_BUFFERS 		8
#define GAI_GL_SHADER_MAX_ATTRIBUTES 	16
struct gai_glshader
{
	u32 			program;
	i32				link_error;
	
	u32				buffers[GAI_GL_SHADER_MAX_BUFFERS];
	u32				attribs[GAI_GL_SHADER_MAX_ATTRIBUTES];

	gai_shader_callback_fn	*callback;
	void					*userdata;
	gai_glrenderer 			*renderer;
	
	#if GAI_ENGINE_DEBUG_MODE
	const char 		*vertex_shader_file;
	gai_fileinfo	last_vertex_shader_info;
	const char 		*fragment_shader_file;
	gai_fileinfo	last_fragment_shader_info;
	#endif
};

struct gai_glrenderer
{
	gai_glshader 	*active_shader;

	gai_glshader 	default_shader;
	gai_glshader 	font_shader;

	u32				multisample_fbo;
	u32				multisample_tex;
	u32 			depth_tex;

	#if GAI_ENGINE_DEBUG_MODE
	gai_growable(gai_glshader*) debug_reloadable_shaders;
	#endif
};

#if GAI_ENGINE_DEBUG_MODE
	#define GAI_ENGINE_DEBUG_SHADER_INTERVAL 2.0f
	#define gai_glshader_set_debug(shader) _gai_glshader_set_debug((shader))
	#define gai_glshader_reload_program(shader) _gai_glshader_reload_program((shader))
#else
	#define gai_glshader_set_debug(shader)
	#define gai_glshader_reload_program(shader)
#endif

//NOTE:(aga) Debug functions for automatic shader reload on modification
GAI_INL void			_gai_glshader_set_debug(gai_glshader *shader);
GAI_INL void 			_gai_glshader_reload_program(gai_glshader *shader);

GAI_DEF void* 			gai_glrenderer_getprocaddress(const char *name);
GAI_DEF void			gai_grenderer_init(gai_glrenderer *renderer);
GAI_DEF void			gai_glrenderer_render(gai_glrenderer *renderer, gai_render_commands *commands, i32 pixel_width, i32 pixel_height);

GAI_INL void 			gai_glshader_register_callback(gai_glshader *shader, gai_shader_callback_fn *callback, void *userdata = 0);
GAI_DEF gai_glshader 	gai_glshader_create_program_from_file(gai_glrenderer *renderer, const char *vertex_shader_file, const char *fragment_shader_file);
GAI_DEF gai_glshader 	gai_glshader_create_program(gai_glrenderer *renderer, char *shared, char *vertex_shader, char *fragment_shader);
GAI_DEF b32 			gai_glshader_delete_program(gai_glshader *program);
GAI_INL void 			gai_glshader_bind(gai_glshader *shader);
GAI_INL void 			gai_glshader_unbind(gai_glshader *shader);
GAI_INL i32 			gai_glshader_get_uniform_location(gai_glshader *shader, const char *name);
GAI_INL i32 			gai_glshader_get_attrib_location(gai_glshader *shader, const char *name);
GAI_INL void 			gai_glshader_set_attrib_array_by_format(gai_glshader *shader, u32 *format, u32 count);

GAI_INL void 			gai_glshader_set_uniform1i(gai_glshader *shader, u32 location, i32 value);
GAI_INL void 			gai_glshader_set_uniform2i(gai_glshader *shader, u32 location, i32 value_a, i32 value_b);
GAI_INL void 			gai_glshader_set_uniform3i(gai_glshader *shader, u32 location, i32 value_a, i32 value_b, i32 value_c);
GAI_INL void 			gai_glshader_set_uniform4i(gai_glshader *shader, u32 location, i32 value_a, i32 value_b, i32 value_c, i32 value_d);
GAI_INL void 			gai_glshader_set_uniformiv(gai_glshader *shader, u32 location, i32 count, i32 *array, i32 elements);
GAI_INL void 			gai_glshader_set_uniform1f(gai_glshader *shader, u32 location, r32 value);
GAI_INL void 			gai_glshader_set_uniform2f(gai_glshader *shader, u32 location, r32 value_a, r32 value_b);
GAI_INL void 			gai_glshader_set_uniform3f(gai_glshader *shader, u32 location, r32 value_a, r32 value_b, r32 value_c);
GAI_INL void 			gai_glshader_set_uniform4f(gai_glshader *shader, u32 location, r32 value_a, r32 value_b, r32 value_c, r32 value_d);
GAI_INL void 			gai_glshader_set_uniformfv(gai_glshader *shader, u32 location, i32 count, r32 *array, i32 elements);
GAI_INL void			gai_glshader_set_uniform_matrix_4fv(gai_glshader *shader, u32 location, i32 count, b32 transpose, mx4 matrix);

GAI_INL void 			gai_glshader_set_uniform1i(gai_glshader *shader, const char *uniform, i32 value);
GAI_INL void 			gai_glshader_set_uniform2i(gai_glshader *shader, const char *uniform, i32 value_a, i32 value_b);
GAI_INL void 			gai_glshader_set_uniform3i(gai_glshader *shader, const char *uniform, i32 value_a, i32 value_b, i32 value_c);
GAI_INL void 			gai_glshader_set_uniform4i(gai_glshader *shader, const char *uniform, i32 value_a, i32 value_b, i32 value_c, i32 value_d);
GAI_INL void 			gai_glshader_set_uniformiv(gai_glshader *shader, const char *uniform, i32 count, i32 *array, i32 elements);
GAI_INL void 			gai_glshader_set_uniform1f(gai_glshader *shader, const char *uniform, r32 value);
GAI_INL void 			gai_glshader_set_uniform2f(gai_glshader *shader, const char *uniform, r32 value_a, r32 value_b);
GAI_INL void 			gai_glshader_set_uniform3f(gai_glshader *shader, const char *uniform, r32 value_a, r32 value_b, r32 value_c);
GAI_INL void 			gai_glshader_set_uniform4f(gai_glshader *shader, const char *uniform, r32 value_a, r32 value_b, r32 value_c, r32 value_d);
GAI_INL void 			gai_glshader_set_uniformfv(gai_glshader *shader, const char *uniform, i32 count, r32 *array, i32 elements);
GAI_INL void			gai_glshader_set_uniform_matrix_4fv(gai_glshader *shader, const char *uniform, i32 count, b32 transpose, mx4 matrix);

#define _INCLUDE_GAI_RENDERER_OPENGL_H
#endif