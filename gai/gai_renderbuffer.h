/*
	Author(s): Andreas Gaida

	gai_renderbuffer.h - v0.1 - https://github.com/LostinAllThatCode/libgai/blob/render_tests/gai/gai_renderbuffer.h

	TODO: Make a documentation as soon as this api is somewhat useable!!!

	Do this:
		#define GAIRB_IMPLEMENTATION
   	before you include this file in *one* C or C++ file to create the implementation.

	All function prefixed with a underscore(_) are internally used functions.
	DO NOT use them if you are not 100% sure what they do.

	Example:
	// TODO: Make an example!
*/
#ifndef GAI_INCLUDE_GAI_RENDERBUFFER_H

#ifdef GAIRB_STATIC
	#define GAIRB_API static
#else
	#define GAIRB_API extern
#endif

#ifdef GAIRB_ASSERT
	#include <assert.h>
	#define GAIRB_ASSERT(x) assert(x)
#else
	#define GAIRB_ASSERT(x)
#endif

#ifndef GAIRB_NO_MATH_AND_DEFINES

#include <math.h>
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286f

#define GAIRB_DEG2RAD(d) ((d)*(M_PI/180))
#define GAIRB_RAD2DEG(r) ((r)*(180/M_PI))

#include <stdint.h>
typedef uint8_t  u8;  typedef uint16_t u16; typedef uint32_t u32; typedef uint64_t u64;
typedef int8_t   i8;  typedef int16_t  i16; typedef int32_t  i32; typedef int32_t  b32;
typedef int64_t  i64; typedef float    r32; typedef double   r64; typedef size_t szx;

/* quick and fast 3d math implementation with operator overloading */

