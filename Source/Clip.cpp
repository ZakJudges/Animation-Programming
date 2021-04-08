#include "Clip.h"

Clip::Clip()
{
    m_name = "no_name_given";
    m_startTime = 0.0f;
    m_endTime = 0.0f;
    m_looping = true;
}

unsigned int Clip::GetBoneAtIndex(unsigned int index)
{
    return m_tracks[index].GetBone();
}

void Clip::SetBoneAtIndex(unsigned int index, unsigned int bone)
{
    m_tracks[index].SetBone(bone);
}

unsigned int Clip::GetSize()
{
    return m_tracks.size();
}

float Clip::Sample(Pose& output, float time)
{
    if (GetDuration() == 0.0f)
    {
        return 0.0f;
    }

    time = FitTimeToClip(time);
    unsigned int size = m_tracks.size();

    for(unsigned int i = 0; i < size; ++i)
    {
        unsigned int j = m_tracks[i].GetBone();
        Transform local = output.GetLocalTransform(j);
        Transform animated = m_tracks[i].Sample(local, time, m_looping);    
        output.SetLocalTransform(j, animated);
    }

    return time;
}

//  Retrieve the TransformTrack object for a specific joint in the clip.
//  Linear search for the specified joint.
TransformTrack& Clip::operator[](unsigned int bone)
{
    for (int i = 0, s = m_tracks.size(); i < s; ++i)
    {
        if (m_tracks[i].GetBone() == bone)
        {
            return m_tracks[i];
        }
    }

    //  No transform track for the specified joint, so create one.
    m_tracks.push_back(TransformTrack());
    m_tracks.back().SetBone(bone);
    return m_tracks[m_tracks.size() - 1];
}

void Clip::RecalculateDuration()
{
    m_startTime = 0.0f;
    m_endTime = 0.0f;
    bool startSet = false;
    bool endSet = false;
    unsigned int trackSize = m_tracks.size();

    //  Loop through all the tracks and find the smallest and largest times.
    for (unsigned int i = 0; i < trackSize; ++i)
    {
        if (m_tracks[i].IsValid())
        {
            float startTime = m_tracks[i].GetStartTime();
            if (startTime < m_startTime || !startSet)
            {
                m_startTime = startTime;
                startSet = true;
            }

            float endTime = m_tracks[i].GetEndTime();
            if (endTime > m_endTime || !endSet)
            {
                m_endTime = endTime;
                endSet = true;
            }
        }
    }
}

std::string& Clip::GetName()
{
    return m_name;
}

void Clip::SetName(const std::string& name)
{
    m_name = name;
}

float Clip::GetDuration()
{
    return m_endTime - m_startTime;
}

float Clip::GetStartTime()
{
    return m_startTime;
}

float Clip::GetEndTime()
{
    return m_endTime;
}

bool Clip::GetLooping()
{
    return m_looping;
}

void Clip::SetLooping(bool isLooping)
{
    m_looping = isLooping;
}

float Clip::FitTimeToClip(float time)
{
    if (m_looping)
    {
        float duration = m_endTime - m_startTime;
        if (duration <= 0.0f)
        {
            return 0.0f;
        }

        //  Scale the input time so that it lies somewhere between the start and end time of the clip.
        time = fmodf(time - m_startTime, m_endTime - m_startTime);
        if (time < 0.0f)
        {
            time += m_endTime - m_startTime;
        }
        time += time + m_startTime;
    }
    else
    {
        if (time < m_startTime)
        {
            time = m_startTime;
        }
        if (time > m_endTime)
        {
            time = m_endTime;
        }
    }

    return time;
}
