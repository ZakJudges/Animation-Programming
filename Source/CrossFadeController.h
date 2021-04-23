#ifndef _H_CROSS_FADE_CONTROLLER
#define _H_CROSS_FADE_CONTROLLER

#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include <vector>

struct CrossFadeTarget
{
	Pose m_pose;
	Clip* m_clip;
	float m_time;
	float m_duration;
	float m_elapsed;
	inline CrossFadeTarget() : m_clip(0), m_time(0.0f), m_duration(0.0f), m_elapsed(0.0f) {}
	inline CrossFadeTarget(Clip* target, Pose& pose, float duration) 
		: m_clip(target), m_time(target->GetStartTime()), m_pose(pose), m_duration(duration), m_elapsed(0.0f) {}
};

//	Fast blending from one animation to another.
class CrossFadeController
{
public:
	CrossFadeController();
	CrossFadeController(Skeleton& skeleton);
	void SetSkeleton(Skeleton& skeleton);
	void Play(Clip* target);
	void FadeTo(Clip* target, float fadeTime);
	void Update(float dt);
	Pose& GetCurrentPose();
	Clip* GetCurrentClip();
protected:
	std::vector<CrossFadeTarget> m_targets;
	Clip* m_clip;
	float m_time;
	Pose m_pose;
	Skeleton m_skeleton;
	bool m_validSkeleton;
};

#endif
