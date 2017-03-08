LRESULT CALLBACK
gaiWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	gaiWindow *wnd = (gaiWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (wnd)
	{
		switch (uMsg)
		{
			case WM_SIZE:
			{
				wnd->width = LOWORD(lParam);
				wnd->height = HIWORD(lParam);
			} break;
			case WM_MOUSEMOVE:
			{
				//i32 x = ((i16) lParam), y = ((i16)((lParam >> 16) & 0xFFFF));
			} break;
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void
gaiWindowToggleFullscreen(gaiWindow *window)
{
	HWND hWnd = window->platform.hWnd;
	// NOTE(casey): This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLong(hWnd, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
		if (GetWindowPlacement(hWnd, &window->platform.position) &&
		        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLong(hWnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
			             MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
			             MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			             MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(hWnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &window->platform.position);
		SetWindowPos(hWnd, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

i32
gaiWindowRegister(HINSTANCE instance, const char *classname)
{
	gai_assert(instance);

	WNDCLASSEXA wnd_class   = {};
	wnd_class.cbSize        = sizeof(wnd_class);
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = sizeof(gaiWindow*);
	wnd_class.lpfnWndProc   = gaiWindowProc;
	wnd_class.hInstance     = instance;
	wnd_class.lpszClassName = classname;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);

	return RegisterClassExA(&wnd_class);
}


i32
gaiWindowCreateEx(gaiWindow* window, const char *title, const char *classname,
                  DWORD style, i32 x, i32 y, i32 width, i32 height, HWND parent,
                  HMENU menu, HINSTANCE instance, void *lparam, b32 show)
{
	gai_assert(window);

	ZeroMemory(window, sizeof(gaiWindow));

	if (!gaiWindowRegister(instance, classname))
	{
		return 0;
	}
	HWND hWnd = CreateWindowA(classname, title, style,
	                          (x == -1 ? CW_USEDEFAULT : x), (y == -1 ? CW_USEDEFAULT : y), (width == -1 ? CW_USEDEFAULT : width), (height == -1 ? CW_USEDEFAULT : height),
	                          parent, menu, instance, lparam);
	if (!hWnd)
	{
		return 0;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) window);

	window->platform.classname = classname;
	window->platform.instance = instance;
	window->platform.hWnd     = hWnd;
	window->platform.hDC      = GetDC(hWnd);

	if (show)
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	return 1;
}

i32
gaiWindowCreate(gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y, const char *classname, gaiWindowTypeEnum flags, i32 *ext, i32 count)
{

	switch (flags)
	{
		case gaiWindowTypeDefault:
		{
			if(!classname) classname = GAI_WINDOW_UUID;

			DWORD dwStyle      = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			HINSTANCE instance = GetModuleHandle(0);
			b32 fullscreen     = false;
			if (ext)
			{
				for (i32 i = 0; i < count; i += 2)
				{
					switch (ext[i])
					{
						case gaiWindowFlagsStyle:      { dwStyle    = ext[i + 1]; } break;
						case gaiWindowFlagsFullscreen: { fullscreen = ext[i + 1]; } break;
					}
				}
			}
			i32 result = gaiWindowCreateEx(window, title, classname, dwStyle, x, y, width, height, 0, 0, instance, 0, 0);
			if (result && fullscreen) gaiWindowToggleFullscreen(window);
			return result;
		} break;
		case gaiWindowTypeOpenGL:
		{
			/*
			if (defined_bits == 1)
			{
				result =  gaiOpenGLCreateContext(window, title, classname, width, height, x, y, major, minor, vsync, msaa, debug, color_bits, depth_bits, stencil_bits);
			}
			*/
			return gaiOpenGLCreateContext(window, title, classname, width, height, x, y, ext, count);
		} break;
		case gaiWindowTypeDirectX:
		{
			gai_printf("directx is currently not supported by this api\n");
			return 0;
		} break;
	}
	return 0;
}

i32
gaiWindowCreate2(gaiWindow *window, gaiWindowTypeEnum flags, i32 *ext, i32 count)
{
	return gaiWindowCreate(window, 0, -1, -1, -1, -1, 0, flags, ext, count);
}

void
gaiWindowShow(gaiWindow *window, u32 flag)
{
	ShowWindow(window->platform.hWnd, flag);
}

void
gaiWindowReplaceWndProc(gaiWindow *window, LRESULT (__stdcall * dwp)(HWND, UINT, WPARAM, LPARAM))
{
	gai_assert(window);

	SetWindowLongPtr(window->platform.hWnd, GWLP_WNDPROC, (LONG_PTR)dwp);
}

inline i32
gaiWindowUpdate(gaiWindow *window, i32 block = 1)
{
	gai_assert(window);
	window->dt = gaiTimeGetTicksSeconds();

	POINT mpos;
	GetCursorPos(&mpos);
	window->input.dtx     = mpos.x - window->input.scrx;
	window->input.dty     = mpos.y - window->input.scry;
	window->input.scrx    = mpos.x;
	window->input.scry    = mpos.y;
	window->input.dtwheel = 0;

	for (i32 i = 0; i < 256; i++)
	{
		gaiKeyState *state = window->input.keys + i;
		if (state->ended_down)
		{
			state->count = 0;
			state->ended_down = false;
		}
	}
	for (i32 i = 0; i < 3; i++)
	{
		gaiKeyState *state = window->input.buttons + i;
		if (state->ended_down)
		{
			state->count = 0;
			state->ended_down = false;
		}
	}

	MSG msg;
	while ( (block ? GetMessage(&msg, 0, 0, 0) : PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		switch (msg.message)
		{
			case WM_QUIT:
			{
				gaiWindowDestroy(window);
				if (window->flags == gaiWindowTypeOpenGL) gaiOpenGLDestroyContext();
				return 0;

			}
			#if 1
			case WM_MOUSEMOVE:   { window->input.x = LOWORD(msg.lParam); window->input.y = HIWORD(msg.lParam); } break;
			case WM_MOUSEWHEEL:  { window->input.dtwheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);} break;
			case WM_MBUTTONUP:   { window->input.buttons[2].ended_down = true; } break;
			case WM_RBUTTONUP:   { window->input.buttons[1].ended_down = true; } break;
			case WM_LBUTTONUP:   { window->input.buttons[0].ended_down = true; } break;
			case WM_MBUTTONDOWN: { window->input.buttons[2].count++; } break;
			case WM_RBUTTONDOWN: { window->input.buttons[1].count++; } break;
			case WM_LBUTTONDOWN: { window->input.buttons[0].count++; } break;
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				u8 key = (u8) msg.wParam;
				gaiKeyState *state = window->input.keys + key;
				if (msg.message == WM_KEYDOWN)
				{
					state->count++;
					state->ended_down = false;
				}
				else if (msg.message == WM_KEYUP)
				{
					state->ended_down = true;
				}
			} break;
			#endif
		}
	}
	return 1;
}

void
gaiWindowSetSize(gaiWindow *window, i32 width, i32 height)
{
	gai_assert(window);
	SetWindowPos(window->platform.hWnd, 0, 0, 0, width, height, SWP_NOMOVE);
}

void
gaiWindowSetTitle(gaiWindow *window, const char *title)
{
	gai_assert(window);
	SetWindowText(window->platform.hWnd, title);
}

void
gaiWindowDestroy(gaiWindow* window)
{
	gai_assert(window);
	DestroyWindow(window->platform.hWnd);
	UnregisterClass(window->platform.classname,  window->platform.instance);
}

void
gaiWindowQuit(gaiWindow *window)
{
	gai_assert(window);
	gaiWindowDestroy(window);
	while (gaiWindowUpdate(window, 0));
}
