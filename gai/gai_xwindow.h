/**
 * @file 				gai_xwindow.h
 *
 * @brief      			Requests a window for all currently supported platforms.
 *
 * @attention 			__ONLY WINDOWS SUPPORT FOR NOW__ \n
 * Renderer | Linker Dependencies (win32) | Linker Dependencies (linux)
 * :--------|:---------------------|:----------------------
 * Default  | user32.lib | Not implemented yet!
 * OpenGL   | user32.lib winmm.lib gdi32.lib opengl32.lib | Not implemented yet!
 * @note  				When using the msvc compiler, \#pragma comment( lib, "xxx" ) commands will automatically link to these libs.\n
 * Otherwise you have to tell the linker which libs it has to link to.
 *
 * @section gaixw_intro Introduction
 *
 * To use this api you need to specifiy some macro which define the window initialization process.
 *
 * Do this:
 * @code{.cpp}#define GAIXW_IMPLEMENTATION@endcode
 * before you include this file in *one* C or C++ file to create the implementation.
 * @subsection gaixw_examples Examples
 * @subsubsection gaixw_gdi_win32_example GDI Example
 * @include xwindow\gdi_win32\main.cpp
 * @subsubsection gaixw_opengl_win32_example OpenGL Example
 * @include xwindow\opengl_win32\main.cpp
 * If you want to get a modern opengl context you have to specify these macros as well
 * @code{.cpp}
 * #define GAIXW_OPENGL
 * #define GAIXW_OPENGL_MAJOR 3
 * #define GAIXW_OPENGL_MINOR 2
 * #define GAIXW_IMPLEMENTATION
 * #include <gai_xwindow.h>
 * @endcode
 * @author Andreas Gaida
 * @date   25.04.2017
 * @see    https://github.com/LostinAllThatCode/libgai
 * @example xwindow\gdi_win32\main.cpp __Example: win32 gai_xwindow.h (gdi)__
 * @example xwindow\opengl_win32\main.cpp __Example: win32 gai_xwindow.h (opengl)__
 */


#ifndef _GAI_INCLUDE_XWINDOW_H

#ifndef GAIXW_STATIC
	/**
	* @brief      Define **GAIXW_STATIC** if you want to make functions **static** instead of **extern**
	*/
	#define GAIXW_API extern
#else
	#define GAIXW_API static
#endif

#ifndef GAIXW_ASSERT
	#include <assert.h>
	/**
	* @brief      Define **GAIXW_ASSERT** before including this file to **disable** assertions
	* @note 	   Uses assert() from the c-standard library by default <assert.h>
	* @param      cond  Assertion condition
	*/
	#define GAIXW_ASSERT(cond) assert(cond)
#endif

#ifdef GAIXW_DEBUG
	#include <stdio.h>
	#ifndef GAIXW_PRINT
		#define GAIXW_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
	#endif
#else
	/**
	* @brief      Define this macro before including this file if you want to redirect the output
	*
	* Default output is set to console. (printf)
	*
	* @b Example (redirect output to OutputDebugStringA on windows)
	* @code{.cpp}
	*     static char GlobalDebugBuffer[4096];
	*     #define GAIXW_PRINT(fmt, ...) snprintf(GlobalDebugBuffer, 4095, fmt, __VA_ARGS__); OutputDebugStringA(GlobalDebugBuffer)
	*     #define GAIXW_IMPLEMENTATION
	*     #include <gai_xwindow.h>
	* @endcode
	* @param      fmt   The format
	* @param      ...   Dynamic parameters (\__VA_ARGS__)
	*/
	#define GAIXW_PRINT(fmt, ...)
#endif

#include <string.h>

#if _WIN32
	#ifndef GAIXW_DONT_INCL_WIN
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
#endif
#if __linux__
	#include <X11/Xlib.h>
#endif
#if __APPLE__
#endif

/**
 * Default classname
 */
#define GAIXW_CLASSNAME "libgai_xwindow_framework"

/**
 * @brief      Renderer backend types
 *
 * To specifiy which backend you want to use, you have to request them like shown below.
 *
 * Read the @ref gaixw_examples section for more information.
 */
enum gaixw_renderer_enum
{
	/**
	* Unknown renderer type
	*/
	gaixwRendererUnknown 		= 0,
	/**
	* Request a X11 renderer backend
	* @note Default on linux platform
	*/
	gaixwRendererX11 			= 1,
	/**
	* Request a GDI renderer backend
	* @note Default on windows platform
	*/
	gaixwRendererGDI 			= 2,
	/**
	* Request a @b DirectX 10 renderer backend
	* @attention ** NOT IMPLEMENTED YET **
	*/
	gaixwRendererDX10 			= 3,
	/**
	* Request a @b DirectX 11 renderer backend
	* @attention ** NOT IMPLEMENTED YET **
	*/
	gaixwRendererDX11 			= 4,
	/**
	* Request a @b DirectX 12 renderer backend
	* @attention ** NOT IMPLEMENTED YET **
	*/
	gaixwRendererDX12 			= 5,
	/**
	 * Request an @b OpenGL renderer backend
	 *
	 * @code{.cpp}
	 * #define GAIXW_OPENGL
	 * #define GAIXW_OPENGL_MAJOR 4	// (optional)
	 * #define GAIXW_OPENGL_MINOR 3	// (optional)
	 * #define GAIXW_IMPLEMENTATION
	 * #include <gai_xwindow.h>
	 * @endcode
	 *
	 * @note If you specifiy @b GAIXW_OPENGL_MAJOR and @b GAIXW_OPENGL_MINOR you can request a specific opengl context version!
	 */
	gaixwRendererOpenGL 		= 6,
	/**
	 * Request a @b vulcan renderer backend
	 * @attention ** NOT IMPLEMENTED YET **
	 */
	gaixwRendererVulcan			= 7,
	gaixwRendererMax
};

