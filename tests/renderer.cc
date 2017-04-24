#include "renderer_shared.h"

#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#define GAIRB_DEBUG_VISUALGRADIENT_CUBE 1
#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#define external extern "C" __declspec (dllexport)
#include <stdio.h>
r32 time, time2;

v3 cam_along_x = V3(5.f, 5.f, 0.f);
v3 cam_along_z = V3(0.f, 10.f, 18.f); 

float func1(float t, int i, int j) { return cosf(t * (i - j) * .125f) * .5f; }
float func2(float t, int i, int j) { return sinf(cosf(t) * (i - j) * .125f) * .5f; }
float func3(float t, int i, int j) { return (cosf(time + j * i)) * .125f * .5f; }

typedef float (*calc)(float t, int i, int j);
calc ptr = func1;

int rotX, rotY;
int a;

inline v2 GetTextureDim(loaded_bitmap *b)
{
	v2 result = V2(b->width,b->height);
	return result;
}

external update_and_render_(UpdateAndRender)
{
	if (time > 5.f)	a = -1;
	if (time <= 0.f) a = 1;
	time += window->frametime.seconds * a;
	time2 += window->frametime.seconds;
	gairb_group g1 = gairb_BeginGroup(V2i(window->info.width, window->info.height), renderbuffer, 0);

	if (gaiMousePressed(window, 2))
	{
		if (ptr == func1) ptr = func2;
		else if (ptr == func2) ptr = func3;
		else if (ptr == func3) ptr = func1;
	}

	if (gaiMouseDown(window, 0))
	{
		rotY += (int)(window->input.dx % 360);
		rotX += (int)(window->input.dy % 20);
	}

	m4x4 modelview = Mat4x4(1.0f) * YRotation( GAIRB_DEG2RAD( rotY ));
	gairb_setup perspective = { PerspectiveProjection((float) window->info.width / (float) window->info.height, 60.f, 0.1f, 100.f) * CameraOrbit(cam_along_z, GAIRB_DEG2RAD(-15.f), GAIRB_DEG2RAD(0)) * modelview };
	gairb_PushSetup(&g1, &perspective);

	int cubes = 50;
	float radius = .5f;
	float height = .125f;

	gairb_PushCube(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0.f, 10.f, 0.f), 40.f, 20.f);
	for (int i = 1; i < cubes; i++)
	{
		for (int j = 1; j < cubes; j++)
		{
			gairb_PushCube(&g1, V4(1.f, 1.f, 1.f, .1f), V3((float) (cubes / 2 - i) * radius, 2.0f + ptr(time, i, j), (float) (cubes / 2 - j) * radius), radius, height);
		}
	}

	v2 dim = GetTextureDim(&platform->assets[1]);
	gairb_PushRect(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0, 10.f, -19.f), dim.x*.01f, dim.y *.01f, gairb_AlignToCenter, 1);

	gairb_setup ortho = { OrthographicProjection((r32)window->info.width, (r32)window->info.height, 1, -1) };
	gairb_PushSetup(&g1, &ortho);

	gairb_PushRect(&g1, (gaixw_IsVSYNC(window) ? V4(.0f, 1.f, 0.f, 1.f) : V4(1.f, 0.0f, 0.f, 1.f)), V3(5.f, 10.f, 0.f), 10.f, 10.f, 0);
	gairb_PushRect(&g1, V4(1.f, 0.f, 0.f, 1.f), V3i(5, 5, 0), 100, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4(0.f, 1.f, 0.f, 1.0f), V3i(5, 5, 0), ((float)(renderbuffer->vertex_count + 4) / (float)renderbuffer->vertex_max) * 100.f, 2, gairb_AlignToTopLeft);

	gairb_PushRect(&g1, &platform->assets[1], V4(1.f), V3(5.f, 50.f, 0.f), dim.x*.125f, dim.y *.125f, gairb_AlignToTopLeft);

	gairb_EndGroup(&g1);
}