struct m3x3 { r32 E[3][3]; }; // ROW MAJOR ORDER
inline m3x3 Mat3x3(r32 a = 0.f) { m3x3 result = { { { a, 0.f, 0.f }, { 0.f, a, 0.f}, { 0.f, 0.f, a } } }; return result; } // Use Mat3x3(1.0f) to get an identity matrix
struct m4x4 { r32 E[4][4]; }; // ROW MAJOR ORDER
inline m4x4 Transpose(m4x4 A) { m4x4 R; for (int j = 0; j <= 3; ++j) for (int i = 0; i <= 3; ++i) R.E[j][i] = A.E[i][j]; return (R); }
inline m4x4 operator*(m4x4 A, m4x4 B) { m4x4 R = {}; for (int r = 0; r <= 3; ++r) for (int c = 0; c <= 3; ++c) for (int i = 0; i <= 3; ++i) R.E[r][c] += A.E[r][i] * B.E[i][c]; return (R); } // TODO: Make a performance based matrix multiplication, this is NOT!
inline m4x4 Mat4x4(r32 a = 0.f) { m4x4 result = { { { a, 0.f, 0.f, 0.f }, { 0.f, a, 0.f, 0.f}, { 0.f, 0.f, a, 0.f }, { 0.f, 0.f, 0.f, a } } }; return result; }
inline m4x4 Scale(r32 x, r32 y, r32 z) { m4x4 result = { { { x, 0, 0, 0 }, { 0, y, 0, 0 }, { 0, 0, z, 0 }, { 0, 0, 0, 1 } } }; return result; }
inline m4x4 XRotation(r32 angle) { r32 c = cosf(angle); r32 s = sinf(angle); m4x4 result = { { { 1, 0, 0, 0 }, { 0, c, -s, 0 }, { 0, s, c, 0 }, { 0, 0, 0, 1 } } }; return result; }
inline m4x4 YRotation(r32 angle) { r32 c = cosf(angle); r32 s = sinf(angle); m4x4 result = { { { c, 0, s, 0 }, { 0, 1, 0, 0 }, { -s, 0, c, 0 }, { 0, 0, 0, 1 } } }; return result; }
inline m4x4 ZRotation(r32 angle) { r32 c = cosf(angle); r32 s = sinf(angle); m4x4 result = { { { c, -s, 0, 0 }, { s, c, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } }; return result; }
inline m4x4 OrthographicProjection(r32 left, r32 right, r32 top, r32 bottom,  r32 znear, r32 zfar)
{
	r32 a = 2 / ( right - left);
	r32 b = 2 / ( top - bottom);
	r32 c = -2 / (zfar - znear);
	r32 d = -((right + left) / (right - left));
	r32 e = -((top + bottom) / (top - bottom));
	r32 f = -((zfar + znear) / (zfar - znear));
	m4x4 R = { { {  a,  0,  0,  d }, {  0,  b,  0,  e }, {  0,  0,  c,  f }, {  0,  0,  0,  1 } } };
	return (R);
}
inline m4x4 OrthographicProjection(r32 width, r32 height, r32 znear, r32 zfar) { return OrthographicProjection(0, width, 0, height, znear, zfar); }
inline m4x4 PerspectiveProjection(r32 aspect, r32 fov, r32 znear, r32 zfar)
{
	r32 s = tanf( GAIRB_DEG2RAD(fov) * .5f)  * znear;
	r32 r = aspect * s, l = -r, t = s, b = -t;
	r32 n = znear;
	r32 f = zfar;

	#if 0
	r32 t1 = (r + l) / (r - l);
	r32 t2 = (t + b) / (t - b);
	#endif

	r32 _a = 2 * n / (r - l);
	r32 _b = 2 * n / (t - b);
	r32 _c = -(f + n) / (f - n);
	r32 _d = -2 * f * n / (f - n);
	m4x4 R = { { { _a,  0,  0,  0 }, {  0, _b,  0,  0 }, {  0,  0, _c, _d }, {  0,  0, -1,  0 } } };
	return R;
}

union v2 { struct { r32 x, y; }; struct { r32 u, v; }; r32 E[2]; };
inline v2 V2(r32 x, r32 y) { v2 result = { x, y }; return result; }
inline v2 V2(r32 xy) { v2 result = { xy, xy }; return result; }
inline v2 V2i(i32 x, i32 y) { v2 result = { (r32)x, (r32)y }; return result; }
inline v2 operator*(r32 a, v2 b) { v2 result; result.x = a * b.x; result.y = a * b.y; return (result); }
inline v2 operator*(v2 b, r32 a) { v2 result = a * b; return (result); }
inline v2 &operator*=(v2 &b, r32 a) { b = a * b; return (b); }
inline v2 operator-(v2 a) { v2 result; result.x = -a.x; result.y = -a.y; return (result); }
inline v2 operator-(v2 a, v2 b) { v2 result; result.x = a.x - b.x; result.y = a.y - b.y; return (result); }
inline v2 &operator-=(v2 &a, v2 b) { a = a - b; return (a); }
inline v2 operator+(v2 a, v2 b) { v2 result; result.x = a.x + b.x; result.y = a.y + b.y; return (result); }
inline v2 &operator+=(v2 &a, v2 b) { a = a + b; return (a); }

union v3 { struct { v2 xy; r32 _ignored0_; }; struct { r32 _ignored0_; v2 yz; }; struct { v2 uv; r32 _ignored0_; }; struct { r32 _ignored0_; v2 vw; }; struct { r32 x, y, z; }; struct { r32 r, g, b; }; struct { r32 u, v, w; }; r32 E[3]; };
inline v3 V3(r32 x, r32 y, r32 z) { v3 result = { x, y, z }; return result; }
inline v3 V3(r32 xyz) { v3 result = { xyz, xyz, xyz }; return result; }
inline v3 V3(v2 xy, r32 z) { v3 result = { xy.x, xy.y, z }; return result; }
inline v3 V3i(i32 x, i32 y, i32 z) { v3 result = { (r32)x, (r32)y, (r32)z }; return result; }
inline v3 Transform(m4x4 a, v3 p, r32 pw = 1.0f) { v3 result; result.x = p.x * a.E[0][0] + p.y * a.E[0][1] + p.z * a.E[0][2] + pw * a.E[0][3]; result.y = p.x * a.E[1][0] + p.y * a.E[1][1] + p.z * a.E[1][2] + pw * a.E[1][3]; result.z = p.x * a.E[2][0] + p.y * a.E[2][1] + p.z * a.E[2][2] + pw * a.E[2][3]; return (result); }
//inline v3 Transform(m4x4 a, v3 p, r32 pw = 1.0f) { v3 r; r.x = p.x * a.E[0][0] + p.y * a.E[1][0] + p.z * a.E[2][0] + pw * a.E[3][0]; r.y = p.x * a.E[0][1] + p.y * a.E[1][1] + p.z * a.E[2][1] + pw * a.E[3][1]; r.z = p.x * a.E[0][2] + p.y * a.E[1][2] + p.z * a.E[2][2] + pw * a.E[3][2]; return (r); }

inline v3 operator*(r32 a, v3 b) { v3 result; result.x = a * b.x; result.y = a * b.y; result.z = a * b.z; return (result); }
inline v3 operator*(v3 b, r32 a) { v3 result = a * b; return (result); }
inline v3 &operator*=(v3 &b, r32 a) { b = a * b; return (b); }
inline v3 operator-(v3 a) { v3 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; return (result); }
inline v3 operator-(v3 a, v3 b) { v3 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; return (result); }
inline v3 &operator-=(v3 &a, v3 b) { a = a - b; return (a); }
inline v3 operator+(v3 a, v3 b) { v3 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; return (result); }
inline v3 &operator+=(v3 &a, v3 b) { a = a + b; return (a); }
inline v3 operator*(m4x4 a, v3 p) { v3 result = Transform(a, p, 1.0f); return (result); }
inline v3 GetColumn(m4x4 A, u32 C) { v3 R = {A.E[0][C], A.E[1][C], A.E[2][C]}; return (R); }
inline v3 GetRow(m4x4 A, u32 R) { v3 Result = {A.E[R][0], A.E[R][1], A.E[R][2]}; return (Result); }
inline v3 Cross(v3 a, v3 b) { v3 result; result.x = (a.y * b.z) - (a.z * b.y); result.y = (a.z * b.x) - (a.x * b.z); result.z = (a.x * b.y) - (a.y * b.x); return result; }
inline r32 Dot(v3 A, v3 B) { r32 Result = A.x * B.x + A.y * B.y + A.z * B.z; return (Result); }
inline r32 LengthSq(v3 A) { r32 Result = Dot(A, A); return (Result); }
inline v3 Normalize(v3 a) { v3 result = a * (1.0f / LengthSq(a)); return (result); }

union v4 { struct { union { v3 xyz; struct { r32 x, y, z; }; }; r32 w; }; struct { union { v3 rgb; struct { r32 r, g, b; }; }; r32 a; }; struct { v2 xy; r32 _ignored0_; r32 _ignored1_; }; struct { r32 _ignored0_; v2 yz; r32 _ignored1_; }; struct { r32 _ignored0_; r32 _ignored1_; v2 zw; }; r32 E[4]; };
inline v4 V4(r32 x, r32 y, r32 z, r32 w) { v4 result = { x, y, z, w }; return result; }
inline v4 V4(r32 xyzw) { v4 result = { xyzw, xyzw, xyzw, xyzw }; return result; }
inline v4 V4(v2 xy, r32 z, r32 w) { v4 result = { xy.x, xy.y, z, w }; return result; }
inline v4 V4(v3 xyz, r32 w) { v4 result = { xyz.x, xyz.y, xyz.z, w }; return result; }
inline v4 V4i(i32 x, i32 y, i32 z, i32 w) { v4 result = { (r32)x, (r32)y, (r32)z, (r32)w }; return result; }
inline v4 operator*(r32 a, v4 b) { v4 result; result.x = a * b.x; result.y = a * b.y; result.z = a * b.z; result.w = a * b.w; return (result); }
inline v4 operator*(v4 b, r32 a) { v4 result = a * b; return (result); }
inline v4 &operator*=(v4 &b, r32 a) { b = a * b; return (b); }
inline v4 operator-(v4 a) { v4 result; result.x = -a.x; result.y = -a.y; result.z = -a.z; result.w = -a.w; return (result); }
inline v4 operator-(v4 a, v4 b) { v4 result; result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z; result.w = a.w - b.w; return (result); }
inline v4 &operator-=(v4 &a, v4 b) { a = a - b; return (a); }
inline v4 operator+(v4 a, v4 b) { v4 result; result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z; result.w = a.w + b.w; return (result); }
inline v4 &operator+=(v4 &a, v4 b) { a = a + b; return (a); }

inline m4x4 Columns3x3(v3 X, v3 Y, v3 Z) { m4x4 R = { { {X.x, Y.x, Z.x, 0}, {X.y, Y.y, Z.y, 0}, {X.z, Y.z, Z.z, 0}, { 0, 0, 0, 1} } }; return (R); }
inline m4x4 Rows3x3(v3 X, v3 Y, v3 Z) { m4x4 R = { { {X.x, X.y, X.z, 0}, {Y.x, Y.y, Y.z, 0}, {Z.x, Z.y, Z.z, 0}, { 0, 0, 0, 1} } }; return (R); }
inline m4x4 Translate(m4x4 A, v3 T) { m4x4 R = A; R.E[0][3] += T.x; R.E[1][3] += T.y; R.E[2][3] += T.z; return R; }
inline m4x4 CameraOrbit(v3 eye, r32 pitch, r32 yaw) { m4x4 cam = YRotation(yaw) * XRotation(pitch); m4x4 R = Rows3x3(GetColumn(cam, 0), GetColumn(cam, 1), GetColumn(cam, 2)); R = Translate(R, -(R * eye)); return (R); }
inline m4x4 CameraLookAt(v3 eye, v3 target, v3 up) { v3 f  = Normalize(eye - target); v3 s  = Normalize(Cross(up, f)); v3 _u = Cross(f, s); v3 _e = V3(-Dot(s, eye), -Dot(_u, eye), -Dot(f, eye)); 	m4x4 R = Translate(Rows3x3(s, _u, f), _e); return (R); }

#endif

struct gairb_textured_vertex
{
	v4 p;
	v2 uv;
	v4 color;
};

struct gairb_setup
{
	m4x4 transform;

	v3 camera_position;
};

enum gairb_entry_type_enum
{
	gairb_EntryType_gairb_entry_textured_quads = 0x1,
};
struct gairb_entry_header
{
	gairb_entry_type_enum type;
};

struct gairb_entry_textured_quads
{
	gairb_entry_header header;
	gairb_setup setup;
	u32 quad_count;
	u32 vertex_array_offset;
};

struct gairb_renderbuffer
{
	u8 *pushbuffer_base;
	u8 *pushbuffer_at;
	u32 pushbuffer_max;

	u32 vertex_max;
	u32 vertex_count;
	gairb_textured_vertex *vertex_array;

	struct loaded_bitmap **quad_textures;
	struct loaded_bitmap *default_texture;

	v4 clear_color;
};
#define gairb_RenderBuffer(clear_color, pushbuffer_max, pushbuffer, vertex_max, vertexbuffer, bitmap_array, default_texture) \
	{ pushbuffer, pushbuffer, pushbuffer_max, vertex_max, 0, (gairb_textured_vertex*) vertexbuffer, bitmap_array, default_texture, clear_color }
/*
	This is just a macro to help initialization of a renderbuffer struct.

	Example code:

		int renderbuffer_size = 1024 * 1024 * 4;  // 4 megabytes
		int vertex_count_max  = 120000;
		int vertexbuffer_size = sizeof(gairb_textured_vertex) * vertex_count_max;
		unsigned char *renderbuffer = (unsigned char *) malloc( renderbuffer_size );
		unsigned char *vertexbuffer = (unsigned char *) malloc( vertexbuffer_size );
		gairb_renderbuffer MyRenderBuffer = gairb_RenderBuffer( V4i(0), renderbuffer_size, renderbuffer, vertex_count_max, vertexbuffer, 0, 0);
*/

struct gairb_group
{
	v2 screen_dim;
	gairb_renderbuffer *commands;
	gairb_entry_textured_quads *current_quads;

	gairb_setup last_setup;
	void *assets;
};

enum { gairb_AlignToTopLeft, gairb_AlignToBottomLeft, gairb_AlignToCenter, gairb_AlignToTopRight, gairb_AlignToBottomRight };

GAIRB_API gairb_group 		gairb_BeginGroup(v2 screen_dim, gairb_renderbuffer *commands, void *assets);
GAIRB_API void 				gairb_EndGroup(gairb_group *group);
GAIRB_API void 				gairb_PushSetup(gairb_group *group, gairb_setup *new_setup);

GAIRB_API void				gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 p1, v2 uv1, v4 c1, v4 p2, v2 uv2, v4 c2, v4 p3, v2 uv3, v4 c3, v4 p4, v2 uv4, v4 c4);
GAIRB_API void 				gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 color, v4 p1, v2 uv1, v4 p2, v2 uv2, v4 p3, v2 uv3, v4 p4, v2 uv4);
GAIRB_API void				gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 color, v3 position, r32 width, r32 height, u16 align = gairb_AlignToTopLeft, u32 flipped = 0);
GAIRB_API void				gairb_PushRect(gairb_group *group, v4 color, v3 position, r32 width, r32 height, u16 align = gairb_AlignToTopLeft);