/**
 * @brief      Renderer flags
 */
enum gaixw_renderer_flags
{
	gaixwFlagsNone          = 0x00,
	gaixwFlagsMinimized		= 0x01,			/**< Window is minimized */
	gaixwFlagsMaximized		= 0x02,			/**< Window is maximized */
	gaixwFlagsFullscreen    = 0x10,			/**< Window is in fullscreen mode */
	gaixwFlagsVSYNC			= 0x20,			/**< Window has vertical sync enabled */
};

struct gaixw_frametime
{
	float seconds;							/**< Time since last frame in seconds. */
	float millis;							/**< Time since last frame in milliseconds. */
	float micros;							/**< Time since last frame in microseconds. */
};

struct gaixw_info
{
	const char *title; 						/**< A pointer to the window title string */
	int width;								/**< Current width of the window */
	int height;								/**< Current height of the window */
	int x;									/**< Current x postion of the window */
	int y;									/**< Current y postion of the window */
	int fps;								/**< Current frames per seconds. @note Does not work for the default window */
};

struct gaixw_input
{
	int x;									/**< Current **x** position of the mouse pointer */
	int y;									/**< Current **y** position of the mouse pointer */
	int dx;									/**< Current **delta** **x** position of the mouse pointer (since last frame) */
	int dy;									/**< Current **delta** **y** position of the mouse pointer (since last frame) */
	int wheel;								/**< Current **wheel** position of the mouse pointer */
	int dwheel;								/**< Current **delta** **wheel** position of the mouse pointer (since last frame) */
	int mod_alt;							/**< Current state of the **ALT** modifier key */
	int mod_ctrl;							/**< Current state of the **CTRL** modifier key */
	int mod_shift;							/**< Current state of the **SHIFT** modifier key */
	unsigned char keys[256];				/**< Current state of all keys */
	unsigned char keys_history[256];		/**< Last state of all keys */
	unsigned char mouse[3];					/**< Current state of all mouse keys */
	unsigned char mouse_history[3];			/**< Last state of all mouse keys */
};

union gaixw_graphics
{
	struct OPENGL
	{
		char 	*vendor;
		char 	*version;
		char 	*renderer;
		char 	*shading_language_version;
	} opengl; /**< OpenGL renderer informations like vendor, version, renderer, shading language version */
	struct DIRECTX 	{ int version; 		} directx;
	struct X11 		{ void 	*__ignored; } x11;
	struct GDI 		{ void 	*__ignored; } gdi;
	struct VULCAN 	{ void 	*__ignored; } vulcan;
};

union gaixw_platform
{
	#if _WIN32
	struct WINDOWS
	{
		HWND 				hwnd;
		HDC 				hdc;
		HINSTANCE 			instance;
		WINDOWPLACEMENT     position;
	} win32;
	#endif
	#if __linux__
	struct LINUX
	{
		Display* 			display;
		Visual* 			visual;
		Window* 			window;
	} linux;
	#endif
};

struct gaixw_renderer
{
	int 				attributes;		/**< Current state of the window. See @ref gaixw_renderer_flags */
	gaixw_renderer_enum type;			/**< Type of the renderer. See @ref gaixw_renderer_enum */
	gaixw_graphics 		graphics;		/**< Renderer interface union for all supported renderer backends. See @ref gaixw_renderer_enum */
};

