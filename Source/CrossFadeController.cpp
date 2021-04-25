#include "CrossFadeController.h"
#include "BlendController.h"

CrossFadeController::CrossFadeController()
{
    m_clip = 0;
    m_time = 0.0f;
    m_validSkeleton = false;
}

CrossFadeController::CrossFadeController(Skeleton& skeleton)
{
    m_clip = 0;
    m_time = 0.0f;
    SetSkeleton(skeleton);
}

void CrossFadeController::SetSkeleton(Skeleton& skeleton)
{
    m_skeleton = skeleton;
    m_pose = skeleton.GetRestPose();
    m_validSkeleton = true;
}

void CrossFadeController::Play(Clip* target)
{
    //  Clear any active crossfades.
    m_targets.clear();
    m_clip = target;
    m_pose = m_skeleton.GetRestPose();
    m_time = target->GetStartTime();
}

void CrossFadeController::FadeTo(Clip* target, float fadeTime)
{
    //  Check if the target clip is valid.
    if (m_clip == 0)
    {
        Play(target);
        return;
    }

    if (m_targets.size() >= 1)
    {
        Clip* clip = m_targets[m_targets.size() - 1].m_clip;
        if (clip == target)
        {
            return;
        }
    }
    else
    {
        if (m_clip == target)
        {
            return;
        }
    }

    m_targets.push_back(CrossFadeTarget(target, m_skeleton.GetRestPose(), fadeTime));
}

//  Play the active animation and blend in any other animations that are in the fade list.
void CrossFadeController::Update(float dt)
{
    if (m_clip == 0 || !m_validSkeleton)
    {
        return;
    }

    //  Set the current animation as the target animation and remove the fade object if an animation has finished fading.
    //  One target removed per frame.
    unsigned int numTargets = m_targets.size();

    //  Remove a crossfade target if the animation has finished.
    for (unsigned int i = 0; i < numTargets; ++i)
    {
        float duration = m_targets[i].m_duration;
        if (m_targets[i].m_elapsed >= duration)
        {
            m_clip = m_targets[i].m_clip;
            m_time = m_targets[i].m_time;
            m_pose = m_targets[i].m_pose;
            m_targets.erase(m_targets.begin() + i);
            break;
        }
    }

    numTargets = m_targets.size();
    m_pose = m_skeleton.GetRestPose();
    m_time = m_clip->Sample(m_pose, m_time + dt);

    for (unsigned int i = 0; i < numTargets; ++i)
    {
        CrossFadeTarget& target = m_targets[i];
        target.m_time = target.m_clip->Sample(target.m_pose, target.m_time + dt);
        target.m_elapsed += dt;
        float t = target.m_elapsed / target.m_duration;
        if (t > 1.0f)
        {
            t = 1.0f;
        }

        //  Blend all joints.
        BlendController::Blend(m_pose, m_pose, target.m_pose, t, -1);
    }
}

Pose& CrossFadeController::GetCurrentPose()
{
    return m_pose;
}

Clip* CrossFadeController::GetCurrentClip()
{
    return m_clip;
}
