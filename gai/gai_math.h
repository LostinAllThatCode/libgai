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

inline r32
gaiMathLerp(r32 A, r32 B, r32 t)
{
    r32 Result;
    Result = A + t*(B-A);
    return Result;
}

// NOTE: 2D vector implementation
union v2
{
    struct
    {
        r32 x, y;
    };
    struct
    {
        r32 u, v;
    };
    struct
    {
        r32 w, h;
    };
    r32 E[2];
};

// NOTE: 2d vector initialization
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

// NOTE: 2D vector operator overloading
inline v2
operator*(r32 A, v2 B)
{
    v2 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    
    return(Result);
}

inline v2
operator*(v2 B, r32 A)
{
    v2 Result = A*B;

    return(Result);
}

inline v2 &
operator*=(v2 &B, r32 A)
{
    B = A * B;

    return(B);
}

inline v2
operator-(v2 A)
{
    v2 Result;

    Result.x = -A.x;
    Result.y = -A.y;

    return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
    A = A + B;

    return(A);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

inline v2 &
operator-=(v2 &A, v2 B)
{
    A = A - B;

    return(A);
}

// NOTE: 3D vector implementation

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

// NOTE: 3D vector initialization
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

// NOTE: 3D vector operator overloading
inline v3
operator*(r32 A, v3 B)
{
    v3 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    
    return(Result);
}

inline v3
operator*(v3 B, r32 A)
{
    v3 Result = A*B;

    return(Result);
}

inline v3 &
operator*=(v3 &B, r32 A)
{
    B = A * B;

    return(B);
}

inline v3
operator-(v3 A)
{
    v3 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;

    return(Result);
}

inline v3
operator+(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return(Result);
}

inline v3 &
operator+=(v3 &A, v3 B)
{
    A = A + B;

    return(A);
}

inline v3
operator-(v3 A, v3 B)
{
    v3 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;

    return(Result);
}

inline v3 &
operator-=(v3 &A, v3 B)
{
    A = A - B;

    return(A);
}

// NOTE: 4D vector implementation
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

// NOTE: 4D vector initialization
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

// NOTE: 4D vector operator overloading
inline v4
operator*(r32 A, v4 B)
{
    v4 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    Result.w = A*B.w;
    
    return(Result);
}

inline v4
operator*(v4 B, r32 A)
{
    v4 Result = A*B;

    return(Result);
}

inline v4 &
operator*=(v4 &B, r32 A)
{
    B = A * B;

    return(B);
}

inline v4
operator-(v4 A)
{
    v4 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;
    Result.w = -A.w;

    return(Result);
}

inline v4
operator+(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;
    Result.w = A.w + B.w;

    return(Result);
}

inline v4 &
operator+=(v4 &A, v4 B)
{
    A = A + B;

    return(A);
}

inline v4
operator-(v4 A, v4 B)
{
    v4 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;
    Result.w = A.w - B.w;

    return(Result);
}

inline v4 &
operator-=(v4 &A, v4 B)
{
    A = A - B;

    return(A);
}

// NOTE: General vector math operations

// NOTE: Dot product / inner product of two vectors
inline r32
gaiMathDotV3(v3 A, v3 B)
{
    r32 Result = A.x*B.x + A.y*B.y + A.z*B.z;

    return(Result);
}

inline r32
gaiMathDotV4(v4 A, v4 B)
{
    r32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

    return(Result);
}

inline v3
gaiMathCrossV3(v3 A, v3 B)
{
    v3 Result;
    Result.x = (A.y*B.z) - (A.z*B.y);
    Result.y = (A.z*B.x) - (A.x*B.z);
    Result.z = (A.x*B.y) - (A.y*B.x);
    return Result;
}

inline v4
gaiMathCrossV4(v4 A, v4 B)
{
    v4 Result;
    Result.x = (A.y*B.z) - (A.z*B.y);
    Result.y = (A.z*B.w) - (A.w*B.z);
    Result.z = (A.w*B.x) - (A.x*B.w);
    Result.w = (A.x*B.y) - (A.y*B.x);
    return Result;
}

inline r32
gaiMathLengthSqV3(v3 A)
{
    r32 Result = gaiMathDotV3(A, A);
    return(Result);
}

inline r32
gaiMathLengthSqV4(v4 A)
{
    r32 Result = gaiMathDotV4(A, A);
    return(Result);
}

inline r32
gaiMathLengthV3(v3 A)
{
    r32 Result = sqrt(gaiMathLengthSqV3(A));
    return(Result);
}

inline r32
gaiMathLengthV4(v4 A)
{
    r32 Result = sqrt(gaiMathLengthSqV4(A));
    return(Result);
}

inline v3
gaiMathNormalizeV3(v3 A)
{
    v3 Result = A * (1.0f / gaiMathLengthV3(A));
    return(Result);
}

inline v4
gaiMathNormalizeV4(v4 A)
{
    v4 Result = A * (1.0f / gaiMathLengthV4(A));
    return(Result);
}

inline v3
gaiMathLerpV3(v3 A, v3 B, r32 t)
{
    v3 Result;
    Result = A + t*(B-A);
    return Result;
}

// NOTE: 3x3 and 4x4 matrix implementation (opengl coloum order)
union mat3x3
{
    struct
    {
        r32 m0, m3, m6;
        r32 m1, m4, m7;
        r32 m2, m5, m8;
    };
    r32 E[9];
};

union mat4x4
{
    struct
    {
        r32 m0, m4, m8,  m12;
        r32 m1, m5, m9,  m13;
        r32 m2, m6, m10, m14;
        r32 m3, m7, m11, m15;
    };
    r32 E[16];
};

// NOTE: General 3x3 and 4x4 matrix operations
inline mat3x3
Mat3(r32 e)
{
  mat3x3 Result = {
      e, 0.0f, 0.0f,
      0.0f, e, 0.0f,
      0.0f, 0.0f, e
  };
  return Result;  
}

inline mat3x3
Mat3(i32 e)
{
    mat3x3 Result = {
        (r32) e, 0.0f, 0.0f,
        0.0f, (r32) e, 0.0f,
        0.0f, 0.0f, (r32) e
    };
    return Result;  
}

inline mat4x4
Mat4(r32 e)
{
  mat4x4 Result = {
      e, 0.0f, 0.0f, 0.0f, 
      0.0f, e, 0.0f, 0.0f,
      0.0f, 0.0f, e, 0.0f,
      0.0f, 0.0f, 0.0f, e
  };
  return Result;  
}

inline mat4x4
Mat4(i32 e)
{
  mat4x4 Result = {
      (r32) e, 0.0f, 0.0f, 0.0f, 
      0.0f, (r32) e, 0.0f, 0.0f,
      0.0f, 0.0f, (r32) e, 0.0f,
      0.0f, 0.0f, 0.0f, (r32) e
  };
  return Result;  
}

inline mat4x4
gaiMathTrlMat(r32 x, r32 y, r32 z)
{
    mat4x4 Result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    z,    1.0f
    };
    return Result;
}

