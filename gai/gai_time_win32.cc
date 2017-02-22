r32
gaiTimeGetTicks()
{
    static b32 initialized;
    if (!initialized)
    {
        QueryPerformanceFrequency(&__libgai_global_frequency);
        QueryPerformanceCounter(&__libgai_global_lasttime);
        initialized = true;
    }

    LARGE_INTEGER now, elapsed;
    QueryPerformanceCounter(&now);
    elapsed.QuadPart = now.QuadPart - __libgai_global_lasttime.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= __libgai_global_frequency.QuadPart;
    __libgai_global_lasttime = now;
    return elapsed.QuadPart;
}

r32
gaiTimeGetTicksSeconds()
{
    return gaiTimeGetTicks() / 1000000.f;
}

r32
gaiTimeGetTicksMillis()
{
    return gaiTimeGetTicks() / 1000.f;
}