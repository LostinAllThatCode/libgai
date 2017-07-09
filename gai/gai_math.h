#ifndef _INCLUDE_GAI_MATH_H

#if !defined(GAI_TYPEDEFS)
	#include <stddef.h>
	#include <stdint.h>
	typedef uint8_t		u8;
	typedef int8_t   	i8;
	typedef uint16_t 	u16;
	typedef int16_t 	i16;
	typedef uint32_t 	u32;
	typedef int32_t 	i32;
	typedef float		r32;
	typedef uint64_t 	u64;
	typedef int64_t 	i64;
	typedef double		r64;
	typedef int32_t 	b32;
	#define true 		1
	#define false 		0
	#define GAI_TYPEDEFS 1
#endif

#if !defined(GAI_MATH_STATIC)
	#if !defined(GAI_MATH_DEF)
		#define GAI_MATH_DEF extern 		/**< Define the macro __GAI_STATIC__ to force all function to be __static__ instead of __extern__ */
	#endif
#else
	#if !defined(GAI_MATH_DEF)
		#define GAI_MATH_DEF static
	#endif
#endif
#define GAI_MATH_INL GAI_MATH_DEF inline

union v2
{
	struct { r32 x, y; };
	struct { r32 u, v; };
	struct { r32 w, h; };
	r32	T[2];
};
inline v2 vec2(r32 x, r32 y) { v2 result; result.x = x; result.y = y; return (result); }
inline v2 vec2i(i32 x, i32 y) { v2 result; result.x = (r32)x; result.y = (r32)y; return (result); }

union v3
{
	struct { r32 x, y, z; };
	struct { r32 r, g, b; };
	struct { r32 u, v, _unused0_; };
	r32	T[3];
};
inline v3 vec3(r32 x, r32 y, r32 z) { v3 result; result.x = x; result.y = y; result.z = z; return (result); }
inline v3 vec3i(i32 x, i32 y, i32 z) { v3 result; result.x = (r32)x; result.y = (r32)y; result.z = (r32)z; return (result); }

union v4
{
	struct { r32 x, y, z, w; };
	struct { r32 r, g, b, a; };
	struct { r32 u, v, _unused0_, _unused1_; };
	r32	T[4];
};
inline v4 vec4(r32 x, r32 y, r32 z, r32 w) { v4 result; result.x = x; result.y = y; result.z = z; result.w = w; return (result); }
inline v4 vec4i(i32 x, i32 y, i32 z, i32 w) { v4 result; result.x = (r32)x; result.y = (r32)y; result.z = (r32)z; result.w = (r32)w; return (result); }

union mx4
{
	r32 T[16];
	r32 R[4][4];
};
inline mx4 mat4( r32 initial = 0.0f )
{
	mx4 result =
	{
		initial, 0.0f, 0.0f, 0.0f,
		0.0f, initial, 0.0f, 0.0f,
		0.0f, 0.0f, initial, 0.0f,
		0.0f, 0.0f, 0.0f, initial
	};
	return (result);
}

inline v2 operator*(v2 A, r32 B) { v2 result; result.x = A.x * B; result.y = A.y * B; return (result); }
inline v2 operator*(r32 A, v2 B) { return(B*A); }
inline v2 operator*=(v2 &A, r32 B) { A = A*B; return (A);}
inline v2 operator-(v2 A, v2 B) { v2 result; result.x = A.x - B.x; result.y = A.y - B.y; return (result); }
inline v2 operator-=(v2 &A, v2 B) { A = A-B; return (A); }
inline v2 operator+(v2 A, v2 B) { v2 result; result.x = A.x + B.x; result.y = A.y + B.y; return (result); }
inline v2 operator+=(v2 &A, v2 B) { A = A+B; return (A); }

