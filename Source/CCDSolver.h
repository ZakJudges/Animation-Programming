#ifndef _H_CCD_SOLVER
#define _H_CCD_SOLVER

#include "Transform.h"
#include <vector>

//	Solve a single IK chain that has one origin and one end effector.
class CCDSolver
{
public:
	CCDSolver();
	unsigned int GetSize();
	void Resize(unsigned int newSize);
	Transform& operator[](unsigned int index);
	Transform GetGlobalTransform(unsigned int index);
	unsigned int GetStepCount();
	void SetStepCount(unsigned int stepCount);
	float GetThreshold();
	void SetThreshold(float value);
	virtual bool Solve(const Transform& target);
protected:
	std::vector<Transform> m_chain;
	unsigned int m_stepCount;
	float m_threshold;
};

#endif
