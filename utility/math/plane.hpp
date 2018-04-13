/**
 *
 * plane.hpp
 *
 * plane implementation[平面]
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-13
 */

#ifndef __ydk_utility_math_plane_hpp__
#define __ydk_utility_math_plane_hpp__

#include <utility/math/math.hpp>
#include <utility/math/vector3d.hpp>
#include <utility/math/matrix.hpp>
#include <base/align.hpp>

namespace utility{
namespace math{

/*
===============================================================================

3D plane with equation: a * x + b * y + c * z + d = 0

===============================================================================
*/

#define	ON_EPSILON					0.1f
#define DEGENERATE_DIST_EPSILON		1e-4f

#define	SIDE_FRONT					0
#define	SIDE_BACK					1
#define	SIDE_ON						2
#define	SIDE_CROSS					3

    // plane sides
#define PLANESIDE_FRONT				0
#define PLANESIDE_BACK				1
#define PLANESIDE_ON				2
#define PLANESIDE_CROSS				3

    // plane types
#define PLANETYPE_X					0
#define PLANETYPE_Y					1
#define PLANETYPE_Z					2
#define PLANETYPE_NEGX				3
#define PLANETYPE_NEGY				4
#define PLANETYPE_NEGZ				5
#define PLANETYPE_TRUEAXIAL			6	// all types < 6 are true axial planes
#define PLANETYPE_ZEROX				6
#define PLANETYPE_ZEROY				7
#define PLANETYPE_ZEROZ				8
#define PLANETYPE_NONAXIAL			9


MS_ALIGN(16) struct plane
{
public:
    float			a;
    float			b;
    float			c;
    float			d;
public:
    inline plane(void) : a(0), b(0), c(0), d(0){
    }

    inline plane(float a, float b, float c, float d){
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }

    inline plane(const vector3d& normal, float dist){
        this->a = normal.x;
        this->b = normal.y;
        this->c = normal.z;
        this->d = -dist;
    }

    inline float    operator[](int32_t index) const{
        return (&a)[index];
    }

    inline float&   operator[](int32_t index){
        return (&a)[index];
    }

    // flips plane
    inline plane    operator-() const{
        return plane(-a, -b, -c, -d);
    }

    // sets normal and sets plane::d to zero
    inline plane&   operator=(const vector3d& v){
        a = v.x;
        b = v.y;
        c = v.z;
        d = 0;
        return *this;
    }

    // add plane equations[方程]
    inline plane    operator+(const plane& p) const{
        return plane(a + p.a, b + p.b, c + p.c, d + p.d);
    }

    // subtract plane equations
    inline plane    operator-(const plane &p) const{
        return plane(a - p.a, b - p.b, c - p.c, d - p.d);
    }

    // normal() *= m
    inline plane&   operator*=(const matrix3& m){
        normal() *= m;
        return *this;
    }

    // compare with epsilon
    inline bool     equals(const plane& p, float epsilon) const{
        return (abs(a - p.a) <= epsilon && 
                abs(b - p.b) <= epsilon &&
                abs(c - p.c) <= epsilon && 
                abs(d - p.d) <= epsilon);
    }

    // compare with epsilon
    inline bool     equals(const plane& p, float normal_epsilon, float dist_epsilon) const{
        return (abs(d - p.d) <= dist_epsilon) && 
                normal().equals(p.normal(), normal_epsilon);
    }

    // exact compare, no epsilon
    inline bool     operator==(const plane& p) const{
        return a == p.a && b == p.b && c == p.c && d == p.d;
    }

    // exact compare, no epsilon
    inline bool     operator!=(const plane &p) const{
        return a != p.a || b != p.b || c != p.c || d != p.d;
    }

    // zero plane
    inline void     zero(void){
        a = b = c = d = 0.0f;
    }

    // sets the normal
    inline void     set_normal(const vector3d& normal){
        a = normal.x;
        b = normal.y;
        c = normal.z;
    }

    // reference to const normal
    inline const vector3d&	normal(void) const{
        return *reinterpret_cast<const vector3d*>(&a);
    }

    // reference to normal
    inline vector3d&    normal(void){
        return *reinterpret_cast<vector3d*>(&a);
    }

    // only normalizes the plane normal, does not adjust d
    inline bool    normalize(){
        return normal().normalize();
    }

    // returns: -d
    inline float    dist(void) const{
        return -d;
    }

    // sets: d = -dist
    inline void     set_dist(float dist){
        d = -dist;
    }

    // returns plane type
    inline int32_t  type(void) const{
        if (normal()[0] == 0.0f) {
            if (normal()[1] == 0.0f) {
                return normal()[2] > 0.0f ? PLANETYPE_Z : PLANETYPE_NEGZ;
            }
            else if (normal()[2] == 0.0f) {
                return normal()[1] > 0.0f ? PLANETYPE_Y : PLANETYPE_NEGY;
            }
            else {
                return PLANETYPE_ZEROX;
            }
        }
        else if (normal()[1] == 0.0f) {
            if (normal()[2] == 0.0f) {
                return normal()[0] > 0.0f ? PLANETYPE_X : PLANETYPE_NEGX;
            }
            else {
                return PLANETYPE_ZEROY;
            }
        }
        else if (normal()[2] == 0.0f) {
            return PLANETYPE_ZEROZ;
        }
        else {
            return PLANETYPE_NONAXIAL;
        }
    }

    // assumes normal is valid
    inline void     fit_through_point(const vector3d& p){
        d = -(normal() | p);
    }

    bool     height_fit(const vector3d* points, int32_t num_points){
        int32_t i = 0;
        float sum_xx = 0.0f, sum_xy = 0.0f, sum_xz = 0.0f;
        float sum_yy = 0.0f, sum_yz = 0.0f;
        vector3d sum, average, dir;

        if (num_points == 1) {
            a = 0.0f;
            b = 0.0f;
            c = 1.0f;
            d = -points[0].z;
            return true;
        }
        if (num_points == 2) {
            dir = points[1] - points[0];
            normal() = (dir ^ vector3d(0, 0, 1)) ^ dir;
            normalize();
            d = -(normal() | points[0]);
            return true;
        }

        sum.zero();
        for (i = 0; i < num_points; i++) {
            sum += points[i];
        }
        average = sum / (float)num_points;

        for (i = 0; i < num_points; i++) {
            dir = points[i] - average;
            sum_xx += dir.x * dir.x;
            sum_xy += dir.x * dir.y;
            sum_xz += dir.x * dir.z;
            sum_yy += dir.y * dir.y;
            sum_yz += dir.y * dir.z;
        }

        matrix2 m(sum_xx, sum_xy, sum_xy, sum_yy);
        if (!m.inverse()) {
            return false;
        }

        a = -sum_xz * m[0][0] - sum_yz * m[0][1];
        b = -sum_xz * m[1][0] - sum_yz * m[1][1];
        c = 1.0f;
        normalize();
        d = -(a * average.x + b * average.y + c * average.z);
        return true;
    }

    inline plane    get_translate(const vector3d& translation) const{
        return plane(a, b, c, d - (translation | normal()));
    }

    inline plane&   translate(const vector3d& translation){
        d -= (translation | normal());
    }

    inline plane    get_rotate(const vector3d& origin, const matrix3& axis) const{
        plane p;
        p.normal() = normal() * axis;
        p.d = d + (origin | normal()) - (origin | p.normal());
        return p;
    }

    inline plane&   rotate(const vector3d& origin, const matrix3& axis){
        d += (origin | normal());
        normal() *= axis;
        d -= (origin | normal());
        return *this;
    }

    inline float    distance(const vector3d& v) const{
        return a * v.x + b * v.y + c * v.z + d;
    }

    inline int      side(const vector3d& v, float epsilon = 0.0f) const{
        float dist = distance(v);
        if (dist > epsilon) {
            return PLANESIDE_FRONT;
        }
        else if (dist < -epsilon) {
            return PLANESIDE_BACK;
        }
        else {
            return PLANESIDE_ON;
        }
    }

    inline bool     line_intersection(const vector3d& start, const vector3d& end) const{
        float d1 = (normal() | start) + d;
        float d2 = (normal() | end) + d;
        if (d1 == d2) {
            return false;
        }
        if (d1 > 0.0f && d2 > 0.0f) {
            return false;
        }
        if (d1 < 0.0f && d2 < 0.0f) {
            return false;
        }
        float fraction = (d1 / (d1 - d2));
        return (fraction >= 0.0f && fraction <= 1.0f);
    }

    // intersection point is start + dir * scale
    inline bool     ray_intersection(const vector3d& start, const vector3d& dir, float& scale) const{
        float d1 = (normal() | start) + d;
        float d2 = (normal() | dir);
        if (d2 == 0.0f) {
            return false;
        }
        scale = -(d1 / d2);
        return true;
    }

    inline bool     plane_intersection(const plane& pl1, vector3d& start, vector3d& dir) const{
        float n00 = normal().length_squared();
        float n01 = normal() | pl1.normal();
        float n11 = pl1.normal().length_squared();
        float det = n00 * n11 - n01 * n01;

        if (abs(det) < 1e-6f) {
            return false;
        }

        float inv_det = 1.0f / det;
        float f0 = (n01 * pl1.d - n11 * d) * inv_det;
        float f1 = (n01 * d - n00 * pl1.d) * inv_det;

        dir = normal() ^ pl1.normal();
        start = f0 * normal() + f1 * pl1.normal();
        return true;
    }

    inline const vector4d&	to_vector4d(void) const{
        return *reinterpret_cast<const vector4d*>(&a);
    }

    inline vector4d&        to_vector4d(void){
        return *reinterpret_cast<vector4d*>(&a);
    }

public:
    /** make plane from points */
    static plane make_from_points(const vector3d& p1, const vector3d& p2, const vector3d& p3){
        plane p;
        // cross product
        p.normal() = (p1 - p2) ^ (p3 - p2);
        if (!p.normalize()){
            return p;
        }

        // dot product
        p.set_dist(-(p.normal() | p2));
        return p;
    }

    /** make plane from vectors */
    static plane make_from_vectors(const vector3d& dir1, const vector3d& dir2, const vector3d& point){
        plane p;
        // cross product
        p.normal() = dir1 ^ dir2;
        if (!p.normalize()){
            return p;
        }

        // dot product
        p.set_dist(-(p.normal() | point));
        return p;
    }

}GCC_ALIGN(16);

} // end namespace utility
} // end namespace utility

#endif