GAIRB_API void				gairb_PushCube(gairb_group *group, loaded_bitmap *bitmap, v4 color, v3 position, r32 radius, r32  height, i32 flip_texture = 0);
GAIRB_API void				gairb_PushCube(gairb_group *group, v4 color, v3 position, r32 radius, r32 height, i32 flip_texture = 0);


#ifdef GAIRB_IMPLEMENTATION
inline void
gairb_PushSetup(gairb_group *group, gairb_setup *new_setup)
{
	group->last_setup = *new_setup;
	group->current_quads = 0;
}

inline GAIRB_API gairb_group
gairb_BeginGroup(v2 screen_dim, gairb_renderbuffer *commands, void *assets)
{
	gairb_group result   = { };
	result.screen_dim    = screen_dim;
	result.commands      = commands;
	result.current_quads = 0;

	gairb_setup initial_setup = {};
	initial_setup.transform   = Mat4x4(1.0f);
	gairb_PushSetup(&result, &initial_setup);

	return result;
}

inline GAIRB_API void gairb_EndGroup(gairb_group *group) {}

#define _gairb_Push( group, type ) (type *) _gairb_Push_(group, sizeof(type), gairb_EntryType_##type)
inline gairb_entry_header *
_gairb_Push_(gairb_group *group, u32 size, gairb_entry_type_enum type)
{
	gairb_entry_header *result = 0;

	u8 *pushbuffer_end = group->commands->pushbuffer_base + group->commands->pushbuffer_max;
	if ((group->commands->pushbuffer_at + size) <= pushbuffer_end)
	{
		result = (gairb_entry_header*) group->commands->pushbuffer_at;
		result->type = type;

		group->commands->pushbuffer_at += size;
	}
	else GAIRB_ASSERT(!"pushbuffer overflow");
	return result;
}

