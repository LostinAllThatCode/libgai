#include <gai_engine.h>

#include <stdio.h>

static r32 time, time2;

#define DT_HISTORY_MAX 4096
static int dt_history_max = DT_HISTORY_MAX;
static int dt_history[DT_HISTORY_MAX];
static int dt_pos;

v3 cam_along_x = V3(5.f, 5.f, 0.f);
v3 cam_along_z = V3(0.f, 10.f, 18.f);

float func1(float t, int i, int j) { return cosf(t * (i - j) * .125f) * .5f; }
float func2(float t, int i, int j) { return sinf(cosf(t) * (i - j) * .125f) * .5f; }
float func3(float t, int i, int j) { return (cosf(time + j * i)) * .125f * .5f; }

typedef float (*calc)(float t, int i, int j);
calc ptr = func1;

int cubes = 60;
int rotX, rotY;
int a;

int fov = 60;

inline float
clamp(float Min, float Value, float Max)
{
	float Result = Value;

	if (Result < Min)
	{
		Result = Min;
	}
	else if (Result > Max)
	{
		Result = Max;
	}

	return (Result);
}

inline v2 GetTextureDim(loaded_bitmap *b)
{
	v2 result = V2(b->width, b->height);
	return result;
}

GAIPF_EXTERNAL update_and_render_(UpdateAndRender)
{
	renderbuffer->clear_color = V4(.41f, .74f, .95f, 1.f);
	dt_history_max = window->width;
	if (time > 5.f)	a = -1;
	if (time <= 0.f) a = 1;
	time += window->dt * a;
	time2 += window->dt;
	gairb_group g1 = gairb_BeginGroup(V2i(window->width, window->height), renderbuffer, 0);

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

	fov = clamp(30.f, fov + window->input.dwheel, 90.f);

	m4x4 modelview = Mat4x4(1.0f) * YRotation( GAIRB_DEG2RAD( rotY ));
	gairb_setup perspective = { PerspectiveProjection((float) window->width / (float) window->height, (float) fov, 0.1f, 1000.f) * CameraOrbit(cam_along_z, GAIRB_DEG2RAD(-15.f), GAIRB_DEG2RAD(0)) * modelview };
	gairb_PushSetup(&g1, &perspective);

	static int adder = 1;
	if (cubes < 1 || cubes > 60) adder *= -1;
	//cubes+=adder;

	float radius = .125f;
	float height = .125f;

	#if 1
	//gairb_PushCube(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0.f, 10.f, 0.f), 40.f, 20.f);
	//gairb_PushRect(&g1, &platform->assets[1], V4(1.f, 1.f, 1.f, 1.f), V3(0, 10.f, -19.f), dim.x*.01f, dim.y *.01f, gairb_AlignToCenter, 1);
	gairb_PushSetup(&g1, &perspective);
	for (int i = 1; i < cubes; i++)
	{
		for (int j = 1; j < cubes; j++)
		{
			#if 1
			gairb_PushCube(&g1, &platform->assets[1], V4(.32f, .27f, .23f, .9f), V3((float) (cubes / 2 - i) * radius, 2.0f + ptr(time, i, j), (float) (cubes / 2 - j) * radius), radius, height);
			#else
			gairb_PushCube(&g1, V4(1.f, 1.f, 1.f, .1f), V3((float) (cubes / 2 - i) * radius, 2.0f, (float) (cubes / 2 - j) * radius), radius, height);
			#endif
		}
	}
	#endif


	#if 1
	v2 dim = GetTextureDim(&platform->assets[1]);
	gairb_setup test = { OrthographicProjection((r32)window->width, (r32)window->height, -2, 2) * Translate(ZRotation(GAIRB_DEG2RAD(rotY)) , V3(window->width/2.f, window->height/2.f, 0.f)) };
	gairb_PushSetup(&g1, &test);

	gairb_PushRect(&g1, &platform->assets[2], V4(1.f, 1.f, 1.f, 1.0f), V3(0.f, 0.f, 0.f), window->width, window->height, gairb_AlignToCenter);
	#endif

	gairb_setup ortho = { OrthographicProjection((r32)window->width, (r32)window->height, -2, 2) };
	gairb_PushSetup(&g1, &ortho);

	gairb_PushRect(&g1, (gaixw_GetAttribute(window, gaixwFlagsVSYNC) ? V4(1.f, 1.f, 1.f, 1.f) : V4(1.f, 0.0f, 0.f, 1.f)), V3(1.f, 5.f, 0.f), 3.f, 40.f, 0);

	float cvertex_p	= ((float)(renderbuffer->vertex_count) / (float)renderbuffer->vertex_max);
	gairb_PushRect(&g1, V4(1.f, 1.f, 1.f, 1.f), V3i(5, 35, 0), 128, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4((cvertex_p) * 1.f, (1.f - cvertex_p) *  1.f, 0.f, 1.f), V3i(5, 35, 1), cvertex_p  * 128.f, 4, gairb_AlignToTopLeft);

	float cbuffer_p = ((float)(renderbuffer->pushbuffer_at - renderbuffer->pushbuffer_base) / (float)renderbuffer->pushbuffer_max);
	gairb_PushRect(&g1, V4(1.f, 1.f, 1.f, 1.f), V3i(5, 55, 0), 128, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4((cbuffer_p) * 1.f, (1.f - cbuffer_p) * 1.f, 0.f, 1.f), V3i(5, 55, 1), cbuffer_p * 128.f, 4, gairb_AlignToTopLeft);

	#if 1
	for (int i = 0; i < dt_history_max; i++) gairb_PushRect(&g1, ( (i >= (dt_pos - 3) && i <= (dt_pos + 3)) ? V4(1.f, 0.f, 1.f, .5f) : V4((dt_history[i] > 16 ? 1.f : 0.f), (dt_history[i] <= 16 ? 1.f : 0.f), 0.f, .5f)), V3i(5 + i, 5, 1), 1, dt_history[i], gairb_AlignToTopLeft);
	#endif
	gairb_EndGroup(&g1);

	dt_history[dt_pos] = window->dt * 1000.f;
	dt_pos++;
	if (dt_pos > dt_history_max - 1) dt_pos = 0;
}
