#include "TransformTrack.h"
#include "Timer.h"

typedef TTransformTrack<VectorTrack, QuaternionTrack>;
typedef TTransformTrack<VectorFastTrack, QuaternionFastTrack>;

template <typename VTRACK, typename QTRACK>
TTransformTrack<VTRACK, QTRACK>::TTransformTrack()
{
    m_bone = 0;
}

template <typename VTRACK, typename QTRACK>
unsigned int TTransformTrack<VTRACK, QTRACK>::GetBone()
{
    return m_bone;
}

template <typename VTRACK, typename QTRACK>
void TTransformTrack<VTRACK, QTRACK>::SetBone(unsigned int bone)
{
    m_bone = bone;
}

template <typename VTRACK, typename QTRACK>
VTRACK& TTransformTrack<VTRACK, QTRACK>::GetPositionTrack()
{
    return m_position;
}

template <typename VTRACK, typename QTRACK>
QTRACK& TTransformTrack<VTRACK, QTRACK>::GetRotationTrack()
{
    return m_rotation;
}

template <typename VTRACK, typename QTRACK>
VTRACK& TTransformTrack<VTRACK, QTRACK>::GetScaleTrack()
{
    return m_scale;
}

//  Get the lowest value of t from the tracks.
template <typename VTRACK, typename QTRACK>
float TTransformTrack<VTRACK, QTRACK>::GetStartTime()
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
template <typename VTRACK, typename QTRACK>
float TTransformTrack<VTRACK, QTRACK>::GetEndTime()
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

template <typename VTRACK, typename QTRACK>
bool TTransformTrack<VTRACK, QTRACK>::IsValid()
{
    //  Tracks are valid if they contain at least two frames.
    return m_position.GetSize() > 1 || m_rotation.GetSize() > 1 || m_scale.GetSize() > 1;
}

template <typename VTRACK, typename QTRACK>
Transform TTransformTrack<VTRACK, QTRACK>::Sample(const Transform& ref, float time, bool isLooping)
{
   PROFILE_FUNCTION();

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

//  Copies track data by value, so is slow.
//      Use at load time only.
FastTransformTrack OptimiseTransformTrack(TransformTrack& input)
{
    FastTransformTrack result;

    result.SetBone(input.GetBone());
    result.GetPositionTrack() = OptimiseTrack<Vector3, 3>(input.GetPositionTrack());
    result.GetRotationTrack() = OptimiseTrack<Quaternion, 4>(input.GetRotationTrack());
    result.GetScaleTrack() = OptimiseTrack<Vector3, 3>(input.GetScaleTrack());

    return result;
}