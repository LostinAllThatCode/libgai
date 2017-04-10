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

#ifndef _GAI_INCLUDE_HOTRELOAD_H

#define GAIHR_TEXTLENGTH_MAX 4096

#ifndef GAIHR_STATIC
	#define GAIHR_API extern
#else
	#define GAIHR_API static
#endif

#if _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment( lib, "user32.lib" )

struct gaihr_platform
{
	HANDLE   mutex;
	HANDLE   event;
	FILETIME last_write_time;
};
#else
struct gaihr_platform { void *null_ptr };
#endif

enum gaihr_flags
{
	gaihr_FlagsNone      		= 0x0,
	gaihr_FlagsDontHandleEvent  = 0x1,
};

struct gaihr_file;
typedef void gaihr_callback(gaihr_file *hotreloadable);
struct gaihr_file
{
	void          	*userdata;
	gaihr_callback	*callback;
	gaihr_platform	platform;
	const char    	*filename;
	int     	  	interval;
	gaihr_flags   	flags;
};

GAIHR_API BOOL 	gaihr_AddFile( gaihr_file *hotreloadable, const char *filename, int interval = 0, gaihr_callback *callback = 0, void *userdata = 0, gaihr_flags flags = gaihr_FlagsNone);

GAIHR_API void	gaihr_WaitForEvent(gaihr_file *hotreloadable);
GAIHR_API BOOL 	gaihr_BeginTicketMutex(gaihr_file *hotreloadable, int timeout = INFINITE);
GAIHR_API void 	gaihr_EndTicketMutex(gaihr_file *hotreloadable);

#ifdef GAIHR_IMPLEMENTATION

#if _WIN32


inline GAIHR_API void
gaihr_WaitForEvent(gaihr_file *hotreloadable)
{
	if (WaitForSingleObject(hotreloadable->platform.event, 0) == WAIT_OBJECT_0)
	{
		WaitForSingleObject(hotreloadable->platform.mutex, INFINITE);
		if (hotreloadable->callback) hotreloadable->callback(hotreloadable);
		ReleaseMutex(hotreloadable->platform.mutex);
		ResetEvent(hotreloadable->platform.event);
	}
}

inline GAIHR_API BOOL
gaihr_BeginTicketMutex(gaihr_file *hotreloadable, int timeout)
{
	BOOL result = (WaitForSingleObject(hotreloadable->platform.mutex, timeout) == WAIT_OBJECT_0);
	return result;
}

inline GAIHR_API void
gaihr_EndTicketMutex(gaihr_file *hotreloadable)
{
	ReleaseMutex(hotreloadable->platform.mutex);
}

DWORD WINAPI
gaihr_WorkerThread(void *data)
{
	gaihr_file *hotreloadable = (gaihr_file *) data;
	while (hotreloadable)
	{
		WIN32_FILE_ATTRIBUTE_DATA file_info_now;
		GetFileAttributesExA(hotreloadable->filename, GetFileExInfoStandard, &file_info_now);
		if ( CompareFileTime(&hotreloadable->platform.last_write_time, &file_info_now.ftLastWriteTime) != 0 )
		{
			hotreloadable->platform.last_write_time = file_info_now.ftLastWriteTime;
			hotreloadable->platform.event           = CreateEvent(0, true, true, 0);

			if (!(hotreloadable->flags & gaihr_FlagsDontHandleEvent))
			{
				gaihr_WaitForEvent(hotreloadable);
			}
		}
		Sleep(hotreloadable->interval);
	}
	return 1;
}

BOOL GAIHR_API
gaihr_AddFile( gaihr_file *hotreloadable, const char *filename, int interval, gaihr_callback *callback, void *userdata, gaihr_flags flags)
{
	BOOL result = false;

	hotreloadable->userdata = userdata;
	hotreloadable->callback = callback;
	hotreloadable->platform = { CreateMutex(0, 0, 0) };
	hotreloadable->filename = filename;
	hotreloadable->interval = interval;
	hotreloadable->flags    = flags;

	if ( CreateThread(0, 0, gaihr_WorkerThread, hotreloadable, 0, 0) != 0)
	{
		result = true;
	}

	return result;
}

#else

#pragma message("libgai [warning]: \"Hot Reloading\" is not implemented for this platform")

#endif

#endif

#define _GAI_HOTRELOAD_H
#endif