inline mat4x4
gaiMathSclMat(r32 x, r32 y, r32 z)
{
    mat4x4 Result = {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Result;
}

inline mat4x4
gaiMathRotMat(r32 Angle, r32 x, r32 y, r32 z)
{
    r32 s = sinf(Angle);
    mat4x4 Result = {
        
    };
    return Result;    
}

inline mat3x3
gaiMathMulMat3(mat3x3 A, mat3x3 B)
{
    mat3x3 Result;
    Result.m0 = (A.m0 * B.m0) + (A.m3 * B.m1) + (A.m6 * B.m2);
    Result.m3 = (A.m0 * B.m3) + (A.m3 * B.m4) + (A.m6 * B.m5);
    Result.m6 = (A.m0 * B.m6) + (A.m3 * B.m7) + (A.m6 * B.m8);
    Result.m1 = (A.m1 * B.m0) + (A.m4 * B.m1) + (A.m7 * B.m2);
    Result.m4 = (A.m1 * B.m3) + (A.m4 * B.m4) + (A.m7 * B.m5);
    Result.m7 = (A.m1 * B.m6) + (A.m4 * B.m7) + (A.m7 * B.m8);
    Result.m2 = (A.m2 * B.m0) + (A.m5 * B.m1) + (A.m8 * B.m2);
    Result.m5 = (A.m2 * B.m3) + (A.m5 * B.m4) + (A.m8 * B.m5);
    Result.m8 = (A.m2 * B.m6) + (A.m5 * B.m7) + (A.m8 * B.m8);
    return Result;
}

inline mat3x3
gaiMathMulMat3(mat3x3 A, r32 V)
{
    mat3x3 Result;
    Result.m0 *= V; Result.m3 *= V; Result.m6 *= V;
    Result.m1 *= V; Result.m4 *= V; Result.m7 *= V;
    Result.m2 *= V; Result.m5 *= V; Result.m8 *= V;
    return Result;
}

inline mat4x4
gaiMathMulMat4(mat4x4 A, mat4x4 B)
{
    mat4x4 Result;
    Result.m0  = (A.m0 * B.m0)  + (A.m4 * B.m1)  + (A.m8  * B.m2)  + (A.m12 * B.m3);
    Result.m1  = (A.m1 * B.m0)  + (A.m5 * B.m1)  + (A.m9  * B.m2)  + (A.m13 * B.m3);
    Result.m2  = (A.m2 * B.m0)  + (A.m6 * B.m1)  + (A.m10 * B.m2)  + (A.m14 * B.m3);
    Result.m3  = (A.m3 * B.m0)  + (A.m7 * B.m1)  + (A.m11 * B.m2)  + (A.m15 * B.m3);
    Result.m4  = (A.m0 * B.m4)  + (A.m4 * B.m5)  + (A.m8  * B.m6)  + (A.m12 * B.m7);
    Result.m5  = (A.m1 * B.m4)  + (A.m5 * B.m5)  + (A.m9  * B.m6)  + (A.m13 * B.m7);
    Result.m6  = (A.m2 * B.m4)  + (A.m6 * B.m5)  + (A.m10 * B.m6)  + (A.m14 * B.m7);
    Result.m7  = (A.m3 * B.m4)  + (A.m7 * B.m5)  + (A.m11 * B.m6)  + (A.m15 * B.m7);
    Result.m8  = (A.m0 * B.m8)  + (A.m4 * B.m9)  + (A.m8  * B.m10) + (A.m12 * B.m11);
    Result.m9  = (A.m1 * B.m8)  + (A.m5 * B.m9)  + (A.m9  * B.m10) + (A.m13 * B.m11);
    Result.m10 = (A.m2 * B.m8)  + (A.m6 * B.m9)  + (A.m10 * B.m10) + (A.m14 * B.m11);
    Result.m11 = (A.m3 * B.m8)  + (A.m7 * B.m9)  + (A.m11 * B.m10) + (A.m15 * B.m11);
    Result.m12 = (A.m0 * B.m12) + (A.m4 * B.m13) + (A.m8  * B.m14) + (A.m12 * B.m15);
    Result.m13 = (A.m1 * B.m12) + (A.m5 * B.m13) + (A.m9  * B.m14) + (A.m13 * B.m15);
    Result.m14 = (A.m2 * B.m12) + (A.m6 * B.m13) + (A.m10 * B.m14) + (A.m14 * B.m15);
    Result.m15 = (A.m3 * B.m12) + (A.m7 * B.m13) + (A.m11 * B.m14) + (A.m15 * B.m15);
    return Result;
}

inline mat4x4
gaiMathMulMat4(mat4x4 A, r32 V)
{
    mat4x4 Result;
    Result.m0 *= V; Result.m4 *= V; Result.m8 *= V;  Result.m12 *= V; 
    Result.m1 *= V; Result.m5 *= V; Result.m9 *= V;  Result.m13 *= V; 
    Result.m2 *= V; Result.m6 *= V; Result.m10 *= V; Result.m14 *= V; 
    Result.m3 *= V; Result.m7 *= V; Result.m11 *= V; Result.m15 *= V; 
    return Result;
}

inline mat4x4
gaiMathTransposeMat4(mat4x4 M)
{
    mat4x4 Temp;
    Temp.m0 = M.m0;   Temp.m4 = M.m1;   Temp.m8 = M.m2;   Temp.m12 = M.m3;
    Temp.m1 = M.m4;   Temp.m5 = M.m5;   Temp.m9 = M.m6;   Temp.m13 = M.m7;
    Temp.m2 = M.m8;   Temp.m6 = M.m9;   Temp.m10 = M.m10; Temp.m14 = M.m11;
    Temp.m3 = M.m12;  Temp.m7 = M.m13;  Temp.m11 = M.m14; Temp.m15 = M.m15;
    return Temp;
}

/*
inline r32
DeterminantMat4x4(mat4x4 A)
{
    r32 Result =
        (A.m0 * (A.m5 * A.m10 * A.m15 + A.m9 * A.m14 * A.m7 + A.m13 * A.m6 * A.m11 - A.m7 * A.m10 * A.m13 - A.m11 * A.m14 * A.m5 - A.m15 * A.m6 * A.m9)) -
        (A.m1 * (A.m4 * A.m10 * A.m15 + A.m8 * A.m14 * A.m7 + A.m12 * A.m6 * A.m11 - A.m7 * A.m10 * A.m12 - A.m11 * A.m14 * A.m4 - A.m15 * A.m6 * A.m8)) +
        (A.m2 * (A.m4 * A.m9 * A.m15 + A.m8 * A.m13 * A.m7 + A.m12 * A.m5 * A.m11 - A.m7 * A.m9 * A.m12 - A.m11 * A.m13 * A.m4 - A.m15 * A.m5 * A.m8)) -
        (A.m3 * (A.m4 * A.m9 * A.m14 + A.m8 * A.m13 * A.m6 + A.m12 * A.m5 * A.m10 - A.m6 * A.m9 * A.m12 - A.m10 * A.m13 * A.m4 - A.m14 * A.m5 * A.m8));
     return Result;
}
*/


inline r32
gaiMathDeterminantMat4(mat4x4 A)
{
    r32 det = 0;
    r32 *m = A.E;
    r32 inv[16];
    
    inv[0]  = m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] + m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
    inv[4]  = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] - m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
    inv[8]  = m[4]  * m[9] * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5] * m[15] + m[8]  * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4]  * m[9] * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5] * m[14] - m[8]  * m[6] * m[13] - m[12] * m[5] * m[10] +  m[12] * m[6] * m[9];
    inv[1]  = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2] * m[15] - m[9]  * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5]  = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2] * m[15] + m[8]  * m[3] * m[14] +  m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9]  = -m[0]  * m[9] * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1] * m[15] - m[8]  * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0]  * m[9] * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1] * m[14] + m[8]  * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2]  = m[1]  * m[6] * m[15] -  m[1]  * m[7] * m[14] - m[5]  * m[2] * m[15] + m[5]  * m[3] * m[14] +  m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6]  = -m[0]  * m[6] * m[15] + m[0]  * m[7] * m[14] + m[4]  * m[2] * m[15] - m[4]  * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0]  * m[5] * m[15] - m[0]  * m[7] * m[13] - m[4]  * m[1] * m[15] + m[4]  * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0]  * m[5] * m[14] + m[0]  * m[6] * m[13] + m[4]  * m[1] * m[14] - m[4]  * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3]  = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7]  = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    return det;
}


