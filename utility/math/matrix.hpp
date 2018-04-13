/**
 *
 * matrix.hpp
 *
 * matrixs implementation, all matrices are row-major except matrix3
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-09-07
 */

#ifndef __ydk_utility_math_matrix_hpp__
#define __ydk_utility_math_matrix_hpp__

#include <utility/math/math.hpp>
#include <utility/math/vector4d.hpp>

#define MATRIX_INVERSE_EPSILON      1e-14
#define MATRIX_EPSILON              1e-6

namespace utility{
namespace math{

/** 
 * matrix2 - 2x2 matrix
 */
struct matrix2
{
public:
    vector2d			mat[2];

public:
    /* Global zero matrix2 */
    static const matrix2& zero_matrix2()
    {
        static const matrix2 z_matrix2(0, 0, 0, 0);

        return z_matrix2;
    }

    /* Global identiy matrix2 */
    static const matrix2& identity_matrix2(){
        static const matrix2 i_matrix2( 1, 0, 0, 1);
        return i_matrix2;
    }

public:
    matrix2(void){}

    /** 
     * @brief initialize the matrix with two vector2d
     */
    explicit matrix2(const vector2d& x, const vector2d& y){
        mat[0].x = x.x; mat[0].y = x.y;
        mat[1].x = y.x; mat[1].y = y.y;
    }

    /** 
     * @brief initizlie the matrix with two element
     */
    explicit matrix2(const float xx, const float xy, const float yx, const float yy){
        mat[0].x = xx; mat[0].y = xy;
        mat[1].x = yx; mat[1].y = yy;
    }

    /** 
     * @brief initialize the matrix with 2x2 array
     */
    explicit matrix2(const float src[2][2]){
        mat[0].x = src[0][0];
        mat[0].y = src[0][1];
        mat[1].x = src[1][0];
        mat[1].y = src[1][1];
    }

    /** 
     * @brief get the specific row of the matrix
     */
    inline const vector2d& operator[](int index) const{
        return mat[index];
    }

    /**
     * @brief get the specific row of the matrix
     */
    inline vector2d&    operator[](int index){
        return mat[index];
    }

    /** 
     * @brief get a negate copy the maxtirx
     */
    inline matrix2  operator-() const{
        return matrix2( -mat[0][0], -mat[0][1],
                        -mat[1][0], -mat[1][1] );
    }

    /** 
     * @brief Gets the result of scaling the matrix2 ( mutiplying each element by a value)
     */
    inline matrix2  operator*(const float scale) const{
        return matrix2( -mat[0][0] * scale, -mat[0][1] * scale,
                        -mat[1][0] * scale, -mat[1][1] * scale);
    }

    /** 
     * @brief each row vector to mutiply the speific vec, and return a vector as result
     */
    inline vector2d operator*(const vector2d& a) const{
        return vector2d( mat[0] | a, mat[1] | a);
    }

    /** 
     * @brief matrix multiplication
     */
    inline matrix2  operator*(const matrix2& a) const{
        return matrix2(
            mat[0].x * a[0].x + mat[0].y * a[1].x,
            mat[0].x * a[0].y + mat[0].y * a[1].y,
            mat[1].x * a[0].x + mat[1].y * a[1].x,
            mat[1].x * a[0].y + mat[1].y * a[1].y );
    }

    /** 
     * @brief  Gets the result of adding two matrix together
     */
    inline matrix2  operator+(const matrix2& a) const{
        return matrix2(
            mat[0].x + a[0].x, mat[0].y + a[0].y,
            mat[1].x + a[1].x, mat[1].y + a[1].y);
    }

    /** 
     * @brief Gets the result of subtracting a matrix2 from this one
     */
    inline matrix2  operator-(const matrix2& a) const{
        return matrix2(
            mat[0].x - a[0].x, mat[0].y - a[0].y,
            mat[1].x - a[1].x, mat[1].y - a[1].y);
    }

    /** 
     * @brief scales the matrix2
     */
    inline matrix2& operator*=(const float scale){
        mat[0].x *= scale; mat[0].y *= scale;
        mat[1].x *= scale; mat[1].y *= scale;
    }

    /** 
     * @brief mutiply the matrix with the other one
     */
    inline matrix2& operator*=(const matrix2& a){
        float x, y;
        x = mat[0].x; y = mat[0].y;
        mat[0].x = x * a[0].x + y * a[1].x;
        mat[0].y = x * a[0].y + y * a[1].y;
        x = mat[1].x; y = mat[1].y;
        mat[1].x = x * a[0].x + y * a[1].x;
        mat[1].y = x * a[0].y + y * a[1].y;
        return *this;
    }

    /** 
     * @brief Adds another matrix2 to this.
     */
    inline matrix2& operator+=(const matrix2& a){
        mat[0].x += a[0].x; mat[0].y += a[0].y;
        mat[1].x += a[1].x; mat[1].y += a[1].y;
        return *this;
    }

    /**   
     * @brief Subtracts another matrix2 from this.
     */
    inline matrix2& operator-=(const matrix2& a){
        mat[0].x -= a[0].x; mat[0].y -= a[0].y;
        mat[1].x -= a[1].x; mat[1].y -= a[1].y;
        return *this;
    }

