#ifndef _GAI_CORE_H

#include "gai_types.h"
#include "gai_utils.h"

//#include "gai_linkedlist.cpp"

#define GAI_WORKQUEUE_IMPLEMENTATION
#include "gai_workqueue.h"

//#define GAI_HASHMAP_IMPLEMENTATION
//#include "gai_hashmap.h"

/* Include stb libraries as well, just because they help a lot */
#include "stb_image.h"        /* image loading support library */
#include "stb_truetype.h"     /* font loading support library */

#define GAI_WINDOW_IMPLEMENTATION
#include "gai_window.h"

#ifdef GAI_OPENGL
#   define GAI_OPENGL_IMPLEMENTATION
#   include "gai_opengl.h"
#   define GAI_OPENGL_SHADERS_IMPLEMENTATION
#   include "gai_opengl_shaders.h"
#endif

#define _GAI_CORE_H
#endif
