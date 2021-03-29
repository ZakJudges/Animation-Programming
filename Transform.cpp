#include "Transform.h"
#include <cmath>

Transform Combine(const Transform& a, const Transform& b)
{
    Transform output;

    output.scale = a.scale * b.scale;
    output.rotation = b.rotation * a.rotation;

    output.position = a.rotation * (a.scale * b.position);
    output.position = a.position + output.position;

    return output;
}

Transform Inverse(const Transform& t)
{
    Transform inv;

    inv.rotation = Inverse(t.rotation);

    inv.scale.x = fabs(t.scale.x) < VECTOR_EPSILON ? 0.0f : 1.0f / t.scale.x;
    inv.scale.y = fabs(t.scale.y) < VECTOR_EPSILON ? 0.0f : 1.0f / t.scale.y;
    inv.scale.z = fabs(t.scale.z) < VECTOR_EPSILON ? 0.0f : 1.0f / t.scale.z;

    Vector3 invTranslation = t.position * -1.0f;
    inv.position = inv.rotation * (inv.scale * invTranslation);

    return inv;
}

Transform Mix(const Transform& a, const Transform& b, float t)
{
    Quaternion bRot = b.rotation;
    if (Dot(a.rotation, bRot) < 0.0f) {
        bRot = -bRot;
    }
    return Transform
    (
        Lerp(a.position, b.position, t),
        NLerp(a.rotation, bRot, t),
        Lerp(a.scale, b.scale, t)
    );
}

bool operator==(const Transform& a, const Transform& b)
{
    return a.position == b.position &&
        a.rotation == b.rotation &&
        a.scale == b.scale;
}

bool operator!=(const Transform& a, const Transform& b)
{
    return !(a == b);
}

Matrix44 TransformToMatrix44(const Transform& t)
{
    // First, extract the rotation basis of the transform
    Vector3 x = t.rotation * Vector3(1, 0, 0);
    Vector3 y = t.rotation * Vector3(0, 1, 0);
    Vector3 z = t.rotation * Vector3(0, 0, 1);

    // Next, scale the basis vectors
    x = x * t.scale.x;
    y = y * t.scale.y;
    z = z * t.scale.z;

    // Extract the position of the transform
    Vector3 p = t.position;

    // Create matrix
    return Matrix44
    (
        x.x, x.y, x.z, 0, // X basis (& Scale)
        y.x, y.y, y.z, 0, // Y basis (& scale)
        z.x, z.y, z.z, 0, // Z basis (& scale)
        p.x, p.y, p.z, 1  // Position
    );
}

Transform Matrix44ToTransform(const Matrix44& m)
{
    Transform output;

    //  Extract position.
    output.position = Vector3(m.v[12], m.v[13], m.v[14]);

    //  Extract rotation.
    output.rotation = Matrix44ToQuaternion(m);

    //  Rotation matrix with no translation.
    Matrix44 rotScaleMat
    (
        m.v[0], m.v[1], m.v[2], 0,
        m.v[4], m.v[5], m.v[6], 0,
        m.v[8], m.v[9], m.v[10], 0,
        0, 0, 0, 1
    );
    Matrix44 invRotMat = QuaternionToMatrix44(Inverse(output.rotation));
    Matrix44 scaleSkewMat = rotScaleMat * invRotMat;

    //  Extract scale.
    output.scale = Vector3(
        scaleSkewMat.v[0],
        scaleSkewMat.v[5],
        scaleSkewMat.v[10]
    );

    return output;
}

Vector3 TransformPoint(const Transform& a, const Vector3& b)
{
    Vector3 output;

    output = a.rotation * (a.scale * b);
    output = a.position + output;

    return output;
}

Vector3 TransformVector(const Transform& a, const Vector3& b)
{
    Vector3 output;

    output = a.rotation * (a.scale * b);

    return output;
}