    /** 
     * @brief scale the matrix
     */
    friend inline matrix2	operator*(const float scale, const matrix2& mat){
        return mat * scale;
    }

    /** 
     * @brief each row of the matrix mutiplty with the spefic vec
     */
    friend inline vector2d	operator*(const vector2d& vec, const matrix2& mat){
        return mat * vec;
    }

    friend inline vector2d& operator*=(vector2d& vec, const matrix2& mat){
        vec = mat * vec;
        return vec;
    }

    /** 
     * @brief Checks for equality with error-tolerant comparison
     * 
     * @param epsilon: Error tolerance
     */
    inline bool    equals(const matrix2& a, float epsilon) const{
        return mat[0].equals(a[0], epsilon) && mat[1].equals(a[1], epsilon);
    }

    /** 
     * @brief Compares this matrix2 against another for equality(exact compare, no epsilon).
     */
    inline bool    operator==(const matrix2& a) const{
        return mat[0] == a[0] && mat[1] == a[1];
    }

    /** 
     * @brief Compares this matrix2 against another for inequality(exact compare, no epsilon).
     */
    inline bool    operator!=(const matrix2 &a) const{
        return mat[0] != a[0] || mat[1] != a[1];
    }

    /** 
     * @brief make the matrix identity matrix
     */
    inline void    identity(void){
        mat[0].x = 1; mat[0].y = 0;
        mat[1].x = 0; mat[1].y = 1;
    }

    /** 
     * @brief check the matrix is identity matrix[单位矩阵]
     */
    inline bool    is_identity(float epsilon = MATRIX_EPSILON) const{
        return equals(identity_matrix2(), epsilon);
    }

    /** 
     * @brief check the matrix is symmetric matrix[对称矩阵]
     */
    inline bool    is_symmetric(float epsilon = MATRIX_EPSILON) const{
        return abs(mat[0][1] - mat[1][0]) < epsilon;
    }

    /** 
     * @brief check is diagonal matrix[对角矩阵,所有非主对角线元素全等于零]
     */
    inline bool    is_diagonal(const float epsilon = MATRIX_EPSILON) const{
        return (abs(mat[0][1]) <= epsilon && abs(mat[1][0]) <= epsilon);
    }

    /** 
     * @brief calculate the trace of the matrix[trA=a11+a22+...+ann, A的对角线元素之和称为A的迹]
     */
    inline float   trace(void) const{
        return (mat[0][0] + mat[1][1]);
    }

    /** 
     * @brief calculate the Determinant of the matrix[矩阵的行列式]
     */
    inline float   determinant(void) const{
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }

    /** 
     * @brief transponse the maxtrix
     */
    inline matrix2& transpose(void){
        float tmp;
        tmp = mat[0][1];
        mat[0][1] = mat[1][0];
        mat[1][0] = tmp;
        return *this;
    }

    /**
     * @brief get the transpose of the maxtix[矩阵的转置]
     */
    inline matrix2 get_transpose(void) const {
        return matrix2( mat[0][0], mat[1][0],
                        mat[0][1], mat[1][1]);
    }

    /** 
     * @brief get the inverse matrix of the maxtrix, returns the inverse ( m * m.Inverse() = identity ) [求矩阵的逆矩阵, 矩阵与其逆矩阵相乘等于单位矩阵]
     */
    inline matrix2 get_inverse(void) const{
        matrix2 nv_mat = *this;
        nv_mat.inverse();
        return nv_mat;
    }

    /** 
     * @brief inverse self
     * returns false if determinant is zero
     */
    inline bool    inverse(void){
        // 2+4 = 6 multiplications
        //		 1 division
        double det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

        if (abs(det) < MATRIX_INVERSE_EPSILON) {
            return false;
        }

        double inv_det = 1.0f / det;

        float a = mat[0][0];
        mat[0][0] =  mat[1][1] * (float)inv_det;
        mat[0][1] = -mat[0][1] * (float)inv_det;
        mat[1][0] = -mat[1][0] * (float)inv_det;
        mat[1][1] =  a         * (float)inv_det;

        return true;
    }

    /**  
     * @brief get the inverse matrix fase
     */
    inline matrix2 get_inverse_fast(void) const{
        matrix2 nv_mat = *this;
        nv_mat.inverse_fast();
        return nv_mat;
    }

    /**
     * @brief inverse self
     * returns false if determinant is zero
     */
    inline bool    inverse_fast(void){
        // 2+4 = 6 multiplications
        //		 1 division
        double det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

        if (abs(det) < MATRIX_INVERSE_EPSILON) {
            return false;
        }

        double inv_det = 1.0f / det;

        float a = mat[0][0];
        mat[0][0] =  mat[1][1] * (float)inv_det;
        mat[0][1] = -mat[0][1] * (float)inv_det;
        mat[1][0] = -mat[1][0] * (float)inv_det;
        mat[1][1] =  a         * (float)inv_det;

        return true;
    }
};

/*********************************************************** 
 * 
 * matrix3 - 3x3 matrix 
 * 
 * matrix
 *
 ***********************************************************/

struct matrix3
{
public:
    vector3d    mat[3];

public:
    /* Global zero matrix3 */
    static const matrix3& zero_matrix3()
    {
        static const matrix3 z_matrix3(0, 0, 0, 
                                       0, 0, 0, 
                                       0, 0, 0);

        return z_matrix3;
    }

