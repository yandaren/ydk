/**
 *
 * vector4d.hpp
 *
 * vector4d implementation
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-04
 */

#ifndef __ydk_utility_math_vector_4d_hpp__
#define __ydk_utility_math_vector_4d_hpp__

#include <utility/math/vector3d.hpp>

namespace utility{
namespace math{

/** 
 * vector4d, 4x1 FLOATs, 16-byte aligned.
 */
MS_ALIGN(16) struct vector4d
{
public:

    /** The vector's X-component. */
    float x;

    /** The vector's Y-component. */
    float y;

    /** The vector's Z-component. */
    float z;

    /** The vector's W-component. */
    float w;

public:
    /**
     * Default constructor
     * Creates and initializes a new vector to zero.
     */
    inline vector4d() : x(0), y(0), z(0), w(0){}

    /**
     * @brief Constructor initializing all components to a single float value.
     *
     * @param v Value to set all components to.
     */
    explicit inline vector4d(float v) : x(v), y(v), z(v), w(v){}

    /**
     * @brief Creates and initializes a new vector from the specified components.
     *
     * @param in_x: X Coordinate.
     * @param in_y: Y Coordinate.
     * @param in_z: Z Coordinate.
     * @param in_w: W Coordinate.
     */
    explicit vector4d(float in_x, float in_y, float in_z, float in_w)
        :x(in_x), y(in_y), z(in_z), w(in_w){}

    /**
     * @brief Constructor.
     *
     * @param InVector 3D Vector to set first three components.
     * @param InW W Coordinate.
     */
    inline vector4d(const vector3d& v, float in_w = 0.0f)
        :x(v.x), y(v.y), z(v.z), w(in_w){}

    /**
     * @brief Creates and initializes a new vector from the specified 2D vectors.
     *
     * @param in_xy: A 2D vector holding the X- and Y-components.
     * @param in_zw: A 2D vector holding the Z- and W-components.
     */
    explicit vector4d(const vector2d& in_xy, const vector2d& in_zw)
        :x(in_xy.x), y(in_xy.y), z(in_zw.x), w(in_zw.y){}

public:
    /**
     * @brief Access a specific component of the vector.
     *
     * @param index: The index of the component.
     * @return Reference to the desired component.
     */
    inline float& operator[](int32_t index){
        return (&x)[index];
    }

    /**
     * @brief Access a specific component of the vector.
     *
     * @param index: The index of the component.
     * @return Copy of the desired component.
     */
    inline float operator[](int32_t index) const{
        return (&x)[index];
    }

    /**
     * @brief Gets a negated copy of the vector.
     *
     * @return A negated copy of the vector.
     */
    inline vector4d operator-() const{
        return vector4d(-x, -y, -z, -w);
    }

    /**
     * @brief Calculate Cross product between this and another vector.
     *
     * @param v: The other vector.
     * @return The Cross product.
     */
    inline vector4d operator^(const vector4d& v) const{
        return vector4d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0.0f);
    }

