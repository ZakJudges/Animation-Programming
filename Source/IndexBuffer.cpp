#include "IndexBuffer.h"
#include "glad.h"

IndexBuffer::IndexBuffer() 
{
	glGenBuffers(1, &m_handle);
	m_count = 0;
}

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &m_handle);
}

unsigned int IndexBuffer::Count() 
{
	return m_count;
}

unsigned int IndexBuffer::GetHandle() 
{
	return m_handle;
}

void IndexBuffer::Set(unsigned int* inputArray, unsigned int arrayLengt) 
{
	m_count = arrayLengt;
	unsigned int size = sizeof(unsigned int);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * m_count, inputArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Set(std::vector<unsigned int>& input)
{
	Set(&input[0], (unsigned int)input.size());
}