    /* Global identiy matrix3 */
    static const matrix3& identity_matrix3(){
        static const matrix3 i_matrix3(1, 0, 0, 
                                       0, 1, 0,
                                       0, 0, 1);
        return i_matrix3;
    }

public:
    matrix3(void){}
    explicit matrix3(const vector3d& x, const vector3d& y, const vector3d& z){
        mat[0] = x; mat[1] = y; mat[2] = z;
    }

    explicit matrix3(const float xx, const float xy, const float xz, const float yx, const float yy, const float yz, const float zx, const float zy, const float zz){
        mat[0].x = xx; mat[0].y = xy; mat[0].z = xz;
        mat[1].x = yx; mat[1].y = yy; mat[1].z = yz;
        mat[2].x = zx; mat[2].y = zy; mat[2].z = zz;
    }

    explicit matrix3(const float src[3][3]){
        mat[0].x = src[0][0]; mat[0].y = src[0][1]; mat[0].z = src[0][2];
        mat[1].x = src[1][0]; mat[1].y = src[1][1]; mat[1].z = src[1][2];
        mat[2].x = src[2][0]; mat[2].y = src[2][1]; mat[2].z = src[2][2];
    }

    inline const vector3d& operator[](int index) const{
        return mat[index];
    }

    inline vector3d&       operator[](int index){
        return mat[index];
    }

    inline matrix3         operator-() const{
        return matrix3( -mat[0][0], -mat[0][1], -mat[0][2], 
                        -mat[1][0], -mat[1][1], -mat[1][2],
                        -mat[2][0], -mat[2][1], -mat[2][2]);
    }

    inline matrix3         operator*(const float a) const{
        return matrix3(
            mat[0].x * a, mat[0].y * a, mat[0].z * a,
            mat[1].x * a, mat[1].y * a, mat[1].z * a,
            mat[2].x * a, mat[2].y * a, mat[2].z * a);
    }

    inline vector3d        operator*(const vector3d& vec) const{
        return vector3d(mat[0] | vec, mat[1] | vec, mat[2] | vec);
    }

    inline matrix3         operator*(const matrix3& a) const{
        matrix3 dst;
        for (int32_t i = 0; i < 3; ++i){
            for (int32_t j = 0; j < 3; ++j){
                for (int32_t k = 0; k < 3; ++k){
                    dst[i][j] += ((*this)[i][k] * a[k][j]);
                }
            }
        }
        return dst;
    }

    inline matrix3         operator+(const matrix3& a) const{
        return matrix3(
            mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z,
            mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z,
            mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z);
    }

    inline matrix3         operator-(const matrix3& a) const{
        return matrix3(
            mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z,
            mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z,
            mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z);
    }

    inline matrix3&        operator*=(const float a){
        mat[0].x *= a; mat[0].y *= a; mat[0].z *= a;
        mat[1].x *= a; mat[1].y *= a; mat[1].z *= a;
        mat[2].x *= a; mat[2].y *= a; mat[2].z *= a;
        return *this;
    }

    inline matrix3&        operator*=(const matrix3& a){
        matrix3 dst = (*this) * a;
        *this = dst;
        return *this;
    }

    inline matrix3&        operator+=(const matrix3& a){
        mat[0].x += a[0].x; mat[0].y += a[0].y; mat[0].z += a[0].z;
        mat[1].x += a[1].x; mat[1].y += a[1].y; mat[1].z += a[1].z;
        mat[2].x += a[2].x; mat[2].y += a[2].y; mat[2].z += a[2].z;
        return *this;
    }

    inline matrix3&        operator-=(const matrix3& a){
        mat[0].x -= a[0].x; mat[0].y -= a[0].y; mat[0].z -= a[0].z;
        mat[1].x -= a[1].x; mat[1].y -= a[1].y; mat[1].z -= a[1].z;
        mat[2].x -= a[2].x; mat[2].y -= a[2].y; mat[2].z -= a[2].z;
        return *this;
    }

    friend inline matrix3  operator*(const float a, const matrix3& mat){
        return mat * a;
    }

    friend inline vector3d operator*(const vector3d& vec, const matrix3& mat){
        return mat * vec;
    }

    friend inline vector3d& operator*=(vector3d &vec, const matrix3 &mat){
        float x = vec | mat[0];
        float y = vec | mat[1];
        float z = vec | mat[2];
        vec.x = x;
        vec.y = y;
        vec.z = z;
        return vec;
    }

    // compare with epsilon
    inline bool            equals(const matrix3& a, float epsilon) const{
        return mat[0].equals(a[0], epsilon) && mat[1].equals(a[1], epsilon) && mat[2].equals(a[2], epsilon);
    }

    // exact compare, no epsilon
    inline bool            operator==(const matrix3 &a) const{
        return mat[0] == a[0] && mat[1] == a[1] && mat[2] == a[2];
    }

    // exact compare, no epsilon
    inline bool            operator!=(const matrix3 &a) const{
        return mat[0] != a[0] || mat[1] != a[1] || mat[2] != a[2];
    }

    inline void            zero(void){
        *this = zero_matrix3();
    }

    inline void            identity(void){
        *this = identity_matrix3();
    }

    inline bool            is_identity(const float epsilon = MATRIX_EPSILON) const{
        return equals(identity_matrix3(), epsilon);
    }

