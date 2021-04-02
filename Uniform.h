#ifndef _H_UNIFORM
#define _H_UNIFORM

#include <vector>

//	Constant data. Set once for all vertices processed.
template <typename T>
class Uniform
{
public:
	static void Set(unsigned int slot, const T& value);
	static void Set(unsigned int slot, T* arr, unsigned int length);
	static void Set(unsigned int slot, std::vector<T>& arr);
private:
	//	Uninstantiable.
	Uniform();
	Uniform(const Uniform&);
	Uniform& operator=(const Uniform&) {}
	~Uniform() {}
};

#endif
