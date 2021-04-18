#ifndef _H_CLIP
#define _H_CLIP

#include <vector>
#include <string>
#include "TransformTrack.h"
#include "Pose.h"

//	Collection of animation tracks for each joint.
//		Always modifies the same joints.
template<typename TRACK>
class TClip
{
public:
	TClip();
	unsigned int GetBoneAtIndex(unsigned int index);
	void SetBoneAtIndex(unsigned int index, unsigned int bone);
	unsigned int GetSize(); 
	float Sample(Pose& output, float time);
	TRACK& operator[](unsigned int bone);
	void RecalculateDuration();
	std::string& GetName();
	void SetName(const std::string& name);
	float GetDuration();
	float GetStartTime();
	float GetEndTime();
	bool GetLooping();
	void SetLooping(bool isLooping);
protected:
	float FitTimeToClip(float time);
protected:
	std::vector<TRACK> m_tracks;
	std::string m_name;
	float m_startTime;
	float m_endTime;
	bool m_looping;
};

typedef TClip<TransformTrack> Clip;
typedef TClip<FastTransformTrack> FastClip;

FastClip OptimiseClip(Clip& input);

#endif