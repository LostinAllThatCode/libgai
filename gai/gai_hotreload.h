/*
==========================================================================================
$Name: gai_hotreload.h$
$Description:
Simple quick and easy hot reloading implementation.

Supported features:
	hotreloadable reloading | Quickly reload dll(s) thread-safe on runtime
$
$Creator: Andreas Gaida$
$Copyright: $
$Example:
	Link to a github example:
								http://www.not-quite-ready-yet.com
$
==========================================================================================
*/

#ifndef _GAI_HOTRELOAD_H

#include "gai_types.h"
#include "gai_utils.h"

#define GAI_HOTRELOAD_MAX_TEXT_LEN 4096

#if _WIN32
#include <windows.h>
struct gaiHotReloadPlatform
{
	HANDLE   mutex;
	HANDLE   event;
	FILETIME last_write_time;
};
#else
struct gaiHotReloadPlatform { void *null_ptr };
#endif

enum gaiHotReloadFlagEnum
{
	gaiHotReloadNone       			= 0x0,
	gaiHotReloadDontHandleEvent     = 0x1,
};

struct gaiHotReloadFile;
typedef void gaiHotReloadCallback(gaiHotReloadFile *hotreloadable);
struct gaiHotReloadFile
{
	void                    	*userdata;
	gaiHotReloadCallback		*callback;
	gaiHotReloadPlatform        platform;
	const char                  *filename;
	int     	                interval;
	gaiHotReloadFlagEnum        flags;
};

#if _WIN32

inline void
gaiHotReloadEventCallback(gaiHotReloadFile *hotreloadable)
{
	if (WaitForSingleObject(hotreloadable->platform.event, 0) == WAIT_OBJECT_0)
	{
		WaitForSingleObject(hotreloadable->platform.mutex, INFINITE);
		if (hotreloadable->callback) hotreloadable->callback(hotreloadable);
		ReleaseMutex(hotreloadable->platform.mutex);
		ResetEvent(hotreloadable->platform.event);
	}
}

inline BOOL
gaiHotReloadBeginMutex(gaiHotReloadFile *hotreloadable, int timeout = INFINITE)
{
	BOOL result = (WaitForSingleObject(hotreloadable->platform.mutex, timeout) == WAIT_OBJECT_0);
	return result;
}

inline void
gaiHotReloadEndMutex(gaiHotReloadFile *hotreloadable)
{
	ReleaseMutex(hotreloadable->platform.mutex);
}

DWORD WINAPI
gaiHotReloadWin32Thread(void *data)
{
	gaiHotReloadFile *hotreloadable = (gaiHotReloadFile *) data;
	while (hotreloadable)
	{
		WIN32_FILE_ATTRIBUTE_DATA file_info_now;
		GetFileAttributesExA(hotreloadable->filename, GetFileExInfoStandard, &file_info_now);
		if ( CompareFileTime(&hotreloadable->platform.last_write_time, &file_info_now.ftLastWriteTime) != 0 )
		{
			hotreloadable->platform.last_write_time = file_info_now.ftLastWriteTime;
			hotreloadable->platform.event           = CreateEvent(0, true, true, 0);

			if (!gai_bitand(hotreloadable->flags, gaiHotReloadDontHandleEvent))
			{
				gaiHotReloadEventCallback(hotreloadable);
			}
		}
		Sleep(hotreloadable->interval);
	}
	return 1;
}

BOOL gaiHotReloadAddFile( gaiHotReloadFile *hotreloadable, const char *filename, int interval = 0, gaiHotReloadCallback *callback = 0, void *userdata = 0, gaiHotReloadFlagEnum flags = gaiHotReloadNone)
{
	BOOL result = false;
	gaiHotReloadFile temp =
	{
		userdata,
		callback,
		{ CreateMutex(0, 0, 0) },
		filename,
		interval,
		flags
	};

	*hotreloadable = temp;

	if ( CreateThread(0, 0, gaiHotReloadWin32Thread, hotreloadable, 0, 0) != 0)
	{
		result = true;
	}

	return result;
}

#else

#pragma message("libgai [warning]: \"Hot Reloading\" is not implemented for this platform")

#endif

#define _GAI_HOTRELOAD_H
#endif
