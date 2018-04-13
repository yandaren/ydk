/**
 *
 * vector3d.hpp
 *
 * vector3d implementation
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-04
 */

#ifndef __ydk_utility_math_vector_3d_hpp__
#define __ydk_utility_math_vector_3d_hpp__

#include <utility/math/vector2d.hpp>

namespace utility{
namespace math{

/** 
 * @brief vector 3d, 16-byte aligned
 */
MS_ALIGN(16) struct vector3d
{
public:
    /** vector3d's X component. */
    float x;

    /** vector3d's Y component. */
    float y;

    /** vector3d's Z component. */
    float z;

public:
    /** A zero vector (0,0,0) */
    static const vector3d& zero_vector()
    {
        static const vector3d z_vector(0, 0, 0);

        return z_vector;
    }

    /** World up vector (0,0,1) */
    static const vector3d& up_vector()
    {
        static const vector3d up_vec(0, 0, 1);

        return up_vec;
    }

    /** Unreal forward vector (1,0,0) */
    static const vector3d& forward_vector()
    {
        static const vector3d f_vec(1, 0, 0);

        return f_vec;
    }

    void zero(){
        x = y = z = 0;
    }

public:
    /** 
     * @brief default constructor
     */
    inline vector3d() :x(0), y(0), z(0){}
    
    /**
     * @brief Constructor initializing all components to a single float value.
     *
     * @param v Value to set all components to.
     */
    explicit inline vector3d(float v) : x(v), y(v), z(v){}

    /**
     * @brief Constructor using initial values for each component.
     *
     * @param in_x X Coordinate.
     * @param in_y Y Coordinate.
     * @param in_z Z Coordinate.
     */
    inline vector3d(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z){}

    /**
     * @brief Constructs a vector from an vector2D and Z value.
     *
     * @param v vector3d to copy from.
     * @param in_z Z Coordinate.
     */
    explicit inline vector3d(const vector2d& v, float in_z) : x(v.x), y(v.y), z(in_z){}

public:
    /**
     * @brief Calculate cross product between this and another vector.
     *
     * @param v The other vector.
     * @return The cross product.
     */
    inline vector3d operator^(const vector3d& v) const{
        return vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    /**
     * @brief Calculate the dot product between this and another vector.
     *
     * @param v The other vector.
     * @return The dot product.
     */
    inline float operator|(const vector3d& v) const{
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * @brief Gets the result of component-wise addition of this and another vector.
     *
     * @param v The vector to add to this.
     * @return The result of vector addition.
     */
    inline vector3d operator+(const vector3d& v) const{
        return vector3d(x + v.x, y + v.y, z + v.z);
    }

    /**
     * @brief Gets the result of component-wise subtraction of this by another vector.
     *
     * @param v The vector to subtract from this.
     * @return The result of vector subtraction.
     */
    inline vector3d operator-(const vector3d& v) const{
        return vector3d(x - v.x, y - v.y, z - v.z);
    }

    /**
     * @brief Gets the result of subtracting from each component of the vector.
     *
     * @param bias How much to subtract from each component.
     * @return The result of subtraction.
     */
    inline vector3d operator-(float bias) const{
        return vector3d(x - bias, y - bias, z - bias);
    }

    /**
     * @brief Gets the result of adding to each component of the vector.
     *
     * @param bias How much to add to each component.
     * @return The result of addition.
     */
    inline vector3d operator+(float bias) const{
        return vector3d(x + bias, y + bias, z + bias);
    }

    /**
     * @brief Gets the result of scaling the vector (multiplying each component by a value).
     *
     * @param scale What to multiply each component by.
     * @return The result of multiplication.
     */
    inline vector3d operator*(float scale) const{
        return vector3d(x * scale, y * scale, z * scale);
    }

    /**
     * @brief scale * vector
     */
    friend vector3d operator*(float scale, const vector3d& v)
    {
        return v.operator* (scale);
    }

    /**
     * @brief Gets the result of dividing each component of the vector by a value.
     *
     * @param scale What to divide each component by.
     * @return The result of division.
     */
    inline vector3d operator/(float scale) const{
        const float r_scale = 1.f / scale;
        return vector3d(x * r_scale, y * r_scale, z * r_scale);
    }

    /**
     * @brief Gets the result of component-wise multiplication of this vector by another.
     *
     * @param v The vector to multiply with.
     * @return The result of multiplication.
     */
    inline vector3d operator*(const vector3d& v) const{
        return vector3d(x * v.x, y * v.y, z * v.z);
    }

    /**
     * @brief Gets the result of component-wise division of this vector by another.
     *
     * @param v The vector to divide by.
     * @return The result of division.
     */
    inline vector3d operator/(const vector3d& v) const{
        return vector3d(x / v.x, y / v.y, z / v.z);
    }

    /**
     * @brief Check against another vector for equality.
     *
     * @param v The vector to check against.
     * @return true if the vectors are equal, false otherwise.
     */
    bool operator==(const vector3d& v) const{
        return x == v.x && y == v.y && z == v.z;
    }

    /**
     * @brief Check against another vector for inequality.
     *
     * @param v The vector to check against.
     * @return true if the vectors are not equal, false otherwise.
     */
    bool operator!=(const vector3d& v) const{
        return x != v.x || y != v.y || z != v.z;
    }

    /**
     * @brief Get a negated copy of the vector.
     *
     * @return a negated copy of the vector.
     */
    inline vector3d operator-() const{
        return vector3d(-x, -y, -z);
    }

    /**
     * @brief Adds another vector to this.
     * Uses component-wise addition.
     *
     * @param v vector3d to add to this.
     * @return the self reference of the vector.
     */
    inline vector3d& operator+=(const vector3d& v){
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    /**
     * @brief Subtracts another vector from this.
     * Uses component-wise subtraction.
     *
     * @param v vector3d to subtract from this.
     * @return the self reference of the vector.
     */
    inline vector3d& operator-=(const vector3d& v){
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    /**
     * @brief Scales the vector.
     *
     * @param scale Amount to scale this vector by.
     * @return the self reference of the vector.
     */
    inline vector3d& operator*=(float scale){
        x *= scale; y *= scale; z *= scale;
        return *this;
    }

    /**
     * @brief Divides the vector by a number.
     *
     * @param v What to divide this vector by.
      * @return the self reference of the vector.
     */
    inline vector3d& operator/=(float v){
        const float rv = 1.f / v;
        x *= rv; y *= rv; z *= rv;
        return *this;
    }

    /**
     * @brief Multiplies the vector with another vector, using component-wise multiplication.
     *
     * @param v What to multiply this vector with.
     * @return the self reference of the vector.
     */
    inline vector3d& operator*=(const vector3d& v){
        x *= v.x; y *= v.y; z *= v.z;
        return *this;
    }

    /**
     * @brief Divides the vector by another vector, using component-wise division.
     *
     * @param V What to divide vector by.
     * @return the self reference of the vector.
     */
    inline vector3d& operator/=(const vector3d& v){
        x /= v.x; y /= v.y; z /= v.z;
        return *this;
    }

    /**
     * @brief Gets specific component of the vector.
     *
     * @param index the index of vector component
     * @return reference to component.
     */
    inline float& operator[](int32_t index){
        return (&x)[index];
    }

    /**
     * @brief Gets specific component of the vector.
     *
     * @param Index the index of vector component
     * @return Copy of the component.
     */
    inline float operator[](int32_t index)const{
        return (&x)[index];
    }

public:
    /**
     * @brief Calculate the cross product of two vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The cross product.
     */
    inline static vector3d cross_product(const vector3d& v1, const vector3d& v2){
        return v1 ^ v2;
    }

    /**
     * @brief Calculate the dot product of two vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The dot product.
     */
    inline static float dot_product(const vector3d& v1, const vector3d& v2){
        return v1 | v2;
    }

    /**
     * @brief Check against another vector for equality, within specified error limits.
     *
     * @param v The vector to check against.
     * @param tolerance Error tolerance.
     * @return true if the vectors are equal within tolerance limits, false otherwise.
     */
    bool equals(const vector3d& v, float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x - v.x) < tolerance && abs(y - v.y) < tolerance && abs(z - v.z) < tolerance;
    }

    /**
     * @brief Checks whether all components of this vector are the same, within a tolerance.
     *
     * @param tolerance Error tolerance.
     * @return true if the vectors are equal within tolerance limits, false otherwise.
     */
    bool all_components_equal(float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x - y) < tolerance && abs(x - z) < tolerance && abs(y - z) < tolerance;
    }


public:

    /**
     * @brief Set the values of the vector directly.
     *
     * @param in_x New X coordinate.
     * @param in_y New Y coordinate.
     * @param in_z New Z coordinate.
     */
    inline void set(float in_x, float in_y, float in_z){
        x = in_x;
        y = in_y;
        z = in_z;
    }

    /**
     * @brief Get the maximum value of the vector's components.
     *
     * @return The maximum value of the vector's components.
     */
    inline float get_max() const{
        return max3(x, y, z);
    }

    /**
     * @brief Get the maximum absolute value of the vector's components.
     *
     * @return The maximum absolute value of the vector's components.
     */
    inline float get_abs_max() const{
        return max3(abs(x), abs(y), abs(z));
    }

    /**
     * @brief Get the minimum value of the vector's components.
     *
     * @return The minimum value of the vector's components.
     */
    inline float get_min() const{
        return min3(x, y, z);
    }

    /**
     * @brief Get the minimum absolute value of the vector's components.
     *
     * @return The minimum absolute value of the vector's components.
     */
    inline float get_abs_min() const{
        return min3(abs(x), abs(y), abs(z));
    }

    /** Gets the component-wise min of two vectors. */
    inline vector3d component_min(const vector3d& other) const{
        return vector3d(min2(x, other.x), min2(y, other.y), min2(z, other.z));
    }

    /** Gets the component-wise max of two vectors. */
    inline vector3d component_max(const vector3d& other) const{
        return vector3d(max2(x, other.x), max2(y, other.y), max2(z, other.z));
    }

    /**
     * @brief Get a copy of this vector with absolute value of each component.
     *
     * @return a copy of this vector with absolute value of each component.
     */
    inline vector3d get_abs() const{
        return vector3d(abs(x), abs(y), abs(z));
    }

    /**
     * @brief Get the length (magnitude) of this vector.
     *
     * @return The length of this vector.
     */
    inline float length() const{
        return sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief Get the squared length of this vector.
     *
     * @return The squared length of this vector.
     */
    inline float length_squared() const{
        return x * x + y * y + z * z;
    }

    /**
     * @brief Get the length of the 2D components of this vector.
     *
     * @return The 2D length of this vector.
     */
    inline float length_2d() const{
        return sqrt(x * x + y * y);
    }

    /**
     * @brief Get the squared length of the 2D components of this vector.
     *
     * @return The squared 2D length of this vector.
     */
    inline float length_squared_2d() const{
        return x * x + y * y;
    }

    /**
     * @brief Checks whether vector is near to zero within a specified tolerance.
     *
     * @param Tolerance Error tolerance.
     * @return true if the vector is near to zero, false otherwise.
     */
    inline bool is_near_zero(float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x) < tolerance && abs(y) < tolerance && abs(z) < tolerance;
    }

    /**
     * @brief Checks whether all components of the vector are exactly zero.
     *
     * @return true if the vector is exactly zero, false otherwise.
     */
    inline bool is_zero() const{
        return x == 0.f && y == 0.f && z == 0.f;
    }

    /**
     * @brief Normalize this vector in-place if it is large enough, set it to (0,0,0) otherwise.
     *
     * @param tolerance Minimum squared length of vector for normalization.
     * @return true if the vector was normalized correctly, false otherwise.
     */
    inline bool normalize(float tolerance = SMALL_NUMBER){
        const float square_sum = x * x + y * y + z * z;
        if (square_sum > tolerance){
            const float scale = inv_sqrt(square_sum);
            x *= scale; y *= scale; z *= scale;
            return true;
        }
        return false;
    }

    /**
     * @brief Checks whether vector is normalized.
     *
     * @return true if Normalized, false otherwise.
     */
    inline bool is_normalized() const{
        return (abs(1.f - length_squared()) < THRESH_VECTOR_NORMALIZED);
    }

    /**
     * @brief Util to convert this vector into a unit direction vector and its original length.
     *
     * @param out_dir Reference passed in to store unit direction vector.
     * @param out_length Reference passed in to store length of the vector.
     */
    inline void to_direction_and_length(vector3d &out_dir, float &out_length) const{
        out_length = length();
        if (out_length > SMALL_NUMBER){
            float one_over_length = 1.0f / out_length;
            out_dir = vector3d(x * one_over_length, y * one_over_length, z * one_over_length);
        }
        else{
            out_dir = vector3d::zero_vector();
        }
    }

    /**
     * @brief Get a copy of the vector as sign only.
     * Each component is set to +1 or -1, with the sign of zero treated as +1.
     *
     * @param A copy of the vector with each component set to +1 or -1
     */
    inline vector3d get_sign_vector() const{
        return vector3d(
            float_select(x, 1.f, -1.f),
            float_select(y, 1.f, -1.f),
            float_select(z, 1.f, -1.f)
            );
    }

    /**
     * @brief Projects 2D components of vector based on Z.
     *
     * @return Projected version of vector based on Z.
     */
    inline vector3d projection() const{
        const float rz = 1.f / z;
        return vector3d(x * rz, y * rz, 1);
    }

    /**
     * @brief Calculates normalized version of vector without checking for zero length.
     *
     * @return Normalized version of vector.
     * @see safe_normal()
     */
    inline vector3d get_unsafe_normal() const{
        const float scale = inv_sqrt(x * x + y * y + z * z);
        return vector3d(x * scale, y * scale, z * scale);
    }

    /**
     * @brief Get a copy of this vector, clamped inside of a cube.
     *
     * @param radius: Half size of the cube.
     * @return A copy of this vector, bound by cube.
     */
    inline vector3d bound_to_cube(float radius) const{
        return vector3d(clamp(x, -radius, radius), clamp(y, -radius, radius), clamp(z, -radius, radius));
    }

    /** Create a copy of this vector, with its magnitude(length) clamped between Min and Max. */
    inline vector3d clamp_length(float min, float max) const{
        float vec_length = length();
        const vector3d vec_dir = (vec_length > SMALL_NUMBER) ? (*this / vec_length) : vector3d::zero_vector();
        vec_length = clamp(vec_length, min, max);
        return vec_length * vec_dir;
    }

    /** Create a copy of this vector, with the 2D magnitude clamped between Min and Max. Z is unchanged. */
    inline vector3d camp_length_2d(float min, float max) const{
        float vec_length_2d = length_2d();
        const vector3d vec_dir = (vec_length_2d > SMALL_NUMBER) ? (*this / vec_length_2d) : vector3d::zero_vector();

        vec_length_2d = clamp(vec_length_2d, min, max);

        return vector3d(vec_length_2d * vec_dir.x, vec_length_2d * vec_dir.y, z);
    }

    /** Create a copy of this vector, with its maximum magnitude(length) clamped to MaxSize. */
    inline vector3d clamp_max_length(float max_length) const{
        if (max_length < KINDA_SMALL_NUMBER){
            return vector3d::zero_vector();
        }
        const float vec_ls = length_squared();
        if (vec_ls > square(max_length)){
            const float scale = max_length * inv_sqrt(vec_ls);
            return vector3d(x * scale, y * scale, z * scale);
        }
        else{
            return *this;
        }
    }

    /** Create a copy of this vector, with the maximum 2D magnitude(length) clamped to MaxSize. Z is unchanged. */
    inline vector3d clamp_max_length_2d(float max_length) const{
        if (max_length < KINDA_SMALL_NUMBER){
            return vector3d(0.f, 0.f, z);
        }
        const float vec_ls_2d = length_squared_2d();
        if (vec_ls_2d > square(max_length)){
            const float scale = max_length * inv_sqrt(vec_ls_2d);
            return vector3d(x * scale, y * scale, z);
        }
    }

    /**
     * @brief Add a vector to this and clamp the result in a cube.
     *
     * @param v vector3d to add.
     * @param radius: Half size of the cube.
     */
    inline void add_bounded(const vector3d& v, float radius = (float)MAX_int16){
        *this = (*this + v).bound_to_cube(radius);
    }

    /**
     * @brief Gets the reciprocal of this vector, avoiding division by zero.
     * Zero components are set to BIG_NUMBER.
     *
     * @return Reciprocal of this vector.
     */
    inline vector3d reciprocal() const{
        vector3d rec_vec;
        rec_vec.x = (x != 0.f) ? 1.f / x : BIG_NUMBER;
        rec_vec.y = (y != 0.f) ? 1.f / y : BIG_NUMBER;
        rec_vec.z = (z != 0.f) ? 1.f / z : BIG_NUMBER;
        return rec_vec;
    }

    /**
     * Check whether X, Y and Z are nearly equal.
     *
     * @param tolerance Specified Tolerance.
     * @return true if X == Y == Z within the specified tolerance.
     */
    inline bool is_uniform(float tolerance = KINDA_SMALL_NUMBER) const{
        return abs(x - y) < tolerance && abs(y - z) < tolerance;
    }

    /**
     * @brief Mirror a vector about a normal vector.
     *
     * @param mirror_normal: Normal vector to mirror about.
     * @return Mirrored vector.
     */
    inline vector3d mirror_by_vector(const vector3d& mirror_normal) const{
        return *this - mirror_normal * (2.f * (*this | mirror_normal));
    }

    /**
     * Rotates around Axis (assumes Axis.Size() == 1).
     *
     * @param angle Angle to rotate (in degrees).
     * @param Axis Axis to rotate around.
     * @return Rotated vector3d.
     */
    inline vector3d rotate_angle_axis(const float angle_in_deg, const vector3d& axis) const{
        const float s = sin(angle_in_deg * kPI / 180.f);
        const float c = cos(angle_in_deg * kPI / 180.f);
        
        const float xx = axis.x * axis.x;
        const float yy = axis.y * axis.y;
        const float zz = axis.z * axis.z;

        const float xy = axis.x * axis.y;
        const float yz = axis.y * axis.z;
        const float zx = axis.z * axis.x;

        const float xs = axis.x * s;
        const float ys = axis.y * s;
        const float zs = axis.z * s;

        const float omc = 1.f - c;

        return vector3d(
            (omc * xx + c ) * x + (omc * xy - zs) * y + (omc * zx + ys) * z,
            (omc * xy + zs) * x + (omc * yy + c ) * y + (omc * yz - xs) * z,
            (omc * zx - ys) * x + (omc * yz + xs) * y + (omc * zz + c ) * z
            );
    }

    /**
     * @brief Gets a normalized copy of the vector, checking it is safe to do so based on the length.
     * Returns zero vector if vector length is too small to safely normalize.
     *
     * @param tolerance Minimum squared vector length.
     * @return A normalized copy if safe, (0,0,0) otherwise.
     */
    inline vector3d get_safe_normal(float tolerance = SMALL_NUMBER) const{
        const float square_sum = x* x + y * y + z * z;
        if (square_sum == 1.f){
            return *this;
        }
        else if (square_sum < tolerance){
            return vector3d::zero_vector();
        }
        const float scale = inv_sqrt(square_sum);
        return vector3d(x * scale, y * scale, z * scale);
    }

    /**
     * @brief Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero.
     * Returns zero vector if vector length is too small to normalize.
     *
     * @param tolerance Minimum squared vector length.
     * @return Normalized copy if safe, otherwise returns zero vector.
     */
    inline vector3d get_safe_normal_2d(float tolerance = SMALL_NUMBER) const{
        const float square_sum = x * x + y * y;

        if (square_sum == 1.f){
            return vector3d(x, y, 0.f);
        }
        else if (square_sum < tolerance){
            return vector3d::zero_vector();
        }
        const float scale = inv_sqrt(square_sum);
        return vector3d(x * scale, y * scale, 0.f);
    }

    /**
     * @brief Returns the cosine of the angle between this vector and another projected onto the XY plane (no Z).
     *
     * @param v the other vector to find the 2D cosine of the angle with.
     * @return The cosine.
     */
    inline float cosine_angle_2D(const vector3d& v) const{
        vector3d v1(*this);
        vector3d v2(v);
        v1.z = 0.0f;
        v2.z = 0.0f;
        v1.normalize();
        v2.normalize();
        return v1 | v2;
    }

    /**
     * @brief Gets a copy of this vector projected onto the input vector.
     *
     * @param v vector3d to project onto, does not assume it is normalized.
     * @return Projected vector.
     */
    inline vector3d project_onto(const vector3d& v) const{
        return (v * ((*this | v) / (v | v)));
    }

    /**
     * @brief Gets a copy of this vector projected onto the input vector, which is assumed to be unit length.
     *
     * @param  normal: vector3d to project onto (assumed to be unit length).
     * @return Projected vector.
     s*/
    inline vector3d project_onto_normal(const vector3d& normal) const{
        return (normal * (*this | normal));
    }

    /**
     * @brief Find good arbitrary axis vectors to represent U and V axes of a plane,
     * using this vector as the normal of the plane.
     *
     * @param axis1 Reference to first axis.
     * @param axis2 Reference to second axis.
     */
    inline void find_best_axis_vectors(vector3d& axis1, vector3d& axis2) const{
        const float nx = abs(x);
        const float ny = abs(y);
        const float nz = abs(z);
        
        // find best basis vectors
        if (nz > nx && nz > ny) axis1 = vector3d(1, 0, 0);
        else                    axis1 = vector3d(0, 0, 1);

        axis1 = (axis1 - *this * (axis1 | *this)).get_safe_normal();
        axis2 = axis1 ^ *this;
    }

    /**  
     * @brief 
     * When this vector contains Euler angles (degrees), ensure that angles are between +/-180 
     */
    inline void unwind_euler(){
        x = normalize_degrees(x);
        y = normalize_degrees(y);
        z = normalize_degrees(z);
    }

    /**
     * @brief Utility to check if there are any NaNs in this vector.
     *
     * @return true if there are any NaNs in this vector, false otherwise.
     */
    inline bool contains_nan() const{
        return (
            is_nan(x) || !is_finite(x) ||
            is_nan(y) || !is_finite(y) ||
            is_nan(z) || !is_finite(z)
            );
    }

    /**
     * @brief Check if the vector is of unit length, with specified tolerance.
     *
     * @param length_squared_tolerance Tolerance against squared length.
     * @return true if the vector is a unit vector within the specified tolerance.
     */
    inline bool is_unit(float length_squared_tolerance = KINDA_SMALL_NUMBER) const{
        return abs(1.f - length_squared()) < length_squared_tolerance;
    }

    /**
     * @brief Convert a direction vector into a 'heading' angle.
     *
     * @return 'Heading' angle between +/-PI. 0 is pointing down +X.
     */
    inline float heading_angle() const{
        vector3d plane_dir = *this;
        plane_dir.z = 0.f;
        plane_dir = plane_dir.get_safe_normal();

        float angle = acos(plane_dir.x);
        if (plane_dir.y < 0.0f){
            angle *= -1.0f;
        }
        return angle;
    }

    /**
     * @brief Create an orthonormal basis from a basis with at least two orthogonal vectors.
     * It may change the directions of the X and Y axes to make the basis orthogonal,
     * but it won't change the direction of the Z axis.
     * All axes will be normalized.
     *
     * @param x_axis - The input basis' XAxis, and upon return the orthonormal basis' XAxis.
     * @param y_axis - The input basis' YAxis, and upon return the orthonormal basis' YAxis.
     * @param z_axis - The input basis' ZAxis, and upon return the orthonormal basis' ZAxis.
     */
    static void create_orthonormal_basis(vector3d& x_axis, vector3d& y_axis, vector3d& z_axis){
        // Project the X and Y axes onto the plane perpendicular to the Z axis.
        x_axis -= (x_axis | z_axis) / (z_axis | z_axis) * z_axis;
        y_axis -= (y_axis | z_axis) / (z_axis | z_axis) * z_axis;

        // If the X axis was parallel to the Z axis, choose a vector which is orthogonal to the Y and Z axes.
        if (x_axis.length_squared() < DELTA*DELTA)
        {
            x_axis = y_axis ^ z_axis;
        }

        // If the Y axis was parallel to the Z axis, choose a vector which is orthogonal to the X and Z axes.
        if (y_axis.length_squared() < DELTA*DELTA)
        {
            y_axis = x_axis ^ z_axis;
        }

        // Normalize the basis vectors.
        x_axis.normalize();
        y_axis.normalize();
        z_axis.normalize();
    }

    /**
     * @brief Compare two points and see if they're within specified distance.
     *
     * @param p1 First vector.
     * @param p2 Second vector.
     * @param dist Specified distance.
     * @return Whether two points are within the specified distance. Uses fast distance approximation (linear per-component distance).
     */
    static bool points_are_near(const vector3d& p1, const vector3d& p2, float dist){
        return(
            abs(p1.x - p2.x) < dist &&
            abs(p1.y - p2.y) < dist &&
            abs(p1.z - p2.z) < dist
            );
    }

    /**
     * @brief Compare two points and see if they're the same, using a threshold.
     *
     * @param p1 First vector.
     * @param p2 Second vector.
     * @return Whether points are the same within a threshold. Uses fast distance approximation (linear per-component distance).
     */
    static bool points_are_same(const vector3d& p1, const vector3d& p2){
        return points_are_near(p1, p2, THRESH_POINTS_ARE_SAME);
    }

    /**
     * @brief Calculate the signed distance (in the direction of the normal) between a point and a plane.
     *
     * @param point: The Point we are checking.
     * @param plane_base: The Base Point in the plane.
     * @param plane_normal: The Normal of the plane (assumed to be unit length).
     * @return Signed distance between point and plane.
     */
    static float point_plane_dist(const vector3d& point, const vector3d& plane_base, const vector3d& plane_normal){
        return (point - plane_base) | plane_normal;
    }

    /**
     * todo:
     * @brief Calculate the projection of a point on the plane defined by counter-clockwise (CCW) points A,B,C.
     *
     * @param point: The point to project onto the plane
     * @param a: 1st of three points in CCW order defining the plane
     * @param b: 2nd of three points in CCW order defining the plane
     * @param c: 3rd of three points in CCW order defining the plane
     * @return Projection of Point onto plane ABC
     */
    static vector3d point_plane_project(const vector3d& point, const vector3d& a, const vector3d& b, const vector3d& c){

    }

    /**
     * @brief Calculate the projection of a point on the plane defined by PlaneBase and PlaneNormal.
     *
     * @param point: The point to project onto the plane
     * @param plane_base: Point on the plane
     * @param plance_normal: Normal of the plane (assumed to be unit length).
     * @return Projection of Point onto plane
     */
    static vector3d point_plane_project(const vector3d& point, const vector3d& plane_base, const vector3d& plane_normal){
        //Find the distance of X from the plane
        //Add the distance back along the normal from the point
        return point - vector3d::point_plane_dist(point, plane_base, plane_normal) * plane_normal;
    }

    /**
     * @brief Calculate the projection of a vector on the plane defined by PlaneNormal.
     *
     * @param  v: The vector to project onto the plane.
     * @param  plane_normal: Normal of the plane (assumed to be unit length).
     * @return Projection of V onto plane.
     */
    static vector3d point_plane_project(const vector3d& v, const vector3d& plane_normal){
        return v - v.project_onto_normal(plane_normal);
    }

    /**
     * @brief Euclidean distance between two points.
     *
     * @param v1 The first point.
     * @param v2 The second point.
     * @return The distance between two points.
     */
    static inline float dist(const vector3d &v1, const vector3d &v2){
        return sqrt(square(v2.x - v1.x) + square(v2.y - v1.y) + square(v2.z - v1.z));
    }

    /**
     * @brief Squared distance between two points.
     *
     * @param v1 The first point.
     * @param v2 The second point.
     * @return The squared distance between two points.
     */
    static inline float dist_squared(const vector3d& v1, const vector3d& v2){
        return square(v2.x - v1.x) + square(v2.y - v1.y) + square(v2.z - v1.z);
    }

    /**
     * @brief Compute pushout of a box from a plane.
     *
     * @param normal: The plane normal.
     * @param size: The size of the box.
     * @return Pushout required.
     */
    static inline float box_pushout(const vector3d& normal, const vector3d& size){
        return abs(normal.x * size.x) + abs(normal.y * size.y) + abs(normal.z * size.z);
    }

    /**
     * @brief See if two normal vectors are nearly parallel, meaning the angle between them is close to 0 degrees.
     *
     * @param  normal1: First normalized vector.
     * @param  normal2: Second normalized vector.
     * @param  parallel_cosine_threshold: Normals are parallel if absolute value of dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
     * @return true if vectors are nearly parallel, false otherwise.
     */
    static bool parallel(const vector3d& normal1, const vector3d& normal2, float parallel_cosine_threshold = THRESH_NORMALS_ARE_PARALLEL){
        const float normal_dot = normal1 | normal2;
        return abs(normal_dot) >= parallel_cosine_threshold;
    }

    /**
     * @brief See if two normal vectors are coincident (nearly parallel and point in the same direction).
     *
     * @param  normal1: First normalized vector.
     * @param  normal2: Second normalized vector.
     * @param  parallel_cosine_threshold: Normals are coincident if dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
     * @return true if vectors are coincident (nearly parallel and point in the same direction), false otherwise.
     */
    static bool coincident(const vector3d& normal1, const vector3d& normal2, float parallel_cosine_threshold = THRESH_NORMALS_ARE_PARALLEL){
        const float normal_dot = normal1 | normal2;
        return normal_dot >= parallel_cosine_threshold;
    }

    /**
     * @brief See if two normal vectors are nearly orthogonal (perpendicular), meaning the angle between them is close to 90 degrees.
     *
     * @param  normal1: First normalized vector.
     * @param  normal2: Second normalized vector.
     * @param  orthogonal_cosine_threshold: Normals are orthogonal if absolute value of dot product (cosine of angle between them) is less than or equal to this. For example: cos(89.0 degrees).
     * @return true if vectors are orthogonal (perpendicular), false otherwise.
     */
    static bool orthogonal(const vector3d& normal1, const vector3d& normal2, float orthogonal_cosine_threshold = THRESH_NORMALS_ARE_ORTHOGONAL){
        const float normal_dot = normal1 | normal2;
        return abs(normal_dot) <= orthogonal_cosine_threshold;
    }

    /**
    * See if two planes are coplanar. They are coplanar if the normals are nearly parallel and the planes include the same set of points.
    *
    * @param base1: The base point in the first plane.
    * @param normal1: The normal of the first plane.
    * @param base2: The base point in the second plane.
    * @param normal2: The normal of the second plane.
    * @param parallel_cosine_threshold: Normals are parallel if absolute value of dot product is greater than or equal to this.
    * @return true if the planes are coplanar, false otherwise.
    */
    static bool coplanar(const vector3d& base1, const vector3d& normal1, const vector3d& base2, const vector3d& normal2, float parallel_cosine_threshold = THRESH_NORMALS_ARE_PARALLEL){
        return (
            vector3d::parallel(normal1, normal2, parallel_cosine_threshold) &&
            vector3d::point_plane_dist(base2, base1, normal1) <= THRESH_POINT_ON_PLANE
            );
    }

    /**
     * @brief Triple product of three vectors: v1 dot (v2 cross v3).
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param v3 The third vector.
     * @return The triple product: v1 dot (v2 cross v3).
     */
    static float triple(const vector3d& v1, const vector3d& v2, const vector3d& v3){
        return
            ( (v1.x * (v2.y * v3.z - v2.z * v3.y))
            + (v1.y * (v2.z * v3.x - v2.x * v3.z))
            + (v1.z * (v2.x * v3.y - v2.y * v3.x)));
    }

    /**
     * @brief Converts a vector containing radian values to a vector containing degree values.
     *
     * @param rad_vector: vector3d containing radian values
     * @return vector3d  containing degree values
     */
    static vector3d radians_to_degrees(const vector3d& rad_vector){
        return rad_vector * (180.f / kPI);
    }

    /**
     * @brief Converts a vector containing degree values to a vector containing radian values.
     *
     * @param deg_vector:	vector3d containing degree values
     * @return vector3d containing radian values
     */
    static vector3d degrees_to_radians(const vector3d& deg_vector){
        return deg_vector * (kPI / 180.f);
    }

}GCC_ALIGN(16);

} // end namespace utility
} // end namespace utility

#endif