#include "BlendController.h"

//	Check if one joint is a descendant of another joint.
bool BlendController::IsInHierarchy(Pose& pose, unsigned int blendRoot, unsigned int search)
{
	//	The desired joint is the blend root.
	if (search == blendRoot)
	{
		return true;
	}

	//	Ascend up the hierarchy from the parent of the desired joint.
	//	If we hit the blend root then the joint is in the hierarchy.
	int p = pose.GetParent(search);
	while (p >= 0)
	{
		if (p == (int)blendRoot)
		{
			return true;
		}

		p = pose.GetParent(p);
	}

	return false;
}

//	Combine two poses, blending all joints that descend from blendRoot.
void BlendController::Blend(Pose& pose, Pose& a, Pose& b, float t, int blendRoot)
{
	unsigned int numJoints = pose.GetSize();
	for (unsigned int i = 0; i < numJoints; ++i)
	{
		//	Skip hierarchy check when the blend root is negative, since the full pose will be blended.
		if (blendRoot >= 0)
		{
			if (!IsInHierarchy(pose, blendRoot, i))
			{
				continue;
			}
		}

		pose.SetLocalTransform(i, Mix(a.GetLocalTransform(i), b.GetLocalTransform(i), t));
	}
}