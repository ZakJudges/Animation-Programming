#ifndef _H_VECTOR4_
#define _H_VECTOR4_

template<typename T>
struct Vector4 {
	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};
		T v[4];
	};
	inline Vector4<T>() : x((T)0), y((T)0), z((T)0), w((T)0) { }
	inline Vector4<T>(T _x, T _y, T _z, T _w) :
		x(_x), y(_y), z(_z), w(_w) { }
	inline Vector4<T>(T* fv) :
		x(fv[0]), y(fv[1]), z(fv[2]), w(fv[3]) { }
};

typedef Vector4<float> Vector4F;
typedef Vector4<int> Vector4I;
typedef Vector4<unsigned int> Vector4UI;

#endif
