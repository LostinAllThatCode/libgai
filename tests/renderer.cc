
struct loaded_bitmap
{
	unsigned char *memory;
	unsigned int id;
	int is_loaded;
	int width;
	int height;
};

#define GAIRB_DEBUG_VISUALGRADIENT_CUBE 1
#define GAIXW_OPENGL
#define GAIXW_IMPLEMENTATION
#include "gai_xwindow.h"
#define GAIRB_IMPLEMENTATION
#include "gai_renderbuffer.h"
#define GAIRGL_IMPLEMENTATION
#include "gai_renderer_opengl.h"

#define external extern "C" __declspec (dllexport)
#include <stdio.h>
r32 time, time2;
gairgl opengl;

v3 cam_along_x = V3(5.f, 5.f, 0.f);
v3 cam_along_z = V3(0.f, 15.f, 19.f);

float func1(float t, int i, int j) { 	return cosf(t * (i) * .125f) * .5f; }
float func2(float t, int i, int j) { 	return sinf(cosf(t) * (i - j) * .125f) * .5f; }
float func3(float t, int i, int j) { 	return (cosf(time + j * i)) * .125f * .5f; }

typedef float (*calc)(float t, int i, int j);
calc ptr = func1;

int rot;
int a;
loaded_bitmap test_texture;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

inline loaded_bitmap
LoadTexture(char *Filename)
{
	loaded_bitmap result = {0};
	stbi_set_flip_vertically_on_load(1);
	int bpp;
	result.memory = stbi_load(Filename, &result.width, &result.height, &bpp, 4);
	if (!result.memory) assert(!"asd");
	return result;
}


external void
UpdateAndRender(gaixw_context *window, gairb_renderbuffer *renderbuffer)
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
		else if (ptr == func3) ptr = 0;
		else if (!ptr) ptr = func1;
	}

	if (gaiMouseDown(window, 0)) rot += (int)window->input.dx % 360;

	m4x4 modelview = Translate(Mat4x4(1.0f) * YRotation( GAIRB_DEG2RAD( rot )) * ZRotation(GAIRB_DEG2RAD( 180.f ) ) ,  V3(0.f, 20.f, 0.f));	
	gairb_setup perspective = { PerspectiveProjection((float) window->info.width / (float) window->info.height, 60.f, 0.1f, 100.f) * CameraOrbit(cam_along_z, GAIRB_DEG2RAD(-20.f), GAIRB_DEG2RAD(0.f)) * modelview };	
	gairb_PushSetup(&g1, &perspective);
	gairb_PushCube(&g1, &test_texture, V4(1.f, 1.0f, 1.0f, 1.f), V3(0.f, 10.f, 0.f), 41.f, 20.f );

	int cubes = 60;
	float radius = .3f;
	float height = .125f;

	modelview = Translate(Mat4x4(1.0f) * YRotation( GAIRB_DEG2RAD( rot )) * ZRotation(GAIRB_DEG2RAD( 0.f ) ) ,  V3(0.f, 0.f, 0.f));	
	perspective = { PerspectiveProjection((float) window->info.width / (float) window->info.height, 60.f, 0.1f, 100.f) * CameraOrbit(cam_along_z, GAIRB_DEG2RAD(-20.f), GAIRB_DEG2RAD(0.f)) * modelview };	
	gairb_PushSetup(&g1, &perspective);

	#if 0
	float r = cosf(sinf(time2 * 1.f));
	float g = sinf(time2 * .25f);
	float b = sinf(time2 * .7f);
	#else
	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	#endif
	for (int i = 1; i < cubes; i++)
	{
		for (int j = 1; j < cubes; j++)
		{
			gairb_PushCube(&g1, V4(r, g, b, .5f), V3((float) (cubes / 2 - i) * radius, 2.0f + (ptr ? ptr(time, i, j) : 0.f), (float) (cubes / 2 - j) * radius), radius, height);
		}
	}
	
	//gairb_PushRect(&g1, &test_texture, V4(1.f, 1.f, 1.f, 1.f), V3(0, 15.f, -19.f), 16, 9, gairb_AlignToCenter);

	gairb_setup ortho = { OrthographicProjection((r32)window->info.width, (r32)window->info.height, 1, -1) };
	gairb_PushSetup(&g1, &ortho);

	gairb_PushRect(&g1, (gaixw_IsVSYNC(window) ? V4(.0f, 1.f, 0.f, 1.f) : V4(1.f, 0.0f, 0.f, 1.f)), V3(5.f, 12.f, 0.f), 10.f, 10.f, 0);
	gairb_PushRect(&g1, V4(.0f, 0.f, 0.f, .5f),  V3(4.f, 11.f, 0.f), 12.f, 12.f, 0);
	gairb_PushRect(&g1, V4(1.f, 0.f, 0.f, 1.f),  V3i(5, 5, 0), 100, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4(0.f, 0.f, 0.f, .5f),  V3i(5, 6, 0), 100, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4(0.f, 1.f, 0.f, 1.0f), V3i(5, 7, 0), ((float)(renderbuffer->vertex_count + 4) / (float)renderbuffer->vertex_max) * 100.f, 1, gairb_AlignToTopLeft);
	gairb_PushRect(&g1, V4(0.f, 0.f, 0.f, .5f),  V3i(5, 8, 0), ((float)(renderbuffer->vertex_count + 4) / (float)renderbuffer->vertex_max) * 100.f, 1, gairb_AlignToTopLeft);

	gairb_EndGroup(&g1);

	gairgl_Render(&opengl, renderbuffer, V2i(window->info.width, window->info.height));
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			gaiWGLInitFunctions();
			gairgl_Initialize(&opengl);

			test_texture = LoadTexture("test.jpg");
			break;
		case DLL_PROCESS_DETACH:
			gairgl_Destroy(&opengl);
			stbi_image_free(test_texture.memory);

			break;
	}
	return TRUE;
}
