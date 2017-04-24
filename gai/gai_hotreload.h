/*
	Author(s): Andreas Gaida

	gai_hotreload.h - v0.1 - https://github.com/LostinAllThatCode/libgai/blob/render_tests/gai/gai_hotreload.h

	NOTE: Only WINDOWS is supported for now.

	This api allows you to track any file changes happen to a file ( not really content wise changes, more filetime changes on save & copy triggers ).
	The tracking will take place on a seperate thread which will be started the first time you add a file via gaihr_AddFile(...) call.
	You can specify diffrent flags on how the event and callback function will be handled.

		gaihr_FlagsNone      			= 0x0,
		gaihr_FlagsDontHandleEvent  	= 0x1, // Indicates whether the thread will not call the callback function on event. !You have to do it on the parents thread yourself.
		gaihr_FlagsDontResetEvent  		= 0x2, // Indicates whether the marked event will not be resetted after calling the callback function. You have to do it yourself.
		gaihr_FlagsSkipInitialChange 	= 0x4, // Indicates whether the initial file change will not result in an event.

	You can use this api to hotreloading of dlls or textures or just plain textfiles.

	Linker dependencies per platform:

		windows : user32.lib
		** Note:
		*		If you are using Microsoft Visual Studios compiler you won't need to specify libs.
		*		All required libs will be include via pragma instruction.

	Do this:
		#define GAIHR_IMPLEMENTATION
   	before you include this file in *one* C or C++ file to create the implementation.

	All function prefixed with a underscore(_) are internally used functions.
	DO NOT use them if you are not 100% sure what they do.

	Example Code:

	// NOTE: A file with the name "testfile.txt" has to be in the directory of the executable.
	//       After running the executable you have to change the files content and save it.
	//		 Or just replace it with another file with the same name.

	#define GAIHR_IMPLEMENTATION
	#include "gai_hotreload.h"

	#include <stdio.h>

	volatile int running = 1; // This will be changed by another thread!

	void reloadFile(gaihr_file *file)
	{
		// Do whatever you want to do, when this happens.
		printf("%s changed!\n", file->filename);
		running = 0;
	}

	int main(int argc, char **argv)
	{
		gaihr_file MyFile = {};
		gaihr_AddFile(&MyFile, "testfile.txt", reloadFile, 0, gaihr_FlagsSkipInitialChange);
		while(running) {Sleep(125);}
		return 0;
	}
*/

#ifndef _GAI_INCLUDE_HOTRELOAD_H

<<<<<<< HEAD
#define GAIHR_TEXTLENGTH_MAX 	4096
#define GAIHR_DEFAULT_INTERVAL 	1000
=======
#define GAIHR_WAIT_INFINITE  -1
#define GAIHR_THREAD_TIMEOUT 1000
#define GAIHR_TEXTLENGTH_MAX 4096
#define GAIHR_FILE_LIMIT	 64
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546

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

#error gai_hotreload.h: Error. This platform is not supported!

#endif

enum gaihr_flags
{
	gaihr_FlagsNone      			= 0x0,
	gaihr_FlagsDontHandleEvent  	= 0x1, // Indicates whether the thread will not call the callback function on event. You have to do it yourself.
	gaihr_FlagsDontResetEvent  		= 0x2, // Indicates whether the marked event will not be resetted after calling the callback function. You have to do it yourself.
	gaihr_FlagsSkipInitialChange 	= 0x4, // Indicates whether the initial file change will not result in an event.
};

struct gaihr_file;
typedef void gaihr_callback(gaihr_file *hotreloadable);
struct gaihr_file
{
	void          	*userdata;
	gaihr_callback	*callback;
	gaihr_platform	platform;
	const char    	*filename;
	gaihr_flags   	flags;
};

<<<<<<< HEAD
GAIHR_API BOOL 	gaihr_AddFile( gaihr_file *hotreloadable, const char *filename, int interval = -1, gaihr_callback *callback = 0, void *userdata = 0, gaihr_flags flags = gaihr_FlagsNone);
=======
/*
	TODO: 	Make a list for all files added. And then loop over the list via next pointer.
			Do not make this a static thing?!
*/
static gaihr_file *_gaihr_files[GAIHR_FILE_LIMIT];

GAIHR_API int 	gaihr_AddFile 			(gaihr_file *file, const char *filename, gaihr_callback *callback = 0, void *userdata = 0, gaihr_flags flags = gaihr_FlagsNone);
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546

GAIHR_API void  gaihr_CreateEvent 		(gaihr_file *file);
GAIHR_API void  gaihr_ResetEvent 		(gaihr_file *file);
GAIHR_API void	gaihr_WaitForEvent 		(gaihr_file *file);
GAIHR_API BOOL 	gaihr_BeginTicketMutex  (gaihr_file *file, int timeout = GAIHR_WAIT_INFINITE);
GAIHR_API void 	gaihr_EndTicketMutex	(gaihr_file *file);

#ifdef GAIHR_IMPLEMENTATION

#if _WIN32

inline GAIHR_API BOOL
gaihr_BeginTicketMutex(gaihr_file *file, int timeout)
{
	BOOL result = (WaitForSingleObject(file->platform.mutex, timeout) == WAIT_OBJECT_0);
	return result;
}

