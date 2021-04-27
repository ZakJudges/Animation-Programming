#include "FABRIKSolver.h"

FABRIKSolver::FABRIKSolver()
{
    m_stepCount = 15;
    m_threshold = 0.00001f;
}

unsigned int FABRIKSolver::GetSize()
{
    return m_chain.size();
}

void FABRIKSolver::Resize(unsigned int newSize)
{
    m_chain.resize(newSize);
    m_worldChain.resize(newSize);
}

unsigned int FABRIKSolver::GetStepCount()
{
    return m_stepCount;
}

void FABRIKSolver::SetStepCount(unsigned int stepCount)
{
    m_stepCount = stepCount;
}

float FABRIKSolver::GetThreshold()
{
    return m_threshold;
}

void FABRIKSolver::SetThreshold(float value)
{
    m_threshold = value;
}

Transform FABRIKSolver::GetLocalTransform(unsigned int index)
{
    return m_chain[index];
}

void FABRIKSolver::SetLocalTransform(unsigned int index, const Transform& transform)
{
    m_chain[index] = transform;
}

Transform FABRIKSolver::GetGlobalTransform(unsigned int index)
{
    //  Get the local transform at the index.
    //  Then concatenate all of the transforms until the root is reached.
    //  Result is the world space transform at this index.
    Transform world = m_chain[index];
    for (int i = (int)index - 1; i >= 0; --i)
    {
        world = Combine(m_chain[i], world);
    }

    return world;
}

bool FABRIKSolver::Solve(const Transform& target)
{
    unsigned int size = GetSize();
    if (size == 0)
    {
        //  No IK chain to solve.
        return false;
    }

    unsigned int last = size - 1;
    float thresholdSq = m_threshold * m_threshold;

    //  Convert the IK chain into world space positions.
    IKChainToWorld();

    Vector3 goal = target.position;
    Vector3 base = m_worldChain[0];

    for (unsigned int i = 0; i < m_stepCount; ++i)
    {
        Vector3 effector = m_worldChain[last];
        if (LengthSq(goal - effector) < thresholdSq)
        {
            //  Convert the IK chain back to local space, since the effector has reached its goal.
            WorldToIKChain();
            return true;
        }

        IterateBackward(goal);
        IterateForward(base);
    }

    //  The end effector did not reach its target in the required step count.
    //      But it could be close, so convert back to local space.
    WorldToIKChain();
    Vector3 effector = GetGlobalTransform(last).position;
    if (LengthSq(goal - effector) < thresholdSq)
    {
        //  Checking if the goal was reached on the last iteration.
        return true;
    }

    return false;
}

//  Convert the IK chain into world space positions.
void FABRIKSolver::IKChainToWorld()
{
    unsigned int size = GetSize();

    //  Get the position of each joint in world space.
    //  Also calculate the lengths from each joint to its parent joint (in world space).
    for (unsigned int i = 0; i < size; ++i)
    {
        Transform world = GetGlobalTransform(i);
        m_worldChain[i] = world.position;
        if (i >= 1)
        {
            Vector3 prev = m_worldChain[i - 1];
            m_lengths[i] = Length(world.position - prev);
        }
    }

    if (size > 0)
    {
        //  The root has no parent joint, so there is no length to calculate.
        m_lengths[0] = 0.0f;
    }
}

//  Convert the world position IK chain back into local space transforms.
//  Work out what rotation was applied to the world chain nodes.
//  Apply the same rotations in local space.
void FABRIKSolver::WorldToIKChain()
{
    unsigned int size = GetSize();
    if (size == 0)
    {
        return;
    }

    for (unsigned int i = 0; i < size - 1; ++i)
    {
        Transform world = GetGlobalTransform(i);
        Transform next = GetGlobalTransform(i + 1);
        Vector3 position = world.position;
        Quaternion rotation = world.rotation;

        //  Create a vector that goes from the current node to the next node.
        //  The old direction from this node to the next node.
        Vector3 toNext = next.position - position;
        //  By undoing the world space rotation, toNext now represents the position in local space.
        toNext = Inverse(rotation) * toNext;

        //  The new direction from this node to the next node.
        Vector3 toDesired = m_worldChain[i + 1] - position; 
        //  By undoing the world space rotatio, toDesired now represents the position of the next node in local space.
        toDesired = Inverse(rotation) * toDesired;

        //  Calculate a quaternion that rotates the old direction to next node, towards the new direction to next node.
        Quaternion delta = FromTo(toNext, toDesired);
        m_chain[i].rotation = delta * m_chain[i].rotation;
    }
}

//  Rearranges the IK chain so that the first link starts at the origin of the chain.
//  Needs to set the initial joint to be at the base, then iterates through all the other joints.
//      Adjusting them to keep the IK chain intact.
//  After this function executes, the end effector might be at the goal if the chain is solvable and has sufficient iterations.
void FABRIKSolver::IterateForward(const Vector3& base)
{
    unsigned int size = GetSize();
    if (size > 0)
    {
        //  Move the first node back to the original base.
        m_worldChain[0] = base;
    }

    for (int i = 1; i < size; ++i)
    {
        //  Move the node towards the previous node (preserving lengths).
        Vector3 direction = Normalised(m_worldChain[i] - m_worldChain[i - 1]);
        Vector3 offset = direction * m_lengths[i];
        m_worldChain[i] = m_worldChain[i - 1] + offset;

    }
}

//  Set the last element in the chain to be at the goal. 
//  This breaks the IK chain.
//  Adjust all other joints using the stored distances so that the chain remains intact.
//  After this function executes, the end effector is always at the goal and the initial joint is probalby no longer at the base.
void FABRIKSolver::IterateBackward(const Vector3& goal)
{
    int size = GetSize();
    if (size > 0)
    {
        //  Move the end effector to the goal.
        m_worldChain[size - 1] = goal;
    }

    for (int i = size - 2; i >= 0; --i)
    {
        //  Move the node towards the next node (preserving lengths).
        Vector3 direction = Normalised(m_worldChain[i] - m_worldChain[i + 1]);
        Vector3 offset = direction * m_lengths[i + 1];
        m_worldChain[i] = m_worldChain[i + 1] + offset;
    }
}


