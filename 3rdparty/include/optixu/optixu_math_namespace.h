
/*
 * Copyright (c) 1993 - 2010 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.  Users and possessors of this source code
 * are hereby granted a nonexclusive, royalty-free license to use this code
 * in individual and commercial software.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.   This source code is a "commercial item" as
 * that term is defined at  48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer  software"  and "commercial computer software
 * documentation" as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 *
 * Any use of this source code in individual and commercial software must
 * include, in the user documentation and internal comments to the code,
 * the above Disclaimer and U.S. Government End Users Notice.
 */

/*
    This file implements common mathematical operations on vector types
    (float3, float4 etc.) since these are not provided as standard by CUDA.

    The syntax is modelled on the Cg standard library.

    This file has also been modified from the original cutil_math.h file.
    cutil_math.h is a subset of this file, and you should use this file in place
    of any cutil_math.h file you wish to use.
*/

#ifndef __optixu_optixu_math_namespace_h__
#define __optixu_optixu_math_namespace_h__

#include "../internal/optix_declarations.h" // For RT_HOSTDEVICE
#include "../internal/optix_datatypes.h"    // For optix::Ray
#include "optixu_vector_functions.h"
#include "../optix_sizet.h"

#if !defined(_WIN32)
// On posix systems uint and ushort are defined when including this file, so we need to
// guarantee this file gets included in order to get these typedefs.
#  include <sys/types.h>
#endif

// #define these constants such that we are sure
// 32b floats are emitted in ptx
#ifndef M_Ef
#define M_Ef        2.71828182845904523536f
#endif
#ifndef M_LOG2Ef
#define M_LOG2Ef    1.44269504088896340736f
#endif
#ifndef M_LOG10Ef
#define M_LOG10Ef   0.434294481903251827651f
#endif
#ifndef M_LN2f
#define M_LN2f      0.693147180559945309417f
#endif
#ifndef M_LN10f
#define M_LN10f     2.30258509299404568402f
#endif
#ifndef M_PIf
#define M_PIf       3.14159265358979323846f
#endif
#ifndef M_PI_2f
#define M_PI_2f     1.57079632679489661923f
#endif
#ifndef M_PI_4f
#define M_PI_4f     0.785398163397448309616f
#endif
#ifndef M_1_PIf
#define M_1_PIf     0.318309886183790671538f
#endif
#ifndef M_2_PIf
#define M_2_PIf     0.636619772367581343076f
#endif
#ifndef M_2_SQRTPIf
#define M_2_SQRTPIf 1.12837916709551257390f
#endif
#ifndef M_SQRT2f
#define M_SQRT2f    1.41421356237309504880f
#endif
#ifndef M_SQRT1_2f
#define M_SQRT1_2f  0.707106781186547524401f
#endif


// __forceinline__ works in cuda, VS, and with gcc.  Leave it as macro in case
// we need to make this per-platform or we want to switch off inlining globally.
#ifndef OPTIXU_INLINE 
#  define OPTIXU_INLINE_DEFINED 1
#  define OPTIXU_INLINE __forceinline__
#endif // OPTIXU_INLINE 

/******************************************************************************/
namespace optix {
#if defined(_WIN32) && !defined(RT_UINT_USHORT_DEFINED)
  // uint and ushort are not already defined on Windows systems or they could have been
  // defined in optixu_math.h.
  typedef unsigned int uint;
  typedef unsigned short ushort;
#else
  // On Posix systems these typedefs are defined in the global namespace, and to avoid
  // conflicts, we'll pull them into this namespace for consistency.
  using ::uint;
  using ::ushort;
#endif //defined(_WIN32)
} // end namespace optix

#if !defined(__CUDACC__)
/* Functions that CUDA provides for device code but are lacking on some host platform */

#include <math.h>


// We need to declare these functions to define them in Windows and to override the system
// library version in Posix systems (it's declared extern).  On Posix the system versions
// are not inlined and cause slower perforance.  In addition on non-Windows systems we
// can't declare them in a namespace, because we need to override the one declared extern
// in the global namespace and subsequent overloaded versions of need to qualify their
// call with the global namespace to avoid auto-casting from float to float3 and friends.

#if defined(_WIN32)
namespace optix {
#endif

OPTIXU_INLINE float fminf(float a, float b)
{
  return a < b ? a : b;
}

OPTIXU_INLINE float fmaxf(float a, float b)
{
  return a > b ? a : b;
}

/* copy sign-bit from src value to dst value */
OPTIXU_INLINE float copysignf(float dst, float src)
{
  union {
    float f;
    unsigned int i;
  } v1, v2, v3;
  v1.f = src;
  v2.f = dst;
  v3.i = (v2.i & 0x7fffffff) | (v1.i & 0x80000000);

  return v3.f;
}



#if defined(_WIN32)
} // end namespace optix
#endif

#endif // #ifndef __CUDACC__

namespace optix {
  // On Posix systems these functions are defined in the global namespace, but we need to
  // pull them into the optix namespace in order for them to be on the same level as
  // the other overloaded functions in optix::.

#if !defined(_WIN32) || defined (__CUDACC__)
  // These functions are in the global namespace on POSIX (not _WIN32) and in CUDA C.
  using ::fminf;
  using ::fmaxf;
  using ::copysignf;
#endif
  using ::expf;
  using ::floorf;

  // These are defined by CUDA in the global namespace.
#ifdef __CUDACC__
  using ::min;
  using ::max;
#else
#if defined(_WIN32) && !defined(NOMINMAX)
#  error "optixu_math_namespace.h needs NOMINMAX defined on windows."
#endif
  OPTIXU_INLINE int max(int a, int b)
  {
    return a > b ? a : b;
  }

