#ifndef _H_TRACK
#define _H_TRACK

#include <vector>
#include "Frame.h"
#include "Interpolation.h"
#include "Vector3.h"
#include "Quaternion.h"

template<typename T, unsigned int N>
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
	int GetFrameIndex(float time, bool isLooping);
	float FitTimeToTrack(float time, bool isLooping);
	T Cast(float* values);
protected:
	std::vector<Frame<N>> m_frames;
	Interpolation m_interpolationType;
};

typedef Track<float, 1> ScalarTrack;
typedef Track<Vector3, 3> VectorTrack;
typedef Track<Quaternion, 4> QuaternionTrack;

#endif