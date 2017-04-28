/**
 * @file gai_hotreload.h
 *
 * @brief Tracks when a file change happened to a file ( For windows it only check for filetime write changes. So it will trigger an event when you save the file or replace it with another )
 *
 * @attention __ONLY WINDOWS IS SUPPORTED__ \n __Linker dependencies: user32.lib__
 *
 * @section gaihr_intro Introduction
 * The tracking will take place on a seperate thread which will be started the first time you add a file via gaihr_Track(...) call.
 * You can specify different flags on how the event and callback function will be handled. See the {@link gaihr_flags} definition.
 *
 * You can use this api to do hotreloading of dll's or texture's or just plain textfile's.
 *
 * Do this:
 *
 *     #define GAIHR_IMPLEMENTATION
 *
 * before you include this file in *one* C++ file to create the implementation.
 *
 * @note
 * __All function prefixed with a underscore(_) are internally used functions.__
 * __DO NOT use them if you are not 100% sure what they do.__
 *
 * @subsection gaihr_code_example Example Code
 *
 * A file with the name "testfile.txt" has to be in the directory of the executable.
 * After running the executable you have to change the file's content and save it
 * or replace it with another file (same filename).
 *
 * @include hotreload\win32\main.cpp
 *
 * @author Andreas Gaida
 * @date   25.04.2017
 * @see    https://github.com/LostinAllThatCode/libgai
 *
 * @example hotreload\win32\main.cpp __Example: win32 gai_hotreload.h__*
 */

#ifndef _GAI_INCLUDE_HOTRELOAD_H

#if _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#define GAIHR_WAIT_INFINITE  -1		/**< Note: Currently the windows specified value for INFINITE. Will possibly be changed! */
#define GAIHR_THREAD_TIMEOUT 1000	/**< Thread sleeping time until it checks for updated files again */
#define GAIHR_FILE_LIMIT	 64		/**< Max files which will be processed by the worker thread. Note: Will be replaced by a linked list! */

#ifndef GAIHR_STATIC
	/**
	* @brief      Define **GAIHR_STATIC** if you want to make functions **static** instead of **extern**
	*/
	#define GAIHR_API extern
#else
	#define GAIHR_API static
#endif

#ifndef GAIHR_ASSERT
	#include <assert.h>
	/**
	* @brief      Define **GAIHR_ASSERT** before including this file to **disable** assertions
	* @note 	  Uses assert() from the c-standard library by default <assert.h>
	* @param      cond  Assertion condition
	*/
	#define GAIHR_ASSERT(cond) assert(cond)
#endif

struct gaihr_file;

/**
 * @brief Callback function macro which will generate a function as specified by @ref gaihr_callback
 *
 * @b Usage @b example:
 * @code{.cpp}
 * GAIHR_CALLBACK(myCallbackFunction)
 * {
 * 		// Do whatever you want here
 * 		printf("%s\n", file->filename);
 * }
 * @endcode
 *
 * The above example will actually generate a function which looks like this:
 * @code{.cpp}
 * void myCallbackFunction(gaihr_file *file)
 * {
 * 		// Do whatever you want here
 * 		printf("%s\n", file->filename);
 * }
 * @endcode
 *
 * See the @ref gaihr_code_example section for a simple demonstration.
 *
 */
#define GAIHR_CALLBACK(name) void name(gaihr_file *file)
/** @brief Callback function which will be called when an event was fired
 * See @ref GAIHR_CALLBACK
 */
typedef void gaihr_callback(gaihr_file *file);

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

union gaihr_filetime
{
	struct { long low, high; } parts; /**< This is here to easily cast windows FILETIME structure to this one. */
	unsigned long long time;
};

union gaihr_platform
{
	#if _WIN32
	struct gaihr_platform_win32
	{
		HANDLE 	mutex;
		HANDLE 	event;
	} win32;
	#endif
	#if __linux__
	struct gaihr_platform_linux
	{
		void* 	mutex;
		void* 	event;
	} linux;
	#endif
};

/**
 * @todo       Think about a way of easily support other platforms.
 */
struct gaihr_file
{
	void          	*userdata;			/**< User passed data. */
	gaihr_callback	*callback;			/**< Callback to the function when an event happens. See @ref GAIHR_CALLBACK */
	gaihr_platform  platform; 			/**< Platform specfic structure for semaphore or mutex handles. */
	gaihr_filetime 	last_write_time; 	/**< Filetime structure. */
	const char    	*filename;  		/**< File to check for changes. */
	gaihr_flags   	flags;				/**< Flags to specify event handling and callback behaviour. */
};

