/**
 *
 * vector2d.hpp
 *
 * vector2d implementation
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-04
 */

#ifndef __ydk_utility_math_vector_2d_hpp__
#define __ydk_utility_math_vector_2d_hpp__

#include <utility/math/math.hpp>
#include <base/align.hpp>

namespace utility{
namespace math{

/*2d vector, 16-byte aligned */
MS_ALIGN(16) struct vector2d
{
    /** Vector's X component. */
    float x;

    /** Vector's Y Component. */
    float y;

public:
    /* Global 2D Zero vector constant (0, 0) */
    static const vector2d& zero_vector()
    {
        static const vector2d zvector(0, 0);

        return zvector;
    }

    /* Global 2D unit vector constant (1, 1) */
    static const vector2d& unit_vector()
    {
        static const vector2d u_vector(1, 1);

        return u_vector;
    }

public:
    inline vector2d() : x(0), y(0){}
    inline vector2d(float in_x, float in_y) : x(in_x), y(in_y){}
    inline vector2d(const vector2d& v) : x(v.x), y(v.y){}

public:
    /** 
     * @brief Gets the result of adding two vectors together
     */
    inline vector2d operator+(const vector2d& v) const{
        return vector2d(x + v.x, y + v.y);
    }

    /** 
     * @brief Gets the result of subtracting a vector from this one
     */
    inline vector2d operator-(const vector2d& v) const{
        return vector2d(x - v.x, y - v.y);
    }

    /** 
     * @brief Gets the result of scaling the vector ( mutiplying each component by a value)
     */
    inline vector2d operator*(const float scale) const{
        return vector2d(x * scale, y * scale);
    }

    /**
    * @brief scale * vector
    */
    friend vector2d operator*(float scale, const vector2d& v)
    {
        return v.operator* (scale);
    }

    /** 
     * @brief Gets the result of diving each component of the vector by a value
     */
    inline vector2d operator/(const float scale) const{
        const float r_scale = 1.f / scale;
        return vector2d(x * r_scale, y * r_scale);
    }

    /** 
     * @brief Gets the result of this vector + float x.
     *
     * @param in_x     Float to add to each component.
     */
    inline vector2d operator+(float in_x) const{
        return vector2d(x + in_x, y + in_x);
    }

    /** 
     * @brief the result of subtracting from each component of the vector.
     *
     * @param in_x		Float to subtract from each component
     */
    inline vector2d operator-(float in_x) const{
        return vector2d(x - in_x, y - in_x);
    }

    /**
     * @brief Gets the result of component-wise multiplication of this vector by another.
     *
     * @param v The other vector to multiply this by.
     */
    inline vector2d operator*(const vector2d& v) const{
        return vector2d(x * v.x, y * v.y);
    }

    /**
     * @brief gets the result of component-wise division of this vector by another.
     *
     * @param V The other vector to divide this by.
     */
    inline vector2d operator/(const vector2d& v) const{
        return vector2d(x / v.x, y / v.y);
    }

    /**
     * @brief Calculates dot product of this vector and another.
     *
     * @param v The other vector.
     * @return the dot product of this vector and another.
     */
    inline float operator|(const vector2d& v) const{
        return x * v.x + y * v.y;
    }

    /**
     * @brief Calculates cross product of this vector and another.
     *
     * @param v The other vector.
     * @return the cross product of this vector and another.
     */
    inline float operator^(const vector2d& v) const{
        return x * v.y - y * v.x;
    }

public:

    /**
     * @brief Compares this vector against another for equality.
     *
     * @param v The vector to compare against.
     * @return true if the two vectors are equal, otherwise false.
     */
    inline bool operator==(const vector2d& v) const{
        return x == v.x && y == v.y;
    }

    /**
     * @brief Compares this vector against another for inequality.
     *
     * @param v The vector to compare against.
     * @return true if the two vectors are not equal, otherwise false.
     */
    inline bool operator!=(const vector2d& v) const{
        return x != v.x || y != v.y;
    }

    /**
     * @brief Gets a negated copy of the vector.
     *
     * @return a negated copy of the vector.
     */
    inline vector2d operator-() const{
        return vector2d(-x, -y);
    }

    /**
     * @brief Adds another vector to this.
     *
     * @param v The other vector to add.
     * @return the self reference of the vector.
     */
    inline vector2d& operator+=(const vector2d& v){
        x += v.x;
        y += v.y;
        return *this;
    }

    /**
     * @brief Subtracts another vector from this.
     *
     * @param V The other vector to subtract.
     * @return the self reference of the vector.
     */
    inline vector2d& operator-=(const vector2d& v){
        x -= v.x;
        y -= v.y;
        return *this;
    }

    /**
     * @brief Scales this vector.
     *
     * @param Scale The scale to multiply vector by.
     * @return the self reference of the vector.
     */
    inline vector2d& operator*=(float scale){
        x *= scale;
        y *= scale;
        return *this;
    }

    /**
     * @brief Divides this vector.
     *
     * @param v What to divide vector by.
     * @return the self reference of the vector.
     */
    inline vector2d& operator/=(float v){
        const float rv = 1.f / v;
        x *= rv; y *= rv;
        return *this;
    }

    /**
     * @brief Multiplies this vector with another vector, using component-wise multiplication.
     *
     * @param V The vector to multiply with.
     * @return the self reference of the vector.
     */
    inline vector2d& operator*=(const vector2d& v){
        x *= v.x;
        y *= v.y;
        return *this;
    }

    /**
     * @breif Divides this vector by another vector, using component-wise division.
     *
     * @param v The vector to divide by.
     * @return the self reference of the vector.
     */
    inline vector2d& operator/=(const vector2d& v){
        x /= v.x;
        y /= v.y;
        return *this;
    }

    /**
     * @brief Gets specific component of the vector.
     *
     * @param Index the index of vector component
     * @return reference to component.
     */
    inline float& operator[](int32_t index){
        return (&x)[index];
    }

    /**
     * @brief Gets specific component of the vector.
     *
     * @param Index the index of vector component
     * @return copy of component value.
     */
    inline float operator[](int32_t index) const{
        return (&x)[index];
    }

public:

    /**
     * @brief Calculates the dot product of two vectors.
     *
     * @param x The first vector.
     * @param y The second vector.
     * @return The dot product.
     */
    inline static float dot_product(const vector2d& x, const vector2d& y){
        return x | y;
    }

    /**
     * @brief Squared distance between two 2D points.
     *
     * @param v1 The first point.
     * @param v2 The second point.
     * @return The squared distance between two 2D points.
     */
    inline static float dist_squared(const vector2d& v1, const vector2d& v2){
        return square(v1.x - v2.x) + square(v1.y - v2.y);
    }

    /**
     * @brief Distance between two 2D points.
     *
     * @param v1 The first point.
     * @param v2 The second point.
     * @return The squared distance between two 2D points.
     */
    inline static float distance(const vector2d& v1, const vector2d& v2){
        return sqrt(vector2d::dist_squared(v1, v2));
    }

    /**
     * @brief Calculate the cross product of two vectors.
     *
     * @param x The first vector.
     * @param y The second vector.
     * @return The cross product.
     */
    inline static float cross_product(const vector2d& x, const vector2d& y){
        return x ^ y;
    }

    /**
     * @brief Checks for equality with error-tolerant comparison.
     *
     * @param v The vector to compare.
     * @param tolerance Error tolerance
     * @return true if the vectors are equal within specified tolerance, otherwise false.
     */
    inline bool equals(const vector2d& v, float tolerance) const{
        return abs(x - v.x) < tolerance && abs(y - v.y) < tolerance;
    }

    /**
     * @brief Set the values of the vector directly.
     *
     * @param in_x New X coordinate.
     * @param in_y New Y coordinate.
     */
    inline void set(float in_x, float in_y){
        x = in_x;
        y = in_y;
    }

    /**
     * @brief Get the maximum value of the vector's components.
     *
     * @return The maximum value of the vector's components.
     */
    inline float get_max() const{
        return max2(x, y);
    }

    /**
     * @brief Get the maximum absolute value of the vector's components.
     *
     * @return The maximum absolute value of the vector's components.
     */
    inline float get_abs_max() const{
        return max2(abs(x), abs(y));
    }

    /**
     * @brief Get the minimum value of the vector's components.
     *
     * @return The minimum value of the vector's components.
     */
    inline float get_min() const{
        return min2(x, y);
    }

    /**
     * @brief Get the length (magnitude) of this vector.
     *
     * @return The length of this vector.
     */
    inline float length() const{
        return sqrt(x * x + y * y);
    }

    /**
     * @brief Get the squared length of this vector.
     *
     * @return The squared length of this vector.
     */
    inline float length_squared() const{
        return x * x + y * y;
    }

    /**
     * @brief Gets a normalized copy of the vector, checking it is safe to do so based on the length.
     * Returns zero vector if vector length is too small to safely normalize.
     *
     * @param tolerance Minimum squared length of vector for normalization.
     * @return A normalized copy of the vector if safe, (0,0) otherwise.
     */
    inline vector2d safe_normal(float tolerance = SMALL_NUMBER) const{
        const float square_sum = x * x + y * y;
        if (square_sum > tolerance){
            const float scale = inv_sqrt(square_sum);
            return vector2d(x * scale, y * scale);
        }
        return vector2d(0.f, 0.f);
    }

    /**
     * @brief Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
     *
     * @param Tolerance Minimum squared length of vector for normalization.
     * @see SafeNormal()
     */
    inline void normalize(float tolerance = SMALL_NUMBER){
        const float square_sum = x * x + y * y;
        if (square_sum > tolerance){
            const float scale = inv_sqrt(square_sum);
            x *= scale;
            y *= scale;
            return;
        }
        x = 0.f;
        y = 0.f;
    }

    /**
     * @brief Checks whether vector is near to zero within a specified tolerance.
     *
     * @param tolerance Error tolerance.
     * @return true if vector is in tolerance to zero, otherwise false.
     */
    inline bool is_neary_zero(float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x) < tolerance && abs(y) < tolerance;
    }

    /**
     * @brief Checks whether all components of the vector are exactly zero.
     *
     * @return true if vector is exactly zero, otherwise false.
     */
    inline bool is_zero() const{
        return x == 0.f && y == 0.f;
    }

    /**
     * @brief Creates a copy of this vector with both axes clamped to the given range.
     * @return New vector with clamped axes.
     */
    inline vector2d clamp_axes(float min_axis_val, float max_axis_val) const{
        return vector2d(clamp(x, min_axis_val, max_axis_val), clamp(y, min_axis_val, max_axis_val));
    }
}GCC_ALIGN(16);

} // end namespace math
} // end namespace utility

#endif