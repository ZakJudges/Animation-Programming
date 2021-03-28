#include "Vector3.h"

#include <cmath>

Vector3 operator+(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 operator-(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3 operator*(const Vector3& vector, float scale)
{
    return Vector3(vector.x * scale, vector.y * scale, vector.z * scale);
}

Vector3 operator*(const Vector3& left, const Vector3& right)
{
    return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}

float Dot(const Vector3& left, const Vector3& right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

float LengthSq(const Vector3& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

float Length(const Vector3& vector)
{
    float lengthSq = LengthSq(vector);
    if (lengthSq < VECTOR_EPSILON)
    {
        return 0.0f;
    }

    return sqrtf(lengthSq);
}

void Normalise(Vector3& vector)
{
    float lengthSq = LengthSq(vector);
    if (lengthSq < VECTOR_EPSILON)
    {
        return;
    }
    float lengthInverse = 1.0f / sqrtf(lengthSq);

    vector.x *= lengthInverse;
    vector.y *= lengthInverse;
    vector.z *= lengthInverse;
}

Vector3 Normalised(Vector3& vector)
{
    float lengthSq = LengthSq(vector);
    if (lengthSq < VECTOR_EPSILON)
    {
        return vector;
    }
    float lengthInverse = 1.0f / sqrtf(lengthSq);

    return Vector3(vector.x * lengthInverse, vector.y * lengthInverse, vector.z * lengthInverse);
}

float Angle(const Vector3& left, const Vector3& right)
{
    float dot = Dot(left, right);
    float lengthLeft = Length(left);
    float lengthRight = Length(right);

    return acosf(dot / (lengthLeft * lengthRight));
}
    
//  Assumes that left and right are already normalised.
float AngleNormalised(const Vector3& left, const Vector3& right)
{
    float dot = Dot(left, right);

    return acosf(dot);
}

//  Project vector a onto vector b. 
//      Result is a vector with length of a, in direction b.
//          i.e. the point that lies on the line b, closes to a.
Vector3 Project(const Vector3& a, const Vector3& b)
{
    float lengthSqB = LengthSq(b);
    if (lengthSqB < VECTOR_EPSILON)
    {
        return Vector3();
    }

    float scale = Dot(a, b) / lengthSqB;

    return b * scale;
}

Vector3 Reject(const Vector3& a, const Vector3& b)
{
    Vector3 projection = Project(a, b);

    return a - projection;
}

Vector3 Cross(const Vector3& left , const Vector3& right)
{
    return Vector3(left.y * right.z - left.z * right.y, 
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x);
}

Vector3 Lerp(const Vector3& a, const Vector3& b, const float t)
{
    return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

Vector3 Slerp(Vector3& a, Vector3& b, const float t)
{
    if (t < 0.01f) 
    {
        return Lerp(a, b, t);
    }

    Vector3 from = Normalised(a);
    Vector3 to = Normalised(b);

    float theta = AngleNormalised(from, to);
    float sinTheta = sinf(theta);

    float startAngle = sinf((1.0f - t) * theta) / sinTheta;
    float endAngle = sinf(t * theta) / sinTheta;

    return from * startAngle + to * endAngle;
}

Vector3 NLerp(const Vector3& a, const Vector3& b, const float t)
{
    Vector3 lerp = Lerp(a, b, t);

    return Normalised(lerp);
}

bool operator==(const Vector3& left, const Vector3& right)
{
    Vector3 difference(left - right);

    if (LengthSq(difference) < VECTOR_EPSILON)
    {
        return true;
    }

    return false;
}

bool operator!=(const Vector3& left, const Vector3& right)
{
    return !(left == right);
}
