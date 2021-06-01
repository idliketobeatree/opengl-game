#include "Noise.hpp"

#include <cstdint>

static inline int_fast32_t floor(float f) {
    return (int_fast32_t) f;
}

static const uint8_t perm[256] = {
        151, 160, 137, 91, 90, 15,
        131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

static inline uint8_t hash(uint8_t i) {
    return perm[i];
}

static float grad(uint8_t hash, float x) {
    const uint8_t h = hash & 0x0f;
    const float grad = (h & 8) ?
                       -(1.0f + (h & 7)) : 1.0f + (h & 7);
    return (grad * x);
}

static float grad(uint8_t hash, float x, float y) {
    const uint8_t h = hash & 0x3f;
    const float u = h < 4 ? x : y;
    const float v = h < 4 ? y : x;
    return ((h & 0x01) ? -u : u) + ((h & 0x02) ? -2.0f * v : 2.0f * v);
}

static float grad(uint8_t hash, float x, float y, float z) {
    const uint8_t h = hash & 0x0f;
    const float u = h < 8 ? x : y;
    const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

/**
 * 1D Perlin simplex noise
 *
 * @param[in] x float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float Noise::noise1f(float x) {
    int32_t i0, i1; // corner coords as integers
    float x0, x1;   // distances to corners
    float t0, t1;   // temporary variables
    float n0, n1;   // noise contributions of 2 corners

    // no skewing necessary in 1D

    i0 = floor(x);
    i1 = i0 + 1;

    x0 = x - i0;
    x1 = x0 - 1.0f;

    t0 = 1.0f - x0 * x0;
    n0 = t0 * t0 * t0 * grad(hash(i0), x0);

    t1 = 1.0f - x1 * x1;
    n1 = t1 * t1 * t1 * grad(hash(i1), x1);

    /// Add contributions from each corner to get the final noise value.
    /// The result is scaled to stay just inside [-1,1]
    return 0.395f * (n0 + n1);
}

/**
 * 2D Perlin simplex noise
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float Noise::noise2f(float x, float y) {
    /// Skewing/Unskewing factors for 2D
    static const float F2 = 0.366025403f; // F2 = 0.5*(sqrt(3.0)-1.0)
    static const float G2 = 0.211324865f; // G2 = (3.0-Math.sqrt(3.0))/6.0

    /// Skew variables
    float   x0, y0;
    float   s, t;           //
    int32_t i, j;           // Offsets for first corner of simplex in (i,j) coords
    int32_t i1, j1;         // Offsets for third corner of simplex in (i,j) coords

    /// Other variables
    float   x1, y1, x2, y2; // offsets for corners in unskewed coords
    float   t0, t1, t2;     //
    uint8_t gi0, gi1, gi2;  // hashed gradient indices

    float   n0, n1, n2;     /// Noise contributions from the three corners

    s = (x + y) * F2; /// Skew the input space to find what cell we're in
    i = floor(x + s);
    j = floor(y + s);

    t = (float) (i + j) * G2; /// Unskew the cell origin back to (x,y) space
    x0 = x - i + t;  // The x,y distances from the cell origin
    y0 = y - j + t;

    /// For the 2D case, the simplex shape is an equilateral triangle.
    /// Determine which simplex we are in.

    if (x0 > y0) {i1 = 1; j1 = 0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else {i1 = 0; j1 = 1;}         // upper triangle, YX order: (0,0)->(0,1)->(1,1)

    x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
    y1 = y0 - j1 + G2;
    x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords
    y2 = y0 - 1.0f + 2.0f * G2;

    // Work out the hashed gradient indices of the three simplex corners
    gi0 = hash(i + hash(j));
    gi1 = hash(i + i1 + hash(j + j1));
    gi2 = hash(i + 1 + hash(j + 1));

    /// Calculate the contribution from the three corners
    t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0.0f) {
        n0 = 0.0f;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0);
    }

    t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0.0f) {
        n1 = 0.0f;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1);
    }

    t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0.0f) {
        n2 = 0.0f;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2);
    }

    /// Add contributions from each corner to get the final noise value.
    /// The result is scaled to stay just inside [-1,1]
    return 45.23065f * (n0 + n1 + n2);
}

/**
 * 3D Perlin simplex noise
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 * @param[in] z float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float Noise::noise3f(float x, float y, float z) {
    /// Skewing/Unskewing factors for 3D
    static const float F3 = 1.0f / 3.0f;
    static const float G3 = 1.0f / 6.0f;

    /// Skew variables
    float   x0, y0, z0;          //
    float   s, t;                // skew/unskew factors

    int32_t i, j, k;             // Offsets for first corner of simplex in (i,j,k) coords
    int32_t i1, j1, k1;          // Offsets for second corner of simplex in (i,j,k) coords
    int32_t i2, j2, k2;          // Offsets for third corner of simplex in (i,j,k) coords

    /// Other variables
    float   x1, y1, z1;          // offsets for corners in unskewed coords
    float   x2, y2, z2;
    float   x3, y3, z3;

    float   t0, t1, t2, t3;      //
    uint8_t gi0, gi1, gi2, gi3;  // hashed gradient indices

    float   n0, n1, n2, n3;      /// Noise contributions from the three corners


    /// Skew the input space to determine which simplex cell we're in
    s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
    i = floor(x + s);
    j = floor(y + s);
    k = floor(z + s);

    t = (i + j + k) * G3;
    x0 = x - i + t; // The x,y,z distances from the cell origin
    y0 = y - j + t;
    z0 = z - k + t;

    /// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    /// Determine which simplex we are in.
    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // X Y Z order
        } else if (x0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; // X Z Y order
        } else {
            i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; // Z X Y order
        }
    } else { // x0<y0
        if (y0 < z0) {
            i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; // Z Y X order
        } else if (x0 < z0) {
            i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; // Y Z X order
        } else {
            i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // Y X Z order
        }
    }

    x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    y1 = y0 - j1 + G3;
    z1 = z0 - k1 + G3;
    x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords
    y2 = y0 - j2 + 2.0f * G3;
    z2 = z0 - k2 + 2.0f * G3;
    x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords
    y3 = y0 - 1.0f + 3.0f * G3;
    z3 = z0 - 1.0f + 3.0f * G3;

    // Work out the hashed gradient indices of the four simplex corners
    gi0 = hash(i + hash(j + hash(k)));
    gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
    gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
    gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

    // Calculate the contribution from the four corners
    t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
    t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
    t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
    t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;

    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0, z0);
    }
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1, z1);
    }
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2, z2);
    }
    if (t3 < 0) {
        n3 = 0.0;
    } else {
        t3 *= t3;
        n3 = t3 * t3 * grad(gi3, x3, y3, z3);
    }
    /// Add contributions from each corner to get the final noise value.
    /// The result is scaled to stay just inside [-1,1]
    return 32.0f*(n0 + n1 + n2 + n3);
}

float lerp(float a, float b, float c) {
    return a + c * (b-a);
}