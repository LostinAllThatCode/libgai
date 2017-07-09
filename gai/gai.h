/**
 * @file  			gai.h
 *
 * @brief 			designed as a cross platform single include header file for the most necessary features (window, dynamic array, qsort, network)
 *
 * __Depends on the specific OS libraries and the standard c library__
 * @author     	    Andreas Gaida
 * @date 			23.05.2017
 */
#ifndef _INCLUDE_GAI_H

#if !defined(GAI_OS_RESOLUTION)
	#if defined(_WIN32) || defined(_WIN64)
		#ifndef GAI_OS_WINDOWS
			#define GAI_OS_WINDOWS 1
		#endif
	#elif defined(__APPLE__) && defined(__MACH__)
		#ifndef GAI_OS_MACOSX
			#define GAI_OS_MACOSX 1
		#endif
	#elif defined(__unix__)
		//TODO:(aga) Go more granular here? Check for __linux__ and __FreeBSD__ ?
		#ifndef GAI_OS_UNIX
			#define GAI_OS_UNIX 1
		#endif
	#else
		#error Unsupported operating system!
	#endif
	#define GAI_OS_RESOLUTION
#endif

#if defined(__clang__)
	#define GAI_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define GAI_COMPILER_GCC 1
#elif defined(_MSC_VER)
	#define GAI_COMPILER_MSVC 1
#else
	#error Unknown compiler
#endif

#if defined(GAI_OS_WINDOWS)
	#if !defined(GAI_NO_WINDOWS_H)
		/// @cond DOXYGEN_IGNORE
		#define NOMINMAX            1
		#define WIN32_LEAN_AND_MEAN 1
		#define WIN32_MEAN_AND_LEAN 1
		#define VC_EXTRALEAN        1
		/// @endcond
		#include <windows.h>
		#include <gl\gl.h>
		#include <d3d11.h>
		#undef NOMINMAX
		#undef WIN32_LEAN_AND_MEAN
		#undef WIN32_MEAN_AND_LEAN
		#undef VC_EXTRALEAN
	#endif
#elif defined(GAI_OS_UNIX)
	#include <X11/X.h>
	#include <X11/Xlib.h>
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif

#if !defined(GAI_TYPEDEFS)
	#include <stddef.h>
	#include <stdint.h>
	typedef uint8_t		u8;
	typedef int8_t   	i8;
	typedef uint16_t 	u16;
	typedef int16_t 	i16;
	typedef uint32_t 	u32;
	typedef int32_t 	i32;
	typedef float		r32;
	typedef uint64_t 	u64;
	typedef int64_t 	i64;
	typedef double		r64;
	typedef int32_t 	b32;
	#define true 		1
	#define false 		0

	#define GAI_TYPEDEFS 1
#endif

/// @cond DOXYGEN_IGNORE
#if !defined(GAI_EXTERN)
	#define GAI_EXTERN extern "C"
#endif
/// @endcond

#if !defined(GAI_STATIC)
	#if !defined(GAI_DEF)
		#define GAI_DEF extern 		/**< Define the macro __GAI_STATIC__ to force all function to be __static__ instead of __extern__ */
	#endif
#else
	#if !defined(GAI_DEF)
		#define GAI_DEF static
	#endif
#endif

/// @cond DOXYGEN_IGNORE
#if !defined(gai_memset) && !defined(gai_memcpy) && !defined(gai_memmove)
	#undef gai_memset
	#undef gai_memcpy
	#undef gai_memmove
	#include <stdlib.h>
	#include <memory.h>   //NOTE:(aga) Is this okay? Do i have to include <string.h> for this?
	#define gai_memset(dst, val, size) 		memset((dst), (val), (size))
	#define gai_memcpy(dst, src, size) 		memcpy((dst), (src), (size))
	#define gai_memmove(dst, src, size) 	memmove((dst), (src), (size))
#endif

#if !defined(gai_malloc) && !defined(gai_free)
	#undef gai_malloc
	#undef gai_free
	#define gai_malloc(size) 				malloc(size)
	#define gai_free(ptr)    				free(ptr)
#endif
/// @endcond

/// @cond DOXYGEN_IGNORE
#define gai_array_size(a)				(sizeof(a)/sizeof(a[0]))
#define gai_offset_of(t, e)				((size_t)&(((t *)0)->e))
#define gai_val_from_ptr(t, p) 			((t)(size_t)(p))
#define gai_ptr_from_val(t, v) 			(t *)((size_t)v)

#define gai_kilobytes(v)				(			  (v) * 1024LL)
#define gai_megabytes(v)				(gai_kilobytes(v) * 1024LL)
#define gai_gigabytes(v)				(gai_megabytes(v) * 1024LL)
#define gai_terabytes(v)				(gai_gigabytes(v) * 1024LL)

#define gai_min(a, b) 					(((a) < (b) ? (a) : (b)))
#define gai_max(a, b) 					(((a) < (b) ? (b) : (a)))

#define gai_swap(type, a, b) 			do { type _tmp = (a); (a) = (b); (b) = _tmp; } while(0)


/// @endcond

#if !defined(GAI_ASSERT)
	#include <assert.h>
	#define GAI_ASSERT(cond) assert(cond)
#endif

/// @cond DOXYGEN_IGNORE
#ifndef GAI_DEBUG_BREAK
	#if defined(_MSC_VER)
		#if _MSC_VER < 1300
			#define GAI_DEBUG_BREAK() __asm int 3
		#else
			#define GAI_DEBUG_BREAK() __debugbreak()
		#endif
	#else
		#define GAI_DEBUG_BREAK() __builtin_trap()
	#endif
#endif
/// @endcond

#define GAI_INL GAI_DEF inline


/**
 *		memory api
 *
 *		_______ _______ _______  _____   ______ __   __      _______  _____  _____
 *		|  |  | |______ |  |  | |     | |_____/   \_/        |_____| |_____]   |
 *		|  |  | |______ |  |  | |_____| |    \_    |         |     | |       __|__
 */


#define GAI_ALLOCATOR_ALLOCATE(name) void* name(size_t size)
typedef GAI_ALLOCATOR_ALLOCATE(gai_alloc_mem_proc);
#define GAI_ALLOCATOR_DEALLOCATE(name) void name(void *address)
typedef GAI_ALLOCATOR_DEALLOCATE(gai_dealloc_mem_proc);
struct gai_allocator
{
	gai_alloc_mem_proc 	  *alloc;
	gai_dealloc_mem_proc  *free;
};

GAI_INL
gai_allocator gai_allocator_init(gai_alloc_mem_proc *allocate_proc, gai_dealloc_mem_proc *deallocate_proc)
{
	GAI_ASSERT(allocate_proc || deallocate_proc);
	gai_allocator result = { allocate_proc, deallocate_proc };
	return result;
}

GAI_INL void gai_memswap(u8 *a, u8 *b, size_t size);



/**
 * 		growable array - use via macros
 *
 *		 ______  ______  _____  _  _  _ _______ ______         _______      _______  _____  _____
 *		|  ____ |_____/ |     | |  |  | |_____| |_____] |      |______      |_____| |_____]   |
 *		|_____| |    \_ |_____| |__|__| |     | |_____] |_____ |______      |     | |       __|__
 *
 * 		Use /MD instead of /MT if you are you using the MSVC compiler.
 */




#define gai_growable(type) type *

#define GAI_GROWABLE_GROW_STRATEGY(x) 		(((x)*2)+8)
#define GAI_GROWABLE_GET_HEADER(x) 			((gai_growable_header*)(x)-1)

struct gai_growable_header
{
	size_t 			count;
	size_t 			capacity;
	gai_allocator 	allocator;
};

