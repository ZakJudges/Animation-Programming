#ifndef _H_TRANSFORM_TRACK
#define _H_TRANSFORM_TRACK

#include "Track.h"
#include "Transform.h"

class TransformTrack
{
public:
	TransformTrack();
	unsigned int GetBone();
	void SetBone(unsigned int bone);
	VectorTrack& GetPositionTrack();
	QuaternionTrack& GetRotationTrack();
	VectorTrack& GetScaleTrack();
	float GetStartTime();
	float GetEndTime();
	bool IsValid();		
	Transform Sample(const Transform& ref, float time, bool isLooping);

protected:
	unsigned int m_bone;
	VectorTrack m_position;
	QuaternionTrack m_rotation;
	VectorTrack m_scale;
};

#endif