inline GAIHR_API void
gaihr_EndTicketMutex(gaihr_file *file)
{
<<<<<<< HEAD
	if (WaitForSingleObject(hotreloadable->platform.event, 0) == WAIT_OBJECT_0)
	{
		if (hotreloadable->callback) hotreloadable->callback(hotreloadable);
		ResetEvent(hotreloadable->platform.event);
	}
=======
	ReleaseMutex(file->platform.mutex);
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546
}

inline GAIHR_API void
gaihr_CreateEvent(gaihr_file *file)
{
	if ( file->platform.event != 0) CloseHandle(file->platform.event);
	file->platform.event = CreateEvent(0, 1, 1, 0);
}

inline GAIHR_API void
gaihr_ResetEvent(gaihr_file *file)
{
	ResetEvent(file->platform.event);
}

inline GAIHR_API void
gaihr_WaitForEvent(gaihr_file *file)
{
	if (WaitForSingleObject(file->platform.event, 0) == WAIT_OBJECT_0)
	{
		gaihr_BeginTicketMutex(file);
		if (file->callback) file->callback(file);
		if (!(file->flags & gaihr_FlagsDontResetEvent)) gaihr_ResetEvent(file);
		gaihr_EndTicketMutex(file);
	}
}

inline GAIHR_API int
_gaihr_CheckFileChanged(gaihr_file *file)
{
	WIN32_FILE_ATTRIBUTE_DATA file_info_now = {};
	GetFileAttributesExA(file->filename, GetFileExInfoStandard, &file_info_now);
	if ( CompareFileTime(&file->platform.last_write_time, &file_info_now.ftLastWriteTime) != 0 )
	{
		if ( (file->flags & gaihr_FlagsSkipInitialChange) && file->platform.last_write_time.dwHighDateTime != 0 )
		{
			return 0;
		}
		else
		{
			file->platform.last_write_time = file_info_now.ftLastWriteTime;
			return 1;
		}
	}
	return 0;
}

DWORD WINAPI
gaihr_WorkerThread(void *data)
{
	for (;;)
	{
		gaihr_file **files = _gaihr_files;
		for (int i = 0; i < (sizeof(_gaihr_files) / sizeof(_gaihr_files[0])); i++)
		{
<<<<<<< HEAD
			//if (gaihr_BeginTicketMutex(hotreloadable, 0))
			{
				hotreloadable->platform.last_write_time = file_info_now.ftLastWriteTime;
				hotreloadable->platform.event           = CreateEvent(0, true, true, 0);
				if (!(hotreloadable->flags & gaihr_FlagsDontHandleEvent)) gaihr_WaitForEvent(hotreloadable);
=======
			gaihr_file *file = files[i];
			if (file)
			{
				if ( _gaihr_CheckFileChanged(file) )
				{
					gaihr_CreateEvent(file);
					if (!(file->flags & gaihr_FlagsDontHandleEvent))
					{
						gaihr_WaitForEvent(file);
					}
				}
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546
			}
			//gaihr_EndTicketMutex(hotreloadable);
		}
		Sleep(GAIHR_THREAD_TIMEOUT);
	}
	return 1;
}

<<<<<<< HEAD
BOOL GAIHR_API
gaihr_AddFile( gaihr_file * hotreloadable, const char *filename, int interval, gaihr_callback * callback, void *userdata, gaihr_flags flags)
=======
GAIHR_API int
gaihr_AddFile(gaihr_file *file, const char *filename, gaihr_callback *callback, void *userdata, gaihr_flags flags)
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546
{
	static int file_count;
	if (!file || file_count >= GAIHR_FILE_LIMIT)
	{
		//GAIHR_ASSERT()
		return 0;
	}

<<<<<<< HEAD
	hotreloadable->userdata = userdata;
	hotreloadable->callback = callback;
	hotreloadable->platform = { CreateMutex(0, 0, 0) };
	hotreloadable->filename = filename;
	hotreloadable->interval = (interval == -1 ? GAIHR_DEFAULT_INTERVAL : interval);
	hotreloadable->flags    = flags;
=======
	file->filename                 = filename;
	file->callback                 = callback;
	file->userdata                 = userdata;
	file->flags                    = flags;
	file->platform.mutex           = CreateMutex(0, 0, 0);
	file->platform.event           = 0;
	file->platform.last_write_time = {0};
>>>>>>> 6148ec74e4480fe5e556c5ace60e949c1ee70546

	if (file_count == 0)
	{
		CreateThread(0, 0, gaihr_WorkerThread, 0, 0, 0);
	}
	_gaihr_files[file_count] = file;
	file_count++;
	return 1;
}

GAIHR_API int
gaihr_RemoveFile(gaihr_file *file)
{
	for (;;)
	{
		gaihr_file **files = _gaihr_files;
		for (int i = 0; i < (sizeof(_gaihr_files) / sizeof(_gaihr_files[0])); i++)
		{
			gaihr_file *target_file = files[i];
			if (target_file == file)
			{
				CloseHandle(file->platform.event);
				CloseHandle(file->platform.mutex);
				files[i] = 0;
				return 1;
			}
		}
	}
	return 0;
}

#else

#error gai_hotreload.h: Error. This platform is not supported!

#endif

#endif

#define _GAI_HOTRELOAD_H
#endif