#pragma once

#include <cstdint>
#include <math.h>

#define PI     3.1415927410125732421875
#define TAU    6.2831853071795864769252
#define TWO_PI 6.2831853071795864769252

template <typename T>
struct Vector3 {
    T x{},y{},z{};

    inline T dot(const Vector3<T> &other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }
    inline Vector3<T> cross(const Vector3<T> &other) const {
        return {
                this->y * other.z - this->z * other.y,
                this->z * other.x - this->x * other.z,
                this->x * other.y - this->y * other.x
        };
    }

    Vector3<T> operator +(const Vector3<T> &other) const {
        return {
                this->x + other.x,
                this->y + other.y,
                this->z + other.z
        };
    }
    void operator +=(const Vector3<T> &other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }

    Vector3<T> operator *(const T scalar) const {
        return {
                this->x * scalar,
                this->y * scalar,
                this->z * scalar
        };
    }
    void operator *=(const T scalar) {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
    }
    friend Vector3<T> operator *(const T scalar, const Vector3<T> &vector) {
        return vector * scalar;
    }

    Vector3<T> operator -(const Vector3<T> &other) const {
        return {
            this->x - other.x,
            this->y - other.y,
            this->z - other.z
        };
    }
    void operator -=(const Vector3<T> &other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
    }

    static inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b) {
        return a.cross(b);
    }
    static inline Vector3<T> dot(const Vector3<T> &a, const  Vector3<T> &b) {
        return a.dot(b);
    }
    static inline Vector3<T> normalize(const Vector3<T> &a) {
        Vector3<T> out;
        T mag = a.magnitude();
        out.x = a.x/mag;
        out.y = a.y/mag;
        out.z = a.z/mag;
        return out;
    }
    inline T magnitude() const {
        T sum = (x * x) + (y * y) + (z * z);
        return sqrt(sum);
    }
};

/**
 * row-major b/c i have a brain
 */
template <typename T>
struct Matrix4 {
    T elems[16];
    /**
     * 0  1  2  3
     * 4  5  6  7
     * 8  9  10 11
     * 12 13 14 15
     */

    Matrix4<T> operator *(Matrix4<T> &that) const {
        return Matrix4<T>{
            elems[0]*that[0]  + elems[1]*that[4]  + elems[2]*that[8]   + elems[3]*that[12],  /// r0*c0
            elems[0]*that[1]  + elems[1]*that[5]  + elems[2]*that[9]   + elems[3]*that[13],  /// r0*c1
            elems[0]*that[2]  + elems[1]*that[6]  + elems[2]*that[10]  + elems[3]*that[14],  /// r0*c2
            elems[0]*that[3]  + elems[1]*that[7]  + elems[2]*that[11]  + elems[3]*that[15],  /// r0*c3

            elems[4]*that[0]  + elems[5]*that[4]  + elems[6]*that[8]   + elems[7]*that[12],  /// r1*c0
            elems[4]*that[1]  + elems[5]*that[5]  + elems[6]*that[9]   + elems[7]*that[13],  /// r1*c1
            elems[4]*that[2]  + elems[5]*that[6]  + elems[6]*that[10]  + elems[7]*that[14],  /// r1*c2
            elems[4]*that[3]  + elems[5]*that[7]  + elems[6]*that[11]  + elems[7]*that[15],  /// r1*c3

            elems[8]*that[0]  + elems[9]*that[4]  + elems[10]*that[8]  + elems[11]*that[12], /// r2*c0
            elems[8]*that[1]  + elems[9]*that[5]  + elems[10]*that[9]  + elems[11]*that[13], /// r2*c1
            elems[8]*that[2]  + elems[9]*that[6]  + elems[10]*that[10] + elems[11]*that[14], /// r2*c2
            elems[8]*that[3]  + elems[9]*that[7]  + elems[10]*that[11] + elems[11]*that[15], /// r2*c3

            elems[12]*that[0] + elems[13]*that[4] + elems[14]*that[8]  + elems[15]*that[12], /// r3*c0
            elems[12]*that[1] + elems[13]*that[5] + elems[14]*that[9]  + elems[15]*that[13], /// r3*c1
            elems[12]*that[2] + elems[13]*that[6] + elems[14]*that[10] + elems[15]*that[14], /// r3*c2
            elems[12]*that[3] + elems[13]*that[7] + elems[14]*that[11] + elems[15]*that[15], /// r3*c3
        };
    }
    Matrix4<T> operator *(T that) const {
        return Matrix4<T>{
            elems[0]*that,  elems[1]*that,  elems[2]*that,  elems[3]*that,
            elems[4]*that,  elems[5]*that,  elems[6]*that,  elems[7]*that,
            elems[8]*that,  elems[9]*that,  elems[10]*that, elems[11]*that,
            elems[12]*that, elems[13]*that, elems[14]*that, elems[15]*that,
        };
    }
    friend Matrix4<T> operator *(T scalar, Matrix4<T> &matrix) {
        return matrix * scalar;
    }
    Matrix4<T> operator +(Matrix4<T> &that) const {
        return Matrix4{
            elems[0]+that[0],   elems[1]+that[1],   elems[2]+that[2],   elems[3]+that[3],
            elems[4]+that[4],   elems[5]+that[5],   elems[6]+that[6],   elems[7]+that[7],
            elems[8]+that[8],   elems[9]+that[9],   elems[10]+that[10], elems[11]+that[11],
            elems[12]+that[12], elems[13]+that[13], elems[14]+that[14], elems[15]+that[15],
        };
    }

    Matrix4<T> operator -(Matrix4<T> &that) const {
        return Matrix4{
            elems[0] -that[0],  elems[1]-that[1],   elems[2]-that[2],   elems[3]-that[3],
            elems[4] -that[4],  elems[5]-that[5],   elems[6]-that[6],   elems[7]-that[7],
            elems[8] -that[8],  elems[9]-that[9],   elems[10]-that[10], elems[11]-that[11],
            elems[12]-that[12], elems[13]-that[13], elems[14]-that[14], elems[15]-that[15],
        };
    }
    T& operator [](uint_fast8_t i) {
        return elems[i];
    }
    Matrix4<T> transpose() const {
        return Matrix4<T>{
            elems[0],  elems[4],  elems[8],  elems[12],
            elems[1],  elems[5],  elems[9],  elems[13],
            elems[2],  elems[6],  elems[10], elems[14],
            elems[3],  elems[7],  elems[11], elems[15],
        };
    }
    static Matrix4<T> identity() {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    }

    operator T* () {
        return elems;
    }
};

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

const static Vector3f UP{0,1,0};