inline gairb_entry_textured_quads *
_gairb_GetQuads(gairb_group *group, u32 quad_count)
{
	if (!group->current_quads)
	{
		group->current_quads = _gairb_Push(group, gairb_entry_textured_quads);
		group->current_quads->vertex_array_offset = group->commands->vertex_count;
		group->current_quads->quad_count = 0;
		group->current_quads->setup = group->last_setup;
	}

	gairb_entry_textured_quads *result = group->current_quads;
	if ((group->commands->vertex_count + 6 * quad_count) > group->commands->vertex_max)
	{
		result = 0;
	}

	return result;
}

inline GAIRB_API void
gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 p1, v2 uv1, v4 c1, v4 p2, v2 uv2, v4 c2, v4 p3, v2 uv3, v4 c3, v4 p4, v2 uv4, v4 c4)
{
	gairb_entry_textured_quads *entry = _gairb_GetQuads(group, 1);
	GAIRB_ASSERT(entry);

	if (!bitmap) bitmap = group->commands->default_texture;
	if (group->commands->quad_textures)
	{
		group->commands->quad_textures[group->commands->vertex_count >> 2] = bitmap;
	}
	++entry->quad_count;


	gairb_textured_vertex *v = group->commands->vertex_array + group->commands->vertex_count;
	group->commands->vertex_count += 6;

	v[0].uv.u  	 = uv1.u; v[0].uv.v 	= uv1.v;
	v[0].p.x 	 = p1.x;  v[0].p.y 		= p1.y;  v[0].p.z 	  = p1.z; v[0].p.w 	   = p1.w;
	v[0].color.x = c1.x;  v[0].color.y  = c1.y;  v[0].color.z = c1.z; v[0].color.w = c1.w;

	v[1].uv.u  	 = uv1.u; v[1].uv.v 	= uv1.v;
	v[1].p.x 	 = p1.x;  v[1].p.y 		= p1.y;  v[1].p.z 	  = p1.z; v[1].p.w 	   = p1.w;
	v[1].color.x = c1.x;  v[1].color.y  = c1.y;  v[1].color.z = c1.z; v[1].color.w = c1.w;

	v[2].uv.u  	 = uv2.u; v[2].uv.v 	= uv2.v;
	v[2].p.x 	 = p2.x;  v[2].p.y 		= p2.y;  v[2].p.z 	  = p2.z; v[2].p.w 	   = p2.w;
	v[2].color.x = c2.x;  v[2].color.y  = c2.y;  v[2].color.z = c2.z; v[2].color.w = c2.w;

	v[3].uv.u  	 = uv3.u; v[3].uv.v 	= uv3.v;
	v[3].p.x 	 = p3.x;  v[3].p.y 		= p3.y;  v[3].p.z 	  = p3.z; v[3].p.w 	   = p3.w;
	v[3].color.x = c3.x;  v[3].color.y  = c3.y;  v[3].color.z = c3.z; v[3].color.w = c3.w;

	v[4].uv.u  	 = uv4.u; v[4].uv.v 	= uv4.v;
	v[4].p.x 	 = p4.x;  v[4].p.y 		= p4.y;  v[4].p.z 	  = p4.z; v[4].p.w 	   = p4.w;
	v[4].color.x = c4.x;  v[4].color.y  = c4.y;  v[4].color.z = c4.z; v[4].color.w = c4.w;

	v[5].uv.u  	 = uv4.u; v[5].uv.v 	= uv4.v;
	v[5].p.x 	 = p4.x;  v[5].p.y 		= p4.y;  v[5].p.z 	  = p4.z; v[5].p.w 	   = p4.w;
	v[5].color.x = c4.x;  v[5].color.y  = c4.y;  v[5].color.z = c4.z; v[5].color.w = c4.w;

	#if 0
	v[1].p = p2; v[1].uv = uv2; v[1].color = c2;
	v[2].p = p3; v[2].uv = uv3; v[2].color = c3;
	v[3].p = p4; v[3].uv = uv4; v[3].color = c4;
	#endif
}