    inline bool            is_symmetric(const float epsilon = MATRIX_EPSILON) const{
        return (
            abs(mat[0][1] - mat[1][0]) <= epsilon &&
            abs(mat[0][2] - mat[2][0]) <= epsilon &&
            abs(mat[1][2] - mat[2][1]) <= epsilon);
    }

    inline bool            is_diagonal(const float epsilon = MATRIX_EPSILON) const{
        return (
            abs(mat[0][1]) <= epsilon &&
            abs(mat[0][2]) <= epsilon &&
            abs(mat[1][0]) <= epsilon &&
            abs(mat[1][2]) <= epsilon &&
            abs(mat[2][0]) <= epsilon &&
            abs(mat[2][1]) <= epsilon);
    }

    inline bool            is_rotated(void) const{
        return *this != identity_matrix3();
    }

    inline void            project_vector(const vector3d& src, vector3d& dst) const{
        dst.x = src | mat[0];
        dst.y = src | mat[1];
        dst.z = src | mat[2];
    }

    inline void            unproject_vector(const vector3d &src, vector3d& dst) const{
        dst = mat[0] * src.x + mat[1] * src.y + mat[2] * src.z;
    }

    inline float           trace(void) const{
        return (mat[0][0] + mat[1][1] + mat[2][2]);
    }

    inline float           determinant(void) const{
        float det2_12_01 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
        float det2_12_02 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
        float det2_12_12 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];

        return mat[0][0] * det2_12_12 - mat[0][1] * det2_12_02 + mat[0][2] * det2_12_01;
    }

    inline matrix3         get_ortho_normalize(void) const{
        matrix3 ortho = *this;
        ortho[0].normalize();
        ortho[2] = vector3d::cross_product(mat[0], mat[1]);
        ortho[2].normalize();
        ortho[1] = vector3d::cross_product(mat[2], mat[0]);
        ortho[1].normalize();
        return ortho;
    }

    inline matrix3&        ortho_normalize(void){
        mat[0].normalize();
        mat[2] = vector3d::cross_product(mat[0], mat[1]);
        mat[2].normalize();
        mat[1] = vector3d::cross_product(mat[2], mat[0]);
        mat[1].normalize();
    }

    inline matrix3         get_transpose(void) const{
        return matrix3(
            mat[0][0], mat[1][0], mat[2][0],
            mat[0][1], mat[1][1], mat[2][1],
            mat[0][2], mat[1][2], mat[2][2]);
    }

    inline matrix3&        transpose(void){
        float tmp0 = mat[0][1];
        mat[0][1] = mat[1][0];
        mat[1][0] = tmp0;
        float tmp1 = mat[0][2];
        mat[0][2] = mat[2][0];
        mat[2][0] = tmp1;
        float tmp2 = mat[1][2];
        mat[1][2] = mat[2][1];
        mat[2][1] = tmp2;

        return *this;
    }

    // returns the inverse ( m * m.Inverse() = identity )
    inline matrix3         get_inverse(void) const{
        matrix3 inv = *this;
        bool r = inv.inverse();
        assert(r);
        return inv;
    }

    // returns false if determinant is zero
    inline bool            inverse(void){
        // 18+3+9 = 30 multiplications
        //			 1 division
        matrix3 inv;

        inv[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
        inv[1][0] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
        inv[2][0] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

        float det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0];

        if (abs(det) < MATRIX_INVERSE_EPSILON) {
            return false;
        }

        float inv_det = 1.0f / det;

        inv[0][1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
        inv[0][2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
        inv[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
        inv[1][2] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
        inv[2][1] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
        inv[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

        mat[0][0] = inv[0][0] * inv_det;
        mat[0][1] = inv[0][1] * inv_det;
        mat[0][2] = inv[0][2] * inv_det;

        mat[1][0] = inv[1][0] * inv_det;
        mat[1][1] = inv[1][1] * inv_det;
        mat[1][2] = inv[1][2] * inv_det;

        mat[2][0] = inv[2][0] * inv_det;
        mat[2][1] = inv[2][1] * inv_det;
        mat[2][2] = inv[2][2] * inv_det;

        return true;
    }

    // returns the inverse ( m * m.Inverse() = identity )
    inline matrix3         get_inverse_fast(void) const{
        matrix3 inv = *this;
        bool r = inv.inverse_fast();
        assert(r);
        return inv;
    }

    // returns false if determinant is zero
    inline bool            inverse_fast(void){
        // 18+3+9 = 30 multiplications
        //			 1 division
        matrix3 inv;

        inv[0][0] = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
        inv[1][0] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
        inv[2][0] = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

        float det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0];

        if (abs(det) < MATRIX_INVERSE_EPSILON) {
            return false;
        }

        float inv_det = 1.0f / det;

        inv[0][1] = mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2];
        inv[0][2] = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
        inv[1][1] = mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0];
        inv[1][2] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];
        inv[2][1] = mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1];
        inv[2][2] = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

        mat[0][0] = inv[0][0] * inv_det;
        mat[0][1] = inv[0][1] * inv_det;
        mat[0][2] = inv[0][2] * inv_det;

        mat[1][0] = inv[1][0] * inv_det;
        mat[1][1] = inv[1][1] * inv_det;
        mat[1][2] = inv[1][2] * inv_det;

