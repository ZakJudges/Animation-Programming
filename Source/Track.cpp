#include "Track.h"
#include "TrackUtils.h"


template Track<float, 1>;
template Track<Vector3, 3>;
template Track<Quaternion, 4>;

template<typename T, int N>
Track<T, N>::Track()
{
	//	Linear interpolation by default.
	m_interpolationType = Interpolation::LINEAR;
}

template<typename T, int N>
void Track<T, N>::Resize(unsigned int size)
{
	m_frames.resize(size);
}

template<typename T, int N>
unsigned int Track<T, N>::GetSize()
{
	return m_frames.size();
}

template<typename T, int N>
Interpolation Track<T, N>::GetInterpolationType()
{
	return m_interpolationType;
}

template<typename T, int N>
void Track<T, N>::SetInterpolationType(Interpolation interpolationType)
{
	m_interpolationType = interpolationType;
}

template<typename T, int N>
float Track<T, N>::GetStartTime()
{
	return m_frames[0].m_time;
}

template<typename T, int N>
float Track<T, N>::GetEndTime()
{
	return m_frames[m_frames.size() - 1].m_time;
}

template<typename T, int N>
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

template<typename T, int N>
Frame<N>& Track<T, N>::operator[](unsigned int index)
{
	return m_frames[index];
}

template<typename T, int N>
T Track<T, N>::SampleConstant(float time, bool isLooping)
{
	int frame = GetFrameIndex(time, isLooping);
	if (frame < 0 || frame >(int)m_frames.size())
	{
		//	Invalid frame index.
		return T();
	}

	return Cast(&m_frames[frame].m_value[0]);
}

template<typename T, int N>
T Track<T, N>::SampleLinear(float time, bool isLooping)
{
	int thisFrame = GetFrameIndex(time, isLooping);
	if (thisFrame < 0 || thisFrame >= m_frames.size() - 1)
	{
		//	Invalid frame index.
		return T();
	}

	int nextFrame = thisFrame + 1;
	
	float trackTime = FitTimeToTrack(time, isLooping);
	float thisTime = m_frames[thisFrame].m_time;

	//	Calculate the length of time between the two frames.
	float frameDelta = m_frames[nextFrame].m_time - thisTime;
	if (frameDelta < 0.0f)
	{
		//	Invalid frame time.
		return T();
	}

	//	Calculate a normalised value of t[0,1] between this frame and the next frame.
	float t = (trackTime - thisTime) / frameDelta;
	T start = Cast(&m_frames[thisFrame].m_value[0]);
	T end = Cast(&m_frames[nextFrame].m_value[0]);

	return TrackUtils::Interpolate(start, end, t);

}

template<typename T, int N>
T Track<T, N>::SampleCubic(float time, bool isLooping)
{
	int thisFrame = GetFrameIndex(time, isLooping);
	if (thisFrame < 0 || thisFrame >= m_frames.size() - 1)
	{
		//	Invalid frame index.
		return T();
	}

	int nextFrame = thisFrame + 1;

	float trackTime = FitTimeToTrack(time, isLooping);
	float thisTime = m_frames[thisFrame].m_time;

	//	Calculate the length of time between the two frames.
	float frameDelta = m_frames[nextFrame].m_time - thisTime;
	if (frameDelta < 0.0f)
	{
		//	Invalid frame time.
		return T();
	}

	//	Calculate a normalised value of t[0,1] between this frame and the next frame.
	float t = (trackTime - thisTime) / frameDelta;

	//	Using memcpy instead of Cast to avoid normalisation.
	T point1 = Cast(&m_frames[thisFrame].m_value[0]);
	T slope1;
	memcpy(&slope1, m_frames[thisFrame].m_out, N * sizeof(float));
	slope1 = slope1 * frameDelta;
	T point2 = Cast(&m_frames[nextFrame].m_value[0]);
	T slope2;
	memcpy(&slope2, m_frames[nextFrame].m_in, N * sizeof(float));
	slope2 = slope2 * frameDelta;

	return EvaluateHermite(t, point1, slope1, point2, slope2);
}

template<typename T, int N>
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

//	Return the frame to the nearest time (frame will be to the left of time).
template<typename T, int N>
int Track<T, N>::GetFrameIndex(float time, bool isLooping)
{
	unsigned int size = m_frames.size();
	if (size <= 1)
	{
		//	Invalid, the track only has one frame or no frames.
		return -1;
	}

	if (isLooping)
	{
		float startTime = m_frames[0].m_time;
		float endTime = m_frames[size - 1].m_time; 
		float duration = endTime - startTime;

		//	Track is looping, so time needs to be relative to duration.
		time = fmodf(time - startTime, endTime - startTime);
		if (time < 0.0f) 
		{
			time += endTime - startTime;
		}
		time = time + startTime;
	}
	else
	{
		if (time < m_frames[0].m_time)
		{
			return 0;
		}
		if (time > m_frames[size - 2].m_time)
		{
			return (int)size - 2;
		}
	}

	//	Loop throught the frames in this track to find the frame nearest to the time.
	//		frame time will always <= time.
	for (int i = (int)size - 1; i >= 0; --i)
	{
		//	TODO: Time this and compare to binary search.
		if (time >= m_frames[i].m_time)
		{
			return i;
		}
	}

	//	Invalid frame index.
	return -1;
}

template<typename T, int N>
float Track<T, N>::FitTimeToTrack(float time, bool isLooping)
{
	unsigned int size = (unsigned int)m_frames.size();
	if (size <= 1) 
	{ 
		return 0.0f;
	}

	float startTime = m_frames[0].m_time;
	float endTime = m_frames[size - 1].m_time;
	float duration = endTime - startTime;

	if (duration <= 0.0f) 
	{ 
		return 0.0f; 
	}

	if (isLooping) 
	{
		time = fmodf(time - startTime, endTime - startTime);
		if (time < 0.0f) 
		{
			time += endTime - startTime;
		}
		time = time + startTime;
	}
	else 
	{
		if (time <= m_frames[0].m_time) 
		{ 
			time = startTime; 
		}
		if (time >= m_frames[size - 1].m_time)
		{ 
			time = endTime;
		}
	}

	return time;
}

template<> float Track<float, 1>::Cast(float* values)
{
	return values[0];
}

template<> Vector3 Track<Vector3, 3>::Cast(float* values)
{
	return Vector3(values[0], values[1], values[2]);
}

template<> Quaternion Track<Quaternion, 4>::Cast(float* values)
{
	Quaternion result(values[0], values[1], values[2], values[3]);
	return Normalised(result);
}


