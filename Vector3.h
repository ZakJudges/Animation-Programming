#ifndef _H_VECTOR3_
#define _H_VECTOR3_

#define VECTOR_EPSILON 0.000001f

struct Vector3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float v[3];
	};

	inline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	inline Vector3(float* arrFloat) : x(arrFloat[0]), y(arrFloat[1]), z(arrFloat[2]) {}
};

Vector3 operator+ (const Vector3& left, const Vector3& right);
Vector3 operator- (const Vector3& left, const Vector3& right);
Vector3 operator* (const Vector3& vector, float scale);
Vector3 operator* (const Vector3& left, const Vector3& right);
float Dot(const Vector3& left, const Vector3& right);
float LengthSq(const Vector3& vector);
float Length(const Vector3& vector);
void Normalise(Vector3& vector);
Vector3 Normalised(Vector3& vector);
float Angle(const Vector3& left, const Vector3& right);
float AngleNormalised(const Vector3& left, const Vector3& right);
Vector3 Project(const Vector3& a, const Vector3& b);
Vector3 Reject(const Vector3& a, const Vector3& b);
Vector3 Cross(const Vector3& a, const Vector3& b);
Vector3 Lerp(const Vector3& a, const Vector3& b, const float t);
Vector3 Slerp(Vector3& a, Vector3& b, const float t);
Vector3 NLerp(const Vector3& a, const Vector3& b, const float t);
bool operator== (const Vector3& left, const Vector3& right);
bool operator!= (const Vector3& left, const Vector3& right);


#endif
