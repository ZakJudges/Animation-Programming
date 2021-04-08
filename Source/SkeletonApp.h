#ifndef H_SKELETON_APP
#define H_SKELETON_APP

#include "Application.h"
#include "Maths.h"
#include "DebugLines.h"
#include "Pose.h"
#include "Clip.h"


class SkeletonApp : public Application
{
public:
	SkeletonApp();
	virtual ~SkeletonApp();
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(float aspectRatio) override;
	virtual void Shutdown() override;

private:
	Pose m_restPose;
	Pose m_currentPose;
	std::vector<Clip> m_clips;
	unsigned int m_currentClip;
	float m_playbackTime;
	DebugDraw* m_restPoseVis;
	DebugDraw* m_currentPoseVis;
};
#endif