#define gai_growable_count(x) 				(GAI_GROWABLE_GET_HEADER(x)->count)
#define gai_growable_capacity(x) 			(GAI_GROWABLE_GET_HEADER(x)->capacity)
#define gai_growable_clear(x) 				(GAI_GROWABLE_GET_HEADER(x)->count = 0)

#define gai_growable_init(x, alc) do { \
		void **__gai_growable_array 		= (void **)&(x); \
		gai_growable_header *gai_growable 	= (gai_growable_header *) alc.alloc( sizeof(gai_growable_header) + sizeof(*(x)) * ( GAI_GROWABLE_GROW_STRATEGY(0) )) ; \
		gai_growable->count 				= 0; \
		gai_growable->capacity 				= GAI_GROWABLE_GROW_STRATEGY(0); \
		gai_growable->allocator 			= alc; \
		*__gai_growable_array 				= (void *)(gai_growable+1); \
	} while (0)

#define gai_growable_free(x) do { \
		gai_growable_header *gai_growable = GAI_GROWABLE_GET_HEADER(x); \
		gai_growable->allocator.free(gai_growable); \
	} while (0)

#define gai_growable_append(x, item) do { \
		gai_growable_header *gai_growable = GAI_GROWABLE_GET_HEADER(x); \
		if ( (gai_growable->capacity < gai_growable->count+1)) \
		{ \
			void **__gai_growable_array = (void **) &(x); \
			*__gai_growable_array 		= gai_growable_grow(gai_growable, GAI_GROWABLE_GROW_STRATEGY(gai_growable->capacity), sizeof(*(x)) ); \
			gai_growable 				= GAI_GROWABLE_GET_HEADER(x); \
		} \
		(x)[gai_growable->count++] = (item); \
	} while (0)

#define gai_growable_append_array(x, items, item_count) do { \
		gai_growable_header *gai_growable = GAI_GROWABLE_GET_HEADER(x); \
		if (gai_growable->capacity < gai_growable->count+(item_count)) \
		{ \
			void **__gai_growable_array = (void **) &(x); \
			*__gai_growable_array 		= gai_growable_grow(gai_growable, gai_growable->count+(item_count), sizeof(*(x)) ); \
			gai_growable 				= GAI_GROWABLE_GET_HEADER(x); \
		} \
		gai_memcpy(x + gai_growable->count, (items), sizeof( (items)[0])*(item_count)); \
		gai_growable->count += (item_count); \
	} while (0)

GAI_INL void  *gai_growable_grow(void *array, size_t new_capacity, size_t element_size);

#if defined(GAI_NETWORK)

/**
 * 		network api ( tcp/udp sockets)
 *
 *		__   _ _______ _______ _  _  _  _____   ______ _     _      _______  _____  _____
 *		| \  | |______    |    |  |  | |     | |_____/ |____/       |_____| |_____]   |
 *		|  \_| |______    |    |__|__| |_____| |    \_ |    \_      |     | |       __|__
 *
 *
 * 		#define GAI_NETWORK before including this file to use it
 */

#if defined(GAI_OS_WINDOWS)
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif

struct gai_socket
{
	void *dest;
};

#if defined(__cplusplus)
extern "C" {
#endif

GAI_DEF b32 		gai_net_init();
GAI_DEF b32 		gai_net_deinit();

GAI_INL gai_socket 	gai_net_open_tcp(char *address, u32 port);
GAI_INL b32 		gai_net_close_tcp(gai_socket *socket);
GAI_INL i32 	   	gai_net_send(gai_socket *socket, u8* data, size_t data_size);
GAI_INL i32    		gai_net_receive(gai_socket *socket, u8* recv_buffer, size_t buffer_size);

#if defined(__cplusplus)
}
#endif

#endif




/**
 *		sorting api
 *
 *		 _____  _______  _____   ______ _______      _______  _____  _____
 *		|   __| |______ |     | |_____/    |         |_____| |_____]   |
 *		|____\| ______| |_____| |    \_    |         |     | |       __|__
 */

#define GAI_SORT_COMPARE_PROC(name) int name(void const *a, void const *b)
typedef GAI_SORT_COMPARE_PROC(gai_sort_compare_fn);

#if defined(__cplusplus)
extern "C" {
#endif

#define 		gai_sort_array(arr, elements, cmp_fn) gai_sort((arr), (elements), sizeof(*(arr)), cmp_fn)
#define 		gai_sort_array_i32(arr, elements) gai_sort((arr), (elements), sizeof(i32), __gai_i32_cmp)
#define 		gai_sort_array_i64(arr, elements) gai_sort((arr), (elements), sizeof(i64), __gai_i64_cmp)
#define 		gai_sort_array_r32(arr, elements) gai_sort((arr), (elements), sizeof(r32), __gai_r32_cmp)
#define 		gai_sort_array_r64(arr, elements) gai_sort((arr), (elements), sizeof(r64), __gai_r64_cmp)
GAI_INL void 	gai_sort(void *array, size_t elements, size_t element_size, gai_sort_compare_fn cmp);

#if defined(__cplusplus)
}
#endif

/**
 * 		time api
 *
 *		_______ _____ _______ _______      _______  _____  _____
 *		   |      |   |  |  | |______      |_____| |_____]   |
 *		   |    __|__ |  |  | |______      |     | |       __|__
 *
 *
 */

struct gai_clock
{
	r64 frequency;
	r64 last_clock;
};

#if defined(__cplusplus)
extern "C" {
#endif

GAI_INL gai_clock 	gai_clock_init();
GAI_INL r64 		gai_clock_get(gai_clock *clock);


#if defined(__cplusplus)
}
#endif



/**
* 		file management api
*
*		_______ _____        _______      _______  _____  _____
*		|______   |   |      |______      |_____| |_____]   |
*		|       __|__ |_____ |______      |     | |       __|__
*
*
*
* For all of these functions the user has to provide the buffer.
* No memory allocation is happening in here!
* @code
gai_file f = gai_file_open("mytest.txt", "rb");
{
	u8 *buffer = (u8*) malloc(f.size + 1);
	i32 read_bytes = gai_file_read_all(&f, buffer);
	buffer[read_bytes] = 0;
	free(buffer);
}
gai_file_close(&f);
* @endcode
*/

#include <stdio.h>

struct gai_file
{
	FILE   *fp;
	size_t size;
};

union gai_filetime
{
	i64 time;
	struct { i32 low, high; };
};

union gai_filesize
{
	i64 size;
	struct { i32 low, high; };
};

struct gai_fileinfo
{
	i32 attributes;
	gai_filetime creation_time;
	gai_filetime modification_time;
	gai_filetime access_time;
	gai_filesize size;
};

#if defined(__cplusplus)
extern "C" {
#endif

GAI_INL gai_file 		gai_file_open(const char *filename, const char *mode);
GAI_INL size_t 			gai_file_write(gai_file *file, void *data, size_t length);
GAI_INL size_t 			gai_file_read(gai_file *file, void *buffer, size_t bytes_to_read, i32 offset);
GAI_INL size_t 			gai_file_read_all(gai_file *file, void *buffer);
GAI_INL void 			gai_file_close(gai_file *file);

GAI_INL b32 			gai_file_exists(const char *filename);
GAI_INL gai_fileinfo  	gai_file_getinfo(const char *filename);

#if defined(__cplusplus)
}
#endif







/**
* 		WINDOW MANAGEMENT API
*
*		_  _  _ _____ __   _ ______   _____  _  _  _      _______  _____  _____
*		|  |  |   |   | \  | |     \ |     | |  |  |      |_____| |_____]   |
*		|__|__| __|__ |  \_| |_____/ |_____| |__|__|      |     | |       __|__
*
*/

