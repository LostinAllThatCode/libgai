/*
==========================================================================================
$Name: gai_workqueue.h$
$Description:
Simple quick and easy workqueue implmentation. NOTE: Only the thread which created the
queue should add work entries to the queue. It's not safe to push new entries from a
thread's callback function.
$
$Creator: Andreas Gaida$
$Copyright: $
$Example:

#define GAI_WORKQUEUE_IMPLEMENTATION
#include "gai_workqueue.h"

#include <stdio.h>
#include <time.h>

GAI_WORKQUEUE_CALLBACK(worker)
{
    int *delay = (int *) data;
    Sleep(*delay);
    printf("Thread [%i] finished work after %i ms \n", GetCurrentThreadId(), *delay);
}
// you can use this function declaration as well. It's your choice
// your_worker_function(gaiWorkQueue *queue, void *data) { ... }

int
main(int argc, char** argv)
{
    srand(time(0));
    int timings[] { rand()%1000, rand()%1000, rand()%1000, rand()%1000, rand()%1000, rand()%1000};

    gaiWorkQueue queue;
    if(gaiWorkQueueCreate(&queue, 6)) {
        gaiWorkQueueAddEntry(&queue, worker, timings);
        gaiWorkQueueAddEntry(&queue, worker, timings+1);
        gaiWorkQueueAddEntry(&queue, worker, timings+2);
        gaiWorkQueueAddEntry(&queue, worker, timings+3);
        gaiWorkQueueAddEntry(&queue, worker, timings+4);
        gaiWorkQueueAddEntry(&queue, worker, timings+5);
        gaiWorkQueueWaitForCompletion(&queue);
    } else {
        printf("Error creating workqueue\n");
    }
    return 0;
}

$
==========================================================================================
*/
#ifndef _GAI_WORKQUEUE_H_

#include "gai_types.h"
#include "gai_utils.h"

#ifndef GAI_WORKQUEUE_MAX
#define GAI_WORKQUEUE_MAX 12
#endif

#ifndef GAI_WORKQUEUE_ENTRY_MAX
#define GAI_WORKQUEUE_ENTRY_MAX 256
#endif

struct gaiWorkQueuePlatform;
#if _WIN32
#include <windows.h>
typedef struct gaiWorkQueuePlatform
{
    HANDLE semaphore_handle;
} gaiWorkQueuePlatform;
#elif __linux__
typedef struct gaiWorkQueuePlatform
{
    void *not_implemented_yet;
} gaiWorkQueuePlatform;
#elif __APPLE__
typedef struct gaiWorkQueuePlatform
{
    void *not_implemented_yet;
} gaiWorkQueuePlatform;
#else
#endif

struct gaiWorkQueue;
#define GAI_WORKQUEUE_CALLBACK(name) void name(gaiWorkQueue *queue, void *data)
typedef GAI_WORKQUEUE_CALLBACK(gaiWorkQueueCallback);

typedef struct
{
    gaiWorkQueueCallback *callback;
    void *data;
} gaiWorkQueueEntry;

struct gaiWorkQueuePlatform; // This structure will be defined differently on each platform
typedef struct gaiWorkQueue
{
    u32 volatile         completion_goal;
    u32 volatile         completion_count;
    u32 volatile         next_entry_to_read;
    u32 volatile         next_entry_to_write;
    gaiWorkQueueEntry    work_entries[GAI_WORKQUEUE_ENTRY_MAX];

    gaiWorkQueuePlatform platform;
} gaiWorkQueue;

// $FUNCTIONS$

#ifdef __cplusplus
extern "C" {
#endif

GAI_DEF int  gaiWorkQueueCreate(gaiWorkQueue * queue, u32 thread_count);
GAI_DEF void gaiWorkQueueCleanup(gaiWorkQueue * queue);
GAI_DEF void gaiWorkQueueAddEntry(gaiWorkQueue * queue, gaiWorkQueueCallback * callback, void *data);
GAI_DEF void gaiWorkQueueWaitForCompletion(gaiWorkQueue *queue);
GAI_DEF u32  gaiWorkQueueGetCurrentThreadId();

#ifdef __cplusplus
}
#endif

// $FUNCTIONS$

// $PLATFORM LAYER IMPLEMENTATION$

#ifdef GAI_WORKQUEUE_IMPLEMENTATION
#if _WIN32
#include "gai_workqueue_win32.cc"
#elif __linux__
#elif __APPLE__
#endif
#endif //GAI_WORKQUEUE_IMPLEMENTATION

// $PLATFORM LAYER IMPLEMENTATION$
#define _GAI_WORKQUEUE_H_
#endif
