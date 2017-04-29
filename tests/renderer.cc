#include "renderer_shared.h"

#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"

#define GAIRB_DEBUG_VISUALGRADIENT_CUBE 1
#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"

#define external extern "C" __declspec (dllexport)
#include <stdio.h>
static r32 time, time2;

#define DT_HISTORY_MAX 1024
static int dt_history[DT_HISTORY_MAX];
static int dt_pos;


v3 cam_along_x = V3(5.f, 5.f, 0.f);
v3 cam_along_z = V3(0.f, 10.f, 18.f); 

float func1(float t, int i, int j) { return cosf(t * (i - j) * .125f) * .5f; }
float func2(float t, int i, int j) { return sinf(cosf(t) * (i - j) * .125f) * .5f; }
float func3(float t, int i, int j) { return (cosf(time + j * i)) * .125f * .5f; }

typedef float (*calc)(float t, int i, int j);
calc ptr = func1;

int cubes = 30;
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
	time += window->dt.seconds * a;
	time2 += window->dt.seconds;
	gairb_group g1 = gairb_BeginGroup(V2i(window->info.width, window->info.height), renderbuffer, 0);

	if (gaixw_MousePressed(window, 2))
	{
		if (ptr == func1) ptr = func2;
		else if (ptr == func2) ptr = func3;
		else if (ptr == func3) ptr = func1;
	}

	if (gaixw_MouseDown(window, 0))
	{
		rotY += (int)(window->input.dx % 360);
		rotX += (int)(window->input.dy % 20);
	}

	m4x4 modelview = Mat4x4(1.0f) * YRotation( GAIRB_DEG2RAD( rotY ));
	gairb_setup perspective = { PerspectiveProjection((float) window->info.width / (float) window->info.height, 60.f, 0.1f, 100.f) * CameraOrbit(cam_along_z, GAIRB_DEG2RAD(-15.f), GAIRB_DEG2RAD(0)) * modelview };
	gairb_PushSetup(&g1, &perspective);

	static int adder = 1;
	if(cubes < 1 || cubes > 60) adder*=-1;
	//cubes+=adder;

	float radius = .5f;
	float height = .125f;
	v2 dim = GetTextureDim(&platform->assets[1]);

#if 1
	gairb_PushCube(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0.f, 10.f, 0.f), 40.f, 20.f);
	for (int i = 1; i < cubes; i++)
	{
		for (int j = 1; j < cubes; j++)
		{
			gairb_PushCube(&g1, V4(1.f, 1.f, 1.f, .1f), V3((float) (cubes / 2 - i) * radius, 2.0f + ptr(time, i, j), (float) (cubes / 2 - j) * radius), radius, height);
		}
	}	
	gairb_PushRect(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0, 10.f, -19.f), dim.x*.01f, dim.y *.01f, gairb_AlignToCenter, 1);
#endif

#if 1
	gairb_setup test = { OrthographicProjection((r32)window->info.width, (r32)window->info.height, 1, -1) * Translate(ZRotation(GAIRB_DEG2RAD(rotY)) , V3(350.f, 150.f, 0.f)) };
	gairb_PushSetup(&g1, &test);

	gairb_PushRect(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 0.6f), V3(0.f, 0.f, 0.f), dim.x*.125f, dim.y *.125f, gairb_AlignToCenter);

#endif
	gairb_setup ortho = { OrthographicProjection((r32)window->info.width, (r32)window->info.height, 1, -1) };
	gairb_PushSetup(&g1, &ortho);

	gairb_PushRect(&g1, (gaixw_GetAttribute(window, gaixwFlagsVSYNC) ? V4(1.f, 1.f, 1.f, 1.f) : V4(1.f, 0.0f, 0.f, 1.f)), V3(1.f, 5.f, 0.f), 3.f, 40.f, 0);

	float cvertex_p	= ((float)(renderbuffer->vertex_count) / (float)renderbuffer->vertex_max);
	gairb_PushRect(&g1, V4(1.f, 1.f, 1.f, 1.f), V3i(5, 5, 0), 128, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4((cvertex_p) * 1.f, (1.f-cvertex_p) *  1.f, 0.f, 1.f), V3i(5, 5, 0), cvertex_p  * 128.f, 4, gairb_AlignToTopLeft);

	float cbuffer_p = ((float)(renderbuffer->pushbuffer_at - renderbuffer->pushbuffer_base) / (float)renderbuffer->pushbuffer_max);
	gairb_PushRect(&g1, V4(1.f, 1.f, 1.f, 1.f), V3i(5, 12, 0), 128, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4((cbuffer_p) * 1.f, (1.f-cbuffer_p) * 1.f, 0.f, 1.f), V3i(5, 12, 0), cbuffer_p * 128.f, 4, gairb_AlignToTopLeft);

#if 1
	for(int i=0; i<DT_HISTORY_MAX;i++) gairb_PushRect(&g1, (dt_history[i] <= 16 ? V4(0.f, 1.f, 0.f, 1.f) : V4((dt_history[i]/16.f) * 1.f, 1.f-(dt_history[i]/16.f) * 1.f, 0.f, 1.f)), V3i(5+i, 20, 0), 1, dt_history[i], gairb_AlignToTopLeft);
#endif
	gairb_EndGroup(&g1);

	dt_history[dt_pos] = window->dt.millis;
	dt_pos++;
	if(dt_pos > DT_HISTORY_MAX-1) dt_pos = 0;
}
