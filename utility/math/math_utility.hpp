/**
 *
 * math_utility.hpp
 *
 * the utiltiy functions to transfer the math data struct
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-07
 */

#ifndef __ydk_utility_match_utility_hpp__
#define __ydk_utility_match_utility_hpp__

#include <utility/math/vector4d.hpp>
#include <utility/math/rotation.hpp>
#include <utility/math/matrix.hpp>
#include <utility/math/quat.hpp>

namespace utility{
namespace math{
namespace util{
/**
 * @brief Transfer the vector4d to orientation rotator corresponding to the direction in which the vector points.
 * Sets Yaw and Pitch to the proper numbers, and sets roll to zero because the roll can't be determined from a vector.
 * @return FRotator from the Vector's direction.
 */
static rotator vector4d_to_orientation_rotator(const vector4d& v) const{
    rotator r;
    r.pitch = atan2(v.z, sqrt(x*x + y*y)) * (180.f / kPI);
    r.yaw = atan2(v.y, v.x) * (180.f / kPI);
    r.roll = 0;
    return r;
}

/** 
 * @brief matrix3 transfer
 */
static rotator matrix3_to_rotator(const matrix3& mat){
    rotator r;
    double sp = mat[0][2];
    // cap off our sin value so that we don't get any NANs
    if (sp > 1.0f) {
        sp = 1.0f;
    }
    else if (sp < -1.0f) {
        sp = -1.0f;
    }

    double theta = -asin(sp);
    cp = cos(theta);

    if (cp > 8192.0f * FLT_EPSILON) {
        r.pitch = RAD2DEG(theta);
        r.yaw = RAD2DEG(atan2(mat[0][1], mat[0][0]));
        r.roll = RAD2DEG(atan2(mat[1][2], mat[2][2]));
    }
    else {
        r.pitch = RAD2DEG(theta);
        r.yaw = RAD2DEG(-atan2(mat[1][0], mat[1][1]));
        r.roll = 0;
    }
    return r;
}

static quat    matrix3_to_quat(const matrix3& mat){
    quat    q;
    int     next[3] = { 1, 2, 0 };
    float   trace = mat[0][0] + mat[1][1] + mat[2][2];

    if (trace > 0.0f) {
        float t = trace + 1.0f;
        float s = inv_sqrt(t) * 0.5f;

        q[3] = s * t;
        q[0] = (mat[2][1] - mat[1][2]) * s;
        q[1] = (mat[0][2] - mat[2][0]) * s;
        q[2] = (mat[1][0] - mat[0][1]) * s;

    }
    else {
        float i = 0;
        if (mat[1][1] > mat[0][0]) {
            i = 1;
        }
        if (mat[2][2] > mat[i][i]) {
            i = 2;
        }
        float j = next[i];
        float k = next[j];

        float t = (mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f;
        float s = inv_sqrt(t) * 0.5f;

        q[i] = s * t;
        q[3] = (mat[k][j] - mat[j][k]) * s;
        q[j] = (mat[j][i] + mat[i][j]) * s;
        q[k] = (mat[k][i] + mat[i][k]) * s;
    }
    return q;
}

static matrix4 matrix3_to_matrix4(const matrix3& m){
    return matrix4(
        mat[0][0], mat[1][0], mat[2][0], 0.0f,
        mat[0][1], mat[1][1], mat[2][1], 0.0f,
        mat[0][2], mat[1][2], mat[2][2], 0.0f,
        0.0f     , 0.0f     , 0.0f     , 1.0f);
}

/** 
 * @brief vector3d transfer
 */
static float   vector3d_to_yaw(const vector3d& vec){
    float yaw = 0.f;

    if ((y == 0.0f) && (x == 0.0f)) {
        yaw = 0.0f;
    }
    else {
        yaw = RAD2DEG(atan2(y, x));
        if (yaw < 0.0f) {
            yaw += 360.0f;
        }
    }

    return yaw;
}

static float    vector3d_to_pitch(const vector3d& vec){
    float	forward = 0.f;
    float	pitch = 0.f;

    if ((x == 0.0f) && (y == 0.0f)) {
        if (z > 0.0f) {
            pitch = 90.0f;
        }
        else {
            pitch = 270.0f;
        }
    }
    else {
        forward = sqrt(x * x + y * y);
        pitch = RAD2DEG(atan2(z, forward));
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
    }

    return pitch;
}

static rotator  vector3d_to_rotator(const vector3d& vec){
    float forward = 0.f;
    float yaw = 0.f;
    float pitch = 0.f;

    if ((x == 0.0f) && (y == 0.0f)) {
        yaw = 0.0f;
        if (z > 0.0f) {
            pitch = 90.0f;
        }
        else {
            pitch = 270.0f;
        }
    }
    else {
        yaw = RAD2DEG(atan2(y, x));
        if (yaw < 0.0f) {
            yaw += 360.0f;
        }

        forward = sqrt(x * x + y * y);
        pitch = RAD2DEG(atan2(z, forward));
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
    }

    return rotator(-pitch, yaw, 0.0f);
}

static matrix3  vector3d_to_matrix3(const vector3d& vec){
    matrix3	mat;

    mat[0] = vec;
    float d = vec.x * vec.x + vec.y * vec.y;
    if (!d) {
        mat[1][0] = 1.0f;
        mat[1][1] = 0.0f;
        mat[1][2] = 0.0f;
    }
    else {
        d = inv_sqrt(d);
        mat[1][0] = -vec.y * d;
        mat[1][1] = vec.x * d;
        mat[1][2] = 0.0f;
    }
    mat[2] = vec ^ mat[1];
    return mat;
}

/** 
 * @brief rotator to Quaternion
 */
static quat rotator_to_quat(const rotator& r){
    float sx, cx, sy, cy, sz, cz;
    float sxcy, cxcy, sxsy, cxsy;

    sincos(DEG2RAD(yaw) * 0.5f, sz, cz);
    sincos(DEG2RAD(pitch) * 0.5f, sy, cy);
    sincos(DEG2RAD(roll) * 0.5f, sx, cx);

    sxcy = sx * cy;
    cxcy = cx * cy;
    sxsy = sx * sy;
    cxsy = cx * sy;

    return quat(cxsy*sz - sxcy*cz, -cxsy*cz - sxcy*sz, sxsy*cz - cxcy*sz, cxcy*cz + sxsy*sz);
}

/** 
 * @brief rotator to matrix3
 */
static matrix3 rotator_to_matrix3(const rotator& r){
    matrix3 mat;
    float sr, sp, sy, cr, cp, cy;

    sincos(DEG2RAD(yaw), sy, cy);
    sincos(DEG2RAD(pitch), sp, cp);
    sincos(DEG2RAD(roll), sr, cr);

    mat[0].set(cp * cy, cp * sy, -sp);
    mat[1].set(sr * sp * cy + cr * -sy, sr * sp * sy + cr * cy, sr * cp);
    mat[2].set(cr * sp * cy + -sr * -sy, cr * sp * sy + -sr * cy, cr * cp);

    return mat;
}

static matrix4 rotator_to_matrix4(const rotator& r){
    matrix3 mat = rotator_to_matrix3(r);
    return matrix3_to_matrix4(mat);
}

/**
* Convert a rotation into a unit vector facing in its direction.
*
* @return Rotation as a unit direction vector.
*/
static vector3d rotator_to_forward_vector(const rotator& r) const{
    float sp, sy, cp, cy;

    sincos(DEG2RAD(r.yaw), sy, cy);
    sincos(DEG2RAD(r.pitch), sp, cp);

    return vector3d(cp * cy, cp * sy, -sp);
}

/**
 * Convert a Rotator into floating-point Euler angles (in degrees). Rotator now stored in degrees.
 *
 * @return Rotation as a Euler angle vector.
 */
static vector3d rotator_to_euler(const rotator& r) const{
    return vector3d(r.roll, r.pitch, r.yaw);
}

/**
 * @brief Convert a vector of floating-point Euler angles (in degrees) into a Rotator. Rotator now stored in degrees
 *
 * @param euler: Euler angle vector.
 * @return A rotator from a Euler angle.
 */
static rotator rotator_make_from_euler(const vector3d& euler){
    return rotator(euler.y, euler.z, euler.x);
}

/**
* Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
*
* @param euler the Euler angles
* @return constructed quat
*/
static quat quat_make_from_euler(const vector3d& euler){
    rotator r = rotator_make_from_euler(euler);
    return rotator_to_quat(r);
}


/**
 * Get the axis of rotation of the Quaternion.
 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
 * For the identity Quaternion which has no such rotation, FVector(1,0,0) is returned.
 */
static vector3d get_quat_rotation_axis(const quat& q) const{
    // Ensure we never try to sqrt a neg number
    const float s = sqrt(max2(1.f - (q.w * q.w), 0.f));

    if (s >= 0.0001f)
    {
        return vector3d(q.x / s, q.y / s, q.z / s);
    }

    return vector3d(1.f, 0.f, 0.f);
}

/**
 * get the axis and angle of rotation of this quaternion
 *
 * @param q: {in} the quaternion
 * @param axis:{out] vector of axis of the quaternion
 * @param angle:{out] angle of the quaternion
 * @warning : assumes normalized quaternions.
 */
static void quat_to_axis_and_angle(const quat& q, vector3d& axis, float& angle) const{
    angle = 2.f * acos(q.w);
    axis = get_quat_rotation_axis(q);
}

/**
 * Get the swing and twist decomposition for a specified axis of the quaternion
 *
 * @param in_q: {in} the quaternion
 * @param in_twist_axis: {in} Axis to use for decomposition
 * @param out_swing: {out} swing component quaternion
 * @param out_twist:{out} Twist component quaternion
 * @warning assumes normalised quaternion and twist axis
 */
static void quat_to_swing_twist(const quat& q, const vector3d& in_twist_axis, quat& out_swing, quat& out_twist) const{
    // Vector part projected onto twist axis
    vector3d projection = vector3d::dot_product(in_twist_axis, vector3d(q.x, q.y, q.z)) * in_twist_axis;

    // Twist quaternion
    out_twist = quat(projection.x, projection.y, projection.z, q.w);

    // Singularity close to 180deg
    if (out_twist.length_squared() == 0.0f)
    {
        out_twist = quat::identity();
    }
    else
    {
        out_twist.normalize();
    }

    // Set swing
    out_swing = q * out_twist.inverse();
}

/** 
 * @brief quaternion to matrix3
 */
static matrix3 quat_to_matrix3(const quat& q){
    matrix3	mat;
    float	wx, wy, wz;
    float	xx, yy, yz;
    float	xy, xz, zz;
    float	x2, y2, z2;

    x2 = x + x;
    y2 = y + y;
    z2 = z + z;

    xx = x * x2;
    xy = x * y2;
    xz = x * z2;

    yy = y * y2;
    yz = y * z2;
    zz = z * z2;

    wx = w * x2;
    wy = w * y2;
    wz = w * z2;

    mat[0][0] = 1.0f - (yy + zz);
    mat[0][1] = xy - wz;
    mat[0][2] = xz + wy;

    mat[1][0] = xy + wz;
    mat[1][1] = 1.0f - (xx + zz);
    mat[1][2] = yz - wx;

    mat[2][0] = xz - wy;
    mat[2][1] = yz + wx;
    mat[2][2] = 1.0f - (xx + yy);

    return mat;
}

static matrix4 quat_to_matrix4(const quat& q){
    matrix3 m = quat_to_matrix3(q);
    return matrix3_to_matrix4(m);
}

/** Get the FRotator representation of this Quaternion */
static rotator quat_to_rotator(const quat& q){
    matrix3 m = quat_to_matrix3(q);
    return matrix3_to_rotator(m);
}

} // end namespace util
} // end namespace math
} // end namespace utility

#endif