/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_TIME_H_

#if _WIN32
	#include <windows.h>
	static LARGE_INTEGER __libgai_global_frequency;
	static LARGE_INTEGER __libgai_global_lasttime;
#else
	#include <time.h>
	clock_t __libgai_global_lasttime;
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
GAI_DEF r32  gaiTimeGetTicks();
GAI_DEF r32  gaiTimeGetTicksSeconds();
GAI_DEF r32  gaiTimeGetTicksMillis();

#ifdef __cplusplus
}
#endif

#ifdef GAI_TIME_IMPLEMENTATION
#if _WIN32
#include <gai_time_win32.cc>
#else
r32 gaiTimeGetTicks()
{
	static b32 initialized;
	if (!initialized)
	{
		__libgai_global_lasttime = clock();
		initialized = true;
	}
	clock_t now = clock();
	r32 elapsed = ((r64) (now - __libgai_global_lasttime)) / CLOCKS_PER_SEC;
	__libgai_global_lasttime = now;
	return elapsed;
}

r32 gaiTimeGetTicksSeconds()
{
	return gaiTimeGetTicks();
}

r32 gaiTimeGetTicksMillis()
{
	return gaiTimeGetTicks() * 1000.f;
}
#endif
#endif

#define _GAI_TIME_H_
#endif