inline GAIRB_API void
gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 color, v4 p1, v2 uv1, v4 p2, v2 uv2, v4 p3, v2 uv3, v4 p4, v2 uv4)
{
	gairb_PushRect(group, bitmap, p1, uv1, color, p2, uv2, color, p3, uv3, color, p4, uv4, color);
}

inline GAIRB_API void
gairb_PushRect(gairb_group *group, v4 color, v4 p1, v2 uv1, v4 p2, v2 uv2, v4 p3, v2 uv3, v4 p4, v2 uv4)
{
	loaded_bitmap *bitmap = group->commands->default_texture;
	gairb_PushRect(group, bitmap, p1, uv1, color, p2, uv2, color, p3, uv3, color, p4, uv4, color);
}

inline GAIRB_API void
gairb_PushRect(gairb_group *group, loaded_bitmap *bitmap, v4 color, v3 position, r32 width, r32 height, u16 align, u32 flipped)
{
	v4 tl = V4(0.f), bl = V4(0.f), br = V4(0.f), tr = V4(0.f);
	switch (align)
	{
		case gairb_AlignToTopLeft:
		{
			tl = V4(position.x, position.y, position.z, 1.f);
			bl = V4(position.x, position.y + height, position.z, 1.f);
			br = V4(position.x + width, position.y + height, position.z, 1.f);
			tr = V4(position.x + width, position.y, position.z, 1.f);
		} break;
		case gairb_AlignToBottomLeft:
		{
			tl = V4(position.x, position.y - height, position.z, 1.f);
			bl = V4(position.x, position.y, position.z, 1.f);
			br = V4(position.x + width, position.y, position.z, 1.f);
			tr = V4(position.x + width, position.y - height, position.z, 1.f);
		} break;
		case gairb_AlignToCenter:
		{
			r32 half_width 	= width * .5f;
			r32 half_height = height * .5f;
			tl = V4(position.x - half_width, position.y - half_height, position.z, 1.f);
			bl = V4(position.x - half_width, position.y + half_height, position.z, 1.f);
			br = V4(position.x + half_width, position.y + half_height, position.z, 1.f);
			tr = V4(position.x + half_width, position.y - half_height, position.z, 1.f);
		} break;
		case gairb_AlignToTopRight:
		{
			tl = V4(position.x - width, position.y, position.z, 1.f);
			bl = V4(position.x - width, position.y + height, position.z, 1.f);
			br = V4(position.x, position.y + height, position.z, 1.f);
			tr = V4(position.x, position.y, position.z, 1.f);
		} break;
		case gairb_AlignToBottomRight:
		{
			tl = V4(position.x - width, position.y - height, position.z, 1.f);
			bl = V4(position.x - width, position.y, position.z, 1.f);
			br = V4(position.x, position.y, position.z, 1.f);
			tr = V4(position.x, position.y - height, position.z, 1.f);
		} break;
		default: { return; };
	}
	if (flipped) gairb_PushRect(group, bitmap, color, tl, V2i(0, 1), bl, V2i(0, 0), tr, V2i(1, 1), br, V2i(1, 0) );
	else gairb_PushRect(group, bitmap, color, bl, V2i(0, 1), br, V2i(1, 1), tl, V2i(0, 0), tr, V2i(1, 0) );
}

inline GAIRB_API void
gairb_PushRect(gairb_group *group, v4 color, v3 position, r32 width, r32 height, u16 align)
{
	loaded_bitmap *bitmap = group->commands->default_texture;
	gairb_PushRect(group, bitmap, color, position, width, height, align);
}