inline v3 operator*(v3 A, r32 B) { v3 result; result.x = A.x * B; result.y = A.y * B; result.z = A.z * B; return (result); }
inline v3 operator*(r32 A, v3 B) { return(B*A); }
inline v3 operator*=(v3 &A, r32 B) { A = A*B; return (A);}
inline v3 operator-(v3 A, v3 B) { v3 result; result.x = A.x - B.x; result.y = A.y - B.y; result.z = A.z - B.z; return (result); }
inline v3 operator-=(v3 &A, v3 B) { A = A-B; return (A); }
inline v3 operator+(v3 A, v3 B) { v3 result; result.x = A.x + B.x; result.y = A.y + B.y; result.z = A.z + B.z; return (result); }
inline v3 operator+=(v3 &A, v3 B) { A = A+B; return (A); }

inline v4 operator*(v4 A, r32 B) { v4 result; result.x = A.x * B; result.y = A.y * B; result.z = A.z * B; result.w = A.w * B; return (result); }
inline v4 operator*(r32 A, v4 B) { return(B*A); }
inline v4 operator*=(v4 &A, r32 B) { A = A*B; return (A);}
inline v4 operator-(v4 A, v4 B) { v4 result; result.x = A.x - B.x; result.y = A.y - B.y; result.z = A.z - B.z; result.w = A.w - B.w; return (result); }
inline v4 operator-=(v4 &A, v4 B) { A = A-B; return (A); }
inline v4 operator+(v4 A, v4 B) { v4 result; result.x = A.x + B.x; result.y = A.y + B.y; result.z = A.z + B.z; result.w = A.w + B.w; return (result); }
inline v4 operator+=(v4 &A, v4 B) { A = A+B; return (A); }

GAI_MATH_INL u32
gai_math_color_pack_RGBA(v4 color)
{
	u32 result = (((i32)(color.a * 255.f) << 24)) |
	             (((i32)(color.b * 255.f) << 16)) |
	             (((i32)(color.g * 255.f) <<  8)) |
	             (((i32)(color.r * 255.f) <<  0));
	return (result);
}

GAI_MATH_INL v4
gai_math_color_unpack_RGBA(u32 color)
{
	v4 result =
	{
		(((color >> 0) & 0xFF) / 255.0f),
		(((color >> 8) & 0xFF) / 255.0f),
		(((color >> 16) & 0xFF) / 255.0f),
		(((color >> 24) & 0xFF) / 255.0f)
	};
	return (result);
}

GAI_MATH_INL u32
gai_math_color_pack_ABGR(v4 color)
{
	u32 result = (((i32)(color.a * 255.f) << 24)) |
	             (((i32)(color.b * 255.f) << 16)) |
	             (((i32)(color.g * 255.f) <<  8)) |
	             (((i32)(color.r * 255.f) <<  0));
	return (result);
}

GAI_MATH_INL v4
gai_math_color_unpack_ABGR(u32 color)
{
	v4 result = { (((color >> 16) & 0xFF) / 255.0f),
	              (((color >>  8) & 0xFF) / 255.0f),
	              (((color >>  0) & 0xFF) / 255.0f),
	              (((color >> 24) & 0xFF) / 255.0f)
	            };
	return (result);
}

GAI_MATH_INL mx4
gai_math_ortho(r32 left, r32 right, r32 bottom, r32 top, r32 znear, r32 zfar)
{
	r32 rl = 2.0f / (right - left);
	r32 tb = 2.0f / (top - bottom);
	r32 fn = -2.0f / (zfar - znear);
	r32	tx = -((right + left) / (right - left));
	r32	ty = -((top + bottom) / (top - bottom));
	r32	tz = -((zfar + znear) / (zfar - znear));

	mx4 result =
	{
		rl, 	0.0f, 	0.0f, 	tx,
		0.0f, 	tb, 	0.0f, 	ty,
		0.0f, 	0.0f, 	fn, 	tz,
		0.0f, 	0.0f, 	0.0f, 	1.0f,
	};
	return (result);
}

#define _INCLUDE_GAI_MATH_H
#endif