struct gaixw_context
{
	gaixw_frametime dt; 					/**< Time since last frame in seconds, milliseconds and microseconds */
	gaixw_info		info;					/**< Window information like title, width, height, x, y ... */
	gaixw_input		input;					/**< Input states for mouse and keyboard */
	gaixw_platform  platform;				/**< Platform layer union for all supported platforms */
	gaixw_renderer  renderer;				/**< Renderer struct with the current render interface */
	int is_running;							/**< Running state of the window. If the window gets closed this value will be 0. */
	int is_visible;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Initializes a window for the current platform
 *
 * @param      window     A pointer to a gaixw_context structure
 * @param      title      (optional) Title
 * @param      width      (optional) Width
 * @param      height     (optional) Height
 * @param      x          (optional) X position
 * @param      y          (optional) Y position
 * @param      classname  (optional) Unique classname for this window instance
 * @param      visible    (optional) Specifies if the window is visible after creation or not
 *
 * @return
 * Returns 	    | Windows									| Linux
 * :----		|:------------------------------------------|:----
 * 1 			| Success 									| Success
 * 0 			| ConvertThreadToFiber failed 				| Not implemented yet!
 * -1 			| RegisterClassExA failed 					| Not implemented yet!
 * -2 			| CreateWindowA failed 						| Not implemented yet!
 * -3 			| (OpenGL) ChoosePixelFormat failed 		| Not implemented yet!
 * -4 			| (OpenGL) SetPixelFormat failed 			| Not implemented yet!
 * -5 			| (OpenGL) wglCreateContext failed 			| Not implemented yet!
 * -6 			| (OpenGL) wglMakeCurrent failed 			| Not implemented yet!
 *
 *
 * __Snippet (default gdi):__
 * @snippet    "..\tests\xwindow\gdi_win32\main.cpp" create_and_init_gaixw_context
 */
GAIXW_API int  			gaixw_Init        					(gaixw_context *window, const char *title = "default window title", int width = -1, int height = -1, int x = -1, int y = -1, const char *classname = GAIXW_CLASSNAME, unsigned int visible = 1);
/**
 * @brief      Deinitializes the window
 *
 * @param      window  A pointer to a gaixw_context structure
 */
GAIXW_API void          gaixw_Deinit						(gaixw_context *window);
/**
 * @brief      Makes the window visible to the user
 *
 * @param      window  A pointer to a gaixw_context structure
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Success
 * 0 			| Failure
 */
GAIXW_API int 			gaixw_Show							(gaixw_context *window);
/**
 * @brief      Hides the window from the user
 *
 * @param      window  A pointer to a gaixw_context structure
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Success
 * 0 			| Failure
 */
GAIXW_API int 			gaixw_Hide							(gaixw_context *window);
/**
 * @brief      Sets the window's title
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param      title   New title of the window
 */
GAIXW_API void 			gaixw_SetTitle						(gaixw_context *window, const char *title);
/**
 * @brief      Updates all internal variables and states.
 * @note       This has to be called every frame.
 *
 * @param      window  A pointer to a gaixw_context structure
 *
 * @return     Time passed since last frame in seconds
 */
GAIXW_API float		 	gaixw_Update						(gaixw_context *window);
/**
 * @brief      Swaps the window's backbuffers
 *
 * @param      window  A pointer to a gaixw_context structure
 */
GAIXW_API void 			gaixw_SwapBuffers					(gaixw_context *window);
/**
 * @brief      Current attribute state of the window
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param  	   attrib  Attribute flags as specified in @ref gaixw_renderer_flags\n
 * @ref gaixwFlagsMinimized\n
 * @ref gaixwFlagsMaximized\n
 * @ref gaixwFlagsFullscreen\n
 * @ref gaixwFlagsVSYNC\n
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Set
 * 0 			| Not set
 */
GAIXW_API unsigned int  gaixw_GetAttribute					(gaixw_context *window, gaixw_renderer_flags attrib);
/**
 * @brief      Sets vertical sync state
 * @note       This is a call to the graphics driver. Since the drivers implementations
 *             are different, this can have different results on different graphic cards.
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param  	   state   1 to enable, 0 to disable
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Success
 * 0 			| Failure
 */
GAIXW_API int  			gaixw_SetVerticalSync				(gaixw_context *window, unsigned int state);
/**
 * @brief      Gets the vertical sync state
 * @note       This is a call to the graphics driver. Since the drivers implementations
 *             are different, this can have different results on different graphic cards.
 *
 * @param      window  A pointer to a gaixw_context structure
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| VSYNC enabled
 * 0 			| VSYNC disabled
 */
GAIXW_API int           gaixw_IsVerticalSyncEnabled			(gaixw_context *window);
/**
 * @brief      Toggles between fullscreen mode and normal mode
 *
 * @param      window  A pointer to a gaixw_context structure
 */
GAIXW_API void  		gaixw_ToggleFullscreen  			(gaixw_context *window);
/**
 * @brief      Current state of fullscreen
 *
 * @param      window  A pointer to a gaixw_context structure
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Fullscreen enabled
 * 0 			| Fullscreen disabled
 */
GAIXW_API int           gaixw_IsFullscreen					(gaixw_context *window);
/**
 * @brief      Current state of the specified mouse button
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param      key
 * Key 	| Description
 * :----		|:-----------
 * 0 			| Left
 * 1 			| Middle
 * 2 			| Right
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Pressed
 * 0 			| Not pressed
 */
GAIXW_API unsigned char gaixw_MouseDown 					(gaixw_context *window, int key);
/**
 * @brief      Current pressed state of the specified mouse button
 * @note       This only returns 1 if the key was **released** before and is now **pressed** (Only occurs once)
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param[in]  key
 * Key 	| Description
 * :----		|:-----------
 * 0 			| Left
 * 1 			| Middle
 * 2 			| Right
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Was pressed
 * 0 			| Not pressed
 */
GAIXW_API unsigned char gaixw_MousePressed 					(gaixw_context *window, int key);
/**
 * @brief      Current released state of the specified mouse button
 * @note       This only returns 1 if the key was **pressed** before and is now **released** (Only occurs once)
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param[in]  key
 * Key 	| Description
 * :----		|:-----------
 * 0 			| Left
 * 1 			| Middle
 * 2 			| Right
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Was released
 * 0 			| Not released
 */
GAIXW_API unsigned char gaixw_MouseReleased					(gaixw_context *window, int key);
/**
 * @brief      Current state of the specified keyboard key
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param      key     Keycode between 0-255
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Pressed
 * 0 			| Not pressed
 */
GAIXW_API unsigned char gaixw_KeyDown 						(gaixw_context *window, int key);
/**
 * @brief      Current pressed state of the specified keyboard key
 * @note       This only returns 1 if the key was **released** before and is now **pressed** (Only occurs once)
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param[in]  key     Keycode between 0-255
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Was pressed
 * 0 			| Not pressed
 */
GAIXW_API unsigned char gaixw_KeyPressed 					(gaixw_context *window, int key);
/**
 * @brief      Current released state of the specified keyboard key
 * @note       This only returns 1 if the key was **pressed** before and is now **released** (Only occurs once)
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param[in]  key     Keycode between 0-255
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Was released
 * 0 			| Not released
 */
GAIXW_API unsigned char gaixw_KeyReleased					(gaixw_context *window, int key);

/**
 * @brief      Gets the procedure's memory address for the specified function name
 *
 * @param      window  A pointer to a gaixw_context structure
 * @param      name    Name of the function
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 0 			| Null ( function with that name not found )
 * Pointer		| A pointer to functions address
 */
GAIXW_API void* 		gaixw_GetProc						(gaixw_context *window, const char *name);

#ifdef __cplusplus
}
#endif

