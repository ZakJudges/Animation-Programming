#ifndef _H_MATRIX44_
#define _H_MATRIX44_

#include "Vector3.h"
#include "Vector4.h"

#define MATRIX_EPSILON 0.000001f

struct Matrix44
{
	union
	{
		float v[16];
		struct
		{
			Vector4F right;
			Vector4F up;
			Vector4F forward;
			Vector4F position;
		};
		struct
		{
			float xx; float xy; float xz; float xw;
			float yx; float yy; float yz; float yw;
			float zx; float zy; float zz; float zw;
			float tx; float ty; float tz; float tw;
		};
		struct
		{
			float c0r0; float c0r1; float c0r2; float c0r3;
			float c1r0; float c1r1; float c1r2; float c1r3;
			float c2r0; float c2r1; float c2r2; float c2r3;
			float c3r0; float c3r1; float c3r2; float c3r3;
		};
		struct
		{
			float r0c0; float r1c0; float r2c0; float r3c0;
			float r0c1; float r1c1; float r2c1; float r3c1;
			float r0c2; float r1c2; float r2c2; float r3c2;
			float r0c3; float r1c3; float r2c3; float r3c3;
		};
	};

	inline Matrix44() : 
		xx(1), xy(0), xz(0), xw(0),
		yx(0), yy(1), yz(0), yw(0),
		zx(0), zy(0), zz(1), zw(0),
		tx(0), ty(0), tz(0), tw(1) {}

	inline Matrix44(float* fv) :
		xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
		yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
		zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
		tx(fv[12]), ty(fv[13]), tz(fv[14]), tw(fv[15]) {}

	inline Matrix44
		(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		xx(_00), xy(_01), xz(_02), xw(_03),
		yx(_10), yy(_11), yz(_12), yw(_13),
		zx(_20), zy(_21), zz(_22), zw(_23),
		tx(_30), ty(_31), tz(_32), tw(_33) { }
};

bool operator==(const Matrix44& a, const Matrix44& b);
bool operator !=(const Matrix44& a, const Matrix44& b);
Matrix44 operator*(const Matrix44& matrix, const float scale);
Matrix44 operator+(const Matrix44& a, const Matrix44& b);
Matrix44 operator*(const Matrix44& a, const Matrix44& b);
Vector4F operator*(const Matrix44& matrix, const Vector4F& vector);
Vector3 TransformVector(const Matrix44& m, const Vector3& v);
Vector3 TransformPoint(const Matrix44& m, const Vector3& v);
Vector3 TransformPoint(const Matrix44& m, const Vector3& v, float& w);
void Transpose(Matrix44& m);
Matrix44 Transposed(const Matrix44& m);
float Determinant(const Matrix44& m);
Matrix44 Adjugate(const Matrix44& m);
Matrix44 Inverse(const Matrix44& m);
void Invert(Matrix44& m);
Matrix44 Frustum(float l, float r, float b, float t, float n, float f);
Matrix44 Perspective(float fov, float aspect, float znear, float zfar);
Matrix44 Ortho(float l, float r, float b, float t, float n, float f);
Matrix44 LookAt(const Vector3& position, const Vector3& target, const Vector3& up);


#endif