        mat[2][0] = inv[2][0] * inv_det;
        mat[2][1] = inv[2][1] * inv_det;
        mat[2][2] = inv[2][2] * inv_det;

        return true;
    }

    inline matrix3         transpose_multiply(const matrix3& b) const{
        return matrix3(
            mat[0].x * b[0].x + mat[1].x * b[1].x + mat[2].x * b[2].x,
            mat[0].x * b[0].y + mat[1].x * b[1].y + mat[2].x * b[2].y,
            mat[0].x * b[0].z + mat[1].x * b[1].z + mat[2].x * b[2].z,
            mat[0].y * b[0].x + mat[1].y * b[1].x + mat[2].y * b[2].x,
            mat[0].y * b[0].y + mat[1].y * b[1].y + mat[2].y * b[2].y,
            mat[0].y * b[0].z + mat[1].y * b[1].z + mat[2].y * b[2].z,
            mat[0].z * b[0].x + mat[1].z * b[1].x + mat[2].z * b[2].x,
            mat[0].z * b[0].y + mat[1].z * b[1].y + mat[2].z * b[2].y,
            mat[0].z * b[0].z + mat[1].z * b[1].z + mat[2].z * b[2].z);
    }

    friend inline void     transpose_multiply(const matrix3& transp, const matrix3& b, matrix3& dst){
        dst = transp.transpose_multiply(b);
    }

    inline matrix3         get_inertia_translate(const float mass, const vector3d& center_of_mass, const vector3d& translation) const{
        matrix3  m;
        vector3d new_center;

        new_center = center_of_mass + translation;

        m[0][0] = mass * ((center_of_mass[1] * center_of_mass[1] + center_of_mass[2] * center_of_mass[2])
            - (new_center[1] * new_center[1] + new_center[2] * new_center[2]));
        m[1][1] = mass * ((center_of_mass[0] * center_of_mass[0] + center_of_mass[2] * center_of_mass[2])
            - (new_center[0] * new_center[0] + new_center[2] * new_center[2]));
        m[2][2] = mass * ((center_of_mass[0] * center_of_mass[0] + center_of_mass[1] * center_of_mass[1])
            - (new_center[0] * new_center[0] + new_center[1] * new_center[1]));

        m[0][1] = m[1][0] = mass * (new_center[0] * new_center[1] - center_of_mass[0] * center_of_mass[1]);
        m[1][2] = m[2][1] = mass * (new_center[1] * new_center[2] - center_of_mass[1] * center_of_mass[2]);
        m[0][2] = m[2][0] = mass * (new_center[0] * new_center[2] - center_of_mass[0] * center_of_mass[2]);

        return (*this) + m;
    }

    inline matrix3&        inertia_translate(const float mass, const vector3d& center_of_mass, const vector3d& translation){
        matrix3  m;
        vector3d new_center;

        new_center = center_of_mass + translation;

        m[0][0] = mass * ((center_of_mass[1] * center_of_mass[1] + center_of_mass[2] * center_of_mass[2])
            - (new_center[1] * new_center[1] + new_center[2] * new_center[2]));
        m[1][1] = mass * ((center_of_mass[0] * center_of_mass[0] + center_of_mass[2] * center_of_mass[2])
            - (new_center[0] * new_center[0] + new_center[2] * new_center[2]));
        m[2][2] = mass * ((center_of_mass[0] * center_of_mass[0] + center_of_mass[1] * center_of_mass[1])
            - (new_center[0] * new_center[0] + new_center[1] * new_center[1]));

        m[0][1] = m[1][0] = mass * (new_center[0] * new_center[1] - center_of_mass[0] * center_of_mass[1]);
        m[1][2] = m[2][1] = mass * (new_center[1] * new_center[2] - center_of_mass[1] * center_of_mass[2]);
        m[0][2] = m[2][0] = mass * (new_center[0] * new_center[2] - center_of_mass[0] * center_of_mass[2]);

        (*this) += m;

        return (*this);
    }

    inline matrix3         get_inertia_rotate(const matrix3& rotation) const{
        // NOTE: the rotation matrix is stored column-major
        return rotation.get_transpose() * (*this) * rotation;
    }

    inline matrix3&        inertia_rotate(const matrix3& rotation){
        // NOTE: the rotation matrix is stored column-major
        *this = rotation.get_transpose() * (*this) * rotation;
        return *this;
    }

    friend inline matrix3  skew_symmetric(const vector3d& src){
        return matrix3(0.0f, -src.z, src.y, src.z, 0.0f, -src.x, -src.y, src.x, 0.0f);
    }
};

/***********************************************************
 *
 * matrix4 - 4x4 matrix
 *
 * matrix
 *
***********************************************************/
struct matrix4
{
public:
    vector4d    mat[4];

    /* Global zero matrix4 */
    static const matrix4& zero_matrix4()
    {
        static const matrix4 z_matrix4(0, 0, 0, 0,
                                       0, 0, 0, 0,
                                       0, 0, 0, 0,
                                       0, 0, 0, 0);

        return z_matrix4;
    }

