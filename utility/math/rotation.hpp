/**
 *
 * roration.hpp
 *
 * roration implementation
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-05
 */

#ifndef __ydk_utility_math_rotation_hpp__
#define __ydk_utility_math_rotation_hpp__

#include <utility/math/math.hpp>

namespace utility{
namespace math{

struct rotator
{
    /*************************************************************** 
     *
     *           Y
     *           |
     *           |
     *           |
     *           |
     *           --------------- X
     *          /
     *         /
     *        /
     *       Z
     *
     ****************************************************************/

public:
    /** Looking up and down (0=Straight Ahead, +Up, -Down), Rotate the object around the X axis */
    float pitch;

    /** Rotating around (running in circles), 0=East, +North, -South, Rotate the object around the Y axis */
    float yaw;

    /** Rotation about axis of screen, 0=Straight, +Clockwise, -CCW, Rotate the object around the Z axis */
    float roll;

public:
    /** Get A rotator of zero degrees on each axis. */
    static const rotator& zero_rotator()
    {
        static const rotator z_rotator(0, 0, 0);

        return z_rotator;
    }

public:
    /**
     * Default constructor
     */
    rotator() : pitch(0), yaw(0), roll(0){}

    /**
     * @brief Constructor
     *
     * @param v Value to set all components to.
     */
    explicit inline rotator(float v) : pitch(v), yaw(v), roll(v){}

    /**
     * @brief Constructor.
     *
     * @param in_pitch: Pitch in degrees.
     * @param in_yaw: Yaw in degrees.
     * @param in_rool: Roll in degrees.
     */
    inline rotator(float in_pitch, float in_yaw, float in_roll) : pitch(in_pitch), yaw(in_yaw), roll(in_roll){}

public:
    // Binary arithmetic operators.

    /**
     * @brief Get the result of adding a rotator to this.
     *
     * @param r: The other rotator.
     * @return The result of adding a rotator to this.
     */
    inline rotator operator+(const rotator& r) const{
        return rotator(pitch + r.pitch, yaw + r.yaw, roll + r.roll);
    }

    /**
     * @brief Get the result of subtracting a rotator from this.
     *
     * @param r: The other rotator.
     * @return The result of subtracting a rotator from this.
     */
    inline rotator operator-(const rotator& r) const{
        return rotator(pitch - r.pitch, yaw - r.yaw, roll - r.roll);
    }

    /**
     * @brief Get the result of scaling this rotator.
     *
     * @param scale The scaling factor.
     * @return The result of scaling.
     */
    inline rotator operator*(float scale) const{
        return rotator(pitch * scale, yaw * scale, roll * scale);
    }

    /**
     * Scale a rotator and return.
     *
     * @param scale: scale to apply to R.
     * @param r: rotator to be scaled.
     * @return Scaled rotator.
     */
    friend rotator operator*(float scale, const rotator& r){
        return r.operator*(scale);
    }

    /**
     * @brief Get a negated copy of the rotator.
     *
     * @return A negated copy of the rotator.
     */
    inline rotator operator-() const{
        return rotator(-pitch, -yaw, -roll);
    }

    /**
     * @brief Checks whether two rotators are identical.
     *
     * @param r: The other rotator.
     * @return true if two rotators are identical, otherwise false.
     */
    inline bool operator==(const rotator& r) const{
        return pitch == r.pitch && yaw == r.yaw && roll == r.roll;
    }

    /**
     * @brief Checks whether two rotators are different.
     *
     * @param r: The other rotator.
     * @return true if two rotators are different, otherwise false.
     */
    inline bool operator!=(const rotator& r) const{
        return pitch != r.pitch || yaw != r.yaw || roll != r.roll;
    }

    // Assignment operators.

    /**
     * @brief Adds another rotator to this.
     *
     * @param r: The other rotator.
     * @return Copy of rotator after addition.
     */
    inline rotator& operator+=(const rotator& r){
        pitch += r.pitch; yaw += r.yaw; roll += r.roll;
        return *this;
    }

    /**
     * @brief Subtracts another rotator from this.
     *
     * @param r: The other rotator.
     * @return Copy of rotator after subtraction.
     */
    inline rotator& operator-=(const rotator& r){
        pitch -= r.pitch; yaw -= r.yaw; roll -= r.roll;
        return *this;
    }

    /**
     * @brief Multiply this rotator by a scaling factor.
     *
     * @param scale The scaling factor.
     * @return Copy of the rotator after scaling.
     */
    inline rotator& operator*=(float scale){
        pitch *= scale; yaw *= scale; roll *= scale;
        return *this;
    }

public:

    // Functions.