/*
	** The cube is pushed to the buffer in triangle strip style.
	* c-d-a-b for the first front face
	* d-h-b-f for right side face
	* and so on ...
	*

		   e-------f			uv textures are specified like this for the each face:
		 / |     / |			[FRONT FACE] a(0, 0), b(1, 0), c(0, 1), d(1 ,1)
		a--|----b  |
		|  g----|--h 			position used as center of the cube
		| /     | /
		c-------d
*/
GAIRB_API void
gairb_PushCube(gairb_group *group, loaded_bitmap *bitmap, v4 color, v3 position, r32 radius, r32 height, i32 flip_texture)
{
	gairb_entry_textured_quads *entry = _gairb_GetQuads(group, 6);
	GAIRB_ASSERT(entry);

	r32 half_height = height * .5f;
	r32 half_width	= radius * .5f;
	v2 uv1, uv2, uv3, uv4;
	if (flip_texture == 1) { uv1 = V2(0, 1); uv2 = V2(0, 0); uv3 = V2(1, 1); uv4 = V2(1, 0); }
	else { uv1 = V2(0, 0); uv2 = V2(0, 1); uv3 = V2(1, 0); uv4 = V2(1, 1); }

	v4 c1  = V4(color.xyz * .25f, color.a);
	v4 c2  = color;

	v4 a = V4(position.x - half_width, position.y + half_height, position.z + half_width, 1.f);
	v4 c = V4(position.x - half_width, position.y - half_height, position.z + half_width, 1.f);
	v4 b = V4(position.x + half_width, position.y + half_height, position.z + half_width, 1.f);
	v4 d = V4(position.x + half_width, position.y - half_height, position.z + half_width, 1.f);
	v4 e = V4(position.x - half_width, position.y + half_height, position.z - half_width, 1.f);
	v4 g = V4(position.x - half_width, position.y - half_height, position.z - half_width, 1.f);
	v4 f = V4(position.x + half_width, position.y + half_height, position.z - half_width, 1.f);
	v4 h = V4(position.x + half_width, position.y - half_height, position.z - half_width, 1.f);

	#if GAIRB_DEBUG_VISUALGRADIENT_CUBE
	gairb_PushRect(group, bitmap, c, uv2, c1, d, uv4, c1, a, uv1, c2, b, uv3, c2);
	gairb_PushRect(group, bitmap, d, uv2, c1, h, uv4, c1, b, uv1, c2, f, uv3, c2);
	gairb_PushRect(group, bitmap, h, uv2, c1, g, uv4, c1, f, uv1, c2, e, uv3, c2);
	gairb_PushRect(group, bitmap, g, uv2, c1, c, uv4, c1, e, uv1, c2, a, uv3, c2);
	gairb_PushRect(group, bitmap, a, uv2, c2, b, uv4, c2, e, uv1, c2, f, uv3, c2);
	gairb_PushRect(group, bitmap, g, uv2, c1, h, uv4, c1, c, uv1, c1, d, uv3, c1);
	#else
	gairb_PushRect(group, bitmap, c, uv2, c2, d, uv4, c2, a, uv1, c2, b, uv3, c2);
	gairb_PushRect(group, bitmap, d, uv2, c2, h, uv4, c2, b, uv1, c2, f, uv3, c2);
	gairb_PushRect(group, bitmap, h, uv2, c2, g, uv4, c2, f, uv1, c2, e, uv3, c2);
	gairb_PushRect(group, bitmap, g, uv2, c2, c, uv4, c2, e, uv1, c2, a, uv3, c2);
	gairb_PushRect(group, bitmap, a, uv2, c2, b, uv4, c2, e, uv1, c2, f, uv3, c2);
	gairb_PushRect(group, bitmap, g, uv2, c2, h, uv4, c2, c, uv1, c2, d, uv3, c2);
	#endif

}

GAIRB_API void
gairb_PushCube(gairb_group *group, v4 color, v3 position, r32 radius, r32 height, i32 flip_texture)
{
	loaded_bitmap *bitmap = group->commands->default_texture;
	gairb_PushCube(group, bitmap, color, position, radius, height, flip_texture);
}

#endif
#define GAI_INCLUDE_GAI_RENDERBUFFER_H
#endif














































#if 0
#include <gai_types.h>
#include <gai_utils.h>
#include <gai_math.h>

struct gaiRendererData
{
	r32 u, v;
	r32 r, g, b, a;
	r32 nx, ny, nz;
	r32 vx, vy, vz;
	r32 __reserved__[4];
}; // 64 byte length

struct gaiRendererMesh
{
	u32 start;
	u32 count;
	/*
	gaiRendererData *data;
	gaiRendererMesh *prev;
	gaiRendererMesh *next;
	*/
};

struct gaiRendererTriangle
{
	u32 material;
	gaiRendererMesh mesh;
	v3 position;
};

struct gaiRenderMeshShader
{
	u32 program;
	u32 projection;
	u32 view;
	u32 model;

	u32 vao;
	u32 vbo;
	u32 ibo;
	u32 size;
	u32 vbo_max;
	u32 offset;

	const char *vs =
	{
		"#version 430\n"
		"layout (location = 0) in vec2 uv;\n"
		"layout (location = 1) in vec4 color;\n"
		"layout (location = 2) in vec3 normal;\n"
		"layout (location = 3) in vec3 vertex;\n"
		"uniform mat4 model = mat4(1);\n"
		"uniform mat4 view  = mat4(1);\n"
		"uniform mat4 proj  = mat4(1);\n"
		"out vec4 _color;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(vertex, 1);\n"
		"   _color = vec4(color);\n"
		"}\n"
	};

	const char *fs =
	{
		"#version 430\n"
		"in vec4 _color;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"    color = _color;\n"
		"}\n"
	};
};

struct gaiRenderDebugFontShader
{
	u32 program;
	u32 projection;
	const char *vs =
	{
		"#version 430\n"
		"layout (location = 0) in vec4 vertex;\n"
		"uniform mat4 proj = mat4(1);\n"
		"void main() {\n"
		"   gl_Position = proj * vec4(vertex.x, vertex.y, 1, 1);\n"
		"}\n"
	};
	const char *fs =
	{
		"#version 430\n"
		"out vec4 color;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(1,1,1,1);\n"
		"}\n"
	};
};

struct gaiRenderer
{
	gaiRenderMeshShader mesh_shader;

	//	gaiRenderDebugFontShader debug_font_shader;

	m4x4 last_projection;
	m4x4 last_view;
};

void
gaiRendererSetViewport(gaiRenderer *renderer, r32 width, r32 height)
{
	glViewport(0, 0, width, height);
}

