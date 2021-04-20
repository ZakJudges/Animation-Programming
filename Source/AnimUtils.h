#ifndef _H_ANIM_UTILS
#define _H_ANIM_UTILs

#include "Skeleton.h"
#include "SkinnedMesh.h"
#include "Clip.h"
#include <map>

typedef std::map<int, int> BoneMap;

class AnimUtils
{
public:
	static BoneMap RearrangeSkeleton(Skeleton& skeleton);
	static void RearrangeMesh(SkinnedMesh& mesh, BoneMap& boneMap);
	static void RearrangeClip(Clip& clip, BoneMap& boneMap);
};

#endif