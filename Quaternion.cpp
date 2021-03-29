#include "Quaternion.h"
#include <cmath>


Quaternion AngleAxis(float angle, const Vector3& axis)
{
    Vector3 norm = Normalised(axis);
    float s = sinf(angle * 0.5f);

    return Quaternion
    (
        norm.x * s,
        norm.y * s,
        norm.z * s,
        cosf(angle * 0.5f)
    );
}

Quaternion FromTo(const Vector3& from, const Vector3& to)
{
    Vector3 f = Normalised(from);
    Vector3 t = Normalised(to);

    if (f == t) {
        return Quaternion();
    }
    else if (f == t * -1.0f) {
        Vector3 ortho = Vector3(1, 0, 0);
        if (fabsf(f.y) < fabsf(f.x)) {
            ortho = Vector3(0, 1, 0);
        }
        if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
            ortho = Vector3(0, 0, 1);
        }

        Vector3 axis = Normalised(Cross(f, ortho));
        return Quaternion(axis.x, axis.y, axis.z, 0);
    }

    Vector3 half = Normalised(f + t);
    Vector3 axis = Cross(f, half);

    return Quaternion
    (
        axis.x,
        axis.y,
        axis.z,
        Dot(f, half)
    );
}

Vector3 GetAxis(const Quaternion& quat)
{
    return Normalised(Vector3(quat.x, quat.y, quat.z));
}

float GetAngle(const Quaternion& quat)
{
    return 2.0f * acosf(quat.w);
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
    return Quaternion
    (
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    );
}

Quaternion operator-(const Quaternion& a, const Quaternion& b)
{
    return Quaternion
    (
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    );
}

Quaternion operator*(const Quaternion& a, float b)
{
    return Quaternion
    (
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b
    );
}

Quaternion operator-(const Quaternion& q)
{
    return Quaternion
    (
        -q.x,
        -q.y,
        -q.z,
        -q.w
    );
}

bool operator==(const Quaternion& left, const Quaternion& right)
{
    return (fabsf(left.x - right.x) <= EPSILON_QUATERNION && fabsf(left.y - right.y) <= EPSILON_QUATERNION && fabsf(left.z - right.z) <= EPSILON_QUATERNION && fabsf(left.w - left.w) <= EPSILON_QUATERNION);
}

bool operator!=(const Quaternion& a, const Quaternion& b)
{
    return !(a == b);
}

bool SameOrientation(const Quaternion& left, const Quaternion& right)
{
    return (fabsf(left.x - right.x) <= EPSILON_QUATERNION && fabsf(left.y - right.y) <= EPSILON_QUATERNION && fabsf(left.z - right.z) <= EPSILON_QUATERNION && fabsf(left.w - left.w) <= EPSILON_QUATERNION)
        || (fabsf(left.x + right.x) <= EPSILON_QUATERNION && fabsf(left.y + right.y) <= EPSILON_QUATERNION && fabsf(left.z + right.z) <= EPSILON_QUATERNION && fabsf(left.w + left.w) <= EPSILON_QUATERNION);
}

float Dot(const Quaternion& a, const Quaternion& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float LengthSq(const Quaternion& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float Length(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < EPSILON_QUATERNION) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void Normalise(Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < EPSILON_QUATERNION) {
        return;
    }
    float invLength = 1.0f / sqrtf(lenSq);

    q.x *= invLength;
    q.y *= invLength;
    q.z *= invLength;
    q.w *= invLength;
}

Quaternion Normalised(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < EPSILON_QUATERNION) {
        return;
    }
    float invLength = 1.0f / sqrtf(lenSq);

    return Quaternion
    (
        q.x * invLength,
        q.y * invLength,
        q.z * invLength,
        q.w * invLength
    );    
}

Quaternion Conjugate(const Quaternion& q)
{
    return Quaternion
    (
        -q.x,
        -q.y,
        -q.z,
        q.w
    );
}

Quaternion Inverse(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < EPSILON_QUATERNION) {
        return Quaternion();
    }
    float invLenSq = 1.0f / lenSq;

    // conjugate / norm
    return Quaternion
    (
        -q.x * invLenSq,
        -q.y * invLenSq,
        -q.z * invLenSq,
        q.w * invLenSq
    );
}

Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2)
{
    return Quaternion
    (
        Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
        -Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
        Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
        -Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w
    );
}

Vector3 operator*(const Quaternion& q, const Vector3& v)
{
    return    q.vector * 2.0f * Dot(q.vector, v) +
        v * (q.scalar * q.scalar - Dot(q.vector, q.vector)) +
        Cross(q.vector, v) * 2.0f * q.scalar;
}

Quaternion Mix(const Quaternion& from, const Quaternion& to, float t)
{
    return from * (1.0f - t) + to * t;
}

Quaternion NLerp(const Quaternion& from, const Quaternion& to, float t)
{
    return Normalised(from + (to - from) * t);
}

Quaternion operator^(const Quaternion& q, float f)
{
    float angle = 2.0f * acosf(q.scalar);
    Vector3 axis = Normalised(q.vector);

    float halfCos = cosf(f * angle * 0.5f);
    float halfSin = sinf(f * angle * 0.5f);

    return Quaternion
    (
        axis.x * halfSin,
        axis.y * halfSin,
        axis.z * halfSin,
        halfCos
    );
}

Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t)
{
    if (fabsf(Dot(start, end)) > 1.0f - EPSILON_QUATERNION) {
        return NLerp(start, end, t);
    }

    return Normalised(((Inverse(start) * end) ^ t) * start);
}

Quaternion LookRotation(const Vector3& direcion, const Vector3& up)
{
    // Find orthonormal basis vectors
    Vector3 f = Normalised(direcion);
    Vector3 u = Normalised(up);
    Vector3 r = Cross(u, f);
    u = Cross(f, r);

    // From world forward to object forward
    Quaternion f2d = FromTo(Vector3(0, 0, 1), f);

    // what direction is the new object up?
    Vector3 objectUp = f2d * Vector3(0, 1, 0);
    // From object up to desired up
    Quaternion u2u = FromTo(objectUp, u);

    // Rotate to forward direction first, then twist to correct up
    Quaternion result = f2d * u2u;
    // Don’t forget to normalize the result
    return Normalised(result);
}

Matrix44 QuaternionToMatrix44(const Quaternion& q)
{
    Vector3 r = q * Vector3(1, 0, 0);
    Vector3 u = q * Vector3(0, 1, 0);
    Vector3 f = q * Vector3(0, 0, 1);

    return Matrix44
    (
        r.x, r.y, r.z, 0,
        u.x, u.y, u.z, 0,
        f.x, f.y, f.z, 0,
        0, 0, 0, 1
    );
}

Quaternion Matrix44ToQuaternion(const Matrix44& m)
{
    Vector3 up = Normalised(Vector3(m.up.x, m.up.y, m.up.z));
    Vector3 forward = Normalised(Vector3(m.forward.x, m.forward.y, m.forward.z));
    Vector3 right = Cross(up, forward);
    up = Cross(forward, right);

    return LookRotation(forward, up);
}