    /* Global identiy matrix4 */
    static const matrix4& identity_matrix4(){
        static const matrix4 i_matrix4(1, 0, 0, 0,
                                       0, 1, 0, 0,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1);
        return i_matrix4;
    }

public:
    inline matrix4(void){}
    inline explicit matrix4(const vector4d& x, const vector4d& y, const vector4d& z, const vector4d& w){
        mat[0] = x; mat[1] = y; mat[2] = z; mat[3] = w;
    }

    inline explicit matrix4(
        const float xx, const float xy, const float xz, const float xw,
        const float yx, const float yy, const float yz, const float yw,
        const float zx, const float zy, const float zz, const float zw,
        const float wx, const float wy, const float wz, const float ww){
        mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
        mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
        mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
        mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
    }

    inline explicit matrix4(const matrix3& rotation, const vector3d& translation){
        // NOTE: idMat3 is transposed because it is column-major
        mat[0][0] = rotation[0][0];
        mat[0][1] = rotation[1][0];
        mat[0][2] = rotation[2][0];
        mat[0][3] = translation[0];
        mat[1][0] = rotation[0][1];
        mat[1][1] = rotation[1][1];
        mat[1][2] = rotation[2][1];
        mat[1][3] = translation[1];
        mat[2][0] = rotation[0][2];
        mat[2][1] = rotation[1][2];
        mat[2][2] = rotation[2][2];
        mat[2][3] = translation[2];
        mat[3][0] = 0.0f;
        mat[3][1] = 0.0f;
        mat[3][2] = 0.0f;
        mat[3][3] = 1.0f;
    }

    inline explicit matrix4(const float src[4][4]){
        mat[0][0] = src[0][0]; mat[0][1] = src[0][1]; mat[0][2] = src[0][2]; mat[0][3] = src[0][3];
        mat[1][0] = src[1][0]; mat[1][1] = src[1][1]; mat[1][2] = src[1][2]; mat[1][3] = src[1][3];
        mat[2][0] = src[2][0]; mat[2][1] = src[2][1]; mat[2][2] = src[2][2]; mat[2][3] = src[2][3];
        mat[3][0] = src[3][0]; mat[3][1] = src[3][1]; mat[3][2] = src[3][2]; mat[3][3] = src[3][3];
    }

    inline const vector4d&  operator[](int index) const{
        return mat[index];
    }

    inline vector4d&        operator[](int index){
        return mat[index];
    }

    inline matrix4          operator*(const float a) const{
        return matrix4(
            mat[0].x * a, mat[0].y * a, mat[0].z * a, mat[0].w * a,
            mat[1].x * a, mat[1].y * a, mat[1].z * a, mat[1].w * a,
            mat[2].x * a, mat[2].y * a, mat[2].z * a, mat[2].w * a,
            mat[3].x * a, mat[3].y * a, mat[3].z * a, mat[3].w * a);
    }

    inline vector4d         operator*(const vector4d& vec) const{
        return vector4d(
            vector4d::dot4(mat[0], vec),
            vector4d::dot4(mat[1], vec),
            vector4d::dot4(mat[2], vec),
            vector4d::dot4(mat[3], vec));
    }

    inline vector3d         operator*(const vector3d &vec) const{
        float s = mat[3].x * vec.x + mat[3].y * vec.y + mat[3].z * vec.z + mat[3].w;
        if (s == 0.0f) {
            return vector3d(0.0f, 0.0f, 0.0f);
        }
        if (s == 1.0f) {
            return vector3d(
                mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w,
                mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w,
                mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w);
        }
        else {
            float inv_s = 1.0f / s;
            return vector3d(
                (mat[0].x * vec.x + mat[0].y * vec.y + mat[0].z * vec.z + mat[0].w) * inv_s,
                (mat[1].x * vec.x + mat[1].y * vec.y + mat[1].z * vec.z + mat[1].w) * inv_s,
                (mat[2].x * vec.x + mat[2].y * vec.y + mat[2].z * vec.z + mat[2].w) * inv_s);
        }
    }

    inline matrix4  operator*(const matrix4& a) const{
        matrix4 dst;
        for (int32_t i = 0; i < 4; ++i){
            for (int32_t j = 0; j < 4; ++j){
                for (int32_t k = 0; k < 4; ++k){
                    dst[i][j] += ((*this)[i][k] * a[k][j]);
                }
            }
        }
        return dst;
    }

    inline matrix4  operator+(const matrix4& a) const{
        return matrix4(
            mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z, mat[0].w + a[0].w,
            mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z, mat[1].w + a[1].w,
            mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z, mat[2].w + a[2].w,
            mat[3].x + a[3].x, mat[3].y + a[3].y, mat[3].z + a[3].z, mat[3].w + a[3].w);
    }

    inline matrix4  operator-(const matrix4& a) const{
        return matrix4(
            mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z, mat[0].w - a[0].w,
            mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z, mat[1].w - a[1].w,
            mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z, mat[2].w - a[2].w,
            mat[3].x - a[3].x, mat[3].y - a[3].y, mat[3].z - a[3].z, mat[3].w - a[3].w);
    }

    inline matrix4& operator*=(float a){
        mat[0].x *= a; mat[0].y *= a; mat[0].z *= a; mat[0].w *= a;
        mat[1].x *= a; mat[1].y *= a; mat[1].z *= a; mat[1].w *= a;
        mat[2].x *= a; mat[2].y *= a; mat[2].z *= a; mat[2].w *= a;
        mat[3].x *= a; mat[3].y *= a; mat[3].z *= a; mat[3].w *= a;
        return *this;
    }