#ifdef GAIXW_IMPLEMENTATION

#ifdef _WIN32

#ifdef GAIXW_OPENGL
#pragma comment( lib, "opengl32.lib" )

#include <gl\gl.h>
typedef char GLchar;
typedef size_t GLsizeiptr;
typedef size_t GLintptr;
typedef void (*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#define GAIXW_FUNCWRAPPER \
	GAIXW_FUNC_DEF(, attach_shader_fn,  				void, 	 	AttachShader, GLuint, GLuint) \
	GAIXW_FUNC_DEF(, compile_shader_fn, 				void, 	 	CompileShader, GLuint) \
	GAIXW_FUNC_DEF(, create_program_fn, 				GLuint,  	CreateProgram, void) \
	GAIXW_FUNC_DEF(, create_shader_fn,  				GLuint,  	CreateShader, GLenum) \
	GAIXW_FUNC_DEF(, delete_program_fn, 				void,    	DeleteProgram, GLuint) \
	GAIXW_FUNC_DEF(, delete_shader_fn,  				void,    	DeleteShader, GLuint) \
	GAIXW_FUNC_DEF(, detach_shader_fn,  			    void,    	DetachShader, GLuint, GLuint) \
	GAIXW_FUNC_DEF(, disable_vertex_attrib_array_fn,  	void,    	DisableVertexAttribArray, GLuint) \
	GAIXW_FUNC_DEF(, enable_vertex_attrib_array_fn,   	void,    	EnableVertexAttribArray, GLuint) \
	GAIXW_FUNC_DEF(, get_program_iv_fn,			    	void,    	GetProgramiv, GLuint, GLenum, GLint*) \
	GAIXW_FUNC_DEF(, get_program_info_log_fn,			void,    	GetProgramInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	GAIXW_FUNC_DEF(, get_shader_info_log_fn,			void,    	GetShaderInfoLog, GLuint, GLsizei, GLsizei*, GLchar*) \
	GAIXW_FUNC_DEF(, get_shader_source_fn, 				void,    	GetShaderSource, GLuint, GLsizei, GLsizei*, GLchar *) \
	GAIXW_FUNC_DEF(, get_uniform_location_fn,         	GLint,   	GetUniformLocation, GLuint, const GLchar*) \
	GAIXW_FUNC_DEF(, link_program_fn,					void,    	LinkProgram, GLuint) \
	GAIXW_FUNC_DEF(, shader_source_fn,                	void,    	ShaderSource, GLuint, GLsizei, GLchar**, GLint*) \
	GAIXW_FUNC_DEF(, use_program, 						void,    	UseProgram, GLuint) \
	GAIXW_FUNC_DEF(, uniform_1f_fn,						void,    	Uniform1f, GLint, GLfloat) \
	GAIXW_FUNC_DEF(, uniform_2f_fn,						void,    	Uniform2f, GLint, GLfloat, GLfloat) \
	GAIXW_FUNC_DEF(, uniform_3f_fn,						void,    	Uniform3f, GLint, GLfloat, GLfloat, GLfloat) \
	GAIXW_FUNC_DEF(, uniform_4f_fn,						void,    	Uniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat) \
	GAIXW_FUNC_DEF(, uniform_1i_fn,						void,    	Uniform1i, GLint, GLint) \
	GAIXW_FUNC_DEF(, uniform_2i_fn,						void,    	Uniform2i, GLint, GLint, GLint) \
	GAIXW_FUNC_DEF(, uniform_3i_fn, 					void,    	Uniform3i, GLint, GLint, GLint, GLint) \
	GAIXW_FUNC_DEF(, uniform_4i_fn, 					void,    	Uniform4i, GLint, GLint, GLint, GLint, GLint) \
	GAIXW_FUNC_DEF(, uniform_1fv_fn,					void,    	Uniform1fv, GLint, GLsizei, const GLfloat*) \
	GAIXW_FUNC_DEF(, uniform_2fv_fn,					void,    	Uniform2fv, GLint, GLsizei, const GLfloat*) \
	GAIXW_FUNC_DEF(, uniform_3fv_fn,					void,    	Uniform3fv, GLint, GLsizei, const GLfloat*) \
	GAIXW_FUNC_DEF(, uniform_4fv_fn,					void,    	Uniform4fv, GLint, GLsizei, const GLfloat*) \
	GAIXW_FUNC_DEF(, uniform_1iv_fn,					void,    	Uniform1iv, GLint, GLsizei, const GLint*) \
	GAIXW_FUNC_DEF(, uniform_2iv_fn,					void,    	Uniform2iv, GLint, GLsizei, const GLint*) \
	GAIXW_FUNC_DEF(, uniform_3iv_fn,					void,    	Uniform3iv, GLint, GLsizei, const GLint*) \
	GAIXW_FUNC_DEF(, uniform_4iv_fn,					void,    	Uniform4iv, GLint, GLsizei, const GLint*) \
	GAIXW_FUNC_DEF(, uniform_matrix_2fv_fn, 			void,    	UniformMatrix2fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAIXW_FUNC_DEF(, uniform_matrix_3fv_fn,				void,    	UniformMatrix3fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAIXW_FUNC_DEF(, uniform_matrix_4fv_fn,				void,    	UniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat *) \
	GAIXW_FUNC_DEF(, validate_program_fn, 				void,	 	ValidateProgram ,GLuint) \
	GAIXW_FUNC_DEF(, vertex_attrib_pointer_fn,   		void,    	VertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void *) \
	GAIXW_FUNC_DEF(, debug_message_callback_fn, 		void,    	DebugMessageCallback, DEBUGPROC, void *) \
	GAIXW_FUNC_DEF(, get_string_i_fn,					GLubyte*,	GetStringi, GLenum, GLuint ) \
	GAIXW_FUNC_DEF(, bind_buffer_fn,					void,	 	BindBuffer, GLenum, GLuint buffer) \
	GAIXW_FUNC_DEF(, delete_buffers_fn,					void, 	 	DeleteBuffers, GLsizei, const GLuint*) \
	GAIXW_FUNC_DEF(, gen_buffers_fn, 					void,    	GenBuffers, GLsizei, GLuint*) \
	GAIXW_FUNC_DEF(, buffer_data_fn, 					void,	 	BufferData, GLenum, GLsizeiptr, const void*, GLenum) \
	GAIXW_FUNC_DEF(, buffer_sub_data_fn,				void, 	 	BufferSubData, GLenum, GLintptr, GLsizeiptr, const void *) \
	GAIXW_FUNC_DEF(, map_buffer_fn, 					GLvoid*, 	MapBuffer, GLenum, GLenum) \
	GAIXW_FUNC_DEF(, unmap_buffer_fn, 					void, 	 	UnmapBuffer, GLenum) \
	GAIXW_FUNC_DEF(, bind_vertex_array_fn,				void, 		BindVertexArray, GLuint) \
	GAIXW_FUNC_DEF(, delete_vertex_arrays_fn,			void, 		DeleteVertexArrays, GLsizei, const GLuint*) \
	GAIXW_FUNC_DEF(, gen_vertex_arrays_fn,				void, 		GenVertexArrays, GLsizei, GLuint*) \
	GAIXW_FUNC_DEF(, map_buffer_range_fn,				GLvoid*,	MapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) \
	GAIXW_FUNC_DEF(, buffer_storage_fn,					void,  		BufferStorage, GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) \
	GAIXW_FUNC_DEF(, blend_equation_fn,					void, 		BlendEquation, GLenum) \
	GAIXW_FUNC_DEF(, blend_func_seperate_fn,			void,		BlendFuncSeparate, GLenum, GLenum, GLenum, GLenum)

#define GAIXW_FUNC_DEF(ext, def, a, b, ...) typedef a (gl_##def) (__VA_ARGS__);
GAIXW_FUNCWRAPPER
#undef GAIXW_FUNC_DEF

#define GAIXW_FUNC_DEF(ext, def, a, b, ...) gl_##def *gl##b;;
GAIXW_FUNCWRAPPER
#undef GAIXW_FUNC_DEF

#define GAIXW_FUNC_DEF(ext, def, a, b, ...) gl##b = (gl_##def *) wglGetProcAddress("gl"#b#ext); GAIXW_ASSERT( gl##b != 0 );
GAIXW_API void
gaixw_GLInitFunctions()
{
	GAIXW_FUNCWRAPPER
}
#undef GAIXW_FUNC_DEF
#undef GAIXW_FNWRAPPER

GAIXW_API int
gaixw_WGLIsSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = 0;
	void* wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtString)
	{
		supported = ((char*(__stdcall*)(HDC)) wglGetExtString)(wglGetCurrentDC());
	}

	if (supported)
	{
		for (const char* p = supported; ; p++)
		{
			p = strstr(p, extension);
			if (!p) return false;
			if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' ')) return true;
		}
	}
	return false;
} // This is just copy & paste from nehe prod. IIRC

