#ifndef _H_TRACK_UTILS
#define _H_TRACK_UTILS

#include "Maths.h"

class TrackUtils
{
private:
	TrackUtils();
public:
	static float Interpolate(float a, float b, float time);
	static Vector3 Interpolate(const Vector3& a, const Vector3& b, float time);
	static Quaternion Interpolate(const Quaternion& a, const Quaternion& b, float time);

    static float AdjustHermiteResult(float result);
    static Vector3 AdjustHermiteResult(const Vector3& result);
    static Quaternion AdjustHermiteResult(const Quaternion& result);

    static void Neighbourhood(const float a, const float b);
    static void Neighbourhood(const Vector3& a, const Vector3& b);
    static void Neighbourhood(const Quaternion& a, Quaternion& b);
};

#endif