    inline matrix4& operator*=(const matrix4& a){
        *this = (*this) * a;
        return *this;
    }

    inline matrix4& operator+=(const matrix4& a){
        mat[0].x += a[0].x; mat[0].y += a[0].y; mat[0].z += a[0].z; mat[0].w += a[0].w;
        mat[1].x += a[1].x; mat[1].y += a[1].y; mat[1].z += a[1].z; mat[1].w += a[1].w;
        mat[2].x += a[2].x; mat[2].y += a[2].y; mat[2].z += a[2].z; mat[2].w += a[2].w;
        mat[3].x += a[3].x; mat[3].y += a[3].y; mat[3].z += a[3].z; mat[3].w += a[3].w;
        return *this;
    }

    inline matrix4& operator-=(const matrix4& a){
        mat[0].x -= a[0].x; mat[0].y -= a[0].y; mat[0].z -= a[0].z; mat[0].w -= a[0].w;
        mat[1].x -= a[1].x; mat[1].y -= a[1].y; mat[1].z -= a[1].z; mat[1].w -= a[1].w;
        mat[2].x -= a[2].x; mat[2].y -= a[2].y; mat[2].z -= a[2].z; mat[2].w -= a[2].w;
        mat[3].x -= a[3].x; mat[3].y -= a[3].y; mat[3].z -= a[3].z; mat[3].w -= a[3].w;
        return *this;
    }

    friend inline matrix4   operator*(float a, const matrix4& mat){
        return mat * a;
    }

    friend inline vector4d  operator*(const vector4d& vec, const matrix4& mat){
        return mat * vec;
    }

    friend inline vector3d  operator*(const vector3d& vec, const matrix4& mat){
        return mat * vec;
    }

    friend inline vector4d& operator*=(vector4d& vec, const matrix4& mat){
        vec = mat * vec;
        return vec;
    }

    friend inline vector3d& operator*=(vector3d& vec, const matrix4& mat){
        vec = mat * vec;
        return vec;
    }

    // compare with epsilon
    inline bool     equals(const matrix4& a, const float epsilon) const{
        return mat[0].equals(a[0], epsilon) &&
               mat[1].equals(a[1], epsilon) &&
               mat[2].equals(a[2], epsilon) &&
               mat[3].equals(a[3], epsilon);
    }

    // exact compare, no epsilon
    inline bool     operator==(const matrix4& a) const{
        return mat[0] == a[0] && 
               mat[1] == a[1] && 
               mat[2] == a[2] && 
               mat[3] == a[3];
    }

    // exact compare, no epsilon
    inline bool		operator!=(const matrix4& a) const{
        return !(*this == a);
    }

    inline void     zero(void){
        *this = zero_matrix4();
    }

    inline void     identity(void){
        *this = identity_matrix4();
    }

    inline bool     is_identity(float epsilon = MATRIX_EPSILON) const{
        return equals(identity_matrix4(), epsilon);
    }

    inline bool     is_symmetric(float epsilon = MATRIX_EPSILON) const{
        for (int32_t i = 1; i < 4; ++i){
            for (int32_t j = 0; j < i; ++j){
                if (abs(mat[i][j] - mat[j][i]) > epsilon){
                    return false;
                }
            }
        }
        return true;
    }

    inline bool     is_diagonal(float epsilon = MATRIX_EPSILON) const{
        for (int32_t i = 0; i < 4; ++i){
            for (int32_t j = 0; j < 4; ++j){
                if (i != j && abs(mat[i][j]) > epsilon){
                    return false;
                }
            }
        }
        return true;
    }

    inline bool     is_rotated(void) const{
        if (!mat[0][1] && !mat[0][2] &&
            !mat[1][0] && !mat[1][2] &&
            !mat[2][0] && !mat[2][1]) {
            return false;
        }
        return true;
    }

    inline void     project_vector(const vector4d& src, vector4d& dst) const{
        dst.x = vector4d::dot4(src, mat[0]);
        dst.y = vector4d::dot4(src, mat[1]);
        dst.z = vector4d::dot4(src, mat[2]);
        dst.w = vector4d::dot4(src, mat[3]);
    }

    inline void     unproject_vector(const vector4d& src, vector4d& dst) const{
        dst = mat[0] * src.x + mat[1] * src.y + mat[2] * src.z + mat[3] * src.w;
    }

    inline float    trace(void) const{
        return (mat[0][0] + mat[1][1] + mat[2][2] + mat[3][3]);
    }

