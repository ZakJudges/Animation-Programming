#include "Matrix44.h"
#include <cmath>
#include <iostream>

bool operator==(const Matrix44& a, const Matrix44& b)
{
    for (int i = 0; i < 16; i++)
    {
        if (fabsf(a.v[i] - b.v[i]) > MATRIX_EPSILON)
        {
            return false;
        }
    }

    return true;
}

bool operator!=(const Matrix44& a, const Matrix44& b)
{
    return !(a == b);
}

Matrix44 operator*(const Matrix44& matrix, const float scale)
{
    return Matrix44
    (
        matrix.xx * scale, matrix.xy * scale, matrix.xz * scale, matrix.xw * scale,
        matrix.yx * scale, matrix.yy * scale, matrix.yz * scale, matrix.yw * scale,
        matrix.zx * scale, matrix.zy * scale, matrix.zz * scale, matrix.zw * scale,
        matrix.tx * scale, matrix.ty * scale, matrix.tz * scale, matrix.tw * scale
    );
}

Matrix44 operator+(const Matrix44& a, const Matrix44& b)
{
    return Matrix44
    (
        a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
        a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
        a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
        a.tx + b.tx, a.ty + b.ty, a.tz + b.tz, a.tw + b.tw
    );
}

#define M4D(aRow, bCol) \
    a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] + \
    a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] + \
    a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] + \
    a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3]

Matrix44 operator*(const Matrix44& a, const Matrix44& b)
{
    return Matrix44
    (
        M4D(0, 0), M4D(1, 0), M4D(2, 0), M4D(3, 0), 
        M4D(0, 1), M4D(1, 1), M4D(2, 1), M4D(3, 1), 
        M4D(0, 2), M4D(1, 2), M4D(2, 2), M4D(3, 2), 
        M4D(0, 3), M4D(1, 3), M4D(2, 3), M4D(3, 3)  
    );
}

#define M4V4D(mRow, x, y, z, w) \
    x * m.v[0 * 4 + mRow] + \
    y * m.v[1 * 4 + mRow] + \
    z * m.v[2 * 4 + mRow] + \
    w * m.v[3 * 4 + mRow]

Vector4F operator*(const Matrix44& m, const Vector4F& v) {
    return Vector4F
    (
        M4V4D(0, v.x, v.y, v.z, v.w),
        M4V4D(1, v.x, v.y, v.z, v.w),
        M4V4D(2, v.x, v.y, v.z, v.w),
        M4V4D(3, v.x, v.y, v.z, v.w)
    );
}

Vector3 TransformVector(const Matrix44& m, const Vector3& v)
{
    return Vector3
    (
        M4V4D(0, v.x, v.y, v.z, 0.0f),
        M4V4D(1, v.x, v.y, v.z, 0.0f),
        M4V4D(2, v.x, v.y, v.z, 0.0f)
    );
}

Vector3 TransformPoint(const Matrix44& m, const Vector3& v)
{
    return Vector3(
        M4V4D(0, v.x, v.y, v.z, 1.0f),
        M4V4D(1, v.x, v.y, v.z, 1.0f),
        M4V4D(2, v.x, v.y, v.z, 1.0f)
    );
}

Vector3 TransformPoint(const Matrix44& m, const Vector3& v, float& w)
{
    float _w = w;
    w = M4V4D(3, v.x, v.y, v.z, _w);
    return Vector3
    (
        M4V4D(0, v.x, v.y, v.z, _w),
        M4V4D(1, v.x, v.y, v.z, _w),
        M4V4D(2, v.x, v.y, v.z, _w)
    );
}

#define M4SWAP(x, y) \
    {float t = x; x = y; y = t; }

void Transpose(Matrix44& m)
{
    M4SWAP(m.yx, m.xy);
    M4SWAP(m.zx, m.xz);
    M4SWAP(m.tx, m.xw);
    M4SWAP(m.zy, m.yz);
    M4SWAP(m.ty, m.yw);
    M4SWAP(m.tz, m.zw);
}

Matrix44 Transposed(const Matrix44& m)
{
    return Matrix44
    (
        m.xx, m.yx, m.zx, m.tx,
        m.xy, m.yy, m.zy, m.ty,
        m.xz, m.yz, m.zz, m.tz,
        m.xw, m.yw, m.zw, m.tw
    );
}

#define M4_3X3MINOR(c0, c1, c2, r0, r1, r2) \
    (m.v[c0 * 4 + r0] * (m.v[c1 * 4 + r1] * m.v[c2 * 4 + r2] - m.v[c1 * 4 + r2] * m.v[c2 * 4 + r1]) - \
     m.v[c1 * 4 + r0] * (m.v[c0 * 4 + r1] * m.v[c2 * 4 + r2] - m.v[c0 * 4 + r2] * m.v[c2 * 4 + r1]) + \
     m.v[c2 * 4 + r0] * (m.v[c0 * 4 + r1] * m.v[c1 * 4 + r2] - m.v[c0 * 4 + r2] * m.v[c1 * 4 + r1]))

