#ifndef _H_ATTRIBUTE
#define _H_ATTRIBUTE

#include <vector>

template<typename T>
class Attribute
{
public:
	Attribute();
	~Attribute();
	void Set(T* inputArray, unsigned int arrayLength);
	void Set(std::vector<T>& input);
	void BindTo(unsigned int slot);
	void UnbindFrom(unsigned int slot);
	unsigned int Count();
	unsigned int GetHandle();
	
private:
	Attribute(const Attribute& other);
	Attribute& operator=(const Attribute& other) {}
	void SetAttribPointer(unsigned int slot);
protected:
	unsigned int m_handle;
	unsigned int m_count;

};

#endif