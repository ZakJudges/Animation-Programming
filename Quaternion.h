#ifndef _H_QUATERNION_
#define _H_QUATERNION_

#include "Vector3.h"
#include "Matrix44.h"

#define EPSILON_QUATERNION 0.000001f

struct Quaternion
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			Vector3 vector;
			float scalar;
		};
		float v[4];
	};

	inline Quaternion() : x(0), y(0), z(0), w(1) {}
	inline Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

Quaternion AngleAxis(float angle, const Vector3& axis);
Quaternion FromTo(const Vector3& from, const Vector3& to);
Vector3 GetAxis(const Quaternion& quat);
float GetAngle(const Quaternion& quat);
Quaternion operator+(const Quaternion& a, const Quaternion& b);
Quaternion operator-(const Quaternion& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, float b);
Quaternion operator-(const Quaternion& q);
bool operator==(const Quaternion& left, const Quaternion& right);
bool operator!=(const Quaternion& a, const Quaternion& b);
bool SameOrientation(const Quaternion& left, const Quaternion& right);
float Dot(const Quaternion& a, const Quaternion& b);
float LengthSq(const Quaternion& q);
float Length(const Quaternion& q);
void Normalise(Quaternion& q);
Quaternion Normalised(const Quaternion& q);
Quaternion Conjugate(const Quaternion& q);
Quaternion Inverse(const Quaternion& q);
Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2);
Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2);
Vector3 operator*(const Quaternion& q, const Vector3& v);
Quaternion Mix(const Quaternion& from, const Quaternion& to, float t);
Quaternion NLerp(const Quaternion& from, const Quaternion& to, float t);
Quaternion operator^(const Quaternion& q, float f);
Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t);
Quaternion LookRotation(const Vector3& direcion, const Vector3& up);
Matrix44 QuaternionToMatrix44(const Quaternion& q);
Quaternion Matrix44ToQuaternion(const Matrix44& m);

#endif