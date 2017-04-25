/**
 * @file gai_hotreload.h
 * 
 * @brief This api allows you to track any file changes happen to a file ( not really content wise changes, more filetime changes on save & copy triggers )
 * 
 * NOTE: ONLY WINDOWS IS SUPPORTED FOR NOW
 * 
 * This api allows you to track any file changes happen to a file ( not really content wise changes, more filetime changes on save & copy triggers ).
 * The tracking will take place on a seperate thread which will be started the first time you add a file via gaihr_Track(...) call.
 * You can specify diffrent flags on how the event and callback function will be handled. See the @@gaihr_flags definition.
 * 
 * 
 * You can use this api to hotreloading of dlls or textures or just plain textfiles.
 * 
 * Windows linker dependancies: user32.lib 
 * 
 * Do this:
 * 
 *     #define GAIHR_IMPLEMENTATION
 * 
 * before you include this file in *one* C++ file to create the implementation.
 * 
 * **IMPORTANT**\n
 * _All function prefixed with a underscore(_) are internally used functions._
 * _DO NOT use them if you are not 100% sure what they do._
 * 
 * ____ 
 * Example Code Snippet:
 * 
 * A file with the name "testfile.txt" has to be in the directory of the executable. 
 * After running the executable you have to change the files content and save it.
 * Or just replace it with another file with the same name.
 * 
 * @code{.cpp} 
 * #define GAIHR_IMPLEMENTATION
 * #include "gai_hotreload.h"
 * 
 * #include <stdio.h>
 * 
 * volatile int running = 1; // This will be changed by another thread!
 * 
 * void reloadFile(gaihr_file *file)
 * {
 *    // Do whatever you want to do, when this happens.
 *    printf("%s changed!\n", file->filename);
 *    running = 0;
 * }
 * 
 * int main(int argc, char **argv)
 * {
 *    gaihr_file MyFile = {};
 *    gaihr_Track(&MyFile, "testfile.txt", reloadFile, 0, gaihr_FlagsSkipInitialChange);
 *    while(running) {Sleep(125);}
 *    return 0;
 * }
 * @endcode
 * @author Andreas Gaida
 * @date 1 Sep 2011
 * @see https://github.com/LostinAllThatCode/libgai
 */

#ifndef _GAI_INCLUDE_HOTRELOAD_H

#define GAIHR_WAIT_INFINITE  -1		/**< Note: Currently the windows specified value for INFINITE. Will possibly be changed! */
#define GAIHR_THREAD_TIMEOUT 1000	/**< Thread sleeping time until it checks for updated files again */
#define GAIHR_FILE_LIMIT	 64		/**< Max files which will be processed by the worker thread. Note: Will be replaced by a linked list! */

#ifndef GAIHR_STATIC
	#define GAIHR_API extern 		/**< Define GAIHR_STATIC is you want to prefix all functions with the extern identifier. Otherwise the static identifier will be used by default. */
#else
	#define GAIHR_API static
#endif

struct gaihr_file;

#if _WIN32

#define WIN32_LEAN_AND_MEAN			/** windows platform WIN32_LEAN_AND_MEAN include macro definition **/
#include <windows.h>
#pragma comment( lib, "user32.lib" )


/**
 * @brief      Platform specific hotreload layer with mutex and event handles. Should not be used outside of this file!
 * 
 * This struct will vary on different platforms. TODO: Add support for multiple platforms!
 */
struct gaihr_platform
{
	HANDLE   mutex; /**< Handle to a windows mutex */
	HANDLE   event; /**< Handle to a windows event */
	FILETIME last_write_time; /**< Timestamp of last change */
};
#else

#error gai_hotreload.h: Error. This platform is not supported!

#endif

/**
 * @brief This flags specify how the event and callback function will be handled.
 */
enum gaihr_flags
{
	gaihr_FlagsNone      			= 0x0, /**< Nothing specified. */
	gaihr_FlagsDontHandleEvent  	= 0x1, /**< Indicates whether the thread will not call the callback function on event. You should call it yourself! */
	gaihr_FlagsDontResetEvent  		= 0x2, /**< Indicates whether the marked event will not be resetted after calling the callback function. You have to do reset it yourself! */
	gaihr_FlagsSkipInitialChange 	= 0x4, /**< Indicates whether the initial file change will not result in an event. */
};

/**
 * Callback function typedef which will be call when an event happens.
 */
typedef void gaihr_callback(gaihr_file *hotreloadable);

/**
 * @brief      Hotreloadable file structure
 * 
 * asdasdasdada asdas
 */
struct gaihr_file
{
	void          	*userdata;	/**< This pointer is passed by the user via gaihr_Track function. */
	gaihr_callback	*callback;	/**< Callback to the function when an event happens. {@link gaihr_Track} */
	gaihr_platform	platform;  	/**< Platform specfic structure for semaphore or mutex handles. */
	const char    	*filename;  /**< File to check for changes */
	gaihr_flags   	flags;		/**< Flags to specify event handling and callback behaviour. */
};

#if 0
// this could be an easy dynamic linked list way of tracking all files
struct gaihr_file_list
{
	gaihr_file *file;
	gaihr_file_list *next;
} __filelist;
#endif

