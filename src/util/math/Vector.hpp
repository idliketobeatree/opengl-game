#pragma once

// TODO: let Vectors convert number types. Not sure how right now.

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

    Vector3<T> operator /(const T scalar) const {
        return {
                this->x / scalar,
                this->y / scalar,
                this->z / scalar
        };
    }
    void operator /=(const T scalar) {
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
    }
    friend Vector3<T> operator /(const T scalar, const Vector3<T> &vector) {
        return vector / scalar;
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