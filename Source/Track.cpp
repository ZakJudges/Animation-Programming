#include "Track.h"
#include "TrackUtils.h"

template<typename T, unsigned int N>
Track<T, N>::Track()
{
	//	Linear interpolation by default.
	m_interpolationType = Interpolation::LINEAR;
}

template<typename T, unsigned int N>
void Track<T, N>::Resize(unsigned int size)
{
	m_frames.resize(size);
}

template<typename T, unsigned int N>
unsigned int Track<T, N>::GetSize()
{
	return m_frames.size();
}

template<typename T, unsigned int N>
Interpolation Track<T, N>::GetInterpolationType()
{
	return m_interpolationType;
}

template<typename T, unsigned int N>
void Track<T, N>::SetInterpolationType(Interpolation interpolationType)
{
	m_interpolationType = interpolationType;
}

template<typename T, unsigned int N>
float Track<T, N>::GetStartTime()
{
	return m_frames[0].m_time;
}

template<typename T, unsigned int N>
float Track<T, N>::GetEndTime()
{
	return m_frames[m_frames.size() - 1].m_time;
}

template<typename T, unsigned int N>
T Track<T, N>::Sample(float time, bool isLooping)
{
	if (m_interpolationType == Interpolation::CUBIC)
	{
		return SampleCubic(time, isLooping);
	}
	else if (m_interpolationType == Interpolation::CONSTANT)
	{
		return SampleConstant(time, isLooping);
	}

	return SampleLinear(time, isLooping);
}

template<typename T, unsigned int N>
Frame<N>& Track<T, N>::operator[](unsigned int index)
{
	return m_frames[index];
}

template<typename T, unsigned int N>
T Track<T, N>::EvaluateHermite(float t, const T& point1, const T& slope1, const T& point2, const T& slope2)
{
	float tt = t * t;
	float ttt = tt * t;

	T p2 = point2;
	TrackUtils::Neighbourhood(point1, p2);

	float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
	float h2 = -2.0f * ttt + 3.0f * tt;
	float h3 = ttt - 2.0f * tt + t;
	float h4 = ttt - tt;

	T result = point1 * h1 + p2 * h2 + slope1 * h3 + slope2 * h4;
	return TrackUtils::AdjustHermiteResult(result);
}
