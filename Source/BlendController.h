#ifndef _H_BLEND_CONTROLLER
#define _H_BLEND_CONTROLLER

#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"

class BlendController
{
public:
	static bool IsInHierarchy(Pose& pose, unsigned int parent, unsigned int search);
	static void Blend(Pose& output, Pose& a, Pose& b, float t, int blendRoot);

private:
	BlendController();
};

#endif