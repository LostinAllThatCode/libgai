/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_MATH_H_

#include "gai_types.h"
#include "gai_utils.h"

#define DEG2RAD(d) ((d)*(M_PI/180))
#define RAD2DEG(r) ((r)*(180/M_PI))

#define _sin(v) sinf(v)
#define _cos(v) cosf(v)
#define _tan(v) tanf(v)

inline r32 gaiMathLerp(r32 v0, r32 v1, r32 t) { return (1 - t) * v0 + t * v1; }
inline r32 gaiMathLerpPrecise(r32 v0, r32 v1, r32 t) { return (v0 + t * (v1 - v0)); }
inline r32 gaiMathClamp(r32 x, r32 min, r32 max)
{
    if (x < min) x = min;
    else if (x > max) x = max;
    return x;
}

union v2
{
    struct { r32 x, y; };
    struct { r32 u, v; };
    struct { r32 w, h; };
    r32 E[2];
};

inline v2
V2(r32 x, r32 y)
{
    v2 Result = { x, y };
    return Result;
}

inline v2
V2(r32 xy)
{
    v2 Result = { xy, xy };
    return Result;
}

inline v2
V2i(i32 x, i32 y)
{
    v2 Result = { (r32) x, (r32) y };
    return Result;
}

inline v2
operator*(r32 A, v2 B)
{
    v2 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;

    return (Result);
}

inline v2
operator*(v2 B, r32 A)
{
    v2 Result = A * B;

    return (Result);
}

inline v2 &
operator*=(v2 &B, r32 A)
{
    B = A * B;

    return (B);
}

inline v2
operator-(v2 A)
{
    v2 Result;

    Result.x = -A.x;
    Result.y = -A.y;

    return (Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return (Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
    A = A + B;

    return (A);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return (Result);
}

inline v2 &
operator-=(v2 &A, v2 B)
{
    A = A - B;

    return (A);
}

union v3
{
    struct { r32 x, y, z; };
    struct { r32 u, v, w; };
    struct { r32 r, g, b; };
    struct { v2 xy; r32 Ignored0_; };
    struct { r32 Ignored1_; v2 yz; };
    struct { v2 uv; r32 Ignored2_; };
    struct { r32 Ignored3_; v2 vw; };
    r32 E[3];
};

inline v3
V3(r32 x, r32 y, r32 z)
{
    v3 Result = { x, y, z };
    return Result;
}

inline v3
V3i(i32 x, i32 y, i32 z)
{
    v3 Result = { (r32) x, (r32) y, (r32) z };
    return Result;
}

inline v3
V3(v2 xy, r32 z)
{
    v3 Result = { xy.x, xy.y, z };
    return Result;
}

inline v3
V3(r32 xyz)
{
    v3 Result = { xyz, xyz, xyz };
    return Result;
}

inline v3
operator*(r32 A, v3 B)
{
    v3 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;

    return (Result);
}

inline v3
operator*(v3 B, r32 A)
{
    v3 Result = A * B;

    return (Result);
}

inline v3 &
operator*=(v3 &B, r32 A)
{
    B = A * B;

    return (B);
}

inline v3
operator-(v3 A)
{
    v3 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;

    return (Result);
}

inline v3
operator+(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return (Result);
}

inline v3 &
operator+=(v3 &A, v3 B)
{
    A = A + B;

    return (A);
}

inline v3
operator-(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;

    return (Result);
}

inline v3 &
operator-=(v3 &A, v3 B)
{
    A = A - B;

    return (A);
}

union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct { r32 x, y, z; };
        };
        r32 w;
    };

    struct
    {
        union
        {
            v3 rgb;
            struct { r32 r, g, b; };
        };
        r32 a;
    };

    struct { v2 xy; r32 Ignored0_; r32 Ignored1_; };
    struct { r32 Ignored2_; v2 yz; r32 Ignored3_; };
    struct { r32 Ignored4_; r32 Ignored5_; v2 zw; };
    r32 E[4];
};
typedef v4 quat;

inline v4
V4(r32 x, r32 y, r32 z, r32 w)
{
    v4 Result = { x, y, z, w };
    return Result;
}

inline v4
V4(v3 xyz, r32 w)
{
    v4 Result = { xyz.x, xyz.y, xyz.z, w};
    return Result;
}

inline v4
operator*(r32 A, v4 B)
{
    v4 Result;

    Result.x = A * B.x;
    Result.y = A * B.y;
    Result.z = A * B.z;
    Result.w = A * B.w;

    return (Result);
}

inline v4
operator*(v4 B, r32 A)
{
    v4 Result = A * B;

    return (Result);
}

inline v4 &
operator*=(v4 &B, r32 A)
{
    B = A * B;

    return (B);
}

inline v4
operator-(v4 A)
{
    v4 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    Result.w = -A.w;

    return (Result);
}

inline v4
operator+(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;
    Result.w = A.w + B.w;

    return (Result);
}

inline v4 &
operator+=(v4 &A, v4 B)
{
    A = A + B;

    return (A);
}