inline mat4x4
gaiMathInverseMat4(mat4x4 A)
{
    mat4x4 Result = A;
    r32 *m = A.E;    
    r32 det = 0;
    r32 inv[16];
    
    inv[0]  = m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] + m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
    inv[4]  = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] - m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
    inv[8]  = m[4]  * m[9] * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5] * m[15] + m[8]  * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4]  * m[9] * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5] * m[14] - m[8]  * m[6] * m[13] - m[12] * m[5] * m[10] +  m[12] * m[6] * m[9];
    inv[1]  = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2] * m[15] - m[9]  * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5]  = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2] * m[15] + m[8]  * m[3] * m[14] +  m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9]  = -m[0]  * m[9] * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1] * m[15] - m[8]  * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0]  * m[9] * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1] * m[14] + m[8]  * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2]  = m[1]  * m[6] * m[15] -  m[1]  * m[7] * m[14] - m[5]  * m[2] * m[15] + m[5]  * m[3] * m[14] +  m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6]  = -m[0]  * m[6] * m[15] + m[0]  * m[7] * m[14] + m[4]  * m[2] * m[15] - m[4]  * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0]  * m[5] * m[15] - m[0]  * m[7] * m[13] - m[4]  * m[1] * m[15] + m[4]  * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0]  * m[5] * m[14] + m[0]  * m[6] * m[13] + m[4]  * m[1] * m[14] - m[4]  * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3]  = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7]  = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    
    if(det)
    {
        det = 1.0f / det;
        for (int i=0;i<16;i++)
        {
            Result.E[i] = inv[i] * det;
        }
    }
    return Result;   
}

