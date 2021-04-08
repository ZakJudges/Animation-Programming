#include "Skeleton.h"

Skeleton::Skeleton()
{
}

Skeleton::Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
    Set(rest, bind, names);
}

void Skeleton::Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names)
{
    m_restPose = rest;
    m_bindPose = bind;
    m_jointNames = names;
    UpdateInverseBindPose();

}

Pose& Skeleton::GetRestPose()
{
    return m_restPose;
}

Pose& Skeleton::GetBindPose()
{
    return m_bindPose;
}

std::vector<Matrix44>& Skeleton::GetInvBindPose()
{
    return m_invBindPose;
}

std::vector<std::string>& Skeleton::GetJointNames()
{
    return m_jointNames;
}

std::string& Skeleton::GetJointName(unsigned int index)
{
    return m_jointNames[index];
}

//  Multiplying by the inverse bind pose puts vertices into skin space.
void Skeleton::UpdateInverseBindPose()
{
    unsigned int size = m_bindPose.GetSize();
    m_invBindPose.resize(size);
    for (unsigned int i = 0; i < size; ++i)
    {
        Transform world = m_bindPose.GetGlobalTransform(i);
        m_invBindPose[i] = Inverse(TransformToMatrix44(world));
    }
}