void
gaixw_GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
	GAIXW_PRINT("Source:   0x%04x\nType:     0x%04x\nMessage:  %s", source, type, message);
	GAIXW_ASSERT(severity != 0x9146 /* HIGH */);
}

GAIXW_API int
gaixw_GLIsSupported(char *extension)
{
	if (glGetStringi)
	{
		GLint n, i;
		glGetIntegerv(0x821D /* GL_NUM_EXTENSIONS */, &n);
		for (i = 0; i < n; i++)
		{
			char *gl_extension = (char *)glGetStringi(GL_EXTENSIONS, i);
			if (strcmp(gl_extension, extension) == 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void
gaixw_GLInitMultisampling(gaixw_context *window)
{
	GLint samples;
	glGetIntegerv(0x9110 /* GL_MAX_INTEGER_SAMPLES */, &samples);
}

GAIXW_API int
gaixw_GLGetSwapInterval()
{
	if (gaixw_WGLIsSupported("WGL_EXT_swap_control"))
	{
		void *wglGetSwapInterval = wglGetProcAddress("wglGetSwapIntervalEXT");
		if (wglGetSwapInterval) return ((int(__stdcall *)(void))wglGetSwapInterval)();
	}
	return 0;
}

GAIXW_API void
gaixw_GLSetSwapInterval(unsigned int vsync)
{
	if (gaixw_WGLIsSupported("WGL_EXT_swap_control"))
	{
		void* wglSwapInterval = wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapInterval) ((GLboolean(__stdcall *)(int))wglSwapInterval)(vsync);
	}
}

#elif defined GAIXW_DIRECTX
#error gai_xwindow.h: Error. DirectX 10 is not implemented yet!
#elif defined GAIXW_VULCAN
#error gai_xwindow.h: Error. Vulcan is not implemented yet!
#endif

#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "winmm.lib" )

GAIXW_API void
gaixw_Deinit(gaixw_context *window)
{
	#ifdef GAIXW_OPENGL
	HGLRC ctx = wglGetCurrentContext();
	if (ctx)
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(ctx);
	}
	#endif

	PostQuitMessage(0);
	MSG msg;
	while ( PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) break;
	}
}

GAIXW_API unsigned int
gaixw_GetAttribute(gaixw_context *window, gaixw_renderer_flags attrib)
{
	return (window->renderer.attributes & attrib ? 1 : 0);
}

GAIXW_API int
gaixw_IsVerticalSyncEnabled(gaixw_context *window)
{
	return gaixw_GetAttribute(window, gaixwFlagsVSYNC);
}

GAIXW_API int
gaixw_IsFullscreen(gaixw_context *window)
{
	return gaixw_GetAttribute(window, gaixwFlagsFullscreen);
}

GAIXW_API unsigned char
gaixw_KeyDown(gaixw_context *window, int key)
{
	return window->input.keys[key];
}

GAIXW_API unsigned char
gaixw_KeyPressed(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.keys_history[key];
	window->input.keys_history[key] = gaixw_KeyDown(window, key);
	return (window->input.keys_history[key] && !previousState);
}

GAIXW_API unsigned char
gaixw_KeyReleased(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.keys_history[key];
	window->input.keys_history[key] = gaixw_KeyDown(window, key);
	return (!window->input.keys_history[key] && previousState);
}

GAIXW_API unsigned char
gaixw_MouseDown(gaixw_context *window, int key)
{
	return window->input.mouse[key];
}

GAIXW_API unsigned char
gaixw_MousePressed(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.mouse_history[key];
	window->input.mouse_history[key] = gaixw_MouseDown(window, key);
	return (window->input.mouse_history[key] && !previousState);
}

GAIXW_API unsigned char
gaixw_MouseReleased(gaixw_context *window, int key)
{
	unsigned int previousState = window->input.mouse_history[key];
	window->input.mouse_history[key] = gaixw_MouseDown(window, key);
	return (!window->input.mouse_history[key] && previousState);
}

LRESULT CALLBACK
gaixw_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gaixw_context *window = (gaixw_context *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (window)
	{
		switch (uMsg)
		{
			case WM_SIZE:
			{
				window->info.width  = LOWORD(lParam);
				window->info.height = HIWORD(lParam);
				switch (wParam)
				{
					case SIZE_RESTORED:
					{
						if (window->renderer.attributes & gaixwFlagsMinimized) window->renderer.attributes = window->renderer.attributes & ~gaixwFlagsMinimized;
						if (window->renderer.attributes & gaixwFlagsMaximized) window->renderer.attributes = window->renderer.attributes & ~gaixwFlagsMaximized;
					} break;
					case SIZE_MAXIMIZED:
					{
						if (window->renderer.attributes & gaixwFlagsMinimized) window->renderer.attributes = window->renderer.attributes & ~gaixwFlagsMinimized;
						window->renderer.attributes |= gaixwFlagsMaximized;
					} break;
					case SIZE_MINIMIZED:
					{
						if (window->renderer.attributes & gaixwFlagsMaximized) window->renderer.attributes = window->renderer.attributes & ~gaixwFlagsMaximized;
						window->renderer.attributes |= gaixwFlagsMinimized;
					} break;
				}
				return 0;
			}
			case WM_MOVE: 			{ window->info.x = (int)(short) LOWORD(lParam); window->info.y = (int)(short) HIWORD(lParam); return 0; }
			case WM_KEYDOWN: 		{ window->input.keys_history[wParam] = window->input.keys[wParam]; window->input.keys[wParam] = 1; return 0; }
			case WM_KEYUP:			{ window->input.keys_history[wParam] = window->input.keys[wParam]; window->input.keys[wParam] = 0; return 0; }
			case WM_MOUSEMOVE:		{ POINTS p = MAKEPOINTS(lParam); window->input.x  = p.x; window->input.y  = p.y; return 0; }
			case WM_LBUTTONDOWN:	{ window->input.mouse_history[0] = window->input.mouse[0]; window->input.mouse[0] = 1; return 0; }
			case WM_LBUTTONUP:		{ window->input.mouse_history[0] = window->input.mouse[0]; window->input.mouse[0] = 0; return 0; }
			case WM_MBUTTONDOWN:	{ window->input.mouse_history[1] = window->input.mouse[1]; window->input.mouse[1] = 1; return 0; }
			case WM_MBUTTONUP:		{ window->input.mouse_history[1] = window->input.mouse[1]; window->input.mouse[1] = 0; return 0; }
			case WM_RBUTTONDOWN:	{ window->input.mouse_history[2] = window->input.mouse[2]; window->input.mouse[2] = 1; return 0; }
			case WM_RBUTTONUP:		{ window->input.mouse_history[2] = window->input.mouse[2]; window->input.mouse[2] = 0; return 0; }
			case WM_MOUSEWHEEL:		{ window->input.dwheel = GET_WHEEL_DELTA_WPARAM(wParam); window->input.wheel  += window->input.dwheel; return 0; }
			case WM_DESTROY:
			{
				#if 0
				_gaiXWindowDeInit(window);
				#ifdef GAIXW_OPENGL
				if (window->renderer.info.opengl.is_modern)
				{
					HGLRC ctx = wglGetCurrentContext();
					if (ctx)
					{
						wglMakeCurrent(0, 0);
						wglDeleteContext(ctx);
					}
				}
				#endif
				PostQuitMessage(0);
				#endif
				window->is_running = false;
				return 0;
			} break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GAIXW_API void
gaixw_ToggleFullscreen(gaixw_context *window)
{
	HWND hWnd = window->platform.win32.hwnd;
	DWORD Style = GetWindowLong(hWnd, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
		if (GetWindowPlacement(hWnd, &window->platform.win32.position) &&
		        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLong(hWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
			             MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
			             MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			             MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		window->renderer.attributes |= gaixwFlagsFullscreen;
	}
	else
	{
		SetWindowLong(hWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &window->platform.win32.position);
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		window->renderer.attributes &= ~gaixwFlagsFullscreen;
	}
}

GAIXW_API void
gaixw_SetTitle(gaixw_context *window, const char *title)
{
	wchar_t buffer[4096];
	MultiByteToWideChar(CP_UTF8, 0, title, -1, buffer, 4095);
	SetWindowTextW(window->platform.win32.hwnd, buffer);
	window->info.title = title;
}

GAIXW_API void
gaixw_SwapBuffers(gaixw_context *window)
{
	#ifdef GAIXW_OPENGL
	SwapBuffers(window->platform.win32.hdc);
	#else
	#endif
}

GAIXW_API int
gaixw_SetVerticalSync(gaixw_context *window, unsigned int state)
{
	unsigned int result = 0;
	#ifdef GAIXW_OPENGL
	gaixw_GLSetSwapInterval(state);
	result = gaixw_GLGetSwapInterval();
	window->renderer.attributes = (result == 1 ? window->renderer.attributes | gaixwFlagsVSYNC : window->renderer.attributes & ~gaixwFlagsVSYNC);
	#endif
	return result;
}

inline GAIXW_API float
gaixw_Update(gaixw_context *window)
{
	MSG msg;
	#ifdef GAIXW_OPENGL
	if (gaixw_KeyReleased(window, VK_F1)) gaixw_ToggleFullscreen(window);
	if (gaixw_KeyReleased(window, VK_F2)) gaixw_SetVerticalSync(window, !gaixw_GetAttribute(window, gaixwFlagsVSYNC));

	static LARGE_INTEGER _frequency;
	static LARGE_INTEGER _ltframetime;
	static int fps;
	static int frames;
	static long double frametime;

	static unsigned int initialized;
	if (!initialized)
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_ltframetime);
		initialized = true;
	}

	LARGE_INTEGER now, elapsed;
	QueryPerformanceCounter(&now);
	elapsed.QuadPart = now.QuadPart - _ltframetime.QuadPart;
	elapsed.QuadPart *= 1000000;
	elapsed.QuadPart /= _frequency.QuadPart;

	window->dt.micros  = (float)elapsed.QuadPart;
	window->dt.millis  = (float)elapsed.QuadPart / 1000.f;
	window->dt.seconds = (float)elapsed.QuadPart / 1000000.f;
	_ltframetime = now;

	int old_x = window->input.x;
	int old_y = window->input.y;
	window->input.dwheel = 0;

	while ( PeekMessageA(&msg, 0, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			window->is_running = false;
			break;
		}
	}

	window->input.dx = window->input.x - old_x;
	window->input.dy = window->input.y - old_y;

	if (frametime >= 1.0f)
	{
		window->info.fps = frames;
		frames = 0;
		frametime = 0;
	}
	else
	{
		frametime += window->dt.seconds;
		frames++;
	}
	#else
	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (window->is_running == false) break;
	}
	#endif
	return (float) window->dt.seconds;
}

GAIXW_API int
gaixw_Show(gaixw_context *window)
{
	int result = (ShowWindow(window->platform.win32.hwnd, SW_SHOW) == 0);
	if (result) window->is_visible = 1;
	return result;
}

GAIXW_API int
gaixw_Hide(gaixw_context *window)
{
	int result = (ShowWindow(window->platform.win32.hwnd, SW_HIDE) == 0);
	if (result) window->is_visible = 0;
	return result;
}

GAIXW_API int
gaixw_Init(gaixw_context *window, const char *title, int width, int height, int x, int y, const char *classname, unsigned int visible)
{
	GAIXW_ASSERT(window);
	if (!ConvertThreadToFiber(0) ) return 0;

	gaixw_context init = { {0}, {title, width, height, x, y}, {0}, {0, 0, GetModuleHandle(0)}, { 0, gaixwRendererGDI, 0 } };
	*window = init;

	WNDCLASSEXA wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = sizeof(gaixw_context*);
	wnd_class.lpfnWndProc   = gaixw_WindowProc;
	wnd_class.hInstance     = window->platform.win32.instance;
	wnd_class.lpszClassName = classname;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassExA(&wnd_class))
	{
		GAIXW_ASSERT(!"Can't register window class.");
		return -1;
	}

	DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	HWND hwnd 	= CreateWindowA(classname, title, style,
	                            (x == -1 ? CW_USEDEFAULT : x),
	                            (y == -1 ? CW_USEDEFAULT : y),
	                            (width == -1 ? CW_USEDEFAULT : width),
	                            (height == -1 ? CW_USEDEFAULT : height),
	                            0, 0, window->platform.win32.instance, window);
	if (!hwnd)
	{
		GAIXW_ASSERT(!"Can't create window.");
		return -2;
	}

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window);
	HDC hdc = GetDC(hwnd);
	window->platform.win32.hwnd = hwnd;
	window->platform.win32.hdc  = hdc;

	#ifdef GAIXW_OPENGL
	window->renderer.type = gaixwRendererOpenGL;
	//timeBeginPeriod(1);
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize                 = sizeof(pfd);
	pfd.nVersion              = 1;
	pfd.iPixelType            = PFD_TYPE_RGBA;
	pfd.dwFlags               = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cColorBits            = 32;
	pfd.cAlphaBits            = 8;
	pfd.cDepthBits            = 32;
	pfd.iLayerType            = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(hdc, &pfd);
	if (!pf)
	{
		GAIXW_ASSERT(!"No valid pxiel format found.");
		DestroyWindow(hwnd);
		return -3;
	}

	if (!SetPixelFormat(hdc, pf, &pfd))
	{
		GAIXW_ASSERT(!"Set pixel format failed.");
		return -4;
	}

	HGLRC old_glctx = wglCreateContext(hdc);
	if (!old_glctx)
	{
		GAIXW_ASSERT(!"Can't create opengl context.");
		DestroyWindow(hwnd);
		return -5;
	}

	if (!wglMakeCurrent(hdc, old_glctx))
	{
		GAIXW_ASSERT(!"Activate opengl context failed..");
		wglDeleteContext(old_glctx);
		DestroyWindow(hwnd);
		return -6;
	}

	window->renderer.graphics.opengl.vendor                    = (char *) glGetString(GL_VENDOR);
	window->renderer.graphics.opengl.version                   = (char *) glGetString(GL_VERSION);
	window->renderer.graphics.opengl.renderer                  = (char *) glGetString(GL_RENDERER);

	if (gaixw_WGLIsSupported("WGL_ARB_create_context"))
	{
		void *wglCreateContextAttribsARB = wglGetProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB)
		{
			int attribs[] =
			{
				#if GAIXW_OPENGL_MAJOR_VERSION
				0x2091 /* WGL_CONTEXT_MAJOR_VERSION_ARB */, GAIXW_OPENGL_MAJOR_VERSION,
				#endif
				#if GAIXW_OPENGL_MINOR_VERSION
				0x2092 /* WGL_CONTEXT_MINOR_VERSION_ARB */, GAIXW_OPENGL_MINOR_VERSION,
				#endif
				//0x9126 /* WGL_CONTEXT_PROFILE_MASK_ARB */, /0x00000001 /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB */,
				0x2094 /* WGL_CONTEXT_FLAGS_ARB */, 0
				#ifdef _DEBUG
				| 0x00000001
				#endif
				,
				0
			};

			HGLRC new_oglctx = ((HGLRC(__stdcall *)(HDC, HGLRC, const int *))wglCreateContextAttribsARB)(hdc, 0, attribs);
			if (new_oglctx)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(old_glctx);
				wglMakeCurrent(hdc, new_oglctx);

				window->renderer.graphics.opengl.vendor                   = (char *) glGetString(GL_VENDOR);
				window->renderer.graphics.opengl.version                  = (char *) glGetString(GL_VERSION);
				window->renderer.graphics.opengl.renderer                 = (char *) glGetString(GL_RENDERER);
				window->renderer.graphics.opengl.shading_language_version = (char *) glGetString(0x8B8C);  // 0x8B8C GL_SHADING_LANGUAGE_VERSION

				gaixw_SetVerticalSync(window, 1);

				gaixw_GLInitFunctions();

				gaixw_GLInitMultisampling(window);

				#ifdef _DEBUG
				glEnable(0x8242 /* GL_DEBUG_OUTPUT_SYNCHRONOUS */ ); // 0x92E0 /* GL_DEBUG_OUTPUT */
				glDebugMessageCallback( gaixw_GLDebugCallback, 0);
				#endif
			}
			else GAIXW_PRINT("Error: wglCreateContextAttribsARB failed to create modern context.");
		}
		else GAIXW_PRINT("Error: wglGetProcAddress failed for wglCreateContextAttribsARB.");
	}
	else GAIXW_PRINT("Warning : This graphics card does not support \"WGL_ARB_create_context\".");

	#else

	#endif

	window->is_running = true;
	if (visible) gaixw_Show(window); else gaixw_Hide(window);
	return 1;
}

#elif __linux__
#error gai_xwindow.h: Error. No linux implementation yet!

#else
#error gai_xwindow.h: Error. This platform is not supported!

#endif

#endif

#define _GAI_INCLUDE_XWINDOW_H
#endif