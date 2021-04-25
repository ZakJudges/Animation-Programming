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

//	Not to be called during run-time.
Pose BlendController::MakeAdditivePose(Skeleton& skeleton, Clip& clip)
{
	//	Getting rest pose makes the result the correct size.
	Pose result = skeleton.GetRestPose();

	//	Set the pose as the sampled pose from the start of the clip.
	//	Result is the additive base pose.
	//	When adding poses, there is now a reference for how much each bone has been transformed in this clip, for any time.
	clip.Sample(result, clip.GetStartTime());

	return result;
}

void BlendController::Add(Pose& output, Pose& inPose, Pose& addPose, Pose& additiveBasePose, int blendRoot)
{
	
	unsigned int numJoints = addPose.GetSize();

	for (int i = 0; i < numJoints; ++i)
	{
		if (blendRoot >= 0 && !IsInHierarchy(addPose, blendRoot, i))
		{
			continue;
		}

		Transform input = inPose.GetLocalTransform(i);

		Transform additive = addPose.GetLocalTransform(i);

		Transform additiveBase = additiveBasePose.GetLocalTransform(i);

		//result pose = input pose + (additive pose = additive base pose)
		Transform result(input.position + (additive.position - additiveBase.position), 
			Normalised(input.rotation * (Inverse(additiveBase.rotation) * additive.rotation)), 
			input.scale + (additive.scale - additiveBase.scale));
		output.SetLocalTransform(i, result);
		
	}
}
