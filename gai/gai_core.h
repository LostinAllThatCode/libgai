#ifndef _GAI_CORE_H

#ifdef GAI_EXPORT
	#define GAI_API __declspec (dllexport)
#else
	#define GAI_API __declspec (dllimport)
#endif

#ifdef GAI_SOURCE
	#ifdef GAI_STATIC
		#define GAI_DEF static
	#else
		#define GAI_DEF extern
	#endif
	#undef  GAI_API
	#define GAI_API
#else
	#define GAI_DEF GAI_API
#endif

#if defined GAI_SOURCE || defined GAI_EXPORT
	#define GAI_WORKQUEUE_IMPLEMENTATION
	#define GAI_WINDOW_IMPLEMENTATION
	#define GAI_OPENGL_IMPLEMENTATION
	#define GAI_OPENGL_SHADERS_IMPLEMENTATION
	#define GAI_TIME_IMPLEMENTATION
#endif

#include <gai_types.h>
#include <gai_utils.h>
#include <gai_hotreload.h>

#include <stb_image.h>        /* image loading support library */
#include <stb_truetype.h>     /* font loading support library */

#include <gai_math.h>
#include <gai_workqueue.h>
#include <gai_time.h>

#include <gai_window.h>
#include <gai_renderer.h>

#define _GAI_CORE_H
#endif
