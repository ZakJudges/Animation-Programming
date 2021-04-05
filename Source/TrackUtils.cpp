#include "TrackUtils.h"

float TrackUtils::Interpolate(float a, float b, float time)
{
    return a + (b - a) * time;
}

Vector3 TrackUtils::Interpolate(const Vector3& a, const Vector3& b, float time)
{
    return Lerp(a, b, time);
}

Quaternion TrackUtils::Interpolate(const Quaternion& a, const Quaternion& b, float time)
{
    Quaternion result = Mix(a, b, time);

    //  Check result for correct neighbourhood:
    if (Dot(a, b) < 0.0f)
    {
        result = Mix(a, -b, time);
    }

    return Normalised(result);
}

float TrackUtils::AdjustHermiteResult(float result)
{
    return result;
}

Vector3 TrackUtils::AdjustHermiteResult(const Vector3& result)
{
    return result;
}

//  Interpolating quaternions requires normalisation.
Quaternion TrackUtils::AdjustHermiteResult(const Quaternion& result)
{
    return Normalised(result);
}

void TrackUtils::Neighbourhood(const float a, const float b)
{
}

void TrackUtils::Neighbourhood(const Vector3& a, const Vector3& b)
{
}

void TrackUtils::Neighbourhood(const Quaternion& a, Quaternion& b)
{
    if (Dot(a, b) < 0)
    {
        //  a and b are in different neighbourhood, so flip b.
        b = -b;
    }
}
