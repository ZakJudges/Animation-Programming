#ifndef _H_TRANSFORM
#define _H_TRANSFORM

#include "Vector3.h"
#include "Matrix44.h"
#include "Quaternion.h"

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	inline Transform() : scale(Vector3(1, 1, 1)) {}
	inline Transform(const Vector3& pos, const Quaternion& rot, const Vector3& s)
		: position(pos), rotation(rot), scale(s) {}
};

Transform Combine(const Transform& a, const Transform& b);
Transform Inverse(const Transform& t);
Transform Mix(const Transform& a, const Transform& b, float t);
bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);
Matrix44 TransformToMatrix44(const Transform& t);
Transform Matrix44ToTransform(const Matrix44& m);
Vector3 TransformPoint(const Transform& a, const Vector3& b);
Vector3 TransformVector(const Transform& a, const Vector3& b);


#endif