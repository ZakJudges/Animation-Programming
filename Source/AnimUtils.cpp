#include "AnimUtils.h"
#include <vector>
#include <list>

BoneMap AnimUtils::RearrangeSkeleton(Skeleton& skeleton)
{
	Pose& restPose = skeleton.GetRestPose();
	Pose& bindPose = skeleton.GetBindPose();
	unsigned int size = restPose.GetSize();

	if (size == 0)
	{
		return BoneMap();
	}

	//	Outer vector represents each bone in the hierarchy. 
	//	Inner vector contains joint indices for all of the child joints of the bone.
	std::vector<std::vector<int>> hierarchy(size);

	std::list<int> process;

	//	Loop through each bone in the skeleton.
	for (unsigned int i = 0; i < size; ++i)
	{
		int parent = restPose.GetParent(i);

		//	Root joint reached when parent index -1.
		if (parent >= 0)
		{
			hierarchy[parent].push_back((int)i);
		}
		else
		{
			process.push_back((int)i);
		}
	}

	//	Map old configuration to new one.
	BoneMap mapForward;
	//	Map new configuration to old one.
	BoneMap mapBackward;

	int index = 0;
	while (process.size() > 0) 
	{
		int current = *process.begin();
		process.pop_front();
		std::vector<int>& children = hierarchy[current];

		unsigned int numChildren = (unsigned int)children.size();
		for (unsigned int i = 0; i < numChildren; ++i) 
		{
			process.push_back(children[i]);
		}

		mapForward[index] = current;
		mapBackward[current] = index;
		index += 1;
	}

	mapForward[-1] = -1;
	mapBackward[-1] = -1;

	Pose newRestPose(size);
	Pose newBindPose(size);
	std::vector<std::string> newNames(size);

	for (unsigned int i = 0; i < size; ++i) 
	{
		int thisBone = mapForward[i];
		newRestPose.SetLocalTransform(i, restPose.GetLocalTransform(thisBone));
		newBindPose.SetLocalTransform(i, bindPose.GetLocalTransform(thisBone));
		newNames[i] = skeleton.GetJointName(thisBone);

		int parent = mapBackward[bindPose.GetParent(thisBone)];
		newRestPose.SetParent(i, parent);
		newBindPose.SetParent(i, parent);
	}

	skeleton.Set(newRestPose, newBindPose, newNames);
	return mapBackward;
}

void AnimUtils::RearrangeMesh(SkinnedMesh& mesh, BoneMap& boneMap)
{
	std::vector<Vector4I>& influences = mesh.GetInfluences();
	unsigned int size = (unsigned int)influences.size();

	for (unsigned int i = 0; i < size; ++i) 
	{
		influences[i].x = boneMap[influences[i].x];
		influences[i].y = boneMap[influences[i].y];
		influences[i].z = boneMap[influences[i].z];
		influences[i].w = boneMap[influences[i].w];
	}

	mesh.UpdateOpenGLBuffers();
}

void AnimUtils::RearrangeClip(Clip& clip, BoneMap& boneMap)
{
	unsigned int size = clip.GetSize();

	for (unsigned int i = 0; i < size; ++i) 
	{
		int joint = (int)clip.GetBoneAtIndex(i);
		unsigned int newJoint = (unsigned int)boneMap[joint];
		clip.SetBoneAtIndex(i, newJoint);
	}
}