    inline float    determinant(void) const{
        // 2x2 sub-determinants
        float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
        float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
        float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
        float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
        float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

        // 3x3 sub-determinants
        float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
        float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
        float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
        float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

        return (-det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3]);
    }

    inline matrix4  get_transpose(void) const{
        matrix4 transp;
        for (int32_t i = 0; i < 4; ++i){
            for (int32_t j = 0; j < 4; ++j){
                transp[i][j] = mat[j][i];
            }
        }
        return transp;
    }

    inline matrix4& transpose(void){
        float temp = 0;
        for (int32_t i = 0; i < 4; i++) {
            for (int32_t j = i + 1; j < 4; j++) {
                temp = mat[i][j];
                mat[i][j] = mat[j][i];
                mat[j][i] = temp;
            }
        }
        return *this;
    }

    // returns the inverse ( m * m.Inverse() = identity )
    inline matrix4  get_inverse(void) const{
        matrix4 inv = *this;
        bool r = inv.inverse();
        assert(r);
        return inv;
    }

    // returns false if determinant is zero
    inline bool     inverse(void){
        // 84+4+16 = 104 multiplications
        //			   1 division

        // 2x2 sub-determinants required to calculate 4x4 determinant
        float det2_01_01 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        float det2_01_02 = mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0];
        float det2_01_03 = mat[0][0] * mat[1][3] - mat[0][3] * mat[1][0];
        float det2_01_12 = mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1];
        float det2_01_13 = mat[0][1] * mat[1][3] - mat[0][3] * mat[1][1];
        float det2_01_23 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

        // 3x3 sub-determinants required to calculate 4x4 determinant
        float det3_201_012 = mat[2][0] * det2_01_12 - mat[2][1] * det2_01_02 + mat[2][2] * det2_01_01;
        float det3_201_013 = mat[2][0] * det2_01_13 - mat[2][1] * det2_01_03 + mat[2][3] * det2_01_01;
        float det3_201_023 = mat[2][0] * det2_01_23 - mat[2][2] * det2_01_03 + mat[2][3] * det2_01_02;
        float det3_201_123 = mat[2][1] * det2_01_23 - mat[2][2] * det2_01_13 + mat[2][3] * det2_01_12;

        float det = (-det3_201_123 * mat[3][0] + det3_201_023 * mat[3][1] - det3_201_013 * mat[3][2] + det3_201_012 * mat[3][3]);

        if (abs(det) < MATRIX_INVERSE_EPSILON) {
            return false;
        }

        float inv_det = 1.0f / det;

        // remaining 2x2 sub-determinants
        float det2_03_01 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
        float det2_03_02 = mat[0][0] * mat[3][2] - mat[0][2] * mat[3][0];
        float det2_03_03 = mat[0][0] * mat[3][3] - mat[0][3] * mat[3][0];
        float det2_03_12 = mat[0][1] * mat[3][2] - mat[0][2] * mat[3][1];
        float det2_03_13 = mat[0][1] * mat[3][3] - mat[0][3] * mat[3][1];
        float det2_03_23 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];

        float det2_13_01 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
        float det2_13_02 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
        float det2_13_03 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
        float det2_13_12 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
        float det2_13_13 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
        float det2_13_23 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];

        // remaining 3x3 sub-determinants
        float det3_203_012 = mat[2][0] * det2_03_12 - mat[2][1] * det2_03_02 + mat[2][2] * det2_03_01;
        float det3_203_013 = mat[2][0] * det2_03_13 - mat[2][1] * det2_03_03 + mat[2][3] * det2_03_01;
        float det3_203_023 = mat[2][0] * det2_03_23 - mat[2][2] * det2_03_03 + mat[2][3] * det2_03_02;
        float det3_203_123 = mat[2][1] * det2_03_23 - mat[2][2] * det2_03_13 + mat[2][3] * det2_03_12;

        float det3_213_012 = mat[2][0] * det2_13_12 - mat[2][1] * det2_13_02 + mat[2][2] * det2_13_01;
        float det3_213_013 = mat[2][0] * det2_13_13 - mat[2][1] * det2_13_03 + mat[2][3] * det2_13_01;
        float det3_213_023 = mat[2][0] * det2_13_23 - mat[2][2] * det2_13_03 + mat[2][3] * det2_13_02;
        float det3_213_123 = mat[2][1] * det2_13_23 - mat[2][2] * det2_13_13 + mat[2][3] * det2_13_12;

        float det3_301_012 = mat[3][0] * det2_01_12 - mat[3][1] * det2_01_02 + mat[3][2] * det2_01_01;
        float det3_301_013 = mat[3][0] * det2_01_13 - mat[3][1] * det2_01_03 + mat[3][3] * det2_01_01;
        float det3_301_023 = mat[3][0] * det2_01_23 - mat[3][2] * det2_01_03 + mat[3][3] * det2_01_02;
        float det3_301_123 = mat[3][1] * det2_01_23 - mat[3][2] * det2_01_13 + mat[3][3] * det2_01_12;

        mat[0][0] = -det3_213_123 * inv_det;
        mat[1][0] = +det3_213_023 * inv_det;
        mat[2][0] = -det3_213_013 * inv_det;
        mat[3][0] = +det3_213_012 * inv_det;

        mat[0][1] = +det3_203_123 * inv_det;
        mat[1][1] = -det3_203_023 * inv_det;
        mat[2][1] = +det3_203_013 * inv_det;
        mat[3][1] = -det3_203_012 * inv_det;

        mat[0][2] = +det3_301_123 * inv_det;
        mat[1][2] = -det3_301_023 * inv_det;
        mat[2][2] = +det3_301_013 * inv_det;
        mat[3][2] = -det3_301_012 * inv_det;

        mat[0][3] = -det3_201_123 * inv_det;
        mat[1][3] = +det3_201_023 * inv_det;
        mat[2][3] = -det3_201_013 * inv_det;
        mat[3][3] = +det3_201_012 * inv_det;

        return true;
    }
};

} // end namespace math
} // end namespace utility

#endif