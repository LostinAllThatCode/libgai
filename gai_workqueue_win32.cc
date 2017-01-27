b32
gaiWorkQueueNext(gaiWorkQueue *queue)
{
    b32 do_sleep = false;
    
    u32 original_next_entry_to_read = queue->next_entry_to_read;
    u32 new_next_entry_to_read = (original_next_entry_to_read + 1) % gai_length(queue->work_entries);
    if(original_next_entry_to_read != queue->next_entry_to_write) {
        u32 index = InterlockedCompareExchange((LONG volatile *) &queue->next_entry_to_read,
                                               new_next_entry_to_read, original_next_entry_to_read);
        if(index == original_next_entry_to_read) {        
            gaiWorkQueueEntry entry = queue->work_entries[index];
            entry.callback(queue, entry.data);
            InterlockedIncrement((LONG volatile *)&queue->completion_count);
        }
    }
    else {
        do_sleep = true;
    }

    return(do_sleep);
}

DWORD WINAPI
gaiWorkQueueThreadProc(LPVOID lpParameter)
{
    gaiWorkQueue *queue = (gaiWorkQueue *) lpParameter;
    for(;;) {
        if(gaiWorkQueueNext(queue)) WaitForSingleObjectEx(queue->platform.semaphore_handle, INFINITE, FALSE);
    }
}

GAI_DEF void
gaiWorkQueueWaitForCompletion(gaiWorkQueue *queue)
{
    while(queue->completion_goal != queue->completion_count)
    {
        gaiWorkQueueNext(queue);
    }

    queue->completion_goal = 0;
    queue->completion_count = 0;
}

GAI_DEF int
gaiWorkQueueCreate(gaiWorkQueue *queue, u32 thread_count)
{
    ZeroMemory(queue, sizeof(gaiWorkQueue));
    GAI_ASSERT(thread_count <= GAI_WORKQUEUE_MAX);
    
    if(thread_count > GAI_WORKQUEUE_MAX) return 0;

    queue->completion_goal     = 0;
    queue->completion_count    = 0;

    queue->next_entry_to_write = 0;
    queue->next_entry_to_read  = 0;

    u32 initial_count = 0;
    queue->platform.semaphore_handle = CreateSemaphoreEx(0, initial_count, thread_count, 0, 0, SEMAPHORE_ALL_ACCESS);
    
    gai_fiz(thread_count) {
        DWORD thread_id;
        HANDLE thread_handle = CreateThread(0, 0, gaiWorkQueueThreadProc, queue, 0, &thread_id);
        CloseHandle(thread_handle);
    }
    return 1;
}

GAI_DEF void
gaiWorkQueueAddEntry(gaiWorkQueue *queue, gaiWorkQueueCallback *callback, void *data)
{
    u32 original_next_entry_to_write = queue->next_entry_to_write;
    u32 new_next_entry_to_write = (queue->next_entry_to_write + 1) % gai_length(queue->work_entries);
    GAI_ASSERT(new_next_entry_to_write != queue->next_entry_to_read);
    
    gaiWorkQueueEntry *entry = queue->work_entries + queue->next_entry_to_write;
    entry->callback = callback;
    entry->data = data;
    InterlockedIncrement((LONG volatile *)&queue->completion_goal);
    _WriteBarrier();
    InterlockedCompareExchange((LONG volatile *) &queue->next_entry_to_write, new_next_entry_to_write, original_next_entry_to_write);
    ReleaseSemaphore(queue->platform.semaphore_handle, 1, 0);
}

GAI_DEF u32
gaiWorkQueueGetCurrentThreadId()
{
    return (u32) GetCurrentThreadId();
}
