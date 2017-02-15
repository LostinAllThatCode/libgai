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

i32
gaiWindowRegister(HINSTANCE instance, const char *classname)
{
	GAI_ASSERT(instance);

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
	GAI_ASSERT(window);

	ZeroMemory(window, sizeof(gaiWindow));

	if (!gaiWindowRegister(instance, classname))
	{
		return 0;
	}

	HWND hWnd = CreateWindowA(classname, title, style, x, y, width, height, parent, menu, instance, lparam);
	if (!hWnd)
	{
		return 0;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) window);

	window->platform.classname = classname;
	window->platform.instance = instance;
	window->platform.hWnd     = hWnd;
	window->platform.ctx      = GetDC(hWnd);

	if (show)
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}

	return 1;
}

i32
gaiWindowCreate(gaiWindow *window, const char *title, i32 width, i32 height, i32 x, i32 y, const char *classname)
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	HINSTANCE instance = GetModuleHandle(0);
	return gaiWindowCreateEx(window, title, classname, dwStyle, x, y, width, height, 0, 0, instance, 0, 1);
}

void
gaiWindowShow(gaiWindow *window, u32 flag)
{
	ShowWindow(window->platform.hWnd, flag);
}

void
gaiWindowReplaceWndProc(gaiWindow *window, LRESULT (__stdcall * dwp)(HWND, UINT, WPARAM, LPARAM))
{
	GAI_ASSERT(window);

	SetWindowLongPtr(window->platform.hWnd, GWLP_WNDPROC, (LONG_PTR)dwp);
}

inline i32
gaiWindowUpdate(gaiWindow *window, i32 block = 1)
{
	GAI_ASSERT(window);
	/*
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
*/
	MSG msg;
	while ( (block ? GetMessage(&msg, 0, 0, 0) : PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		switch (msg.message)
		{
			case WM_QUIT: { gaiWindowDestroy(window); return 0; }
			#if 0
			case WM_MOUSEMOVE:   { window->input.x = LOWORD(msg.lParam); window->input.y = HIWORD(msg.lParam); } break;
			case WM_MOUSEWHEEL:  { window->input.dtwheel = GET_WHEEL_DELTA_WPARAM(msg.wParam); } break;
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
	GAI_ASSERT(window);
	SetWindowPos(window->platform.hWnd, 0, 0, 0, width, height, SWP_NOMOVE);
}

void
gaiWindowSetTitle(gaiWindow *window, const char *title)
{
	GAI_ASSERT(window);
	SetWindowText(window->platform.hWnd, title);
}

void
gaiWindowDestroy(gaiWindow* window)
{
	GAI_ASSERT(window);
	DestroyWindow(window->platform.hWnd);
	UnregisterClass(window->platform.classname,  window->platform.instance);
}

void
gaiWindowQuit(gaiWindow *window)
{
	GAI_ASSERT(window);
	gaiWindowDestroy(window);
	while (gaiWindowUpdate(window, 0));
}
