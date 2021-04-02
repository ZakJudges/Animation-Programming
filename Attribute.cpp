#include "Attribute.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "glad.h"

template Attribute<int>;
template Attribute<float>;
template Attribute<Vector2F>;
template Attribute<Vector3>;
template Attribute<Vector4F>;
template Attribute<Vector4I>;
template Attribute<Quaternion>;

template<typename T>
Attribute<T>::Attribute()
{
	//	Generate an OpenGL buffer and store the handle.
	glGenBuffers(1, &m_handle);
	m_count = 0;
}

template<typename T>
Attribute<T>::~Attribute()
{
	glDeleteBuffers(1, &m_handle);
}

template<typename T>
void Attribute<T>::Set(T* inputArray, unsigned int arrayLength)
{
	m_count = arrayLength;
	unsigned int size = sizeof(T);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size * m_count, inputArray, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::Set(std::vector<T>& input)
{
	Set(&input[0], (unsigned int)input.size());
}

template<typename T>
unsigned int Attribute<T>::Count()
{
	//	The total number of attributes there are.
	return m_count;
}

template<typename T>
unsigned int Attribute<T>::GetHandle()
{
	return m_handle;
}

template<typename T>
void Attribute<T>::BindTo(unsigned int slot)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glEnableVertexAttribArray(slot);
	SetAttribPointer(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void Attribute<T>::UnbindFrom(unsigned int slot)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glDisableVertexAttribArray(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<>
void Attribute<int>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribIPointer(slot, 1, GL_INT, 0, (void*)0);
}

template<>
void Attribute<Vector4I>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribIPointer(slot, 4, GL_INT, 0, (void*)0);
}

template<>
void Attribute<float>::SetAttribPointer(unsigned int slot) 
{
	glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
}


template<>
void Attribute<Vector2F>::SetAttribPointer(unsigned int slot) {
	glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void Attribute<Vector3>::SetAttribPointer(unsigned int slot) {
	glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void Attribute<Vector4F>::SetAttribPointer(unsigned int slot) {
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void Attribute<Quaternion>::SetAttribPointer(unsigned int slot) {
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}