static gaihr_file *_gaihr_files[GAIHR_FILE_LIMIT]; // TODO: Remove this and replace it with a linked list

/**
 * @brief      Adds a file to a worker thread, which internally tracks if the given file was modified. If so it will fire an event
 *             which will result in a function call to the callback function or handled differently depending on the specified flags.
 *
 * @param      file      Handle to a gaihr_file instance.
 * @param      filename  Filename of the file to track.
 * @param      callback  (optional) Callback function pointer when event happens.
 * @param      userdata  (optional) Pointer to user specific data.
 * @param      flags     (optional) Flags to handle file changes and callbacks.
 * @return     1 on success, 0 on failure.
 */
GAIHR_API unsigned int 	gaihr_Track 			(gaihr_file *file, const char *filename, gaihr_callback *callback = 0, void *userdata = 0, gaihr_flags flags = gaihr_FlagsNone);

/**
 * @brief      Removes a file from the worker thread.
 * @param      file  Handle to a gaihr_file instance.
 * @return     1 on success, 0 on failure.
 */
GAIHR_API unsigned int 	gaihr_Untrack 		(gaihr_file *file);

/**
 * @brief      Grabs a mutex for the specified gaihr_file handle and waits for an event. If a callback function is specifed and an event happend the callback function will be called.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void			gaihr_WaitForEvent 		(gaihr_file *file);

/**
 * @brief      Opens a mutex to access the data safely in a multi-threaded way.
 * @param      file     Handle to a gaihr_file instance.
 * @param      timeout  Time to grab the mutex. (Can take longer if the mutex is used by another thread and was not released yet!)
 * @return     1 if the mutex was succesfully opened, 0 on failure (mutex is probably used by another thread).
 */
GAIHR_API unsigned int 	gaihr_BeginTicketMutex  (gaihr_file *file, int timeout = GAIHR_WAIT_INFINITE);

/**
 * @brief      Releases the previously opened mutex to allow other thread to use this mutex.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void 	gaihr_EndTicketMutex	(gaihr_file *file);

/**
 * @brief      Signals a file change to the user.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void  		_gaihr_CreateEvent 		(gaihr_file *file);

/**
 * @brief      Resets the previously set signal.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void  		_gaihr_ResetEvent 		(gaihr_file *file);

#ifdef GAIHR_IMPLEMENTATION

#if _WIN32

inline GAIHR_API unsigned int
gaihr_BeginTicketMutex(gaihr_file *file, int timeout)
{
	unsigned int result = (WaitForSingleObject(file->platform.mutex, timeout) == WAIT_OBJECT_0);
	return result;
}

inline GAIHR_API void
gaihr_EndTicketMutex(gaihr_file *file)
{
	ReleaseMutex(file->platform.mutex);
}

inline GAIHR_API void
_gaihr_CreateEvent(gaihr_file *file)
{
	if ( file->platform.event != 0) CloseHandle(file->platform.event);
	file->platform.event = CreateEvent(0, 1, 1, 0);
}

inline GAIHR_API void
_gaihr_ResetEvent(gaihr_file *file)
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
		if (!(file->flags & gaihr_FlagsDontResetEvent)) _gaihr_ResetEvent(file);
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
_gaihr_WorkerThread(void *data)
{
	for (;;)
	{
		gaihr_file **files = _gaihr_files;
		for (int i = 0; i < (sizeof(_gaihr_files) / sizeof(_gaihr_files[0])); i++)
		{
			gaihr_file *file = files[i];
			if (file)
			{
				if ( _gaihr_CheckFileChanged(file) )
				{
					_gaihr_CreateEvent(file);
					if (!(file->flags & gaihr_FlagsDontHandleEvent))
					{
						gaihr_WaitForEvent(file);
					}
				}
			}
		}
		Sleep(GAIHR_THREAD_TIMEOUT);
	}
	return 1;
}

GAIHR_API unsigned int
gaihr_Track(gaihr_file *file, const char *filename, gaihr_callback *callback, void *userdata, gaihr_flags flags)
{
	static int file_count;
	if (!file || file_count >= GAIHR_FILE_LIMIT)
	{
		//GAIHR_ASSERT()
		return 0;
	}

	file->filename                 = filename;
	file->callback                 = callback;
	file->userdata                 = userdata;
	file->flags                    = flags;
	file->platform.mutex           = CreateMutex(0, 0, 0);
	file->platform.event           = 0;
	file->platform.last_write_time = {0};

	if (file_count == 0)
	{
		CreateThread(0, 0, _gaihr_WorkerThread, 0, 0, 0);
		#if 0
		__filelist.file = file;
		__filelist.next = 0;
		#endif
	}
	else
	{
		#if 0
		// linked list implementation
		gaihr_file_list *nextfree = &__filelist;
		while (nextfree->next != 0) nextfree = nextfree->next;

		nextfree->next = (gaihr_file_list *) malloc( sizeof(gaihr_file_list) );
		if (nextfree->next)
		{
			nextfree->next->file = file;
			nextfree->next->next = 0;
		}
		#endif
	}
	_gaihr_files[file_count] = file;

	file_count++;
	return 1;
}

GAIHR_API unsigned int
gaihr_Untrack(gaihr_file *file)
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

#define _GAI_INCLUDE_HOTRELOAD_H 
#endif