enum gai_window_flags
{
	gaiWindowFlagsNone         	= 0x00,
	gaiWindowFlagsMinimized		= 0x01,			/**< Window is minimized */
	gaiWindowFlagsMaximized		= 0x02,			/**< Window is maximized */
	gaiWindowFlagsBorderless	= 0x04,			/**< Window is in borderless fullscreen mode */
	gaiWindowFlagsFullscreen 	= 0x10			/**< Window is in real fullscreen mode */
};

enum gai_window_driver_type
{
	gaiWindowDriverDefault, // This is the platforms default window framework
	gaiWindowDriverDX11,
	gaiWindowDriverOpenGL,
};

enum gai_mouse_button { gaiMouseLeft, gaiMouseMiddle, gaiMouseRight, gaiMouseExtra1, gaiMouseExtra2 };
enum gai_key_state_enum { gaiKeystateDown = 0x1, gaiKeystatePressed = 0x2, gaiKeystateReleased = 0x4 };
typedef u8 gai_key_state;

enum gai_callback_type_enum { gaiInvalidCallback = 0x0, gaiInputCallback = 0x1, };
typedef void gai_input_callback_proc(i32 key, u8 state, void *userdata);
struct gai_window_callback
{
	u16 type;
	union
	{
		void 						*callback;
		gai_input_callback_proc 	*input_callback;
	};
	void *userdata;
};

struct gai_window
{
	void	*handle;
	i32     x, y, w, h;
	u32 	flags;
	r64 	dt;
	r64 	time;
	u32    	was_closed, request_close;

	gai_window_driver_type type;
	union CTX
	{
		struct OPENGL
		{
			i32 major;
			i32 minor;
			i32 core;
			i32 debug;
			void *context;
		} opengl;
		#if defined(GAI_OS_WINDOWS)
		struct DIRECTX11
		{
			IDXGISwapChain 		*swapchain;
			ID3D11Device 		*dev;
			ID3D11DeviceContext *devcon;
		} dx11;
		#endif
	} ctx;

	struct INPUT
	{
		gai_key_state keyboard_keys[255];
		gai_key_state mouse_buttons[5];
		i32 mouse_x, mouse_y, mouse_dx, mouse_dy, mouse_wheel;
	} input;

	gai_clock clock;

	gai_growable(gai_window_callback) callbacks;

	#if defined(GAI_OS_WINDOWS)
	WINDOWPLACEMENT placement_win32;
	#elif defined(GAI_OS_UNIX)
	Window unix_wnd;
	#endif
};

#if defined(__cplusplus)
extern "C" {
#endif

GAI_DEF i32 	gai_window_init(gai_window *window, const char *title, i32 width, i32 height, u32 flags);
GAI_DEF i32 	gai_window_init_gl(gai_window *window, const char *title, i32 width, i32 height, u32 flags, i32 minor, i32 major, b32 core_profile, b32 debug);
GAI_DEF i32 	gai_window_init_dx(gai_window *window, const char *title, i32 width, i32 height, u32 flags);
GAI_DEF i32 	gai_window_destroy(gai_window *window);
GAI_DEF void 	gai_window_vsync(gai_window *window, b32 state);
GAI_INL b32 	gai_window_vsync_enabled(gai_window *window);
GAI_INL void	gai_window_handle_messages(gai_window *window, b32 block);
GAI_INL void	gai_window_display(gai_window *window);
GAI_DEF void 	gai_window_show(gai_window *window);
GAI_DEF void 	gai_window_hide(gai_window *window);
GAI_DEF b32		gai_window_set_foreground(gai_window *window);
GAI_INL b32		gai_window_is_foreground(gai_window *window);
GAI_INL void	gai_window_set_position(gai_window *window, i32 x, i32 y);
GAI_INL void    gai_window_borderless(gai_window *window, b32 state);

GAI_DEF void	gai_window_register_callback(gai_window *window, gai_callback_type_enum type, void *callback);
GAI_DEF void	gai_window_unregister_callback(gai_window *window, void *callback);

GAI_INL b32		gai_key_down(gai_window *window, i32 key);
GAI_INL b32		gai_key_released(gai_window *window, i32 key);
GAI_INL b32		gai_key_pressed(gai_window *window, i32 key);
GAI_INL b32		gai_mouse_down(gai_window *window, i32 btnid);
GAI_INL b32		gai_mouse_released(gai_window *window, i32 btnid);
GAI_INL b32		gai_mouse_pressed(gai_window *window, i32 btnid);


#if defined(__cplusplus)
}
#endif


#if defined(GAI_IMPLEMENTATION)

/**
*
*			_____
*			|_   _|
*			  | |
*			  | |
*			 _| |_
*			|_____|
*
*
*			 __  __
*			|  \/  |
*			| \  / |
*			| |\/| |
*			| |  | |
*			|_|  |_|
*
*
*			 _____
*			|  __ \
*			| |__) |
*			|  ___/
*			| |
*			|_|
*
*
*			 _
*			| |
*			| |
*			| |
*			| |____
*			|______|
*
*
*			 ______
*			|  ____|
*			| |__
*			|  __|
*			| |____
*			|______|
*
*
*			 __  __
*			|  \/  |
*			| \  / |
*			| |\/| |
*			| |  | |
*			|_|  |_|
*
*
*			 ______
*			|  ____|
*			| |__
*			|  __|
*			| |____
*			|______|
*
*
*			 _   _
*			| \ | |
*			|  \| |
*			| . ` |
*			| |\  |
*			|_| \_|
*
*
*			 _______
*			|__   __|
*			   | |
*			   | |
*			   | |
*			   |_|
*
*
*
*			    /\
*			   /  \
*			  / /\ \
*			 / ____ \
*			/_/    \_
*
*
*			 _______
*			|__   __|
*			   | |
*			   | |
*			   | |
*			   |_|
*
*
*			 _____
*			|_   _|
*			  | |
*			  | |
*			 _| |_
*			|_____|
*
*
*			  ____
*			 / __ \
*			| |  | |
*			| |  | |
*			| |__| |
*			 \____/
*
*
*			 _   _
*			| \ | |
*			|  \| |
*			| . ` |
*			| |\  |
*			|_| \_|
*
*/


/**
 *		MEMORY IMPLEMENTATION (PLATFORM INDEPENDENT)
 *
 *		 _______ _______ _______  _____   ______ __   __      _____ _______  _____
 *		 |  |  | |______ |  |  | |     | |_____/   \_/          |   |  |  | |_____] |
 *		 |  |  | |______ |  |  | |_____| |    \_    |         __|__ |  |  | |       |_____
 *
 */

GAI_INL void
gai_memswap(u8 *a, u8 *b, size_t size)
{
	if (a == b) return;

	if ( size == 4 )
	{
		gai_swap(u32, *(u32*)a, *(u32*)b);
	}
	else if ( size == 8 )
	{
		gai_swap(u64, *(u64*)a, *(u64*)b);
	}
	else if ( size < 8 )
	{
		//TODO:(aga) memswap for 1,2,3,5,6,7 byte sized things!
		GAI_ASSERT(!"TODO");
	}
	else
	{
		char buffer[256];
		i32  buf_size = sizeof(buffer);

		while (size > buf_size)
		{
			gai_memswap(a, b, buf_size);
			a += buf_size;
			b += buf_size;
			size -= buf_size;
		}

		gai_memcpy(buffer, a, size);
		gai_memcpy(a, b, size);
		gai_memcpy(b, buffer, size);
	}
}



/**
 *		GROWABLE IMPLEMENTATION (PLATFORM INDEPENDENT)
 *		 ______  ______  _____  _  _  _ _______ ______         _______      _____ _______  _____
 *		|  ____ |_____/ |     | |  |  | |_____| |_____] |      |______        |   |  |  | |_____] |
 *		|_____| |    \_ |_____| |__|__| |     | |_____] |_____ |______      __|__ |  |  | |       |_____
 */