  OPTIXU_INLINE int min(int a, int b)
  {
    return a < b ? a : b;
  }
#endif

} // end namespace optix


namespace optix {

/* Bit preserving casting functions */
/******************************************************************************/

#ifdef __CUDACC__

  using ::float_as_int;
  using ::int_as_float;

#else

OPTIXU_INLINE int float_as_int( float f )
{
  union {
    float f;
    int i;
  } v1;

  v1.f = f;
  return v1.i;
}


OPTIXU_INLINE float int_as_float( int i )
{
  union {
    float f;
    int i;
  } v1;

  v1.i = i;
  return v1.f;
}

#endif 


/* float functions */
/******************************************************************************/

/* lerp */
OPTIXU_INLINE RT_HOSTDEVICE float lerp(float a, float b, float t)
{
    return a + t*(b-a);
}

/* bilerp */
OPTIXU_INLINE RT_HOSTDEVICE float bilerp(float x00, float x10, float x01, float x11,
                                  float u, float v)
{
  return lerp( lerp( x00, x10, u ), lerp( x01, x11, u ), v );
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE float clamp(float f, float a, float b)
{
    return fmaxf(a, fminf(f, b));
}

/* int2 functions */
/******************************************************************************/

/* negate */
OPTIXU_INLINE RT_HOSTDEVICE int2 operator-(int2 a)
{
    return make_int2(-a.x, -a.y);
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE int2 operator+(int2 a, int2 b)
{
    return make_int2(a.x + b.x, a.y + b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(int2 &a, int2 b)
{
    a.x += b.x; a.y += b.y;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE int2 operator-(int2 a, int2 b)
{
    return make_int2(a.x - b.x, a.y - b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE int2 operator-(int2 a, int b)
{
    return make_int2(a.x - b, a.y - b);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator-=(int2 &a, int2 b)
{
    a.x -= b.x; a.y -= b.y;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE int2 operator*(int2 a, int2 b)
{
    return make_int2(a.x * b.x, a.y * b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE int2 operator*(int2 a, int s)
{
    return make_int2(a.x * s, a.y * s);
}
OPTIXU_INLINE RT_HOSTDEVICE int2 operator*(int s, int2 a)
{
    return make_int2(a.x * s, a.y * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(int2 &a, int s)
{
    a.x *= s; a.y *= s;
}

/* float2 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE float2 make_float2(float s)
{
    return make_float2(s, s);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 make_float2(int2 a)
{
    return make_float2(float(a.x), float(a.y));
}

/* negate */
OPTIXU_INLINE RT_HOSTDEVICE float2 operator-(float2 a)
{
    return make_float2(-a.x, -a.y);
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE float2 operator+(float2 a, float2 b)
{
    return make_float2(a.x + b.x, a.y + b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator+(float2 a, float b)
{
    return make_float2(a.x + b, a.y + b);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator+(float a, float2 b)
{
    return make_float2(a + b.x, a + b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(float2 &a, float2 b)
{
    a.x += b.x; a.y += b.y;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE float2 operator-(float2 a, float2 b)
{
    return make_float2(a.x - b.x, a.y - b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator-(float2 a, float b)
{
    return make_float2(a.x - b, a.y - b);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator-(float a, float2 b)
{
    return make_float2(a - b.x, a - b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator-=(float2 &a, float2 b)
{
    a.x -= b.x; a.y -= b.y;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE float2 operator*(float2 a, float2 b)
{
    return make_float2(a.x * b.x, a.y * b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator*(float2 a, float s)
{
    return make_float2(a.x * s, a.y * s);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator*(float s, float2 a)
{
    return make_float2(a.x * s, a.y * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float2 &a, float2 s)
{
    a.x *= s.x; a.y *= s.y;
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float2 &a, float s)
{
    a.x *= s; a.y *= s;
}

/* divide */
OPTIXU_INLINE RT_HOSTDEVICE float2 operator/(float2 a, float2 b)
{
    return make_float2(a.x / b.x, a.y / b.y);
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator/(float2 a, float s)
{
    float inv = 1.0f / s;
    return a * inv;
}
OPTIXU_INLINE RT_HOSTDEVICE float2 operator/(float s, float2 a)
{
    return make_float2( s/a.x, s/a.y );
}
OPTIXU_INLINE RT_HOSTDEVICE void operator/=(float2 &a, float s)
{
    float inv = 1.0f / s;
    a *= inv;
}

/* lerp */
OPTIXU_INLINE RT_HOSTDEVICE float2 lerp(float2 a, float2 b, float t)
{
    return a + t*(b-a);
}

/* bilerp */
OPTIXU_INLINE RT_HOSTDEVICE float2 bilerp(float2 x00, float2 x10, float2 x01, float2 x11,
                                   float u, float v)
{
  return lerp( lerp( x00, x10, u ), lerp( x01, x11, u ), v );
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE float2 clamp(float2 v, float a, float b)
{
    return make_float2(clamp(v.x, a, b), clamp(v.y, a, b));
}

OPTIXU_INLINE RT_HOSTDEVICE float2 clamp(float2 v, float2 a, float2 b)
{
    return make_float2(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y));
}

/* dot product */
OPTIXU_INLINE RT_HOSTDEVICE float dot(float2 a, float2 b)
{
    return a.x * b.x + a.y * b.y;
}

/* length */
OPTIXU_INLINE RT_HOSTDEVICE float length(float2 v)
{
    return sqrtf(dot(v, v));
}

/* normalize */
OPTIXU_INLINE RT_HOSTDEVICE float2 normalize(float2 v)
{
    float invLen = 1.0f / sqrtf(dot(v, v));
    return v * invLen;
}

/* floor */
OPTIXU_INLINE RT_HOSTDEVICE float2 floor(const float2 v)
{
  return make_float2(::floorf(v.x), ::floorf(v.y));
}

/* reflect */
OPTIXU_INLINE RT_HOSTDEVICE float2 reflect(float2 i, float2 n)
{
	return i - 2.0f * n * dot(n,i);
}

/* faceforward */
/* Returns N if dot(i, nref) > 0; else -N; */
/* Typical usage is N = faceforward(N, -ray.dir, N); */
/* Note that this is opposite of what faceforward does in Cg and GLSL */
OPTIXU_INLINE RT_HOSTDEVICE float2 faceforward(float2 n, float2 i, float2 nref)
{
  return n * copysignf( 1.0f, dot(i, nref) );
}

/* exp */
OPTIXU_INLINE RT_HOSTDEVICE float2 expf(float2 v)
{
  return make_float2(::expf(v.x), ::expf(v.y));
}


/* float3 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(float s)
{
    return make_float3(s, s, s);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(float2 a)
{
    return make_float3(a.x, a.y, 0.0f);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(float2 a, float s)
{
    return make_float3(a.x, a.y, s);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(float4 a)
{
   return make_float3(a.x, a.y, a.z);  /* discards w */
}
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(int3 a)
{
    return make_float3(float(a.x), float(a.y), float(a.z));
}

/* negate */
OPTIXU_INLINE RT_HOSTDEVICE float3 operator-(float3 a)
{
    return make_float3(-a.x, -a.y, -a.z);
}

/* min */
OPTIXU_INLINE RT_HOSTDEVICE float3 fminf(float3 a, float3 b)
{
	return make_float3(fminf(a.x,b.x), fminf(a.y,b.y), fminf(a.z,b.z));
}

/* max */
OPTIXU_INLINE RT_HOSTDEVICE float3 fmaxf(float3 a, float3 b)
{
	return make_float3(fmaxf(a.x,b.x), fmaxf(a.y,b.y), fmaxf(a.z,b.z));
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE float3 operator+(float3 a, float3 b)
{
    return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator+(float3 a, float b)
{
    return make_float3(a.x + b, a.y + b, a.z + b);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator+(float a, float3 b)
{
    return make_float3(a + b.x, a + b.y, a + b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(float3 &a, float3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE float3 operator-(float3 a, float3 b)
{
    return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator-(float3 a, float b)
{
    return make_float3(a.x - b, a.y - b, a.z - b);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator-(float a, float3 b)
{
    return make_float3(a - b.x, a - b.y, a - b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator-=(float3 &a, float3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE float3 operator*(float3 a, float3 b)
{
    return make_float3(a.x * b.x, a.y * b.y, a.z * b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator*(float3 a, float s)
{
    return make_float3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator*(float s, float3 a)
{
    return make_float3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float3 &a, float3 s)
{
    a.x *= s.x; a.y *= s.y; a.z *= s.z;
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float3 &a, float s)
{
    a.x *= s; a.y *= s; a.z *= s;
}

/* divide */
OPTIXU_INLINE RT_HOSTDEVICE float3 operator/(float3 a, float3 b)
{
    return make_float3(a.x / b.x, a.y / b.y, a.z / b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator/(float3 a, float s)
{
    float inv = 1.0f / s;
    return a * inv;
}
OPTIXU_INLINE RT_HOSTDEVICE float3 operator/(float s, float3 a)
{
    return make_float3( s/a.x, s/a.y, s/a.z );
}
OPTIXU_INLINE RT_HOSTDEVICE void operator/=(float3 &a, float s)
{
    float inv = 1.0f / s;
    a *= inv;
}

/* lerp */
OPTIXU_INLINE RT_HOSTDEVICE float3 lerp(float3 a, float3 b, float t)
{
    return a + t*(b-a);
}

/* bilerp */
OPTIXU_INLINE RT_HOSTDEVICE float3 bilerp(float3 x00, float3 x10, float3 x01, float3 x11,
                                   float u, float v)
{
  return lerp( lerp( x00, x10, u ), lerp( x01, x11, u ), v );
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE float3 clamp(float3 v, float a, float b)
{
    return make_float3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}

OPTIXU_INLINE RT_HOSTDEVICE float3 clamp(float3 v, float3 a, float3 b)
{
    return make_float3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}

/* dot product */
OPTIXU_INLINE RT_HOSTDEVICE float dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* cross product */
OPTIXU_INLINE RT_HOSTDEVICE float3 cross(float3 a, float3 b)
{
    return make_float3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

/* length */
OPTIXU_INLINE RT_HOSTDEVICE float length(float3 v)
{
    return sqrtf(dot(v, v));
}

/* normalize */
OPTIXU_INLINE RT_HOSTDEVICE float3 normalize(float3 v)
{
    float invLen = 1.0f / sqrtf(dot(v, v));
    return v * invLen;
}

/* floor */
OPTIXU_INLINE RT_HOSTDEVICE float3 floor(const float3 v)
{
  return make_float3(::floorf(v.x), ::floorf(v.y), ::floorf(v.z));
}

/* reflect */
OPTIXU_INLINE RT_HOSTDEVICE float3 reflect(float3 i, float3 n)
{
	return i - 2.0f * n * dot(n,i);
}

/* faceforward */
/* Returns N if dot(i, nref) > 0; else -N; */
/* Typical usage is N = faceforward(N, -ray.dir, N); */
/* Note that this is opposite of what faceforward does in Cg and GLSL */
OPTIXU_INLINE RT_HOSTDEVICE float3 faceforward(float3 n, float3 i, float3 nref)
{
  return n * copysignf( 1.0f, dot(i, nref) );
}

/* exp */
OPTIXU_INLINE RT_HOSTDEVICE float3 expf(float3 v)
{
  return make_float3(::expf(v.x), ::expf(v.y), ::expf(v.z));
}


/* float4 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float s)
{
    return make_float4(s, s, s, s);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float3 a)
{
    return make_float4(a.x, a.y, a.z, 0.0f);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float3 a, float w)
{
    return make_float4(a.x, a.y, a.z, w);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(int4 a)
{
    return make_float4(float(a.x), float(a.y), float(a.z), float(a.w));
}

/* negate */
OPTIXU_INLINE RT_HOSTDEVICE float4 operator-(float4 a)
{
    return make_float4(-a.x, -a.y, -a.z, -a.w);
}

/* min */
OPTIXU_INLINE RT_HOSTDEVICE float4 fminf(float4 a, float4 b)
{
	return make_float4(fminf(a.x,b.x), fminf(a.y,b.y), fminf(a.z,b.z), fminf(a.w,b.w));
}

/* max */
OPTIXU_INLINE RT_HOSTDEVICE float4 fmaxf(float4 a, float4 b)
{
	return make_float4(fmaxf(a.x,b.x), fmaxf(a.y,b.y), fmaxf(a.z,b.z), fmaxf(a.w,b.w));
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE float4 operator+(float4 a, float4 b)
{
    return make_float4(a.x + b.x, a.y + b.y, a.z + b.z,  a.w + b.w);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator+(float4 a, float b)
{
    return make_float4(a.x + b, a.y + b, a.z + b,  a.w + b);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator+(float a, float4 b)
{
    return make_float4(a + b.x, a + b.y, a + b.z,  a + b.w);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(float4 &a, float4 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE float4 operator-(float4 a, float4 b)
{
    return make_float4(a.x - b.x, a.y - b.y, a.z - b.z,  a.w - b.w);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator-(float4 a, float b)
{
    return make_float4(a.x - b, a.y - b, a.z - b,  a.w - b);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator-(float a, float4 b)
{
    return make_float4(a - b.x, a - b.y, a - b.z,  a - b.w);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator-=(float4 &a, float4 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE float4 operator*(float4 a, float4 s)
{
    return make_float4(a.x * s.x, a.y * s.y, a.z * s.z, a.w * s.w);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator*(float4 a, float s)
{
    return make_float4(a.x * s, a.y * s, a.z * s, a.w * s);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator*(float s, float4 a)
{
    return make_float4(a.x * s, a.y * s, a.z * s, a.w * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float4 &a, float4 s)
{
    a.x *= s.x; a.y *= s.y; a.z *= s.z; a.w *= s.w;
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(float4 &a, float s)
{
    a.x *= s; a.y *= s; a.z *= s; a.w *= s;
}

/* divide */
OPTIXU_INLINE RT_HOSTDEVICE float4 operator/(float4 a, float4 b)
{
    return make_float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator/(float4 a, float s)
{
    float inv = 1.0f / s;
    return a * inv;
}
OPTIXU_INLINE RT_HOSTDEVICE float4 operator/(float s, float4 a)
{
    return make_float4( s/a.x, s/a.y, s/a.z, s/a.w );
}
OPTIXU_INLINE RT_HOSTDEVICE void operator/=(float4 &a, float s)
{
    float inv = 1.0f / s;
    a *= inv;
}

/* lerp */
OPTIXU_INLINE RT_HOSTDEVICE float4 lerp(float4 a, float4 b, float t)
{
    return a + t*(b-a);
}

/* bilerp */
OPTIXU_INLINE RT_HOSTDEVICE float4 bilerp(float4 x00, float4 x10, float4 x01, float4 x11,
                                   float u, float v)
{
  return lerp( lerp( x00, x10, u ), lerp( x01, x11, u ), v );
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE float4 clamp(float4 v, float a, float b)
{
    return make_float4(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b), clamp(v.w, a, b));
}

OPTIXU_INLINE RT_HOSTDEVICE float4 clamp(float4 v, float4 a, float4 b)
{
    return make_float4(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z), clamp(v.w, a.w, b.w));
}

/* dot product */
OPTIXU_INLINE RT_HOSTDEVICE float dot(float4 a, float4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/* length */
OPTIXU_INLINE RT_HOSTDEVICE float length(float4 r)
{
    return sqrtf(dot(r, r));
}

/* normalize */
OPTIXU_INLINE RT_HOSTDEVICE float4 normalize(float4 v)
{
    float invLen = 1.0f / sqrtf(dot(v, v));
    return v * invLen;
}

/* floor */
OPTIXU_INLINE RT_HOSTDEVICE float4 floor(const float4 v)
{
  return make_float4(::floorf(v.x), ::floorf(v.y), ::floorf(v.z), ::floorf(v.w));
}

/* reflect */
OPTIXU_INLINE RT_HOSTDEVICE float4 reflect(float4 i, float4 n)
{
	return i - 2.0f * n * dot(n,i);
}

/* faceforward */
/* Returns N if dot(i, nref) > 0; else -N; */
/* Typical usage is N = faceforward(N, -ray.dir, N); */
/* Note that this is opposite of what faceforward does in Cg and GLSL */
OPTIXU_INLINE RT_HOSTDEVICE float4 faceforward(float4 n, float4 i, float4 nref)
{
  return n * copysignf( 1.0f, dot(i, nref) );
}

/* exp */
OPTIXU_INLINE RT_HOSTDEVICE float4 expf(float4 v)
{
  return make_float4(::expf(v.x), ::expf(v.y), ::expf(v.z), ::expf(v.w));
}

/* int3 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE int3 make_int3(int s)
{
    return make_int3(s, s, s);
}
OPTIXU_INLINE RT_HOSTDEVICE int3 make_int3(float3 a)
{
    return make_int3(int(a.x), int(a.y), int(a.z));
}

/* negate */
OPTIXU_INLINE RT_HOSTDEVICE int3 operator-(int3 a)
{
    return make_int3(-a.x, -a.y, -a.z);
}

/* min */
OPTIXU_INLINE RT_HOSTDEVICE int3 min(int3 a, int3 b)
{
    return make_int3(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}

/* max */
OPTIXU_INLINE RT_HOSTDEVICE int3 max(int3 a, int3 b)
{
    return make_int3(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE int3 operator+(int3 a, int3 b)
{
    return make_int3(a.x + b.x, a.y + b.y, a.z + b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(int3 &a, int3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE int3 operator-(int3 a, int3 b)
{
    return make_int3(a.x - b.x, a.y - b.y, a.z - b.z);
}

OPTIXU_INLINE RT_HOSTDEVICE void operator-=(int3 &a, int3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE int3 operator*(int3 a, int3 b)
{
    return make_int3(a.x * b.x, a.y * b.y, a.z * b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE int3 operator*(int3 a, int s)
{
    return make_int3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE int3 operator*(int s, int3 a)
{
    return make_int3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(int3 &a, int s)
{
    a.x *= s; a.y *= s; a.z *= s;
}

/* divide */
OPTIXU_INLINE RT_HOSTDEVICE int3 operator/(int3 a, int3 b)
{
    return make_int3(a.x / b.x, a.y / b.y, a.z / b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE int3 operator/(int3 a, int s)
{
    return make_int3(a.x / s, a.y / s, a.z / s);
}
OPTIXU_INLINE RT_HOSTDEVICE int3 operator/(int s, int3 a)
{
    return make_int3(s /a.x, s / a.y, s / a.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator/=(int3 &a, int s)
{
    a.x /= s; a.y /= s; a.z /= s;
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE int clamp(int f, int a, int b)
{
    return max(a, min(f, b));
}

OPTIXU_INLINE RT_HOSTDEVICE int3 clamp(int3 v, int a, int b)
{
    return make_int3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}

OPTIXU_INLINE RT_HOSTDEVICE int3 clamp(int3 v, int3 a, int3 b)
{
    return make_int3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}


/* uint3 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE uint3 make_uint3(unsigned int s)
{
    return make_uint3(s, s, s);
}
OPTIXU_INLINE RT_HOSTDEVICE uint3 make_uint3(float3 a)
{
    return make_uint3((unsigned int)a.x, (unsigned int)a.y, (unsigned int)a.z);
}

/* min */
OPTIXU_INLINE RT_HOSTDEVICE uint3 min(uint3 a, uint3 b)
{
    return make_uint3(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}

/* max */
OPTIXU_INLINE RT_HOSTDEVICE uint3 max(uint3 a, uint3 b)
{
    return make_uint3(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}

/* addition */
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator+(uint3 a, uint3 b)
{
    return make_uint3(a.x + b.x, a.y + b.y, a.z + b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator+=(uint3 &a, uint3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}

/* subtract */
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator-(uint3 a, uint3 b)
{
    return make_uint3(a.x - b.x, a.y - b.y, a.z - b.z);
}

OPTIXU_INLINE RT_HOSTDEVICE void operator-=(uint3 &a, uint3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}

/* multiply */
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator*(uint3 a, uint3 b)
{
    return make_uint3(a.x * b.x, a.y * b.y, a.z * b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator*(uint3 a, unsigned int s)
{
    return make_uint3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator*(unsigned int s, uint3 a)
{
    return make_uint3(a.x * s, a.y * s, a.z * s);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator*=(uint3 &a, unsigned int s)
{
    a.x *= s; a.y *= s; a.z *= s;
}

/* divide */
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator/(uint3 a, uint3 b)
{
    return make_uint3(a.x / b.x, a.y / b.y, a.z / b.z);
}
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator/(uint3 a, unsigned int s)
{
    return make_uint3(a.x / s, a.y / s, a.z / s);
}
OPTIXU_INLINE RT_HOSTDEVICE uint3 operator/(unsigned int s, uint3 a)
{
    return make_uint3(s / a.x, s / a.y, s / a.z);
}
OPTIXU_INLINE RT_HOSTDEVICE void operator/=(uint3 &a, unsigned int s)
{
    a.x /= s; a.y /= s; a.z /= s;
}

/* clamp */
OPTIXU_INLINE RT_HOSTDEVICE unsigned int clamp(unsigned int f, unsigned int a, unsigned int b)
{
    return max(a, min(f, b));
}

OPTIXU_INLINE RT_HOSTDEVICE uint3 clamp(uint3 v, unsigned int a, unsigned int b)
{
    return make_uint3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}

OPTIXU_INLINE RT_HOSTDEVICE uint3 clamp(uint3 v, uint3 a, uint3 b)
{
    return make_uint3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}

/* equality */
OPTIXU_INLINE RT_HOSTDEVICE bool operator==(uint3 a, uint3 b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

OPTIXU_INLINE RT_HOSTDEVICE bool operator!=(uint3 a, uint3 b)
{
  return a.x != b.x || a.y != b.y || a.z != b.z;
}

/* int4 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int s)
{
    return make_int4(s, s, s, s);
}

/* equality */
OPTIXU_INLINE RT_HOSTDEVICE bool operator==(int4 a, int4 b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

/* uint4 functions */
/******************************************************************************/

/* additional constructors */
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(unsigned int s)
{
    return make_uint4(s, s, s, s);
}




/* Narrowing */
OPTIXU_INLINE RT_HOSTDEVICE int2 make_int2(int3 v0) { return make_int2( v0.x, v0.y ); }
OPTIXU_INLINE RT_HOSTDEVICE int2 make_int2(int4 v0) { return make_int2( v0.x, v0.y ); }
OPTIXU_INLINE RT_HOSTDEVICE int3 make_int3(int4 v0) { return make_int3( v0.x, v0.y, v0.z ); }
OPTIXU_INLINE RT_HOSTDEVICE uint2 make_uint2(uint3 v0) { return make_uint2( v0.x, v0.y ); }
OPTIXU_INLINE RT_HOSTDEVICE uint2 make_uint2(uint4 v0) { return make_uint2( v0.x, v0.y ); }
OPTIXU_INLINE RT_HOSTDEVICE uint3 make_uint3(uint4 v0) { return make_uint3( v0.x, v0.y, v0.z ); }
OPTIXU_INLINE RT_HOSTDEVICE float2 make_float2(float3 v0) { return make_float2( v0.x, v0.y ); }
OPTIXU_INLINE RT_HOSTDEVICE float2 make_float2(float4 v0) { return make_float2( v0.x, v0.y ); }

/* Assemble from smaller vectors */
OPTIXU_INLINE RT_HOSTDEVICE int3 make_int3(int v0, int2 v1) { return make_int3( v0, v1.x, v1.y ); }
OPTIXU_INLINE RT_HOSTDEVICE int3 make_int3(int2 v0, int v1) { return make_int3( v0.x, v0.y, v1 ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int v0, int v1, int2 v2) { return make_int4( v0, v1, v2.x, v2.y ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int v0, int2 v1, int v2) { return make_int4( v0, v1.x, v1.y, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int2 v0, int v1, int v2) { return make_int4( v0.x, v0.y, v1, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int v0, int3 v1) { return make_int4( v0, v1.x, v1.y, v1.z ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int3 v0, int v1) { return make_int4( v0.x, v0.y, v0.z, v1 ); }
OPTIXU_INLINE RT_HOSTDEVICE int4 make_int4(int2 v0, int2 v1) { return make_int4( v0.x, v0.y, v1.x, v1.y ); }
OPTIXU_INLINE RT_HOSTDEVICE uint3 make_uint3(unsigned int v0, uint2 v1) { return make_uint3( v0, v1.x, v1.y ); }
OPTIXU_INLINE RT_HOSTDEVICE uint3 make_uint3(uint2 v0, unsigned int v1) { return make_uint3( v0.x, v0.y, v1 ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(unsigned int v0, unsigned int v1, uint2 v2) { return make_uint4( v0, v1, v2.x, v2.y ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(unsigned int v0, uint2 v1, unsigned int v2) { return make_uint4( v0, v1.x, v1.y, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(uint2 v0, unsigned int v1, unsigned int v2) { return make_uint4( v0.x, v0.y, v1, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(unsigned int v0, uint3 v1) { return make_uint4( v0, v1.x, v1.y, v1.z ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(uint3 v0, unsigned int v1) { return make_uint4( v0.x, v0.y, v0.z, v1 ); }
OPTIXU_INLINE RT_HOSTDEVICE uint4 make_uint4(uint2 v0, uint2 v1) { return make_uint4( v0.x, v0.y, v1.x, v1.y ); }
OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(float v0, float2 v1) { return make_float3( v0, v1.x, v1.y ); }
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float v0, float v1, float2 v2) { return make_float4( v0, v1, v2.x, v2.y ); }
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float v0, float2 v1, float v2) { return make_float4( v0, v1.x, v1.y, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float2 v0, float v1, float v2) { return make_float4( v0.x, v0.y, v1, v2 ); }
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float v0, float3 v1) { return make_float4( v0, v1.x, v1.y, v1.z ); }
OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(float2 v0, float2 v1) { return make_float4( v0.x, v0.y, v1.x, v1.y ); }

OPTIXU_INLINE RT_HOSTDEVICE float fmaxf(float2 a)
{
  return fmaxf(a.x, a.y);
}

OPTIXU_INLINE RT_HOSTDEVICE float fmaxf(float3 a)
{
  return fmaxf(fmaxf(a.x, a.y), a.z);
}

OPTIXU_INLINE RT_HOSTDEVICE float fmaxf(float4 a)
{
  return fmaxf(fmaxf(a.x, a.y), fmaxf(a.z, a.w));
}

OPTIXU_INLINE RT_HOSTDEVICE float fminf(float2 a)
{
  return fminf(a.x, a.y);
}

OPTIXU_INLINE RT_HOSTDEVICE float fminf(float3 a)
{
  return fminf(fminf(a.x, a.y), a.z);
}

OPTIXU_INLINE RT_HOSTDEVICE float fminf(float4 a)
{
  return fminf(fminf(a.x, a.y), fminf(a.z, a.w));
}

OPTIXU_INLINE RT_HOSTDEVICE float2 make_float2(uint2 a)
{
    return make_float2(float(a.x), float(a.y));
}

OPTIXU_INLINE RT_HOSTDEVICE float3 make_float3(uint3 a)
{
    return make_float3(float(a.x), float(a.y), float(a.z));
}

OPTIXU_INLINE RT_HOSTDEVICE float4 make_float4(uint4 a)
{
    return make_float4(float(a.x), float(a.y), float(a.z), float(a.w));
}

/* Common helper functions */
/******************************************************************************/

/* Return a smooth value in [0,1], where the transition from 0
   to 1 takes place for values of x in [edge0,edge1]. */
OPTIXU_INLINE RT_HOSTDEVICE float smoothstep( float edge0, float edge1, float x )
{
    /*assert( edge1 > edge0 ); */
    const float t = clamp( (x-edge0) / (edge1-edge0), 0.0f, 1.0f );
    return t*t * ( 3.0f - 2.0f*t );
}

/* Simple mapping from [0,1] to a temperature-like RGB color. */
OPTIXU_INLINE RT_HOSTDEVICE float3 temperature( float t )
{
    const float b = t < 0.25f ? smoothstep( -0.25f, 0.25f, t ) : 1.0f-smoothstep( 0.25f, 0.5f, t );
    const float g = t < 0.5f  ? smoothstep( 0.0f, 0.5f, t ) :
               (t < 0.75f ? 1.0f : 1.0f-smoothstep( 0.75f, 1.0f, t ));
    const float r = smoothstep( 0.5f, 0.75f, t );
    return make_float3( r, g, b );
}


/**
 *  \brief Intersect ray with CCW wound triangle.  Returns non-normalize normal vector.
 */ 
__device__ OPTIXU_INLINE bool intersect_triangle( const Ray& ray,
                                               const float3& p0,
                                               const float3& p1,
                                               const float3& p2,
                                               float3& n,
                                               float&  t,
                                               float&  beta,
                                               float&  gamma )
{
  float3 e0 = p1 - p0;
  float3 e1 = p0 - p2;
  n  = cross( e0, e1 );

  float v   = dot( n, ray.direction );
  float r   = 1.0f / v;

  float3 e2 = p0 - ray.origin;
  float va  = dot( n, e2 );
  t         = r*va;

  // Initialize these to reduce their liveness when we leave the function without
  // computing their value.
  beta = 0;
  gamma = 0;
  
  if(t < ray.tmax && t > ray.tmin) {
    float3 i   = cross( e2, ray.direction );
    float v1   = dot( i, e1 );
    beta = r*v1;
    if(beta >= 0.0f){
      float v2 = dot( i, e0 );
      gamma = r*v2;
      n = -n;
      return ( (v1+v2)*v <= v*v && gamma >= 0.0f );
    }
  }
  return false;
}


/*
  calculates refraction direction
  r   : refraction vector
  i   : incident vector
  n   : surface normal
  ior : index of refraction ( n2 / n1 )
  returns false in case of total internal reflection, in that case r is
          initialized to (0,0,0).
*/
OPTIXU_INLINE RT_HOSTDEVICE bool refract(float3& r, float3 i, float3 n, float ior)
{
  float3 nn = n;
  float negNdotV = dot(i,nn);
  float eta;

  if (negNdotV > 0.0f)
  {
    eta = ior;
    nn = -n;
    negNdotV = -negNdotV;
  }
  else
  {
    eta = 1.f / ior;
  }

  const float k = 1.f - eta*eta * (1.f - negNdotV * negNdotV);

  if (k < 0.0f) {
    // Initialize this value, so that r always leaves this function initialized.
    r = make_float3(0.f);
    return false;
  } else {
    r = normalize(eta*i - (eta*negNdotV + sqrtf(k)) * nn);
    return true;
  }
}

/* Schlick approximation of Fresnel reflectance */
OPTIXU_INLINE RT_HOSTDEVICE float fresnel_schlick(float cos_theta, float exponent = 5.0f,
                                                            float minimum  = 0.0f,
                                                            float maximum  = 1.0f)
{
  /*
     clamp the result of the arithmetic due to floating point precision:
     the result should lie strictly within [minimum, maximum]
    return clamp(minimum + (maximum - minimum) * powf(1.0f - cos_theta, exponent),
                 minimum, maximum);

  */

  /* The max doesn't seem like it should be necessary, but without it you get
     annoying broken pixels at the center of reflective spheres where cos_theta ~ 1.
  */
  return clamp(minimum + (maximum - minimum) * powf(fmaxf(0.0f,1.0f - cos_theta), exponent),
               minimum, maximum);
}

OPTIXU_INLINE RT_HOSTDEVICE float3 fresnel_schlick(float cos_theta, float exponent,
                                            float3 minimum, float3 maximum)
{
  return make_float3(fresnel_schlick(cos_theta, exponent, minimum.x, maximum.x),
                     fresnel_schlick(cos_theta, exponent, minimum.y, maximum.y),
                     fresnel_schlick(cos_theta, exponent, minimum.z, maximum.z));
}


// Calculate the NTSC luminance value of an rgb triple
OPTIXU_INLINE RT_HOSTDEVICE float luminance( const float3& rgb )
{
  const float3 ntsc_luminance = { 0.30f, 0.59f, 0.11f };
  return  dot( rgb, ntsc_luminance );
}

// Calculate the CIE luminance value of an rgb triple
OPTIXU_INLINE RT_HOSTDEVICE float luminanceCIE( const float3& rgb )
{
  const float3 cie_luminance = { 0.2126f, 0.7152f, 0.0722f };
  return  dot( rgb, cie_luminance );
}

OPTIXU_INLINE RT_HOSTDEVICE void cosine_sample_hemisphere( float u1, float u2, float3& p )
{
  // Uniformly sample disk.
  const float r   = sqrtf( u1 );
  const float phi = 2.0f*M_PIf * u2;
  p.x = r * cosf( phi );
  p.y = r * sinf( phi );

  // Project up to hemisphere.
  p.z = sqrtf( fmaxf( 0.0f, 1.0f - p.x*p.x - p.y*p.y ) );
}

// Maps concentric squares to concentric circles (Shirley and Chiu)
OPTIXU_INLINE RT_HOSTDEVICE float2 square_to_disk( float2 sample )
{
  float phi, r;

  const float a = 2.0f * sample.x - 1.0f;
  const float b = 2.0f * sample.y - 1.0f;

  if (a > -b)
  {
    if (a > b)
    {
      r = a;
      phi = (float)M_PI_4f * (b/a);
    }
    else
    {
      r = b;
      phi = (float)M_PI_4f * (2.0f - (a/b));
    }
  }
  else
  {
    if (a < b)
    {
      r = -a;
      phi = (float)M_PI_4f * (4.0f + (b/a));
    }
    else
    {
      r = -b;
      phi = (b) ? (float)M_PI_4f * (6.0f - (a/b)) : 0.0f;
    }
  }

  return make_float2( r * cosf(phi), r * sinf(phi) );
}

// Convert cartesian coordinates to polar coordinates
OPTIXU_INLINE RT_HOSTDEVICE float3 cart_to_pol(float3 v)
{
  float azimuth;
  float elevation;
  float radius = length(v);

  float r = sqrtf(v.x*v.x + v.y*v.y);
  if (r > 0.0f)
  {
    azimuth   = atanf(v.y / v.x);
    elevation = atanf(v.z / r);

    if (v.x < 0.0f)
      azimuth += M_PIf;
    else if (v.y < 0.0f)
      azimuth += M_PIf * 2.0f;
  }
  else
  {
    azimuth = 0.0f;

    if (v.z > 0.0f)
      elevation = +M_PI_2f;
    else
      elevation = -M_PI_2f;
  }

  return make_float3(azimuth, elevation, radius);
}

// Orthonormal basis
struct Onb
{
  OPTIXU_INLINE RT_HOSTDEVICE Onb( const float3& normal )
    {
      m_normal = normal;

      if( fabs(m_normal.x) > fabs(m_normal.z) )
      {
        m_binormal.x = -m_normal.y;
        m_binormal.y =  m_normal.x;
        m_binormal.z =  0;
      }
      else
      {
        m_binormal.x =  0;
        m_binormal.y = -m_normal.z;
        m_binormal.z =  m_normal.y;
      }

      m_binormal = normalize(m_binormal);
      m_tangent = cross( m_binormal, m_normal );
    }

  OPTIXU_INLINE RT_HOSTDEVICE void inverse_transform( float3& p )
    {
      p = p.x*m_tangent + p.y*m_binormal + p.z*m_normal;
    }

  float3 m_tangent;
  float3 m_binormal;
  float3 m_normal;
};

} // end namespace optix

/*
 * When looking for operators for a type, only the scope the type is defined in (plus the
 * global scope) is searched.  In order to make the operators behave properly we are
 * pulling them into the global namespace.
 */
#if defined(RT_PULL_IN_VECTOR_TYPES)
using optix::operator-;
using optix::operator-=;
using optix::operator+;
using optix::operator+=;
using optix::operator*;
using optix::operator*=;
using optix::operator/;
using optix::operator/=;
using optix::operator==;
#endif // #if defined(RT_PULL_IN_VECTOR_TYPES)

/*
 * Here are a list of functions that are overloaded in both the global and optix
 * namespace.  If they have a global namespace version, then the overloads in the optix
 * namespace need to be pulled in, so that all the overloads are on the same level.
 */

/* These are defined by CUDA in the global namespace */
#if defined(RT_PULL_IN_VECTOR_FUNCTIONS)
#define RT_DEFINE_HELPER(type) \
  using optix::make_##type##1; \
  using optix::make_##type##2; \
  using optix::make_##type##3; \
  using optix::make_##type##4;

/* Some types didn't exist until CUDA 3.0.  CUDA_VERSION isn't defined while
 * building CUDA code, so we also need to check the CUDART_VERSION value. */
#if (CUDA_VERSION >= 3000) || (CUDART_VERSION >= 3000)
#define RT_DEFINE_HELPER2(type) RT_DEFINE_HELPER(type)
#else
#define RT_DEFINE_HELPER2(type) \
  using ::make_##type##1; \
  using ::make_##type##2;
#endif


RT_DEFINE_HELPER(char)
RT_DEFINE_HELPER(uchar)
RT_DEFINE_HELPER(short)
RT_DEFINE_HELPER(ushort)
RT_DEFINE_HELPER(int)
RT_DEFINE_HELPER(uint)
RT_DEFINE_HELPER(long)
RT_DEFINE_HELPER(ulong)
RT_DEFINE_HELPER(float)
RT_DEFINE_HELPER2(longlong)
RT_DEFINE_HELPER2(ulonglong)
RT_DEFINE_HELPER2(double)

#undef RT_DEFINE_HELPER
#undef RT_DEFINE_HELPER2

#endif // #if defined(RT_PULL_IN_VECTOR_FUNCTIONS)

/* These are defined by CUDA and non-Windows platforms in the global namespace. */
#if !defined(_WIN32) || defined (__CUDACC__)
using optix::fmaxf;
using optix::fminf;
using optix::copysignf;
#endif

/* These are always in the global namespace. */
using optix::expf;
using optix::floor;

/* These are defined by CUDA in the global namespace */
#if defined (__CUDACC__)
using optix::max;
using optix::min;
#endif

#ifdef OPTIXU_INLINE_DEFINED
#  undef OPTIXU_INLINE_DEFINED
#  undef OPTIXU_INLINE
#endif

#endif // #ifndef __optixu_optixu_math_namespace_h__
