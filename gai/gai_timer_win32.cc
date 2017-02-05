void 
gaiTimerInit(gaiTimer *timer)
{
    static b32 initialized;
    if(!initialized)
    {
        QueryPerformanceFrequency(&timer->frequency);
        QueryPerformanceCounter(&timer->starttime);
        initialized = true;
    }
}

r32
gaiTimerGetTicks(gaiTimer *timer)
{
    LARGE_INTEGER now, elapsed;
    QueryPerformanceCounter(&now);
    elapsed.QuadPart = now.QuadPart - timer->starttime.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= timer->frequency.QuadPart;
    timer->starttime = now;
    return elapsed.QuadPart;
}

r32
gaiTimerGetTicksSeconds(gaiTimer *timer)
{
    return gaiTimerGetTicks(timer) / 1000000.f;
}

r32
gaiTimerGetTicksMillis(gaiTimer *timer)
{
    return gaiTimerGetTicks(timer) / 1000.f;
}