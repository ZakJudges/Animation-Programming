#ifndef _H_POSE
#define _H_POSE

#include <vector>
#include "Transform.h"
#include "Matrix44.h"

//	Holds the transformation of every bone at a point in time during an animation.
class Pose
{
public:
	Pose();
	Pose(const Pose& pose);
	Pose& operator=(const Pose& pose);
	Pose(unsigned int jointCount);
	void Resize(unsigned int size);
	unsigned int GetSize();
	int GetParent(unsigned int index);
	void SetParent(unsigned int index, int parent);
	Transform GetLocalTransform(unsigned int index);
	void SetLocalTransform(unsigned int index, const Transform& transform);
	Transform GetGlobalTransform(unsigned int index);
	Transform operator[](unsigned int index);
	void GetMatrixPalette(std::vector<Matrix44>& out);
	bool operator==(const Pose& other);
	bool operator!=(const Pose& other);

protected:
	std::vector<Transform> m_joints;
	std::vector<int> m_parents;
};

#endif