inline mat4x4
gaiMathFrustum(r32 Left, r32 Right, r32 Bottom, r32 Top, r32 Near, r32 Far)
{
    mat4x4 Result = Mat4(0);
#if 0
    Result.m0  = (Near * 2.0f) / (Right - Left);
    Result.m5  = (Near * 2.0f) / (Top - Bottom);
    Result.m8  = (Right + Left) / (Right - Left);
    Result.m9  = (Top + Bottom) / (Top - Bottom);
    Result.m10 = -((Far + Near) / (Far - Near));
    Result.m11 = -1.0f;
    Result.m14 = -((2.0f * Far * Near) / (Far - Near));
    Result = gaiMathTransposeMat4(Result);
#else
    Result.m0  = (Near * 2.0f) / (Right - Left);
    Result.m2  = (Right + Left) / (Right - Left);
    Result.m5  = (Near * 2.0f) / (Top - Bottom);
    Result.m6  = (Top + Bottom) / (Top - Bottom);
    Result.m10 = -(Far + Near) / (Far - Near);
    Result.m11 = -(2.0f * Far * Near) / (Far - Near);
    Result.m14 = -1.0f;
#endif
    
    return Result;
}

inline mat4x4
gaiMathPerspective(r32 FoV, r32 Aspect, r32 Near, r32 Far)
{
    r32 Top   = Near * tanf(FoV * M_PI / 360);
    r32 Right = Top * Aspect;
    return gaiMathFrustum(-Right, Right, -Top, Top, Near, Far);
}