i32
gaiRendererCreate(gaiRenderer *renderer, r32 width, r32 height)
{
	gaiRendererSetViewport(renderer, width, height);

	renderer->last_view = Identity4();
	renderer->last_projection = Identity4();

	//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);

	#if 0
	u32 program = gaiOpenGLProgramCreate();
	gaiOpenGLShaderLoad(program, renderer->debug_font_shader.vs, GL_VERTEX_SHADER);
	gaiOpenGLShaderLoad(program, renderer->debug_font_shader.fs, GL_FRAGMENT_SHADER);
	if (!gaiOpenGLProgramLink(program)) return -1;
	renderer->debug_font_shader.projection = gaiOpenGLProgramGetUniform(program, "proj");
	renderer->debug_font_shader.program    = program;

	m4x4 ortho = Orthographic(0.f, (r32) width, (r32) height, 0.f, -1.f, 1.f);
	glUniformMatrix4fv(renderer->debug_font_shader.projection, 1, GL_FALSE,  (const GLfloat*) &ortho);


	glGenVertexArrays(1, &renderer->debug_font_vao);
	glBindVertexArray(renderer->debug_font_vao);

	glGenBuffers(1, &renderer->debug_font_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->debug_font_vbo);
	glBufferData(GL_ARRAY_BUFFER, 99999, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) (i64) 0);

	glBindVertexArray(0);
	#endif

	u32 program = gaiOpenGLProgramCreate();
	gaiOpenGLShaderLoad(program, renderer->mesh_shader.vs, GL_VERTEX_SHADER);
	gaiOpenGLShaderLoad(program, renderer->mesh_shader.fs, GL_FRAGMENT_SHADER);
	if (!gaiOpenGLProgramLink(program)) return -2;
	renderer->mesh_shader.projection = gaiOpenGLProgramGetUniform(program, "proj");
	renderer->mesh_shader.view       = gaiOpenGLProgramGetUniform(program, "view");
	renderer->mesh_shader.model      = gaiOpenGLProgramGetUniform(program, "model");
	renderer->mesh_shader.program    = program;

	i32 max_elements = 4096;
	u32 data_size    = sizeof(gaiRendererData);
	u32 max_size     = data_size * max_elements;

	glGenVertexArrays(1, &renderer->mesh_shader.vao);
	glBindVertexArray(renderer->mesh_shader.vao);

	glGenBuffers(1, &renderer->mesh_shader.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->mesh_shader.vbo);
	glBufferData(GL_ARRAY_BUFFER, max_size, 0, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 8);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 24);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 64, (void *) (i64) 36);

	glBindVertexArray(0);

	renderer->mesh_shader.size  = data_size;
	renderer->mesh_shader.vbo_max = max_size;
	renderer->mesh_shader.offset = 0;

	return 1;
}

void
gaiRendererDraw(gaiRenderer *renderer)
{
	glDrawArrays(GL_TRIANGLES, 0, renderer->mesh_shader.offset / renderer->mesh_shader.size);
}

void
gaiRendererDraw(gaiRenderer *renderer, GLenum mode, i32 first, i32 count)
{
	glDrawArrays(mode, first, count);
}


void
gaiRendererSetProjection(gaiRenderer *renderer, r32 fov, r32 aspect, m4x4 *view, r32 znear = 0.1f, r32 zfar = 100.f)
{

	renderer->last_projection = Perspective(aspect, fov, znear, zfar);
	renderer->last_view       = *view;
	//glUseProgram(renderer->mesh_shader.program);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);
}

void
gaiRendererSetProjection(gaiRenderer *renderer, m4x4 *projection, m4x4 *view)
{
	renderer->last_projection = *projection;
	renderer->last_view       = *view;
	glUseProgram(renderer->mesh_shader.program);
	glBindVertexArray(renderer->mesh_shader.vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->mesh_shader.vbo);
	glUniformMatrix4fv(renderer->mesh_shader.view, 1, GL_TRUE,  (const GLfloat*) &renderer->last_view);
	glUniformMatrix4fv(renderer->mesh_shader.projection, 1, GL_TRUE,  (const GLfloat*) &renderer->last_projection);
}

inline gaiRendererMesh
gaiRendererPushGrid(gaiRenderer *renderer, r32 cell_size, i32 size, v4 color = V4(1, 1, 1, 1))
{
	gaiRendererMesh mesh = {};
	r32 s = (r32) size * cell_size;
	mesh.start = renderer->mesh_shader.offset / renderer->mesh_shader.size;
	mesh.count = ((size * 2) + 1) * 4;
	gaiRendererData data[] =
	{
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, 0, 0, (r32)  s, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, 0, 0, (r32) - s, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, (r32)  s, 0, 0, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, (r32) - s, 0, 0, 0.f, 0.f, 0.f, 0.f,
	};
	glBufferSubData(GL_ARRAY_BUFFER, renderer->mesh_shader.offset, sizeof(data), data);
	renderer->mesh_shader.offset += sizeof(data);
	for (i32 i = 1; i <= s; i++)
	{
		r32 x = i * cell_size;
		r32 z = i * cell_size;

		gaiRendererData data[] =
		{
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  x, 0,  s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  x, 0, -s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -x, 0,  s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -x, 0, -s, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -s, 0,  z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  s, 0,  z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -s, 0, -z, 0.f, 0.f, 0.f, 0.f,
			0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  s, 0, -z, 0.f, 0.f, 0.f, 0.f,
		};
		glBufferSubData(GL_ARRAY_BUFFER, renderer->mesh_shader.offset, sizeof(data), data);
		renderer->mesh_shader.offset += sizeof(data);
	}
	return mesh;
}