    /**
     * @brief Set the values of the roation directly.
     *
     * @param in_pitch: New pitch value.
     * @param in_yaw: New yaw value.
     * @param in_roll: New roll value
     */
    inline void set(float in_pitch, float in_yaw, float in_roll){
        pitch = in_pitch;
        yaw = in_yaw;
        roll = in_roll;
    }

    /**
     * @brief Checks whether rotator is nearly zero, within specified tolerance.
     *
     * @param tolerance: Error Tolerance.
     * @return true if rotator is nearly zero, within specified tolerance, otherwise false.
     */
    inline bool is_near_zero(float tolerance = KINDA_SMALL_NUMBER) const{
        return (
            abs(normalize180(pitch)) < tolerance &&
            abs(normalize180(yaw  )) < tolerance &&
            abs(normalize180(roll )) < tolerance
            );
    }

    /**
     * @brief Checks whether this has exactly zero rotation.
     *
     * @return true if this has exactly zero rotation, otherwise false.
     */
    inline bool is_zero() const{
        return (normalize360(pitch) == 0.f) && (normalize360(yaw) == 0.f) && (normalize360(roll) == 0.f);
    }

    /**
     * @brief Checks whether two rotators are equal, within specified tolerance.
     *
     * @param other: The other rotator.
     * @param tolerance: Error Tolerance.
     * @return true if two rotators are equal, within specified tolerance, otherwise false.
     */
    inline bool equals(const rotator& other, float tolerance = KINDA_SMALL_NUMBER) const{
        return (
            abs(normalize180(pitch - other.pitch)) < KINDA_SMALL_NUMBER &&
            abs(normalize180(yaw - other.yaw)) < KINDA_SMALL_NUMBER &&
            abs(normalize180(roll - other.roll)) < KINDA_SMALL_NUMBER
            );
    }

    /**
     * @brief Adds to each component of the rotator.
     *
     * @param delta_pitch: Change in pitch. (+/-)
     * @param delta_yaw: Change in yaw. (+/-)
     * @param delta_roll: Change in roll. (+/-)
     * @return Copy of rotator after addition.
     */
    inline rotator& add(float delta_pitch, float delta_yaw, float delta_roll){
        pitch += delta_pitch;
        yaw   += delta_yaw;
        roll  += delta_roll;
        return *this;
    }

    /**
     * @brief Gets the rotation values so they fall within the range [0,360]
     *
     * @return Clamped version of rotator.
     */
    inline void normalize360(){
        pitch = normalize360(pitch);
        yaw = normalize360(yaw);
        roll = normalize360(roll);
    }

    /**
    * Create a copy of this rotator and denormalize, clamping each axis to 0 - 360.
    *
    * @return Denormalized copy of this rotator
    */
    inline rotator get_normalize360() const{
        rotator rot = *this;
        rot.normalize360();
        return rot;
    }

    /**
     * @brief In-place normalize, normalize all the angle to fall within the range [-180,180] , removes all winding and creates the "shortest route" rotation.
     */
    inline void normalize180(){
        pitch = normalize180(pitch);
        yaw = normalize180(yaw);
        roll = normalize180(roll);
    }

    /**
    * Create a copy of this rotator and normalize180, removes all winding and creates the "shortest route" rotation.
    *
    * @return Normalized copy of this rotator
    */
    inline rotator get_normalize180() const{
        rotator rot = *this;
        rot.normalize180();
        return rot;
    }

    /**
     * @brief Utility to check if there are any NaNs in this Rotator.
     *
     * @return true if there are any NaNs in this Rotator, otherwise false.
     */
    inline bool contains_nan() const{
        return (
            is_nan(pitch) || !is_finite(pitch) ||
            is_nan(yaw) || !is_finite(yaw) ||
            is_nan(roll) || !is_finite(roll)
            );
    }

public:

    /**
     * @brief Clamps an angle to the range of [0, 360].
     *
     * @param angle: The angle to clamp.
     * @return The clamped angle.
     */
    static float normalize360(float angle){
        // returns the angele in the range(-360, 360)
        angle = fmod(angle, 360.f);
        if (angle < 0.f){
            // shift to [0, 360) range
            angle += 360.f;
        }
        return angle;
    }

    /**
     * @brief Clamps an angle to the range of [-180, 180].
     *
     * @param Angle The Angle to clamp.
     * @return The clamped angle.
     */
    static float normalize180(float angle){
        // return the angle in the range [0, 360)
        angle = normalize360(angle);

        if (angle > 180.f){
            // shit to (-180, 180]
            angle -= 360.f;
        }
        return angle;
    }
};

} // end namespace math
}// end namespace utility

#endif