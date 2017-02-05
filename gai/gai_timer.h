/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_TIMER_H_

#include <gai_types.h>
#include <gai_utils.h>

#ifdef __cplusplus
extern "C" {
#endif

#if _WIN32
#include <windows.h>
typedef struct
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER starttime;
} gaiTimer;
#elif __linux__
#elif __APPLE__
#else
#endif

GAI_DEF void gaiTimerInit(gaiTimer *timer);
GAI_DEF r32  gaiTimerGetTicks(gaiTimer *timer);
GAI_DEF r32  gaiTimerGetTicksSeconds(gaiTimer *timer);
GAI_DEF r32  gaiTimerGetTicksMillis(gaiTimer *timer);

#ifdef __cplusplus
}
#endif

#ifdef GAI_TIMER_IMPLEMENTATION
	#include <gai_timer_win32.cc>
#endif
#define _GAI_TIMER_H_
#endif
