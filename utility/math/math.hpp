/**
 *
 * math.hpp
 *
 * some common math function
 * 
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-04
 */

#ifndef __ydk_utility_math_math_hpp__
#define __ydk_utility_math_math_hpp__

#include <stdint.h>
#include <float.h>

namespace utility{
namespace math{
	/** some constant */
static const float kPI             = 3.14159265358979323846f;
static const float kTWO_PI         = 2.0f * kPI;
static const float kHALF_PI        = 0.5f * kPI;
static const float kE              = 2.71828182845904523536f;
static const float kSQRT_TWO       = 1.41421356237309504880f;
static const float kSQRT_THREE     = 1.73205080756887729352f;
static const float kM_DEG2RAD      = kPI / 180.f;
static const float kM_RAD2DEG      = 180.0f / kPI;
static const float kINFINITY       = 1e30f;
static const float kFLT_EPSILON    = 1.192092896e-07f;

#define DEG2RAD(a)		( (a) * math::kM_DEG2RAD )
#define RAD2DEG(a)		( (a) * math::kM_RAD2DEG )

#define SMALL_NUMBER        (1.e-8f)
#define KINDA_SMALL_NUMBER  (1.e-4f)
#define BIG_NUMBER          (3.4e+38f)

//
// Magic numbers for numerical precision.
//
#define THRESH_POINT_ON_PLANE           (0.10f)		/* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE            (0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME          (0.00002f)	/* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR          (0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME         (0.00002f)	/* Two normal points are same if within this distance */
/* Making this too large results in incorrect CSG classification and disaster */
#define THRESH_VECTORS_ARE_NEAR         (0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
/* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define THRESH_SPLIT_POLY_WITH_PLANE    (0.25f)		/* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY     (0.01f)		/* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED        (0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define THRESH_NORMALS_ARE_PARALLEL     (0.999845f)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define THRESH_NORMALS_ARE_ORTHOGONAL   (0.017455f)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define THRESH_VECTOR_NORMALIZED        (0.01f)		/** Allowed error for a normalized vector (against squared magnitude) */
#define THRESH_QUAT_NORMALIZED          (0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

// Magic numbers for numerical precision.
#define DELTA           (0.00001f)

#define MIN_uint8       ((uint8_t)  0x00)
#define MIN_uint16      ((uint16_t) 0x0000)
#define MIN_uint32      ((uint32_t) 0x00000000)
#define MIN_uint64      ((uint64_t) 0x0000000000000000)
#define MIN_int8        ((int8_t)   -128)
#define MIN_int16       ((int16_t)  -32768)
#define MIN_int32       ((int32_t)  0x80000000)
#define MIN_int64       ((int64_t)  0x8000000000000000)

#define MAX_uint8       ((uint8_t)  0xff)
#define MAX_uint16      ((uint16_t) 0xffff)
#define MAX_uint32      ((uint32_t) 0xffffffff)
#define MAX_uint64      ((uint64_t) 0xffffffffffffffff)
#define MAX_int8        ((int8_t)   0x7f)
#define MAX_int16       ((int16_t)  0x7fff)
#define MAX_int32       ((int32_t)  0x7fffffff)
#define MAX_int64       ((int64_t)  0x7fffffffffffffff)

#define MIN_flt         (1.175494351e-38F)			/* min positive value */
#define MAX_flt         (3.402823466e+38F)
#define MIN_dbl         (2.2250738585072014e-308)	/* min positive value */
#define MAX_dbl         (1.7976931348623158e+308)	


/** functions */

static float fmod(float x, float y){
    return ::fmodf(x, y);
}

static float sin(float x){
	return ::sinf(x);
}

static float asin(float x){
	if (x <= -1.0f) {
		return -kHALF_PI;
	}
	if (x >= 1.0f) {
		return kHALF_PI;
	}
	return ::asinf(x);
}

static float cos(float x){
	return ::cosf(x);
}

static float acos(float x){
	if (x <= -1.0f) {
		return kPI;
	}
	if (x >= 1.0f) {
		return 0.0f;
	}
	return ::acosf(x);
}

static float tan(float x){
	return ::tanf(x);
}

static float atan(float x){
	return ::atanf(x);
}

static float atan2(float y, float x){
	return ::atan2f(y, x);
}

static float sqrt(float x){
	return ::sqrtf(x);
}

// get sine and cosine result of x
static void  sincos(float x, float& s, float& c){
    s = sin(x);
    c = cos(x);
}

static float inv_sqrt(float x){
	return 1.0f / sqrt(x);
}


static float pow(float x, float y){
	return ::powf(x, y);
}

static float exp(float x){
	return ::expf(x);
}

static float log(float x){
	return ::logf(x);
}

static float logx(float base, float x){
	return log(x) / log(base);
}

static float log2(float x){
	return ::log2f(x);
}

static bool is_nan(float x){
	return (((*(const unsigned long *)&x) & 0x7f800000) == 0x7f800000);
}

static bool is_finite(float x){
	return (((*(const unsigned long *)&x) & 0x7fffffff) == 0x7f800000);
}

static float float_select(float comparand, float value_ge_zero, float value_lt_zero){
    return comparand >= 0.f ? value_ge_zero : value_lt_zero;
}

/** Given a heading which may be outside the +/- PI range, 'unwind' it back into that range. */
static float normalize_radians(float a){
    while (a > kPI){
        a -= ((float)kPI * 2.0f);
    }
    while (a < -kPI){
        a += ((float)kPI * 2.0f);
    }
    return a;
}

/** Utility to ensure angle is between +/- 180 degrees by unwinding. */
static float normalize_degrees(float a){
    while (a > 180.f){
        a -= 360.f;
    }
    while (a < -180.f){
        a += 360.f;
    }
    return a;
}

template<typename T>
inline T abs(const T x){
	return (x >= (T)0 ? x : -x);
}

/** returns 1, 0 or -1 depending on relation of T to 0 */
template<typename T>
inline T sign(const T x){
	return (f > 0) ? 1 : ((f < 0) ? -1 : 0);
}

template<typename T>
inline T square(const T x){
	return x * x;
}

template<typename T>
inline T cube(const T x){
	return x * x * x;
}

/** clamps x to be between min and max */
template<typename T>
inline T clamp(const T x, const T min, const T max){
	return x < min ? min : x < max ? x : max;
}

template<typename T>
inline T max2(const T x, const T y){
	return x > y ? x : y;
}

template<typename T>
inline T max3(const T x, const T y, const T z){
	return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
}

template<typename T>
inline T min2(const T x, const T y){
	return x < y ? x : y;
}

template<typename T>
inline T min3(const T x, const T y, const T z){
	return (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
}

} // end namespace math
} // end namespace utility

#endif