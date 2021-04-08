#ifndef _H_SKELETON
#define _H_SKELETON

#include <vector>
#include <string>
#include "Pose.h"
#include "Maths.h"

//	Represents a rig that can be reused accross multiple skinned meshes.
class Skeleton
{
public:
	Skeleton();
	Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);
	void Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);
	Pose& GetRestPose();
	Pose& GetBindPose();
	std::vector<Matrix44>& GetInvBindPose();
	std::vector<std::string>& GetJointNames();
	std::string& GetJointName(unsigned int index);
protected:
	void UpdateInverseBindPose();
protected:
	Pose m_restPose;
	Pose m_bindPose;
	std::vector<Matrix44> m_invBindPose;
	std::vector<std::string> m_jointNames;
};

#endif