inline mat4x4
gaiMathLookAt(v3 Eye, v3 Target, v3 Up)
{
    mat4x4 Result = Mat4(0);
    v3 z = gaiMathNormalizeV3(Eye - Target);
    v3 x = gaiMathNormalizeV3(gaiMathCrossV3(Up, z));
    v3 y = gaiMathNormalizeV3(gaiMathCrossV3(z, x));
    Result.m0 = x.x;
    Result.m1 = x.y;
    Result.m2 = x.z;
    Result.m3 = gaiMathDotV3(x, -Eye);
    Result.m4 = y.x;
    Result.m5 = y.y;
    Result.m6 = y.z;
    Result.m7 = gaiMathDotV3(y, -Eye);
    Result.m8 = z.x;
    Result.m9 = z.y;
    Result.m10 = z.z;
    Result.m11 = gaiMathDotV3(z, -Eye);
    Result.m15 = 1.0f;
    return Result;
}

inline mat4x4
gaiMathOrtho(r32 Left, r32 Right, r32 Bottom, r32 Top, r32 Near, r32 Far)
{
    mat4x4 Result = Mat4(0);
    Result.m0 = 2 / (Right - Left);
    Result.m5 = 2 / (Top - Bottom);
    Result.m10 = -2 / (Far - Near);
    Result.m12 = -((Right + Left) / (Right - Left));
    Result.m13 = -((Top + Bottom) / (Top - Bottom));
    Result.m14 = -((Far + Near) / (Far - Near));
    Result.m15 = 1;
    
    Result = gaiMathTransposeMat4(Result);
    return Result;
}

#define _GAI_MATH_H_
#endif