GAI_INL void *
gai_growable_grow(void *array, size_t new_capacity, size_t element_size)
{
	gai_growable_header *old_header = (gai_growable_header *) array;
	if (new_capacity < old_header->capacity) return (old_header + 1);
	size_t required_size = sizeof(gai_growable_header) + (new_capacity * element_size);
	gai_growable_header *new_header = (gai_growable_header *) old_header->allocator.alloc( required_size );
	gai_memmove(new_header, old_header, sizeof(gai_growable_header) + old_header->count * element_size);
	new_header->allocator 	= old_header->allocator;
	new_header->capacity 	= new_capacity;
	new_header->count    	= old_header->count;
	new_header->allocator.free(old_header);
	return (new_header + 1);
};


#if defined(GAI_NETWORK)

#if defined(GAI_OS_WINDOWS)

/**
 *		network implementation (WINDOWS)
 *
 *		____ ____ _     ____ ____     _ _ _ _ _  _ ___  ____ _ _ _ ____
 *		| __ |__| |     |  | [__      | | | | |\ | |  \ |  | | | | [__
 *		|__] |  | | ___ |__| ___] ___ |_|_| | | \| |__/ |__| |_|_| ___]
 *
 *		__   _ _______ _______  _____   ______ _     _      _____ _______  _____
 *		| \  | |______    |    |     | |_____/ |____/         |   |  |  | |_____] |
 *		|  \_| |______    |    |_____| |    \_ |    \_      __|__ |  |  | |       |_____
 */

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

b32
gai_net_init()
{
	WSADATA wsadata;
	return (WSAStartup(MAKEWORD(2, 2), &wsadata) == 0);
}

b32
gai_net_deinit()
{
	return (WSACleanup() == 0);
}

inline gai_socket
gai_net_open_tcp(char *address, u32 port)
{
	gai_socket result = {0};
	SOCKET *client_socket = (SOCKET*) &result.dest;
	struct addrinfo *info = 0, *ptr = 0, hints;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family 	= AF_UNSPEC;
	hints.ai_socktype 	= SOCK_STREAM;
	hints.ai_protocol 	= IPPROTO_TCP;

	char port_as_string[10] = {0};
	itoa(port, port_as_string, 10);

	i32 status = 0;
	// Resolve the server address and port
	status = getaddrinfo(address, port_as_string, &hints, &info);
	if ( status != 0 )
	{
		return result;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = info; ptr != NULL ; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		*client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (*client_socket == INVALID_SOCKET)
		{
			return result;
		}

		// Connect to server.
		status = connect( *client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (status == SOCKET_ERROR)
		{
			closesocket(*client_socket);
			*client_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(info);

	return result;
}

b32
gai_net_close_tcp(gai_socket *socket)
{
	SOCKET *client_socket = (SOCKET*) &socket->dest;
	shutdown(*client_socket, SD_BOTH);
	closesocket(*client_socket);
	return 1;
};

i32
gai_net_send(gai_socket *socket, u8* data, size_t data_size)
{
	SOCKET *psocket = (SOCKET*) &socket->dest;
	SOCKET csocket  = *psocket;
	return send(csocket, (char*) data, data_size, 0);
}

i32
gai_net_receive(gai_socket *socket, u8* recv_buffer, size_t buffer_size)
{
	SOCKET *psocket = (SOCKET*) &socket->dest;
	SOCKET csocket  = *psocket;
	return recv(csocket, (char*) recv_buffer, buffer_size, 0);
}

#else
#error MISSING IMPLMENTATION [NETWORK] FOR THIS PLATFORM
#endif

#endif


/**
 * 		sorting implementation (PLATFORM INDEPENDENT)
 *
 *		_______  _____   ______ _______ _____ __   _  ______      _____ _______  _____
 *		|______ |     | |_____/    |      |   | \  | |  ____        |   |  |  | |_____] |
 *		______| |_____| |    \_    |    __|__ |  \_| |_____|      __|__ |  |  | |       |_____
 *
 *
 *		(took the most parts from ginger bill https://github.com/gingerBill/gb/blob/master/gb.h)
 *		Thanks a lot mate!
 */


#define GAI__SORT_STACK_SIZE            64
#define GAI__SORT_INSERT_SORT_THRESHOLD  8

#define GAI__SORT_PUSH(_base, _limit) do { \
		stack_ptr[0] = (_base); \
		stack_ptr[1] = (_limit); \
		stack_ptr += 2; \
	} while (0)


#define GAI__SORT_POP(_base, _limit) do { \
		stack_ptr -= 2; \
		(_base)  = stack_ptr[0]; \
		(_limit) = stack_ptr[1]; \
	} while (0)


#define _GAI_COMPARE_PROC_GEN(type) \
	static GAI_SORT_COMPARE_PROC(__gai_##type##_cmp) \
	{ \
		type p = (*(type*) a); \
		type q = (*(type*) b); \
		return p < q ? -1 : p > q; \
	}

_GAI_COMPARE_PROC_GEN(i32);
_GAI_COMPARE_PROC_GEN(i64);
_GAI_COMPARE_PROC_GEN(r32);
_GAI_COMPARE_PROC_GEN(r64);
_GAI_COMPARE_PROC_GEN(size_t);

#undef _GAI_COMPARE_PROC_GEN

void
gai_sort(void *array, size_t count, size_t el_size, gai_sort_compare_fn cmp)
{
	u8 *l, *r;
	u8 *base = (u8*) array;
	u8 *end  = base + (count * el_size);

	u8 *stack[GAI__SORT_STACK_SIZE] = {0};
	u8 **stack_ptr = stack;

	size_t threshold = GAI__SORT_INSERT_SORT_THRESHOLD * el_size;

	for (;;)
	{
		if ((end - base) > threshold)
		{
			l = base + el_size;
			r = end - el_size;

			gai_memswap( ((end - base) / el_size / 2)*el_size + base, base, el_size );
			if (cmp(l, r) > 0) 	gai_memswap(l, r, el_size);
			if (cmp(base, r) > 0) gai_memswap(base, r, el_size);
			if (cmp(l, base) > 0) gai_memswap(l, base, el_size);

			for (;;)
			{
				do l += el_size; while (cmp(l, base) < 0);
				do r -= el_size; while (cmp(r, base) > 0);
				if (l > r) break;
				gai_memswap(l, r, el_size);
			}
			gai_memswap(base, r, el_size);

			if (r - base > end - l)
			{
				GAI__SORT_PUSH(base, r);
				base = l;
			}
			else
			{
				GAI__SORT_PUSH(l, end);
				end = r;
			}
		}
		else
		{
			for (r = base, l = r + el_size;
			        l < end;
			        r = l, l += el_size)
			{
				for (; cmp(r, r + el_size) > 0; r -= el_size)
				{
					gai_memswap(r, r + el_size, el_size);
					if (r == base) break;
				}
			}

			if (stack_ptr == stack) break;
			GAI__SORT_POP(base, end);
		}
	}
}


/**
*		time measurement implementation (IN FUNCTION IMPLEMENATION PER PLATFORM)
*
*		_______ _____ _______ _______      _____ _______  _____
*		   |      |   |  |  | |______        |   |  |  | |_____] |
*		   |    __|__ |  |  | |______      __|__ |  |  | |       |_____
*
*
*		NOTE:(aga) BE AWARE : This implementation has platform specifics in the actual function implementation!
*/

GAI_INL gai_clock
gai_clock_init()
{
	gai_clock result;
	#if defined(GAI_OS_WINDOWS)
	LARGE_INTEGER frequency, current;
	QueryPerformanceFrequency(&frequency);
	result.frequency = frequency.QuadPart;
	QueryPerformanceCounter(&current);
	result.last_clock = current.QuadPart;
	#else
#error MISSING IMPLMENTATION [TIME] FOR THIS PLATFORM
	#endif
	return result;
}

GAI_INL r64
gai_clock_get(gai_clock *clock)
{
	r64 result = 0;
	#if defined(GAI_OS_WINDOWS)
	LARGE_INTEGER now;
	if (clock)
	{
		QueryPerformanceCounter(&now);
		result = (now.QuadPart - clock->last_clock) / clock->frequency;
		clock->last_clock = now.QuadPart;
	}
	else
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&now);
		result = now.QuadPart / frequency.QuadPart;
	}
	#else
#error MISSING IMPLMENTATION [TIME] FOR THIS PLATFORM
	#endif
	return result;
}

