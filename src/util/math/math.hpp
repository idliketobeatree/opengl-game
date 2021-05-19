#pragma once

#include <cstdint>
#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"

#define PI     3.1415927410125732421875
#define TAU    6.2831853071795864769252
#define TWO_PI 6.2831853071795864769252

/// Vectors
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector3<uint8_t> Vector3u8;
typedef Vector3<uint16_t> Vector3u16;
typedef Vector3<uint32_t> Vector3u32;
typedef Vector3<uint64_t> Vector3u64;
typedef Vector3u32 Vector3u;

typedef Vector3<int8_t> Vector3i8;
typedef Vector3<int16_t> Vector3i16;
typedef Vector3<int32_t> Vector3i32;
typedef Vector3<int64_t> Vector3i64;
typedef Vector3i32 Vector3i;

/// Colors
typedef Vector3<float> Color3f;
typedef Vector3<double> Color3d;
typedef Vector3<uint8_t> Color3u8;

/// Matrices
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

typedef Matrix4<uint8_t> Matrix4u8;
typedef Matrix4<uint16_t> Matrix4u16;
typedef Matrix4<uint32_t> Matrix4u32;
typedef Matrix4<uint64_t> Matrix4u64;
typedef Matrix4u32 Matrix4u;

typedef Matrix4<int8_t> Matrix4i8;
typedef Matrix4<int16_t> Matrix4i16;
typedef Matrix4<int32_t> Matrix4i32;
typedef Matrix4<int64_t> Matrix4i64;
typedef Matrix4i32 Matrix4i;

const static Vector3f UP{0,1,0};