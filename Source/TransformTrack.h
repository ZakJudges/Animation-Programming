#ifndef _H_TRANSFORM_TRACK
#define _H_TRANSFORM_TRACK

#include "Track.h"
#include "Transform.h"

template <typename VTRACK, typename QTRACK>
class TTransformTrack
{
public:
	TTransformTrack();
	unsigned int GetBone();
	void SetBone(unsigned int bone);
	VTRACK& GetPositionTrack();
	QTRACK& GetRotationTrack();
	VTRACK& GetScaleTrack();
	float GetStartTime();
	float GetEndTime();
	bool IsValid();		
	Transform Sample(const Transform& ref, float time, bool isLooping);

protected:
	unsigned int m_bone;
	VTRACK m_position;
	QTRACK m_rotation;
	VTRACK m_scale;
};

typedef TTransformTrack<VectorTrack, QuaternionTrack> TransformTrack;
typedef TTransformTrack<VectorFastTrack, QuaternionFastTrack> FastTransformTrack;

FastTransformTrack OptimiseTransformTrack(TransformTrack& input);

#endif