/**
*		file implementation (PLATFORM INDEPENDANT)
*
*		_______ _____        _______      _____ _______  _____
*		|______   |   |      |______        |   |  |  | |_____] |
*		|       __|__ |_____ |______      __|__ |  |  | |       |_____
*
*/

static FILE *
__gai_fopen(char const *filename, char const *mode)
{
	FILE *f;
	#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&f, filename, mode))
		f = 0;
	#else
	f = fopen(filename, mode);
	#endif
	return f;
}

GAI_INL gai_file
gai_file_open(const char *filename, const char *mode)
{
	gai_file result = {};
	FILE *fp = __gai_fopen(filename, mode);
	if (fp)
	{
		result.fp   = fp;
		//TODO:(aga) As the documentation says SEEK_END is not for portability. Search for a clean way to find the EOF.
		fseek(fp, 0, SEEK_END);
		result.size = ftell(fp);
		rewind(fp);
	}
	return result;
}

GAI_INL size_t
gai_file_write(gai_file *file, void *data, size_t length)
{
	size_t result = 0;
	if (file->fp && data && length > 0)
	{
		result = fwrite(data, 1, length, file->fp);
	}
	return result;
}

GAI_INL size_t
gai_file_read(gai_file *file, void *buffer, size_t bytes_to_read, i32 offset)
{
	size_t result = 0;
	if (file->fp && buffer && bytes_to_read > 0)
	{
		fseek(file->fp, offset, SEEK_SET);
		result = fread(buffer, 1, bytes_to_read, file->fp);
		rewind(file->fp);
	}
	return result;
}

GAI_INL size_t
gai_file_read_all(gai_file *file, void *buffer)
{
	return gai_file_read(file, buffer, file->size, 0);
}

GAI_INL void
gai_file_close(gai_file *file)
{
	if (file->fp)
	{
		fclose(file->fp);
	}
}

#if defined(GAI_OS_WINDOWS)

/**
*		file implementation (WINDOWS)
*
*		____ ____ _     ____ ____     _ _ _ _ _  _ ___  ____ _ _ _ ____
*		| __ |__| |     |  | [__      | | | | |\ | |  \ |  | | | | [__
*		|__] |  | | ___ |__| ___] ___ |_|_| | | \| |__/ |__| |_|_| ___]
*
*		_______ _____        _______      _____ _______  _____
*		|______   |   |      |______        |   |  |  | |_____] |
*		|       __|__ |_____ |______      __|__ |  |  | |       |_____
*
*/

GAI_INL b32
gai_file_exists(const char *filename)
{
	b32 result = false;
	DWORD fileAttr = GetFileAttributesA(filename);
	if (fileAttr != INVALID_FILE_ATTRIBUTES) result = true;
	return result;
}

GAI_INL gai_fileinfo
gai_file_getinfo(const char *filename)
{
	gai_fileinfo result = {0};
	WIN32_FILE_ATTRIBUTE_DATA file_info = {};
	int success = GetFileAttributesExA(filename, GetFileExInfoStandard, &file_info);
	if (success)
	{
		result.attributes             = file_info.dwFileAttributes;
		result.creation_time.low      = file_info.ftCreationTime.dwLowDateTime;
		result.creation_time.high     = file_info.ftCreationTime.dwHighDateTime;
		result.access_time.low        = file_info.ftLastAccessTime.dwLowDateTime;
		result.access_time.high       = file_info.ftLastAccessTime.dwHighDateTime;
		result.modification_time.low  = file_info.ftLastWriteTime.dwLowDateTime;
		result.modification_time.high = file_info.ftLastWriteTime.dwHighDateTime;
		result.size.high 			  = file_info.nFileSizeHigh;
		result.size.low 			  = file_info.nFileSizeLow;
	}
	return result;
}

#else
#error MISSING IMPLMENTATION [FILE] FOR THIS PLATFORM
#endif	// GAI_OS_WINDOWs

/**
*		WINDOW MANAGEMENT IMPLEMENTATION (PLATFORM INDEPENDANT)
*
*		_  _  _ _____ __   _ ______   _____  _  _  _      _____ _______  _____
*		|  |  |   |   | \  | |     \ |     | |  |  |        |   |  |  | |_____] |
*		|__|__| __|__ |  \_| |_____/ |_____| |__|__|      __|__ |  |  | |       |_____
*
*
*/

i32 __gai_platform_window_init(gai_window *window, const char *title, i32 width, i32 height, u32 flags);
b32 __gai_platform_gl_is_supported(const char *extension);
i32 __gai_platform_gl_set_swap_interval(i32 interval);
i32 __gai_platform_gl_get_swap_interval();

GAI_DEF void
gai_window_vsync(gai_window *window, b32 state)
{
	switch (window->type)
	{
		case gaiWindowDriverOpenGL:
		{
			__gai_platform_gl_set_swap_interval(state);
		} break;
		default:
		{
			GAI_ASSERT(!"VSYNC is either not supported or not implemented for this renderer.");
		} break;
	}
}

GAI_INL i32
gai_window_vsync_enabled(gai_window *window)
{
	i32 result = 0;
	switch (window->type)
	{
		case gaiWindowDriverOpenGL:
		{
			result = (__gai_platform_gl_get_swap_interval() > 0);
		} break;
		default:
		{
			GAI_ASSERT(!"VSYNC is either not supported or not implemented for this renderer.");
		} break;
	}
	return result;
}


GAI_DEF i32
gai_window_init(gai_window *window, const char *title, i32 width, i32 height, u32 flags)
{
	window->type = gaiWindowDriverDefault;
	return __gai_platform_window_init(window, title, width, height, flags);
}

GAI_DEF i32
gai_window_init_dx(gai_window *window, const char *title, i32 width, i32 height, u32 flags)
{
	window->type = gaiWindowDriverDX11;
	return __gai_platform_window_init(window, title, width, height, flags);
}

GAI_DEF i32
gai_window_init_gl(gai_window *window, const char *title, i32 width, i32 height, u32 flags,
                   i32 major, i32 minor, b32 core_profile, b32 debug)
{
	window->type             = gaiWindowDriverOpenGL;
	window->ctx.opengl.major = major;
	window->ctx.opengl.minor = minor;
	window->ctx.opengl.core  = core_profile;
	window->ctx.opengl.debug = debug;
	return __gai_platform_window_init(window, title, width, height, flags);
}

GAI_INL b32
__gai_is_key_state(gai_key_state key_state, gai_key_state_enum state)
{
	return (key_state &= state);
}

