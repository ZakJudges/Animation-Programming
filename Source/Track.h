#ifndef _H_TRACK
#define _H_TRACK

#include <vector>
#include "Frame.h"
#include "Interpolation.h"
#include "Maths.h"

//	Represents animation data for the transform component per bone.
//	i.e. positions over time for one bone, rotations over time, or scale over time.
//		Must have at least two frames of data to be valid.
template<typename T, int N>
class Track
{
public:
	Track();
	void Resize(unsigned int size);
	unsigned int GetSize();
	Interpolation GetInterpolationType();
	void SetInterpolationType(Interpolation interpolationType);
	float GetStartTime();
	float GetEndTime();
	T Sample(float time, bool isLooping);
	Frame<N>& operator[](unsigned int index);
protected:
	T SampleConstant(float time, bool isLooping);
	T SampleLinear(float time, bool isLooping);
	T SampleCubic(float time, bool isLooping);
	T EvaluateHermite(float t, const T& point1, const T& slope1, const T& point2, const T& slope2);
	virtual int GetFrameIndex(float time, bool isLooping);
	float FitTimeToTrack(float time, bool isLooping);
	T Cast(float* values);
protected:
	std::vector<Frame<N>> m_frames;
	Interpolation m_interpolationType;


};

typedef Track<float, 1> ScalarTrack;
typedef Track<Vector3, 3> VectorTrack;
typedef Track<Quaternion, 4> QuaternionTrack;
//
////	Fast track is sampled using a lookup table.
////	Results in GetFrameIndex() having constant complexity, and is more suited to longer animation clips/higher frame count.
//template <typename T, int N>
//class FastTrack : public Track<T, N>
//{
//public:
//	FastTrack();
//	FastTrack(unsigned int sampleFrequency = 60);
//	void UpdateIndexLookupTable();
//	//FastTrack<T, N> OptimiseTrack(Track<T, N>& track);
//protected:
//	virtual int GetFrameIndex(float time, bool isLooping) override;
//protected:
//	std::vector<unsigned int> m_sampledFrames;
//	unsigned int m_sampleFrequency;
//};
//
//typedef FastTrack<float, 1> ScalarFastTrack;
//typedef FastTrack<Vector3, 3> VectorFastTrack;
//typedef FastTrack<Quaternion, 4> QuaternionFastTrack;
//

#endif
