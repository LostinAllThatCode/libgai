i32
gaiWindowCreate(gaiWindow *window, const char *title, i32 width, i32 height, i32 x = 0, i32 y = 0)
{
    GAI_ASSERT(window);

    Display *d = XOpenDisplay(0);
    if(!d) return -1;
    
    Window w = XCreateWindow(d, DefaultRootWindow(d), x, y, width, height, 0, CopyFromParent, CopyFromParent,
                             CopyFromParent, 0, 0);

    XMapWindow(d, w);
    XFlush(d);
    
    return 1;
}