GAI_INL void
__gai_key_state_set(gai_key_state *state, b32 is_down, gai_window_callback *callbacks, u32 key)
{
	if (state)
	{
		b32 was_down = (*state & gaiKeystateDown) != 0;
		is_down = is_down != 0;
		if (is_down) *state |= gaiKeystateDown; else *state &= ~gaiKeystateDown;
		if (!was_down && is_down) *state |= gaiKeystatePressed; else *state &= ~gaiKeystatePressed;
		if (was_down && !is_down) *state |= gaiKeystateReleased; else *state &= ~gaiKeystateReleased;

		if (was_down || is_down)
		{
			i32 registered_callbacks = gai_growable_count(callbacks);
			for (i32 i = 0; i < registered_callbacks; i++)
			{
				gai_window_callback r = callbacks[i];
				if (r.type == gaiInputCallback)
				{
					r.input_callback(key, *state, r.userdata);
				}
			}
		}
	}
}

GAI_INL b32
gai_key_down(gai_window *window, i32 key)
{
	return __gai_is_key_state(window->input.keyboard_keys[key], gaiKeystateDown);
}

GAI_INL b32
gai_key_released(gai_window *window, i32 key)
{
	return __gai_is_key_state(window->input.keyboard_keys[key], gaiKeystateReleased);
}

GAI_INL b32
gai_key_pressed(gai_window *window, i32 key)
{
	return __gai_is_key_state(window->input.keyboard_keys[key], gaiKeystatePressed);
}

GAI_INL b32
gai_mouse_down(gai_window *window, i32 btnid)
{
	return __gai_is_key_state(window->input.mouse_buttons[btnid], gaiKeystateDown);
}

GAI_INL b32
gai_mouse_released(gai_window *window, i32 btnid)
{
	return __gai_is_key_state(window->input.mouse_buttons[btnid], gaiKeystateReleased);
}

GAI_INL b32
gai_mouse_pressed(gai_window *window, i32 btnid)
{
	return __gai_is_key_state(window->input.mouse_buttons[btnid], gaiKeystatePressed);
}

GAI_INL void
gai_window_register_callback(gai_window *window, gai_callback_type_enum type, void *callback, void *userdata)
{
	gai_window_callback tcallback;
	switch (type)
	{
		case gaiInputCallback:
		{
			tcallback.type = type;
			tcallback.input_callback = (gai_input_callback_proc *) callback;
			tcallback.userdata = userdata;
			gai_growable_append(window->callbacks, tcallback);
		} break;
		default:
		{
			GAI_ASSERT(!"Unknown callback type");
		}
	}
}

GAI_INL void
gai_window_unregister_callback(gai_window *window, void *callback)
{
	gai_window_callback *callbacks = window->callbacks;
	i32 registered_callbacks = gai_growable_count(callbacks);
	for (i32 i = 0; i < registered_callbacks; i++)
	{
		gai_window_callback r = callbacks[i];
		if (r.callback == callback)
		{
			r.type = 0;
			r.callback = 0;
			//TODO:(aga) Either change gai_growable to be able to remove stuff correctly or insert into unused array indicies and only append
			//			 if the growable array is at max items!
		}
	}
}

#if defined(GAI_OS_WINDOWS)

/**
*		WINDOW MANAGEMENT IMPLEMENTATION (WINDOWS)
*
*		____ ____ _     ____ ____     _ _ _ _ _  _ ___  ____ _ _ _ ____
*		| __ |__| |     |  | [__      | | | | |\ | |  \ |  | | | | [__
*		|__] |  | | ___ |__| ___] ___ |_|_| | | \| |__/ |__| |_|_| ___]
*
*		_  _  _ _____ __   _ ______   _____  _  _  _      _____ _______  _____
*		|  |  |   |   | \  | |     \ |     | |  |  |        |   |  |  | |_____] |
*		|__|__| __|__ |  \_| |_____/ |_____| |__|__|      __|__ |  |  | |       |_____
*
*
*/
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "d3d11.lib")

typedef char * (*wgl_get_extensions_strings_arb)(HDC hdc);
b32
__gai_platform_gl_is_supported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = 0;
	wgl_get_extensions_strings_arb wglGetExtString = (wgl_get_extensions_strings_arb) wglGetProcAddress("wglGetExtensionsStringARB");
	if (wglGetExtString) supported = wglGetExtString(wglGetCurrentDC());

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
}

typedef int (*wgl_get_swap_interval_ext)(void);
i32
__gai_platform_gl_get_swap_interval()
{
	i32 result = 0;
	if (__gai_platform_gl_is_supported("WGL_EXT_swap_control"))
	{
		wgl_get_swap_interval_ext wglGetSwapInterval = (wgl_get_swap_interval_ext) wglGetProcAddress("wglGetSwapIntervalEXT");
		if (wglGetSwapInterval) result = wglGetSwapInterval();
	}
	return result;
}

typedef BOOL (*wgl_swap_interval_ext)(int interval);
i32
__gai_platform_gl_set_swap_interval(i32 interval)
{
	i32 result = 0;
	if (__gai_platform_gl_is_supported("WGL_EXT_swap_control"))
	{
		wgl_swap_interval_ext wglSwapIntervalEXT = (wgl_swap_interval_ext) wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapIntervalEXT) result = wglSwapIntervalEXT(interval);
	}
	return result;
}

LRESULT CALLBACK
__gai_wndproc_win32(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gai_window *window = (gai_window *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (window)
	{
		switch (uMsg)
		{
			case WM_CLOSE:
			{
				window->request_close = true;
				return 0;
			} break;
			case WM_DESTROY:
			{
				window->was_closed = true;
				return 0;
			} break;
			case WM_SIZE:
			{
				window->w  = LOWORD(lParam);
				window->h = HIWORD(lParam);
				switch (wParam)
				{
					case SIZE_RESTORED:
					{
						if (window->flags & gaiWindowFlagsMinimized) window->flags = window->flags & ~gaiWindowFlagsMinimized;
						if (window->flags & gaiWindowFlagsMaximized) window->flags = window->flags & ~gaiWindowFlagsMaximized;
					} break;
					case SIZE_MAXIMIZED:
					{
						if (window->flags & gaiWindowFlagsMinimized) window->flags = window->flags & ~gaiWindowFlagsMinimized;
						window->flags |= gaiWindowFlagsMaximized;
					} break;
					case SIZE_MINIMIZED:
					{
						if (window->flags & gaiWindowFlagsMaximized) window->flags = window->flags & ~gaiWindowFlagsMaximized;
						window->flags |= gaiWindowFlagsMinimized;
					} break;
				}
			} break;
			case WM_MOVE:
			{
				window->x = (int)(short) LOWORD(lParam);
				window->y = (int)(short) HIWORD(lParam);
			} break;
			case WM_INPUT:
			{
				RAWINPUT raw = {0};
				u32 size = sizeof(RAWINPUT);

				if (!GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER)))
				{
					GAI_ASSERT(!"ASDASD");
				}

				switch (raw.header.dwType)
				{
					case (RIM_TYPEKEYBOARD):
					{
						RAWKEYBOARD *raw_kb = &raw.data.keyboard;
						u32 vk = raw_kb->VKey;
						u32 scan_code = raw_kb->MakeCode;
						u32 flags = raw_kb->Flags;

						b32 is_e0   = (flags & RI_KEY_E0) != 0;
						b32 is_e1   = (flags & RI_KEY_E1) != 0;
						b32 is_up   = (flags & RI_KEY_BREAK) != 0;
						b32 is_down = !is_up;

						if (vk == 255)
						{
							return 0;
						}
						else if (vk == VK_SHIFT)
						{
							vk = MapVirtualKeyW(scan_code, MAPVK_VSC_TO_VK_EX);
						}
						else if (vk == VK_NUMLOCK)
						{
							scan_code = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC) | 0x100;
						}

						if (is_e1)
						{
							if (vk == VK_PAUSE)
								scan_code = 0x45;
							else
								scan_code = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);
						}

						switch (vk)
						{
							case VK_CONTROL: vk = (is_e0) ? VK_RCONTROL : VK_LCONTROL; break;
							case VK_MENU:    vk = (is_e0) ? VK_RMENU    : VK_LMENU;   break;
						}

						__gai_key_state_set(&window->input.keyboard_keys[vk], is_down, window->callbacks, vk);
					} break;
					case RIM_TYPEMOUSE:
					{
						RAWMOUSE *raw_mouse = &raw.data.mouse;
						u16 flags = raw_mouse->usButtonFlags;
						long dx = +raw_mouse->lLastX;
						long dy = -raw_mouse->lLastY;

						if (flags & RI_MOUSE_WHEEL)
							window->input.mouse_wheel = raw_mouse->usButtonData;

						window->input.mouse_dx = dx;
						window->input.mouse_dy = dy;
					} break;
				}
				return 0;
			} break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GAI_INL void
