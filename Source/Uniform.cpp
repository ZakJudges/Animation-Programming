#include "Uniform.h"
#include "Maths.h"
#include "glad.h"

template Uniform<int>;
template Uniform<Vector4I>;
template Uniform<Vector2I>;
template Uniform<float>;
template Uniform<Vector2F>;
template Uniform<Vector3>;
template Uniform<Vector4F>;
template Uniform<Quaternion>;
template Uniform<Matrix44>;

#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> \
void Uniform<tType>::Set(unsigned int slot, tType* data, unsigned int length) { \
    gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
}

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, Vector4I, int)
UNIFORM_IMPL(glUniform2iv, Vector2I, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, Vector2F, float)
UNIFORM_IMPL(glUniform3fv, Vector3, float)
UNIFORM_IMPL(glUniform4fv, Vector4F, float)
UNIFORM_IMPL(glUniform4fv, Quaternion, float)

template<>
void Uniform<Matrix44>::Set(unsigned int slot, Matrix44* inputArray, unsigned int arrayLength) {
	glUniformMatrix4fv(slot, (GLsizei)arrayLength, false, (float*)&inputArray[0]);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, const T& value)
{
	Set(slot, (T*)&value, 1);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, std::vector<T>& values)
{
	Set(slot, &values[0], (unsigned int)values.size());
}

