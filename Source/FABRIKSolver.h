#ifndef _H_FABRIK_SOLVER
#define _H_FABRIK_SOLVER

#include "Transform.h"
#include "Vector3.h"
#include <vector>

class FABRIKSolver
{
public:
	FABRIKSolver();
	unsigned int GetSize();
	void Resize(unsigned int newSize);
	unsigned int GetStepCount();
	void SetStepCount(unsigned int stepCount);
	float GetThreshold();
	void SetThreshold(float value);
	Transform GetLocalTransform(unsigned int index);
	void SetLocalTransform(unsigned int index, const Transform& transform);
	Transform GetGlobalTransform(unsigned int index);
	bool Solve(const Transform& target);
private:
	void IKChainToWorld();
	void IterateForward(const Vector3& base);
	void IterateBackward(const Vector3& goal);
	void WorldToIKChain();
private:
	std::vector<Transform> m_chain;
	std::vector<Vector3> m_worldChain;
	std::vector<float> m_lengths;
	unsigned int m_stepCount;
	float m_threshold;
};

#endif