gai_window_borderless(gai_window *window, b32 state)
{
	HWND hWnd = (HWND) window->handle;
	DWORD Style = GetWindowLong(hWnd, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
		if (GetWindowPlacement(hWnd, &window->placement_win32) &&
		        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLong(hWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
			             MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
			             MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			             MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		window->flags |= gaiWindowFlagsBorderless;
	}
	else
	{
		SetWindowLong(hWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &window->placement_win32);
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		window->flags &= ~gaiWindowFlagsBorderless;
	}
}

typedef HGLRC (*wgl_create_context_attribs_arb)(HDC hDC, HGLRC hShareContext, const int *attribList);

i32
__gai_platform_window_init(gai_window *window, const char *title, i32 width, i32 height, u32 flags)
{
	WNDCLASSEXA wnd_class   = {sizeof(wnd_class)};
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = sizeof(gai_window*);
	wnd_class.lpfnWndProc   = __gai_wndproc_win32;
	wnd_class.hInstance     = GetModuleHandle(0);
	wnd_class.lpszClassName = "gai_window_classname_win32";
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassExA(&wnd_class))
	{
		GAI_ASSERT(!"Can't register window class.");
		return -1;
	}


	DWORD style = WS_OVERLAPPEDWINDOW;// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	HWND hwnd 	= CreateWindowExA(0, wnd_class.lpszClassName, title, style,
	                             CW_USEDEFAULT,
	                             CW_USEDEFAULT,
	                             (width 	== -1 ? CW_USEDEFAULT : width),
	                             (height == -1 ? CW_USEDEFAULT : height),
	                             0, 0, wnd_class.hInstance, 0);
	if (!hwnd)
	{
		GAI_ASSERT(!"Can't create window.");
		return -2;
	}

	//NOTE:(aga) This will make the windows client area the actual requested size
	if (width > 0 && height > 0)
	{
		RECT rc = {0, 0, width, height};
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);
		SetWindowPos( hwnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE ) ;
	}
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) window);

	window->was_closed      = 0;
	window->request_close   = 0;
	window->w               = width;
	window->h               = height;
	window->flags           = flags;
	window->handle          = (void*) hwnd;

	gai_growable_init(window->callbacks, gai_allocator_init(malloc, free));

	gai_memset(&window->input, 0, sizeof(window->input)); // NOTE:(aga) Initialize input keys to zero

	RAWINPUTDEVICE rid[2] = {0};

	// NOTE: Keyboard
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage     = 0x06;
	rid[0].dwFlags     = 0x00000030/*RIDEV_NOLEGACY*/; // NOTE(aga): Do not generate legacy messages such as WM_KEYDOWN
	rid[0].hwndTarget  = hwnd;

	// NOTE: Mouse
	rid[1].usUsagePage = 0x01;
	rid[1].usUsage     = 0x02;
	rid[1].dwFlags     = 0; // NOTE(aga): adds HID mouse and also allows legacy mouse messages to allow for window movement etc.
	rid[1].hwndTarget  = hwnd;

	if (RegisterRawInputDevices(rid, gai_array_size(rid), sizeof(rid[0])) == false)
	{
		DWORD err = GetLastError();
		GAI_ASSERT(!"Failed to initialize raw input device for win32.");
	}

	switch (window->type)
	{
		case gaiWindowDriverDefault: {} break;
		case gaiWindowDriverOpenGL:
		{
			HDC hdc 				  = GetDC(hwnd);

			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize                 = sizeof(pfd);
			pfd.nVersion              = 1;
			pfd.iPixelType            = PFD_TYPE_RGBA;
			pfd.dwFlags               = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
			pfd.cColorBits            = 24;
			pfd.cAlphaBits            = 8;
			pfd.cDepthBits            = 32;
			pfd.iLayerType            = PFD_MAIN_PLANE;

			int pf = ChoosePixelFormat(hdc, &pfd);
			if (!pf)
			{
				GAI_ASSERT(!"No valid pxiel format found.");
				return -3;
			}

			if (!SetPixelFormat(hdc, pf, &pfd))
			{
				GAI_ASSERT(!"Set pixel format failed.");
				return -4;
			}

			HGLRC old_glctx = wglCreateContext(hdc);
			if (!old_glctx)
			{
				GAI_ASSERT(!"Can't create opengl context.");
				return -5;
			}
			window->ctx.opengl.context = old_glctx;
			if (!wglMakeCurrent(hdc, old_glctx))
			{
				GAI_ASSERT(!"Activate opengl context failed..");
				wglDeleteContext(old_glctx);
				return -6;
			}

			if (__gai_platform_gl_is_supported("WGL_ARB_create_context"))
			{
				wgl_create_context_attribs_arb wglCreateContextAttribsARB = (wgl_create_context_attribs_arb) wglGetProcAddress("wglCreateContextAttribsARB");
				if (wglCreateContextAttribsARB)
				{
					// Creates a compatibility context if the core version is not requested!
					int attribs[] =
					{
						0x2091 /* WGL_CONTEXT_MAJOR_VERSION_ARB */, gai_max(window->ctx.opengl.major, 1),
						0x2092 /* WGL_CONTEXT_MINOR_VERSION_ARB */, gai_max(window->ctx.opengl.minor, 0),
						0x9126 /* WGL_CONTEXT_PROFILE_MASK_ARB */,
						(window->ctx.opengl.core == true
						? 0x00000001 /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB */
						: 0x00000002 /* WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB */
						),
						0x2094 /* WGL_CONTEXT_FLAGS_ARB */, (window->ctx.opengl.debug ? 0x00000001 : 0),
						0
					};
					HGLRC new_oglctx = wglCreateContextAttribsARB(hdc, 0, attribs);
					if (new_oglctx)
					{
						wglMakeCurrent(0, 0);
						wglDeleteContext(old_glctx);
						wglMakeCurrent(hdc, new_oglctx);
						glGetIntegerv(0x821B /* GL_MAJOR_VERSION */, &window->ctx.opengl.major);
						glGetIntegerv(0x821C /* GL_MINOR_VERSION */, &window->ctx.opengl.minor);
						window->ctx.opengl.context = new_oglctx;
					}
					else GAI_ASSERT(!"wglCreateContextAttribsARB failed to create modern context.");
				}
				else GAI_ASSERT(!"wglGetProcAddress failed for wglCreateContextAttribsARB.");
			}
		} break;
		case gaiWindowDriverDX11:
		{
			// create a struct to hold information about the swap chain
			DXGI_SWAP_CHAIN_DESC scd;

			// clear out the struct for use
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

			// fill the swap chain description struct
			scd.BufferCount       = 1;                                  		// one back buffer
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     			// use 32-bit color
			scd.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;    		// how swap chain is to be used
			scd.OutputWindow      = hwnd;                               		// the window to be used
			scd.SampleDesc.Count  = 4;                               			// how many multisamples
			scd.Windowed          = TRUE;                               		// windowed/full-screen mode
			scd.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching


			// create a device, device context and swap chain using the information in the scd struct
			D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE,
			                              0, 0, 0, 0,
			                              D3D11_SDK_VERSION,
			                              &scd,
			                              &window->ctx.dx11.swapchain,
			                              &window->ctx.dx11.dev,
			                              0,
			                              &window->ctx.dx11.devcon);
		} break;
		default: { GAI_ASSERT(!"rendertype is not supported"); return -11; } break;
	}


	//NOTE:(aga) This mask(bitmask) defines the cpu core usage for this thread. We only use 1 core for the main thread to have accurate timings!
	SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	window->clock = gai_clock_init();
	return 1;
}

