#include "Pose.h"
#include "Timer.h"

Pose::Pose()
{
}

Pose::Pose(const Pose& pose)
{
    *this = pose;
}

Pose& Pose::operator=(const Pose& pose)
{
    //  Check that the pose will not be assigned to itself.
    if (&pose == this)
    {
        return *this;
    }

    //  Resize joints and parents to match.
    if (m_parents.size() != pose.m_parents.size())
    {
        m_parents.resize(pose.m_parents.size());
    }
    if (m_joints.size() != pose.m_joints.size())
    {
        m_joints.resize(pose.m_joints.size());
    }

    //  Copy the pose and parent data.
    if (m_parents.size() != 0)
    {
        memcpy(&m_parents[0], &pose.m_parents[0], sizeof(int) * m_parents.size());
    }
    if (m_joints.size() != 0)
    {
        memcpy(&m_joints[0], &pose.m_joints[0], sizeof(Transform) * m_joints.size());
    }

    return *this;

}

Pose::Pose(unsigned int jointCount)
{
    Resize(jointCount);
}

void Pose::Resize(unsigned int size)
{
    m_parents.resize(size);
    m_joints.resize(size);
}

//  Assumed that joint count == parent count.
unsigned int Pose::GetSize()
{
    return m_joints.size();
}

int Pose::GetParent(unsigned int index)
{
    return m_parents[index];
}

void Pose::SetParent(unsigned int index, int parent)
{
    m_parents[index] = parent;
}

Transform Pose::GetLocalTransform(unsigned int index)
{
    return m_joints[index];
}

void Pose::SetLocalTransform(unsigned int index, const Transform& transform)
{
    m_joints[index] = transform;
}

//  Combine all the transforms until the root bone is reached.
Transform Pose::GetGlobalTransform(unsigned int index)
{
    //PROFILE_FUNCTION();
    Transform result = m_joints[index];

    for (int i = m_parents[index]; i >= 0; i = m_parents[i])
    {
        result = Combine(m_joints[i], result);
    }

    return result;
}

//  Gets global joint transform.
Transform Pose::operator[](unsigned int index)
{
    return GetGlobalTransform(index);
}

//  Loop through every joint in the pose and calculate its global transform.
//  TODO: Make more efficient (currently recalculating joints multiple times).
void Pose::GetMatrixPalette(std::vector<Matrix44>& out)
{
    PROFILE_FUNCTION();

    unsigned int size = GetSize();
    if (out.size() != size)
    {
        out.resize(size);
    }

    //for (unsigned int i = 0; i < size; ++i)
    //{
    //    Transform transform = GetGlobalTransform(i);
    //    out[i] = TransformToMatrix44(transform);
    //}
    int i = 0;
    for (; i < size; ++i)
    {
        int parent = m_parents[i];
        if (parent > i)
        {
            //  Transforms are not in the same 'chain', so will need to recalculate the global transforms.  
            break;
        }
        Matrix44 joint = TransformToMatrix44(m_joints[i]);
        if (parent >= 0)
        {
            //  Since parent index is lower than i, the global transform of the parent joint has already been caluclated.
            //  Calculate the global transform of this joint, using the result from the last step of i.
            joint = out[parent] * joint;
        }
        out[i] = joint;
    }

    for (; i < size; ++i)
    {
        Transform t = GetGlobalTransform(i);
        out[i] = TransformToMatrix44(t);
    }
}

bool Pose::operator==(const Pose& other)
{
    if (m_joints.size() != other.m_joints.size())
    {
        return false;
    }

    if (m_parents.size() != other.m_parents.size())
    {
        return false;
    }

    unsigned int size = (unsigned int)m_joints.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        Transform thisLocal = m_joints[i];
        Transform otherLocal = other.m_joints[i];
        int thisParent = m_parents[i];
        int otherParent = other.m_parents[i];

        if (thisParent != otherParent)
        {
            return false;
        }
        if (thisLocal.position != otherLocal.position)
        {
            return false;
        }
        if (thisLocal.rotation != otherLocal.rotation)
        {
            return false;
        }
        if (thisLocal.scale != otherLocal.scale)
        {
            return false;
        }
    }

    return true;
}

bool Pose::operator!=(const Pose& other)
{
    return !(*this == other);
}
