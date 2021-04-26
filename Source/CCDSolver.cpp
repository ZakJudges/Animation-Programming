#include "CCDSolver.h"
#include "Maths.h"

CCDSolver::CCDSolver()
{
    m_stepCount = 15;
    m_threshold = 0.00001f;
}

unsigned int CCDSolver::GetSize()
{
    m_chain.size();
}

void CCDSolver::Resize(unsigned int newSize)
{
    m_chain.resize(newSize);
}

Transform& CCDSolver::operator[](unsigned int index)
{
    return m_chain[index];
}

Transform CCDSolver::GetGlobalTransform(unsigned int index)
{
    unsigned int size = m_chain.size();

    Transform world = m_chain[index];

    //  Start with the transform of the index, then descend the chain until the root (0) is reached.
    for (int i = index - 1; i >= 0; --i)
    {
        world = Combine(m_chain[i], world);
    }

    return world;
}

unsigned int CCDSolver::GetStepCount()
{
    return m_stepCount;
}

void CCDSolver::SetStepCount(unsigned int stepCount)
{
    m_stepCount = stepCount;
}

float CCDSolver::GetThreshold()
{
    return m_threshold;
}

void CCDSolver::SetThreshold(float value)
{
    m_threshold = value;
}

bool CCDSolver::Solve(const Transform& target)
{
    unsigned int size = GetSize();
    if (size == 0)
    {
        return false;
    }
    unsigned int last = size - 1;

    float thresholdSq = m_threshold * m_threshold;

    Vector3 goal = target.position;

    for (unsigned int i = 0; i < m_stepCount; ++i)
    {
        Vector3 effector = GetGlobalTransform(last).position;

        if (LengthSq(goal - effector) < thresholdSq)
        {
            return true;
        }

        //  Loop through the IK chain:
        //      Starting at the second last joint, since rotating the last has no effect on other joints.
        for (int j = (int)size - 2; j >= 0; --j)
        {
            //  Get the world transform of the joint and end effector.
            effector = GetGlobalTransform(last).position;
            Transform joint = GetGlobalTransform(j);
            Vector3 position = joint.position;
            Quaternion rotation = joint.rotation;

            //  Find a vector from the position of the joint to the position of the end effector.
            Vector3 toEffector = effector - position;

            //  Find another vector from the position of the current joint to the position of the goal.
            Vector3 toGoal = goal - position;

            //  Find a quaternion that rotates toEffector to toGoal.
            Quaternion effectorToGoal;
            if (LengthSq(toGoal) > 0.00001f)
            {
                effectorToGoal = FromTo(toEffector, toGoal);
            }

            //  Rotate the joint towards the goal.
            Quaternion worldRotated = rotation * effectorToGoal;

            //  Moving back into joint's local space.
            Quaternion localRotated = worldRotated * Inverse(rotation);

            m_chain[j].rotation = localRotated * m_chain[j].rotation;

            //  As the joints moves, check how close the end effector is to the goal at each iteration.
            effector = GetGlobalTransform(last).position;
            if (LengthSq(goal - effector) < thresholdSq)
            {
                return true;
            }
        }
    }

    //  Goal not reached in specified number of steps.
    return false;

}