    /**
     * @brief Gets the result of adding a vector to this.
     *
     * @param v: The vector to add.
     * @return The result of vector addition.
     */
    inline vector4d operator+(const vector4d& v) const{
        return vector4d(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    /**
     * @brief Gets the result of subtracting a vector from this.
     *
     * @param v: The vector to subtract.
     * @return The result of vector subtraction.
     */
    inline vector4d operator-(const vector4d& v) const{
        return vector4d(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    /**
     * @brief Gets the result of scaling this vector.
     *
     * @param scale: The scaling factor.
     * @return The result of vector scaling.
     */
    inline vector4d operator*(float scale) const{
        return vector4d(x * scale, y * scale, z * scale, w * scale);
    }

    /**
     * @brief Scales a vector.
     *
     * @param scale: The scaling factor.
     * @param v: The vector to scale.
     * @return The result of scaling.
     */
    friend inline vector4d operator*(float scale, const vector4d& v)
    {
        return v.operator*(scale);
    }

    /**
     * @brief Gets the result of dividing this vector.
     *
     * @param scale: What to divide by.
     * @return The result of division.
     */
    inline vector4d operator/(float scale) const{
        const float r_scale = 1.f / scale;
        return vector4d(x * r_scale, y * r_scale, z * r_scale, w * r_scale);
    }

    /**
     * @brief Gets the result of dividing this vector.
     *
     * @param v: What to divide by.
     * @return The result of division.
     */
    inline vector4d operator/(const vector4d& v) const{
        return vector4d(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    /**
     * @brief Gets the result of multiplying a vector with this.
     *
     * @param v: The vector to multiply.
     * @return The result of vector multiplication.
     */
    inline vector4d operator*(const vector4d& v) const{
        return vector4d(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    /**
     * @brief Adds another vector to this one.
     *
     * @param v: The other vector to add.
     * @return the self reference of the vector.
     */
    inline vector4d& operator+=(const vector4d& v){
        x += v.x; y += v.y; z += v.z; w += v.w;
        return *this;
    }

    /**
     * @brief Gets the result of multiplying a vector with another Vector (component wise).
     *
     * @param v: The vector to multiply.
     * @return the self reference of the vector.
     */
    inline vector4d& operator*=(const vector4d& v){
        x *= v.x; y *= v.y; z *= v.z; w *= v.w;
        return *this;
    }

    /**
     * @brief Gets the result of dividing a vector with another Vector (component wise).
     *
     * @param v: The vector to divide with.
     * @return the self reference of the vector.
     s*/
    inline vector4d& operator/=(const vector4d& v){
        x /= v.x; y /= v.y; z /= v.z; w /= v.w;
        return *this;
    }

    /**
     * @brief Gets the result of scaling this vector.
     *
     * @param scale: The scaling factor.
     * @return the self reference of the vector.
     */
    inline vector4d& operator*=(float scale){
        x *= scale; y *= scale; z *= scale; w *= scale;
        return *this;
    }

    /**
     * @brief Checks for equality against another vector.
     *
     * @param v: The other vector.
     * @return true if the two vectors are the same, otherwise false.
     */
    inline bool operator==(const vector4d& v) const{
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    /**
     * @brief Checks for inequality against another vector.
     *
     * @param v: The other vector.
     * @return true if the two vectors are different, otherwise false.
     */
    inline bool operator!=(const vector4d& v) const{
        return x != v.x || y != v.y || z != v.z || w != v.w;
    }

public:
    /**
     * @brief Calculates 3D Dot product of two 4D vectors.
     *
     * @param v1: The first vector.
     * @param v2: The second vector.
     * @return The 3D Dot product.
     */
    inline static float dot3(const vector4d& v1, const vector4d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    /**
     * @brief Calculates 4D Dot product.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The 4D Dot Product.
     */
    inline static float dot4(const vector4d& v1, const vector4d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    /**
     * @brief Error tolerant comparison.
     *
     * @param v: Vector to compare against.
     * @param tolerance: Error Tolerance.
     * @return true if the two vectors are equal within specified tolerance, otherwise false.
     */
    inline bool equals(const vector4d& v, float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x - v.x) <= tolerance && abs(y - v.y) <= tolerance && abs(z - v.z) <= tolerance && abs(w - v.w) <= tolerance;
    }

    /**
     * @brief Check if the vector is of unit length, with specified tolerance.
     *
     * @param length_squared_tolerance: Tolerance against squared length.
     * @return true if the vector is a unit vector within the specified tolerance.
     */
    inline bool is_unit3(float length_squared_tolerance = KINDA_SMALL_NUMBER) const{
        return abs(1.0f - length_squared_3d()) < length_squared_tolerance;
    }

    /**
     * @brief Returns a normalized copy of the vector if safe to normalize.
     *
     * @param tolerance: Minimum squared length of vector for normalization.
     * @return A normalized copy of the vector or a zero vector.
     */
    inline vector4d get_safe_normal(float tolerance = SMALL_NUMBER) const{
        const float square_sum = x * x + y * y + z * z;
        if (square_sum > tolerance){
            const float scale = inv_sqrt(square_sum);
            return vector4d(x * scale, y * scale, z * scale, 0.0f);
        }
        return vector4d(0.f);
    }

    /**
     * @brief Calculates normalized version of vector without checking if it is non-zero.
     *
     * @return Normalized version of vector.
     */
    inline vector4d get_unsafe_normal3() const{
        const float scale = inv_sqrt(x * x + y * y + z * z);
        return vector4d(x * scale, y * scale, z * scale, 0.0f);
    }

    /**
     * @brief Set all of the vectors coordinates.
     *
     * @param in_x: New X Coordinate.
     * @param in_y: New Y Coordinate.
     * @param in_z: New Z Coordinate.
     * @param in_w: New W Coordinate.
     */
    inline void set(float in_x, float in_y, float in_z, float in_w){
        x = in_x; y = in_y; z = in_z; w = in_w;
    }

    /**
     * @brief Get the length of this vector not taking W component into account.
     *
     * @return The length of this vector.
     */
    inline float length_3d() const{
        return sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief Get the squared length of this vector not taking W component into account.
     *
     * @return The squared length of this vector.
     */
    inline float length_squared_3d() const{
        return x * x + y * y + z * z;
    }

    /**
     * @brief Get the length (magnitude) of this vector, taking the W component into account
     *
     * @return The length of this vector
     */
    inline float length() const{
        return sqrt(x * x + y * y + z * z + w * w);
    }

    /**
     * @brief Get the squared length of this vector, taking the W component into account
     *
     * @return The squared length of this vector
     */
    inline float length_squared() const{
        return x * x + y * y + z * z + w * w;
    }

    /** Utility to check if there are any non-finite values (NaN or Inf) in this vector. */
    inline bool contains_nan() const{
        return (
            is_nan(x) || !is_finite(x) ||
            is_nan(y) || !is_finite(y) ||
            is_nan(z) || !is_finite(z) ||
            is_nan(w) || !is_finite(w)
            );
    }

    /** Utility to check if all of the components of this vector are nearly zero given the tolerance. */
    inline bool is_nearly_zero3(float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x) <= tolerance && abs(y) <= tolerance && abs(z) <= tolerance;
    }

    /** Reflect vector. */
    inline vector4d reflect3(const vector4d& normal) const{
        return 2.0f * dot3(*this, normal) * normal - *this;
    }

    /**
     * @brief Find good arbitrary axis vectors to represent U and V axes of a plane,
     * given just the normal.
     */
    inline void find_best_axis_vectors3(vector4d& axis1, vector4d& axis2) const{
        const float nx = abs(x);
        const float ny = abs(y);
        const float nz = abs(z);

        // find best basis vectors
        if (nz > nx && nz > ny) axis1 = vector4d(1, 0, 0, 0);
        else                    axis1 = vector4d(0, 0, 1, 0);

        axis1 = (axis1 - *this * dot3(axis1, *this)).get_safe_normal();
        axis2 = axis1 ^ *this;
    }

}GCC_ALIGN(16);

} // end namespace math
} // end namespace utility


#endif