inline v4
operator-(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    Result.w = A.w - B.w;

    return (Result);
}

inline v4 &
operator-=(v4 &A, v4 B)
{
    A = A - B;

    return (A);
}

inline r32
gaiMathDotV3(v3 A, v3 B)
{
    r32 Result = A.x * B.x + A.y * B.y + A.z * B.z;

    return (Result);
}

inline r32
gaiMathDotV4(v4 A, v4 B)
{
    r32 Result = A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;

    return (Result);
}

inline v3
gaiMathCrossV3(v3 A, v3 B)
{
    v3 Result;
    Result.x = (A.y * B.z) - (A.z * B.y);
    Result.y = (A.z * B.x) - (A.x * B.z);
    Result.z = (A.x * B.y) - (A.y * B.x);
    return Result;
}

inline v4
gaiMathCrossV4(v4 A, v4 B)
{
    v4 Result;
    Result.x = (A.y * B.z) - (A.z * B.y);
    Result.y = (A.z * B.w) - (A.w * B.z);
    Result.z = (A.w * B.x) - (A.x * B.w);
    Result.w = (A.x * B.y) - (A.y * B.x);
    return Result;
}

inline r32
gaiMathLengthSqV3(v3 A)
{
    r32 Result = gaiMathDotV3(A, A);
    return (Result);
}

inline r32
gaiMathLengthSqV4(v4 A)
{
    r32 Result = gaiMathDotV4(A, A);
    return (Result);
}

inline r32
gaiMathLengthV3(v3 A)
{
    r32 Result = sqrt(gaiMathLengthSqV3(A));
    return (Result);
}

inline r32
gaiMathLengthV4(v4 A)
{
    r32 Result = sqrt(gaiMathLengthSqV4(A));
    return (Result);
}

inline v3
gaiMathNormalizeV3(v3 A)
{
    v3 Result = A * (1.0f / gaiMathLengthV3(A));
    return (Result);
}

inline v4
gaiMathNormalizeV4(v4 A)
{
    v4 Result = A * (1.0f / gaiMathLengthV4(A));
    return (Result);
}

inline v3
Normalize(v3 a)
{
    v3 result = a * (1.0f / gaiMathLengthV3(a));
    return (result);
}