float Determinant(const Matrix44& m) 
{
    return  m.v[0] * M4_3X3MINOR(1, 2, 3, 1, 2, 3)
        - m.v[4] * M4_3X3MINOR(0, 2, 3, 1, 2, 3)
        + m.v[8] * M4_3X3MINOR(0, 1, 3, 1, 2, 3)
        - m.v[12] * M4_3X3MINOR(0, 1, 2, 1, 2, 3);
}
Matrix44 Adjugate(const Matrix44& m)
{
    // Cofactor(M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
    Matrix44 cofactor;

    cofactor.v[0] = M4_3X3MINOR(1, 2, 3, 1, 2, 3);
    cofactor.v[1] = -M4_3X3MINOR(1, 2, 3, 0, 2, 3);
    cofactor.v[2] = M4_3X3MINOR(1, 2, 3, 0, 1, 3);
    cofactor.v[3] = -M4_3X3MINOR(1, 2, 3, 0, 1, 2);

    cofactor.v[4] = -M4_3X3MINOR(0, 2, 3, 1, 2, 3);
    cofactor.v[5] = M4_3X3MINOR(0, 2, 3, 0, 2, 3);
    cofactor.v[6] = -M4_3X3MINOR(0, 2, 3, 0, 1, 3);
    cofactor.v[7] = M4_3X3MINOR(0, 2, 3, 0, 1, 2);

    cofactor.v[8] = M4_3X3MINOR(0, 1, 3, 1, 2, 3);
    cofactor.v[9] = -M4_3X3MINOR(0, 1, 3, 0, 2, 3);
    cofactor.v[10] = M4_3X3MINOR(0, 1, 3, 0, 1, 3);
    cofactor.v[11] = -M4_3X3MINOR(0, 1, 3, 0, 1, 2);

    cofactor.v[12] = -M4_3X3MINOR(0, 1, 2, 1, 2, 3);
    cofactor.v[13] = M4_3X3MINOR(0, 1, 2, 0, 2, 3);
    cofactor.v[14] = -M4_3X3MINOR(0, 1, 2, 0, 1, 3);
    cofactor.v[15] = M4_3X3MINOR(0, 1, 2, 0, 1, 2);

    return Transposed(cofactor);
}

Matrix44 Inverse(const Matrix44& m)
{
    float det = Determinant(m);

    if (det == 0.0f) { 
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        return Matrix44();
    }
    Matrix44 adj = Adjugate(m);

    return adj * (1.0f / det);
}

void Invert(Matrix44& m)
{
    float det = Determinant(m);

    if (det == 0.0f) {
        std::cout << "WARNING: Trying to invert a matrix with a zero determinant\n";
        return;
    }
   
    m = Adjugate(m) * (1.0f / det);
}

Matrix44 Frustum(float l, float r, float b, float t, float n, float f)
{
    if (l == r || t == b || n == f) {
        std::cout << "WARNING: Trying to create invalid frustum\n";
        return Matrix44(); 
    }
    return Matrix44
    (
        (2.0f * n) / (r - l), 0, 0, 0,
        0, (2.0f * n) / (t - b), 0, 0,
        (r + l) / (r - l), (t + b) / (t - b), (-(f + n)) / (f - n), -1,
        0, 0, (-2 * f * n) / (f - n), 0
    );
}

Matrix44 Perspective(float fov, float aspect, float znear, float zfar)
{
    float ymax = znear * tanf(fov * 3.14159265359f / 360.0f);
    float xmax = ymax * aspect;

    return Frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

Matrix44 Ortho(float l, float r, float b, float t, float n, float f)
{
    if (l == r || t == b || n == f) {
        return Matrix44();
    }
    return Matrix44
    (
        2.0f / (r - l), 0, 0, 0,
        0, 2.0f / (t - b), 0, 0,
        0, 0, -2.0f / (f - n), 0,
        -((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1
    );
}

Matrix44 LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
    Vector3 forward = target - position;
    Vector3 frameForward = Normalised(forward) * -1.0f;

    Vector3 frameRight = Cross(up, frameForward);
    if (frameRight == Vector3(0, 0, 0)) {
        return Matrix44();
    }
    Normalise(frameRight);

    Vector3 frameUp = Cross(frameForward, frameRight);
    Normalise(frameUp);

    Vector3 t = Vector3
    (
        -Dot(frameRight, position),
        -Dot(frameUp, position),
        -Dot(frameForward, position)
    );

    return Matrix44
    (
        frameRight.x, frameUp.x, frameForward.x, 0,
        frameRight.y, frameUp.y, frameForward.y, 0,
        frameRight.z, frameUp.z, frameForward.z, 0,
        t.x, t.y, t.z, 1
    );
}
