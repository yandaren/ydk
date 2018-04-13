/**
 *
 * quat.hpp
 *
 * quaternion implementation[四元组]
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-12
 */

#ifndef __ydk_utility_math_quat_hpp__
#define __ydk_utility_math_quat_hpp__

#include <utility/math/math.hpp>
#include <utility/math/vector3d.hpp>
#include <base/align.hpp>

namespace utility{
namespace math{

/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */
MS_ALIGN(16) struct quat 
{
public:

	/** The quaternion's X-component. */
	float x;

	/** The quaternion's Y-component. */
	float y;

	/** The quaternion's Z-component. */
	float z;

	/** The quaternion's W-component. */
	float w;

public:
	/** Identity quaternion. */
    static const quat& identity(){
        static const quat identity_quat(0, 0, 0, 1);
        return identity_quat;
    }

public:

	/** Default constructor. */
	inline quat():x(0),y(0),z(0),w(0) {}

	/**
	 * Constructor.
	 *
	 * @param in_x: x component of the quaternion
	 * @param in_y: y component of the quaternion
	 * @param in_z: z component of the quaternion
	 * @param in_w: w component of the quaternion
	 */
    inline quat(float in_x, float in_y, float in_z, float in_w)
        : x(in_x), y(in_y), z(in_z), w(in_w){
    }

	/**
	 * Copy constructor.
	 *
	 * @param Q A quat object to use to create new quaternion from.
	 */
    inline quat(const quat& q)
        :x(q.x), y(q.y), z(q.z), w(q.w){
    }

	/**
	 * Creates and initializes a new quaternion from the a rotation around the given axis.
	 *
	 * @param axis: assumed to be a normalized vector
	 * @param angle: angle to rotate above the given axis (in radians)
	 */
    quat(const vector3d& axis, float angle_in_rad){
        const float half_a = 0.5f * angle_in_rad;
        float s = sin(half_a);
        float c = cos(half_a);

        x = s * axis.x;
        y = s * axis.y;
        z = s * axis.z;
        w = c;
    }

public:
    /**
     * @brief Access a specific component of the quat.
     *
     * @param index: The index of the component.
     * @return Reference to the desired component.
     */
    inline float& operator[](int32_t index){
        assert((index >= 0) && (index < 4));
        return (&x)[index];
    }

    /**
     * @brief Access a specific component of the quat.
     *
     * @param index: The index of the component.
     * @return Copy of the desired component.
     */
    inline float operator[](int32_t index) const{
        assert((index >= 0) && (index < 4));
        return (&x)[index];
    }

	/**
	 * Copy another quat into this one
	 *
	 * @return reference to this quat
	 */
    inline quat& operator=(const quat& other){
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
    inline quat operator+(const quat& q) const{
        return quat(x + q.x, y + q.y, z + q.z, w + q.w);
    }

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param q: The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
    inline quat operator-(const quat& q) const{
        return quat(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    /**
     * Get the result of scaling this quaternion.
     *
     * @param scale: The scaling factor.
     * @return The result of scaling.
     */
    inline quat operator*(const float scale) const{
        return quat(x * scale, y * scale, z * scale, w * scale);
    }

    /**
     * Gets the result of multiplying this by another quaternion (this * Q).
     *
     *
     * @param Q The Quaternion to multiply this by.
     * @return The result of multiplication (this * Q).
     */
    inline quat operator*(const quat& q) const{
        return quat(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y + y * q.w + z * q.x - x * q.z,
            w * q.z + z * q.w + x * q.y - y * q.x,
            w * q.w - x * q.x - y * q.y - z * q.z);
    }

    /**
    * Rotate a vector by this quaternion.
    *
    * @param v the vector to be rotated
    * @return vector after rotation
    * @see RotateVector
    */
    inline vector3d operator*(const vector3d& v) const{
        float xxzz = x * x - z * z;
        float wwyy = w * w - y * y;

        float xw2 = x * w * 2.0f;
        float xy2 = x * y * 2.0f;
        float xz2 = x * z * 2.0f;
        float yw2 = y * w * 2.0f;
        float yz2 = y * z * 2.0f;
        float zw2 = z * w * 2.0f;

        return vector3d(
            (xxzz + wwyy) * v.x + (xy2 + zw2) * v.y + (xz2 - yw2) * v.z,
            (xy2  - zw2 ) * v.x + (y*y + w*w - x*x - z*z) * v.y + (yz2 + xw2) * v.z,
            (xz2 + yw2) * v.x + (yz2 - xw2) * v.y + (wwyy - xxzz) * v.z
            );
    }

    /**
     * Divide this quaternion by scale.
     *
     * @param scale What to divide by.
     * @return new Quaternion of this after division by scale.
     */
    inline quat operator/(const float scale) const{
        const float recip = 1.0f / scale;
        return quat(x * recip, y * recip, z * recip, w * recip);
    }

    /**
     * Calculates dot product of two quaternions.
     *
     * @param q: The other quaternions.
     * @return The dot product.
     */
    inline float operator|(const quat& q) const{
        return x * q.x + y * q.y + z * q.z + w * q.w;
    }

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param q: The other Quaternion.
	 * @param tolerance: Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
    inline bool equals(const quat& q, float tolerance = KINDA_SMALL_NUMBER) const{
        return (abs(x - q.x) <= tolerance &&
                abs(y - q.y) <= tolerance &&
                abs(z - q.z) <= tolerance &&
                abs(w - q.w) <= tolerance);
    }

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param tolerance: Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
    inline bool is_identity(float tolerance = SMALL_NUMBER) const{
        return equals(identity(), tolerance);
    }

    /**
     * Adds to this quaternion.
     * This is a component-wise addition; composing quaternions should be done via multiplication.
     *
     * @param q: The quaternion to add to this.
     * @return Result after addition.
     */
    inline quat& operator+=(const quat& q){
        x += q.x; y += q.y; z += q.z; w += q.w;
        return *this;
    }

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param q: The other quaternion.
	 * @return reference to this after subtraction.
	 */
    inline quat& operator-=(const quat& q){
        x -= q.x; y -= q.y; z -= q.z; w -= q.w;
        return *this;
    }

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
    inline quat& operator*=(const quat& q){
        *this = *this * q;
        return *this;
    }
	
	/**
	 * Multiply this quaternion by a scaling factor.
	 *
	 * @param scale: The scaling factor.
	 * @return a reference to this after scaling.
	 */
    inline quat& operator*=(const float scale){
        x *= scale; y *= scale; z *= scale; w *= scale;
        return *this;
    }
	
	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
    inline quat& operator/=(const float scale){
        const float recip = 1.0f / scale;
        x *= recip; y *= recip; z *= recip; w *= recip;
        return *this;
    }

 	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param q: The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
    bool operator==(const quat& q) const{
        return x == q.x && y == q.y && z == q.y && w == q.w;
    }

 	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param q: The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
    bool operator!=(const quat& q) const{
        return x != q.x || y != q.y || z != q.z || w != q.w;
    }

public:

	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param tolerance: Minimum squared length of quaternion for normalization.
	 */
    inline quat& normalize(float tolerance = SMALL_NUMBER){
        const float square_sum = x * x + y * y + z * z + w * w;
        if (square_sum > tolerance){
            const float scale = inv_sqrt(square_sum);
            x *= scale;
            y *= scale;
            z *= scale;
            w *= scale;
        }
        else{
            *this = identity();
        }
        return *this;
    }

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param tolerance Minimum squared length of quaternion for normalization.
	 */
    inline quat get_normalized(float tolerance = SMALL_NUMBER) const{
        quat q = *this;
        q.normalize(tolerance);
        return q;
    }

	// Return true if this quaternion is normalized
    inline bool is_normalized() const{
        return abs(1.f - length_squared()) < THRESH_QUAT_NORMALIZED;
    }

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
    inline float length() const{
        return sqrt(x * x + y * y + z * z + w * w);
    }

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
    inline float length_squared() const{
        return (x * x + y * y + z * z + w * w);
    }

	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
    inline quat log() const{
        quat r;
        r.w = 0.f;
        if (abs(w) < 1.f){
            const float angle = acos(w);
            const float sin_angle = sin(angle);
            if (abs(sin_angle) >= SMALL_NUMBER){
                const float scale = sin_angle / angle;
                r.x = scale * x;
                r.y = scale * y;
                r.z = scale * z;
                return r;
            }
        }

        r.x = x;
        r.y = y;
        r.z = z;
        return r;
    }

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
    inline quat exp() const{
        const float angle = sqrt(x * x + y * y + z * z);
        const float sin_angle = sin(angle);
        quat r;
        r.w = cos(angle);
        if (abs(sin_angle) >= SMALL_NUMBER){
            const float scale = sin_angle / angle;
            r.x = scale * x;
            r.y = scale * y;
            r.z = scale * z;
        }
        else{
            r.x = x;
            r.y = y;
            r.z = z;
        }
        return r;
    }

	/**
	 * @return inverse of this quaternion
	 */
    inline quat inverse() const{
        return quat(-x, -y, -z, w);
    }

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
    inline void enforce_shortest_arc_with(const quat& other){
        const float dot_result = other | *this;
        const float bias = float_select(dot_result, 1.0f, -1.0f);
        x *= bias; y *= bias; z *= bias; w *= bias;
    }

	/** Find the angular distance between two rotation quaternions (in radians) */
    inline float angular_distance(const quat& q) const{
        float inner_prod = x * q.x + y * q.y + z * q.z + w * q.w;
        return acos(2 * inner_prod * inner_prod - 1.f);
    }

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
	 *
	 * @return true if there are any non-finite values in this Quaternion, otherwise false.
	 */
    inline bool contains_nan() const{
        return(!is_finite(x) ||
               !is_finite(y) ||
               !is_finite(z) ||
               !is_finite(w));
    }

protected:
    //
    // Based on:
    // http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
    // http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
    //
    static quat findbetween_helper(const vector3d& a, const vector3d& b, float normal_ab)
    {
        float w = normal_ab + vector3d::dot_product(a, b);
        quat r;

        if (w >= 1e-6f * normal_ab)
        {
            r = quat(a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x,
                w);
        }
        else
        {
            // A and B point in opposite directions
            w = 0.f;
            r = abs(a.x) > abs(a.y)
                ? quat(-a.z, 0.f, a.x, w)
                : quat(0.f, -a.z, a.y, w);
        }

        r.normalize();
        return r;
    }

public:

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
    static inline quat find_between(const vector3d& vec1, const vector3d& vec2)
	{
        return find_between_vectors(vec1, vec2);
	}

    /**
    * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
    */
    static inline quat find_between_vectors(const vector3d& a, const vector3d& b){
        const float normal_ab = sqrt(a.length_squared() * b.length_squared());
        return findbetween_helper(a, b, normal_ab);
    }

	/**
	 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	 */
    static inline quat find_between_normals(const vector3d& a, const vector3d& b){
        const float normal_ab = 1.f;
        return findbetween_helper(a, b, normal_ab);
    }

	/**
	 * Error measure (angle) between two quaternions, ranged [0..1].
	 * Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though 
	 * @note normalized input is expected.
	 */
    static inline float error(const quat& q1, const quat& q2){
        const float cosom = abs(q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
        return (abs(cosom) < 0.9999999f) ? acos(cosom)*(1.f / kPI) : 0.0f;
    }

	/**
	 * quat::Error with auto-normalization.
	 */
    static inline float error_auto_normalize(const quat& a, const quat& b){
        quat q1 = a;
        q1.normalize();
        quat q2 = b;
        q2.normalize();

        return quat::error(q1, q2);
    }

	/** 
	 * Fast Linear Quaternion Interpolation.
	 * Result is NOT normalized.
	 */
    static inline quat fast_lerp(const quat& a, const quat& b, const float alpha){
        // To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
        const float dot_result = (a | b);
        const float bias = float_select(dot_result, 1.0f, -1.0f);
        return (b * alpha) + (a * (bias * (1.f - alpha)));
    }

	/** 
	 * Bi-Linear Quaternion interpolation.
	 * Result is NOT normalized.
	 */
    static inline quat fast_bilerp(const quat& p00, const quat& p10, const quat& p01, const quat& p11, float frac_x, float frac_y){
        return quat::fast_lerp(
            quat::fast_lerp(p00, p10, frac_x),
            quat::fast_lerp(p01, p11, frac_x),
            frac_y);
    }


	/** Spherical interpolation. Will correct alignment. Result is NOT normalized. */
    static quat slerp_not_normalized(const quat& q1, const quat& q2, float slerp){
        // Get cosine of angle between quats.
        const float raw_cosom = q1 | q2;

        // Unaligned quats - compensate, results in taking shorter route.
        const float cosom = float_select(raw_cosom, raw_cosom, -raw_cosom);

        float scale0, scale1;

        if (cosom < 0.9999f)
        {
            const float omega = acos(cosom);
            const float inv_sin = 1.f / sin(omega);
            scale0 = sin((1.f - slerp) * omega) * inv_sin;
            scale1 = sin(slerp * omega) * inv_sin;
        }
        else
        {
            // Use linear interpolation.
            scale0 = 1.0f - slerp;
            scale1 = slerp;
        }

        // In keeping with our flipped Cosom:
        scale1 = float_select(raw_cosom, scale1, -scale1);

        quat r;

        r.x = scale0 * q1.x + scale1 * q2.x;
        r.y = scale0 * q1.y + scale1 * q2.y;
        r.z = scale0 * q1.z + scale1 * q2.z;
        r.w = scale0 * q1.w + scale1 * q2.w;

        return r;
    }

	/** Spherical interpolation. Will correct alignment. Result is normalized. */
	static inline quat slerp(const quat& q1, const quat& q2, float slerp)
	{
        return slerp_not_normalized(q1, q2, slerp).get_normalized();
	}

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is NOT normalized.
	 */
    static quat slerp_fullpath_not_normalized(const quat& quat1, const quat& quat2, float alpha){
        const float cos_angle = clamp(quat1 | quat2, -1.f, 1.f);
        const float angle = acos(cos_angle);

        if (abs(angle) < KINDA_SMALL_NUMBER)
        {
            return quat1;
        }

        const float sin_angle = sin(angle);
        const float inv_sin_angle = 1.f / sin_angle;

        const float scale0 = sin((1.0f - alpha) * angle) * inv_sin_angle;
        const float scale1 = sin(alpha * angle) * inv_sin_angle;

        return quat1 * scale0 + quat2 * scale1;
    }

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is normalized.
	 */
	static inline quat slerp_fullpath(const quat &quat1, const quat &quat2, float alpha)
	{
        return slerp_fullpath_not_normalized(quat1, quat2, alpha).get_normalized();
	}
	
	/**
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 * This will correct alignment by ensuring that the shortest path is taken.
	 */
    static quat squad(const quat& quat1, const quat& tang1, const quat& quat2, const quat& tang2, float alpha){
        // Always slerp along the short path from quat1 to quat2 to prevent axis flipping.
        // This approach is taken by OGRE engine, amongst others.
        const quat q1 = quat::slerp_not_normalized(quat1, quat2, alpha);
        const quat q2 = quat::slerp_fullpath_not_normalized(tang1, tang2, alpha);
        quat r = slerp_fullpath(q1, q2, 2.f * alpha * (1.f - alpha));

        return r;
    }

	/**
	 * Simpler Squad that doesn't do any checks for 'shortest distance' etc.
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 */
    static quat squad_fullpath(const quat& quat1, const quat& tang1, const quat& quat2, const quat& tang2, float alpha){
        const quat q1 = quat::slerp_fullpath_not_normalized(quat1, quat2, alpha);
        const quat q2 = quat::slerp_fullpath_not_normalized(tang1, tang2, alpha);
        quat r = quat::slerp_fullpath(q1, q2, 2.f * alpha * (1.f - alpha));

        return r;
    }

	/**
	 * Calculate tangents between given points
	 *
	 * @param prev_p: quaternion at P-1
	 * @param p: quaternion to return the tangent
	 * @param next_p quaternion P+1
	 * @param tension @todo document
	 * @param out_tan Out control point
	 */
    static void csalc_tangents(const quat& prev_p, const quat& p, const quat& next_p, float tension, quat& out_tan){
        const quat inv_p = p.inverse();
        const quat part1 = (inv_p * prev_p).log();
        const quat part2 = (inv_p * next_p).log();

        const quat pre_exp = (part1 + part2) * -0.5f;

        out_tan = p * pre_exp.exp();
    }

} GCC_ALIGN(16);

} // end namespace math
} // end namespace utility

#endif