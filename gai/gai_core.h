#ifndef _GAI_CORE_H

#ifdef GAI_EXPORT
	#define GAI_API __declspec (dllexport)
#else
	#define GAI_API __declspec (dllimport)
#endif

#ifdef GAI_STATIC
	#undef  GAI_API
	#define GAI_API
	#define GAI_DEF static
#else
	#define GAI_DEF GAI_API extern
#endif

#if defined GAI_STATIC || defined GAI_EXPORT 
	#define GAI_WORKQUEUE_IMPLEMENTATION
	#define GAI_WINDOW_IMPLEMENTATION
	#define GAI_OPENGL_IMPLEMENTATION
	#define GAI_OPENGL_SHADERS_IMPLEMENTATION
	#define GAI_TIMER_IMPLEMENTATION
#endif

#include "gai_types.h"
#include "gai_utils.h"

#include "stb_image.h"        /* image loading support library */
#include "stb_truetype.h"     /* font loading support library */

#include "gai_workqueue.h"
#include "gai_timer.h"
#include "gai_window.h"
#include "gai_opengl.h"
#include "gai_opengl_shaders.h"

#define _GAI_CORE_H
#endif