void
gaiRendererPushTriangle(gaiRenderer *renderer, v3 offset = V3(0, 0, 0), v4 color = V4(1, 1, 1, 1))
{
	gaiRendererData data[] =
	{
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -1.f + offset.x, -1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  1.f + offset.x, -1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  0.f + offset.x,  1.f + offset.y, 0.f + offset.z, 0.f, 0.f, 0.f, 0.f},
	};
	i32 size = sizeof(data);
	glBufferSubData(GL_ARRAY_BUFFER, renderer->mesh_shader.offset, size, data);
	renderer->mesh_shader.offset += size;
}

void
gaiRendererPushTriangle(gaiRenderer *renderer, v3 p1, v2 uv1, v4 c1, v3 p2, v2 uv2, v4 c2, v3 p3, v2 uv3, v4 c3)
{
	gaiRendererData data[] =
	{
		{uv1.u, uv1.v, c1.r, c1.g, c1.b, c1.a, 0.f, 0.f, 0.f, p1.x, p1.y, p1.z},
		{uv2.u, uv2.v, c2.r, c2.g, c2.b, c2.a, 0.f, 0.f, 0.f, p2.x, p2.y, p2.z},
		{uv3.u, uv3.v, c3.r, c3.g, c3.b, c3.a, 0.f, 0.f, 0.f, p3.x, p3.y, p3.z},
	};
	i32 size = sizeof(data);
	glBufferSubData(GL_ARRAY_BUFFER, renderer->mesh_shader.offset, size, data);
	renderer->mesh_shader.offset += size;
}

void
gaiRendererPushRectangle(gaiRenderer *renderer, v3 p1, v2 uv1, v4 c1, v3 p2, v2 uv2, v4 c2, v3 p3, v2 uv3, v4 c3, v3 p4, v2 uv4, v4 c4)
{
	gaiRendererPushTriangle(renderer, p1, uv1, c1, p2, uv2, c2, p3, uv3, c3);
	gaiRendererPushTriangle(renderer, p1, uv1, c1, p3, uv3, c3, p4, uv4, c4);
}

inline gaiRendererMesh
gaiRendererPushQuad(gaiRenderer *renderer, v3 center = V3(0, 0, 0), r32 width = 1.f, r32 height = 1.f, v4 color = V4(1, 1, 1, 1))
{
	gaiRendererMesh mesh = {};
	r32 half_width = .5f * width;
	r32 half_heigth = .5f * height;

	v3 p1 = center + V3(-half_width, -half_heigth,  half_width);
	v3 p2 = center + V3( half_width, -half_heigth,  half_width);
	v3 p3 = center + V3( half_width,  half_heigth,  half_width);
	v3 p4 = center + V3(-half_width,  half_heigth,  half_width);
	v3 p5 = center + V3(-half_width, -half_heigth, -half_width);
	v3 p6 = center + V3( half_width, -half_heigth, -half_width);
	v3 p7 = center + V3( half_width,  half_heigth, -half_width);
	v3 p8 = center + V3(-half_width,  half_heigth, -half_width);

	v2 uv     = V2(0);
	v4 top    = .75f * color;
	v4 bottom = .25f * color;
	v4 c1     = V4(top.r, top.g, top.b, color.a);
	v4 c2     = V4(bottom.r, bottom.g, bottom.b, color.a);

	mesh.start = renderer->mesh_shader.offset / renderer->mesh_shader.size;
	gaiRendererPushRectangle(renderer, p1, uv, c2, p2, uv, c2, p3, uv, c1, p4, uv, c1);
	gaiRendererPushRectangle(renderer, p5, uv, c2, p8, uv, c1, p7, uv, c1, p6, uv, c2);
	gaiRendererPushRectangle(renderer, p1, uv, c2, p2, uv, c2, p6, uv, c2, p5, uv, c2);
	gaiRendererPushRectangle(renderer, p4, uv, color, p8, uv, color, p7, uv, color, p3, uv, color);
	gaiRendererPushRectangle(renderer, p2, uv, c2, p6, uv, c2, p7, uv, c1, p3, uv, c1);
	gaiRendererPushRectangle(renderer, p1, uv, c2, p4, uv, c1, p8, uv, c1, p5, uv, c2);
	mesh.count = 36;
	return mesh;
}


// Immediate mode drawing! Pretty slow.
void
gaiRendererDrawTriangleImmediate(gaiRenderer *renderer, m4x4 *model, v4 color = V4(1, 1, 1, 1))
{
	glUseProgram(renderer->mesh_shader.program);
	glBindVertexArray(renderer->mesh_shader.vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->mesh_shader.vbo);

	gaiRendererData data[] =
	{
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f, -1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  1.f, -1.f, 0.f},
		{0.f, 0.f, color.r, color.g, color.b, color.a, 0.f, 0.f, 0.f,  0.f,  1.f, 0.f},
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
	if (model) glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);
	else glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void
gaiRendererDrawMeshImmediate(gaiRenderer *renderer, m4x4 *model, gaiRendererData *data, size_t size)
{
	glUseProgram(renderer->mesh_shader.program);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	gai_assert(size < renderer->mesh_shader.vbo_max);

	glBindVertexArray(renderer->mesh_shader.vao);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->mesh_shader.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	if (model) glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) model);
	else glUniformMatrix4fv(renderer->mesh_shader.model, 1, GL_TRUE,  (const GLfloat*) Identity4().E);
	glDrawArrays(GL_TRIANGLES, 0, size / sizeof(gaiRendererData));
}
#endif