inline v3
Cross(v3 a, v3 b)
{
    v3 result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

inline r32
Dot(v3 a, v3 b)
{
    r32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return (result);
}

/*
inline v3
gaiMathLerpV3(v3 A, v3 B, r32 t)
{
    v3 Result;
    Result = A + t * (B - A);
    return Result;
}
*/

struct m3x3
{
    r32 E[3][3];
};

struct m4x4
{
    r32 E[4][4];
};

inline m3x3
Identity3()
{
    m3x3 Result =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    return Result;
}

inline m4x4
Identity4()
{
    m4x4 Result =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Result;
}

extern v3
Transform(m4x4 A, v3 P, r32 Pw = 1.0f)
{
    v3 R;
    #if 1
    R.x = P.x * A.E[0][0] + P.y * A.E[0][1] + P.z * A.E[0][2] + Pw * A.E[0][3];
    R.y = P.x * A.E[1][0] + P.y * A.E[1][1] + P.z * A.E[1][2] + Pw * A.E[1][3];
    R.z = P.x * A.E[2][0] + P.y * A.E[2][1] + P.z * A.E[2][2] + Pw * A.E[2][3];
    #else
    R.x = P.x * A.E[0][0] + P.y * A.E[1][0] + P.z * A.E[2][0] + Pw * A.E[3][0];
    R.y = P.x * A.E[0][1] + P.y * A.E[1][1] + P.z * A.E[2][1] + Pw * A.E[3][1];
    R.z = P.x * A.E[0][2] + P.y * A.E[1][2] + P.z * A.E[2][2] + Pw * A.E[3][2];
    #endif
    return (R);
}

m4x4
operator*(m4x4 A, m4x4 B)
{
    // NOTE(casey): This is written to be instructive, not optimal!

    m4x4 R = {};

    for (int r = 0; r <= 3; ++r) // NOTE(casey): Rows (of A)
    {
        for (int c = 0; c <= 3; ++c) // NOTE(casey): Column (of B)
        {
            for (int i = 0; i <= 3; ++i) // NOTE(casey): Columns of A, rows of B!
            {
                R.E[r][c] += A.E[r][i] * B.E[i][c];
            }
        }
    }

    return (R);
}

inline v3
operator*(m4x4 A, v3 P)
{
    v3 R = Transform(A, P, 1.0f);
    return (R);
}


inline m4x4
Scale(r32 x, r32 y, r32 z)
{
    m4x4 result =
    {
        {
            { x, 0, 0, 0 },
            { 0, y, 0, 0 },
            { 0, 0, z, 0 },
            { 0, 0, 0, 1 }
        }
    };
    return result;
}

inline m4x4
XRotation(r32 angle)
{
    r32 c = _cos(angle);
    r32 s = _sin(angle);
    m4x4 result =
    {
        {
            { 1, 0,  0, 0 },
            { 0, c, -s, 0 },
            { 0, s,  c, 0 },
            { 0, 0,  0, 1 }
        }
    };
    return result;
}

inline m4x4
YRotation(r32 angle)
{
    r32 c = _cos(angle);
    r32 s = _sin(angle);
    m4x4 result =
    {
        {
            {  c, 0,  s, 0 },
            {  0, 1,  0, 0 },
            { -s, 0,  c, 0 },
            {  0, 0,  0, 1 }
        }
    };
    return result;
}


inline m4x4
ZRotation(r32 angle)
{
    r32 c = _cos(angle);
    r32 s = _sin(angle);
    m4x4 result =
    {
        {
            { c, -s,  0,  0 },
            { s,  c,  0,  0 },
            { 0,  0,  1,  0 },
            { 0,  0,  0,  1 }
        }
    };
    return result;
}



extern m4x4
Columns3x3(v3 X, v3 Y, v3 Z)
{
    m4x4 R =
    {
        {   {X.x, Y.x, Z.x, 0},
            {X.y, Y.y, Z.y, 0},
            {X.z, Y.z, Z.z, 0},
            {  0,   0,   0, 1}
        }
    };

    return (R);
}

extern m4x4
Rows3x3(v3 X, v3 Y, v3 Z)
{
    m4x4 R =
    {
        {   {X.x, X.y, X.z, 0},
            {Y.x, Y.y, Y.z, 0},
            {Z.x, Z.y, Z.z, 0},
            {  0,   0,   0, 1}
        }
    };

    return (R);
}

inline v3
GetColumn(m4x4 A, u32 C)
{
    v3 R = {A.E[0][C], A.E[1][C], A.E[2][C]};
    return (R);
}

inline v3
GetRow(m4x4 A, u32 R)
{
    v3 Result = {A.E[R][0], A.E[R][1], A.E[R][2]};
    return (Result);
}

inline m4x4
Transpose(m4x4 A)
{
    m4x4 R;

    for (int j = 0; j <= 3; ++j)
    {
        for (int i = 0; i <= 3; ++i)
        {
            R.E[j][i] = A.E[i][j];
        }
    }

    return (R);
}

extern m4x4
Translate(m4x4 A, v3 T)
{
    m4x4 R = A;
    R.E[0][3] += T.x;
    R.E[1][3] += T.y;
    R.E[2][3] += T.z;
    return (R);
}

extern m4x4
CameraTransform(v3 X, v3 Y, v3 Z, v3 P)
{
    // TODO(casey): It seems really suspicious that unary negation binds first
    // to the m4x4... is that actually the C++ grammar?  I guess it is :(
    m4x4 R = Rows3x3(X, Y, Z);
    R = Translate(R, -(R * P));
    return (R);
}

inline m4x4
Perspective(r32 aspect, r32 fov, r32 znear, r32 zfar)
{
    r32 s = _tan(DEG2RAD(fov) * .5f)  * znear;
    r32 r = aspect * s, l = -r, t = s, b = -t;
    r32 n = znear;
    r32 f = zfar;

    r32 t1 = (r + l) / (r - l);
    r32 t2 = (t + b) / (t - b);

    r32 _a = 2 * n / (r - l);
    r32 _b = 2 * n / (t - b);
    r32 _c = -(f + n) / (f - n);
    r32 _d = -2 * f * n / (f - n);
    m4x4 R =
    {
        {
            { _a,  0,  0,  0 },
            {  0, _b,  0,  0 },
            {  0,  0, _c, _d },
            {  0,  0, -1,  0 }
        }
    };
    return R;
}

inline m4x4
Orthographic(r32 left, r32 right, r32 top, r32 bottom, r32 znear, r32 zfar)
{
    r32 a = 2 / (right - left);
    r32 b = 2 / (top - bottom);
    r32 c = -2 / (zfar - znear);
    r32 d = -( (zfar + znear) / (zfar - znear) );
    r32 e = -( (right + left) / (right - left) );
    r32 f = -( (top + bottom) / (top - bottom) );
    m4x4 R =
    {
        {
            {  a,  0,  0,  e },
            {  0,  b,  0,  f },
            {  0,  0,  c,  d },
            {  0,  0,  0,  1 }
        }
    };
    return (R);
}

inline m4x4
CameraOrbit(v3 eye, r32 pitch, r32 yaw)
{
    m4x4 cam = YRotation(yaw) * XRotation(pitch);
    m4x4 R = Rows3x3(GetColumn(cam, 0), GetColumn(cam, 1), GetColumn(cam, 2));
    R = Translate(R, -(R * eye));
    return (R);
}

inline m4x4
CameraLookAt(v3 eye, v3 target, v3 up)
{
    v3 f  = Normalize(eye - target);
    v3 s  = Normalize(Cross(up, f));
    v3 _u = Cross(f, s);
    v3 _e = V3(-Dot(s, eye), -Dot(_u, eye), -Dot(f, eye));
    m4x4 R = Translate(Rows3x3(s, _u, f), _e);
    return (R);
}

#define _GAI_MATH_H_
#endif