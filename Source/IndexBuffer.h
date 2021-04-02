#ifndef _H_INDEX_BUFFER
#define _H_INDEX_BUFFER

#include <vector>

class IndexBuffer 
{
public:
	IndexBuffer();
	~IndexBuffer();
	void Set(unsigned int* inputArray, unsigned int arrayLengt);
	void Set(std::vector<unsigned int>& input);
	unsigned int Count();
	unsigned int GetHandle();

private:
	IndexBuffer(const IndexBuffer& other);
	IndexBuffer& operator=(const IndexBuffer& other);

private:
	unsigned int m_handle;
	unsigned int m_count;
};


#endif
