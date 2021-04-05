#include "TransformTrack.h"

TransformTrack::TransformTrack()
{
    m_bone = 0;
}

unsigned int TransformTrack::GetBone()
{
    return m_bone;
}

void TransformTrack::SetBone(unsigned int bone)
{
    m_bone = bone;
}

VectorTrack& TransformTrack::GetPositionTrack()
{
    return m_position;
}

QuaternionTrack& TransformTrack::GetRotationTrack()
{
    return m_rotation;
}

VectorTrack& TransformTrack::GetScaleTrack()
{
    return m_scale;
}

//  Get the lowest value of t from the tracks.
float TransformTrack::GetStartTime()
{
    float result = 0.0f;
    bool isSet = false;

    if (m_position.GetSize() > 1)
    {
        result = m_position.GetStartTime();
        isSet = true;
    }

    if (m_rotation.GetSize() > 1)
    {
        if (m_position.GetStartTime() < result || !isSet)
        {
            result = m_position.GetStartTime();
        }
        isSet = true;
    }

    if (m_scale.GetSize() > 1)
    {
        if (m_scale.GetStartTime() < result || !isSet)
        {
            result = m_scale.GetStartTime();
            isSet = true;
        }
    }

    return result;
}

//  Get the highest value of t from the tracks.
float TransformTrack::GetEndTime()
{
    float result = 0.0f;
    bool isSet = false;

    if (m_position.GetSize() > 1)
    {
        result = m_position.GetEndTime();
        isSet = true;
    }

    if (m_rotation.GetSize() > 1)
    {
        if (m_position.GetEndTime() > result || !isSet)
        {
            result = m_position.GetEndTime();
        }
        isSet = true;
    }

    if (m_scale.GetSize() > 1)
    {
        if (m_scale.GetEndTime() > result || !isSet)
        {
            result = m_scale.GetEndTime();
            isSet = true;
        }
    }

    return result;
}

bool TransformTrack::IsValid()
{
    //  Tracks are valid if they contain at least two frames.
    return m_position.GetSize() > 1 || m_rotation.GetSize() > 1 || m_scale.GetSize() > 1;
}

Transform TransformTrack::Sample(const Transform& ref, float time, bool isLooping)
{
    Transform result = ref;

    if (m_position.GetSize() > 1)
    {
        result.position = m_position.Sample(time, isLooping);
    }

    if (m_rotation.GetSize() > 1)
    {
        result.rotation = m_rotation.Sample(time, isLooping);
    }

    if (m_scale.GetSize() > 1)
    {
        result.scale = m_scale.Sample(time, isLooping);
    }

    return result;
}