#if 0
// this could be an easy dynamic linked list way of tracking all files
struct gaihr_file_list
{
	gaihr_file *file;
	gaihr_file_list *next;
} __filelist;
#else
static gaihr_file *_gaihr_files[GAIHR_FILE_LIMIT]; // TODO: Remove this and replace it with a linked list
#endif

/**
 * @brief      Adds a file to a worker thread, which internally tracks if the given file was modified. If so it will fire an event
 *             which will result in a function call to the callback function or handled differently depending on the specified flags.
 *
 * @param      file      A pointer to a gaihr_file structure.
 * @param      filename  Filename of the file to track (relative or full).
 * @param      callback  (optional) A pointer to he callback function. See @ref GAIHR_CALLBACK
 * @param      userdata  (optional) A pointer to userdefined memory.
 * @param      flags     (optional) Flags to handle events and callbacks. See @ref gaihr_flags
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Success
 * 0 			| Failure
 */
GAIHR_API unsigned int 		gaihr_Track 			(gaihr_file *file, const char *filename, gaihr_callback *callback = 0, void *userdata = 0, gaihr_flags flags = gaihr_FlagsNone);
/**
 * @brief      Removes a file from the worker thread.
 * @param      file  Handle to a gaihr_file instance.
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Success
 * 0 			| Failure
 */
GAIHR_API unsigned int 		gaihr_Untrack 			(gaihr_file *file);
/**
 * @brief      Grabs a mutex for the specified gaihr_file handle and waits for an event. If a callback function is specifed and an event happend the callback function will be called.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void				gaihr_WaitForEvent 		(gaihr_file *file);
/**
 * @brief      Opens a mutex to access the data safely in a multi-threaded way.
 * @param      file     Handle to a gaihr_file instance.
 * @param      timeout  Time to grab the mutex. (Can take longer if the mutex is used by another thread and was not released yet!)
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * 1 			| Mutex was succesfully opened
 * 0 			| Failure (mutex is probably used by another thread)
 */
GAIHR_API unsigned int 		_gaihr_BeginTicketMutex  (gaihr_file *file, int timeout = GAIHR_WAIT_INFINITE);
/**
 * @brief      Releases the previously opened mutex to allow other thread to use this mutex.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void 				_gaihr_EndTicketMutex	(gaihr_file *file);
/**
 * @brief      Signals a file change to the user.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void  			_gaihr_SetEvent 		(gaihr_file *file);
/**
 * @brief      Resets the previously set signal.
 * @param      file  Handle to a gaihr_file instance.
 */
GAIHR_API void  			_gaihr_ResetEvent 		(gaihr_file *file);
/**
 * @brief      Compare two gaihr_filetimes to each other.
 *
 * @param      a     A pointer to a gaihr_filetime structure
 * @param      b     A pointer to a gaihr_filetime structure
 *
 * @return
 * Return Code 	| Description
 * :----		|:-----------
 * -1 			| a is higher than b
 * 0 			| a equals b
 * 1 			| b is higher than a
 */
GAIHR_API int 				_gaihr_CompareFileTime	(gaihr_filetime *a, gaihr_filetime *b);
/**
 * @brief      Loops through all files to track
 *
 * @param      files  A pointer to an array of gaihr_file structures
 */
GAIHR_API void 				_gaihr_DoWork			(gaihr_file **files);

#ifdef GAIHR_IMPLEMENTATION

#if _WIN32
#pragma comment( lib, "user32.lib" )

GAIHR_API void*
_gaihr_CreateMutex()
{
	return CreateMutex(0, 0, 0);
}

GAIHR_API void
_gaihr_CloseMutex(void *mutex)
{
	CloseHandle(mutex);
}

inline GAIHR_API unsigned int
_gaihr_BeginTicketMutex(gaihr_file *file, int timeout)
{
	unsigned int result = (WaitForSingleObject(file->platform.win32.mutex, timeout) == WAIT_OBJECT_0);
	return result;
}

inline GAIHR_API void
_gaihr_EndTicketMutex(gaihr_file *file)
{
	ReleaseMutex(file->platform.win32.mutex);
}

GAIHR_API void*
_gaihr_CreateEvent(unsigned int initial_state)
{
	return CreateEvent(0, 1, initial_state, 0);
}

GAIHR_API void
_gaihr_CloseEvent(void *event)
{
	CloseHandle(event);
}

inline GAIHR_API void
_gaihr_SetEvent(gaihr_file *file)
{
	SetEvent(file->platform.win32.event);
}