GAI_DEF void
gai_window_show(gai_window *window)
{
	ShowWindow((HWND)window->handle, SW_SHOW);
}

GAI_DEF void
gai_window_hide(gai_window *window)
{
	ShowWindow((HWND)window->handle, SW_HIDE);
}

GAI_INL void
gai_window_handle_messages(gai_window *window, b32 block)
{
	for (i32 i = 0; i < gai_array_size(window->input.keyboard_keys); i++)
	{
		gai_key_state *state = window->input.keyboard_keys + i;
		b32 is_down = (*state & gaiKeystateDown) != 0;
		__gai_key_state_set(state, is_down, window->callbacks, i);
	}

	DWORD buttons[5] = 	{ VK_LBUTTON, VK_MBUTTON, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, };
	for (i32 i = 0; i < gai_array_size(buttons); i++)
		__gai_key_state_set(window->input.mouse_buttons + i, GetAsyncKeyState(buttons[i]) < 0, window->callbacks, i);

	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient((HWND)window->handle, &mouse_pos);
	{
		i32 x = mouse_pos.x;
		i32 y = window->h - 1 - mouse_pos.y;
		window->input.mouse_dx = x - window->input.mouse_x;
		window->input.mouse_dy = y - window->input.mouse_y;
		window->input.mouse_x = x;
		window->input.mouse_y = y;
	}

	MSG msg;
	for (;;)
	{
		i32 ret = PeekMessage(&msg, (HWND) window->handle, 0, 0, PM_REMOVE);
		if (ret == 0) break;

		switch (msg.message)
		{
			case WM_QUIT: { window->request_close = 1; } break;
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} break;
		}
	}

	window->dt 		= gai_clock_get(&window->clock);
	window->time 	+= window->dt;
}

GAI_INL void
gai_window_display(gai_window *window)
{
	switch (window->type)
	{
		case gaiWindowDriverOpenGL:
		{
			HDC hdc = GetDC((HWND)window->handle);
			SwapBuffers(hdc);
			ReleaseDC(NULL, hdc);
		} break;
		default:
		{} break;
	}
}

GAI_DEF i32
gai_window_destroy(gai_window *window)
{
	switch (window->type)
	{
		case gaiWindowDriverOpenGL:
		{
			HGLRC ctx = (HGLRC) window->ctx.opengl.context;
			if (ctx)
			{
				wglMakeCurrent(0, 0);
				wglDeleteContext(ctx);
			}
		} break;
		case gaiWindowDriverDX11:
		{
			window->ctx.dx11.swapchain->Release();
			window->ctx.dx11.dev->Release();
			window->ctx.dx11.devcon->Release();
		} break;
		default:
		{} break;
	}

	gai_growable_free(window->callbacks);
	//UnregisterClassA(platform->os.win32.classname, (HDC)platform->os.win32.instance);
	return DestroyWindow((HWND)window->handle);
}

GAI_DEF b32
gai_window_set_foreground(gai_window *window)
{
	return (SetForegroundWindow((HWND)window->handle) != 0);
}

GAI_INL b32
gai_window_is_foreground(gai_window *window)
{
	return ( (HWND)window->handle == GetForegroundWindow() );
}

GAI_INL void
gai_window_set_position(gai_window *window, i32 x, i32 y)
{
	SetWindowPos((HWND)window->handle, 0, x, y, 0, 0, SWP_NOSIZE);
}

#elif defined(GAI_OS_UNIX)

/**
*		WINDOW MANAGEMENT IMPLEMENTATION (UNIX)
*
*		____ ____ _     ____ ____     _  _ _  _ _ _  _
*		| __ |__| |     |  | [__      |  | |\ | |  \/
*		|__] |  | | ___ |__| ___] ___ |__| | \| | _/\_
*
*		_  _  _ _____ __   _ ______   _____  _  _  _      _____ _______  _____
*		|  |  |   |   | \  | |     \ |     | |  |  |        |   |  |  | |_____] |
*		|__|__| __|__ |  \_| |_____/ |_____| |__|__|      __|__ |  |  | |       |_____
*
*
*/

i32
__gai_platform_window_init(gai_window *window, const char *title, i32 width, i32 height, u32 flags)
{
	Display *d;
	Window root, w;
	int s;
	d = XOpenDisplay(0);
	if (!d)
	{
		GAI_ASSERT(!"Failed to create display");
	}
	s = DefaultScreen(d);
	root = DefaultRootWindow(d);

	switch (window->type)
	{
		case gaiWindowDriverOpenGL:
		{
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			XVisualInfo *vi = glXChooseVisual(d, 0, att);
			if (vi == NULL)
			{
				return -1;
			}
			Colormap cmap = XCreateColormap(d, root, vi->visual, AllocNone);

			XSetWindowAttributes swa;
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask;
			w = XCreateWindow(d, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

			XMapWindow(d, w);
			XStoreName(d, w, title);

			GLXContext glc = glXCreateContext(d, vi, NULL, GL_TRUE);
			glXMakeCurrent(d, w, glc);

			glGetIntegerv(0x821B /* GL_MAJOR_VERSION */, &window->ctx.opengl.major);
			glGetIntegerv(0x821C /* GL_MINOR_VERSION */, &window->ctx.opengl.minor);

		} break;
		default:
		{ GAI_ASSERT(!"asdasdas"); } break;
	}

	window->handle = (void *) d;
	window->unix_wnd = w;
	return 1;
}

GAI_INL void
gai_window_handle_messages(gai_window *window, b32 block)
{
	window->dt = 0.016f;
	Display *d = (Display*) window->handle;

	for (i32 i = 0; i < gai_array_size(window->input.keyboard_keys); i++)
	{
		gai_key_state *state = window->input.keyboard_keys + i;
		b32 is_down = (*state & gaiKeystateDown) != 0;
		__gai_key_state_set(state, is_down, window->callbacks, i);
	}

	while ( XPending ( d ) )     // check for events from the x-server
	{
		XEvent  xev;
		XNextEvent( d, &xev );
		if (xev.type == Expose)
		{
			XWindowAttributes gwa;
			XGetWindowAttributes(d, window->unix_wnd, &gwa);
			window->w = gwa.width;
			window->h = gwa.height;
		}
		if ( xev.type == MotionNotify ) {  } // if mouse has moved
		if ( xev.type == KeyPress )
		{
			//window->was_closed = true;
			//window->request_close = true;
		}
	}
}

GAI_INL void
gai_window_display(gai_window *window)
{
	Display *d = (Display*) window->handle;
	glXSwapBuffers(d, window->unix_wnd);
}

#else
#error  MISSING IMPLMENTATION [WINDOW] FOR THIS PLATFORM
#endif // GAI_WINDOW_OS_IMPLEMENTATIONS

#endif // GAI_IMPLEMENTATION

/// @cond DOXYGEN_IGNORE
#define _INCLUDE_GAI_H
/// @endcond
#endif