inline GAIHR_API void
_gaihr_ResetEvent(gaihr_file *file)
{
	ResetEvent(file->platform.win32.event);
}

inline GAIHR_API unsigned int
_gaihr_WaitEvent(gaihr_file *file, int timeout)
{
	return (WaitForSingleObject(file->platform.win32.event, timeout) == WAIT_OBJECT_0);
}

inline GAIHR_API unsigned int
_gaihr_CheckFileChanged(gaihr_file *file)
{
	unsigned int result = 0;
	WIN32_FILE_ATTRIBUTE_DATA file_info_now = {};

	int file_attrib_result = GetFileAttributesExA(file->filename, GetFileExInfoStandard, &file_info_now);
	if (file_attrib_result)
	{
		int has_changed = (_gaihr_CompareFileTime(&file->last_write_time, (gaihr_filetime*) &file_info_now.ftLastWriteTime) == 1);
		if (has_changed)
		{
			if (!((file->flags & gaihr_FlagsSkipInitialChange) && (file->last_write_time.time == 0))) result = 1;
			file->last_write_time.parts.low 	= file_info_now.ftLastWriteTime.dwLowDateTime;
			file->last_write_time.parts.high 	= file_info_now.ftLastWriteTime.dwHighDateTime;
		}
	}
	return result;
}

DWORD WINAPI
_gaihr_WorkerThread(void *data)
{
	for (;;)
	{
		_gaihr_DoWork(_gaihr_files);
		Sleep(GAIHR_THREAD_TIMEOUT);
	}
	return 1;
}

GAIHR_API int
_gaihr_CreateThread()
{
	return (CreateThread(0, 0, _gaihr_WorkerThread, 0, 0, 0) != 0);
}

#else
#error gai_hotreload.h: Error. This platform is not supported!
#endif // PLATFORM IMPLEMENTATIONS

inline GAIHR_API void
_gaihr_DoWork(gaihr_file **files)
{
	if (files)
	{
		for (int i = 0; i < (sizeof(_gaihr_files) / sizeof(_gaihr_files[0])); i++)
		{
			gaihr_file *file = files[i];
			if (file)
			{
				if ( _gaihr_CheckFileChanged(file) )
				{
					_gaihr_SetEvent(file);
					if (!(file->flags & gaihr_FlagsDontHandleEvent)) gaihr_WaitForEvent(file);
				}
			}
		}
	}
}

inline GAIHR_API void
gaihr_WaitForEvent(gaihr_file * file)
{
	if (_gaihr_WaitEvent(file, 0))
	{
		_gaihr_BeginTicketMutex(file);
		if (file->callback) file->callback(file);
		if (!(file->flags & gaihr_FlagsDontResetEvent)) _gaihr_ResetEvent(file);
		_gaihr_EndTicketMutex(file);
	}
}

inline GAIHR_API int
_gaihr_CompareFileTime(gaihr_filetime * a, gaihr_filetime * b)
{
	int result = (((b->time - a->time)) > 0) - (((b->time - a->time) < 0));
	return result;
}

GAIHR_API unsigned int
gaihr_Track(gaihr_file * file, const char *filename, gaihr_callback * callback, void *userdata, gaihr_flags flags)
{
	static int file_count;
	GAIHR_ASSERT(file && file_count < GAIHR_FILE_LIMIT);

	file->filename                 = filename;
	file->callback                 = callback;
	file->userdata                 = userdata;
	file->flags                    = flags;
	file->platform.win32.mutex     = _gaihr_CreateMutex();
	file->platform.win32.event     = _gaihr_CreateEvent(0);
	file->last_write_time 		   = {0};

	_gaihr_files[file_count] = file;
	if (file_count == 0)
	{
		_gaihr_CreateThread();
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
	file_count++;
	return 1;
}

GAIHR_API unsigned int
gaihr_Untrack(gaihr_file * file)
{
	for (;;)
	{
		gaihr_file **files = _gaihr_files;
		for (int i = 0; i < (sizeof(_gaihr_files) / sizeof(_gaihr_files[0])); i++)
		{
			gaihr_file *target_file = files[i];
			if (target_file == file)
			{
				_gaihr_CloseEvent(file->platform.win32.event);
				_gaihr_CloseMutex(file->platform.win32.mutex);
				files[i] = 0;
				return 1;
			}
		}
	}
	return 0;
}
#endif	// GAIHR_IMPLEMENTATION



#define _GAI_INCLUDE_HOTRELOAD_H
#endif