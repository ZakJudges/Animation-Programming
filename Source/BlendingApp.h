#ifndef _H_BLENDING_APP
#define _H_BLENDING_APP

#include "Application.h"
#include "Maths.h"
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include "Shader.h"
#include "Texture.h"
#include "SkinnedMesh.h"
#include <vector>
#include "CrossFadeController.h"

struct AnimationInstance
{
	unsigned int m_clip;
	float m_time;
	Pose m_pose;
};

class BlendingApp : public Application
{
public:
	BlendingApp();
	virtual ~BlendingApp();
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(float aspectRatio) override;
	virtual void Shutdown() override;

protected:
	Shader* m_shader;
	Texture* m_texture;
	std::vector<SkinnedMesh> m_meshes;
	std::vector<Clip> m_clips;
	Skeleton m_skeleton;
	Pose m_pose;
	std::vector<Matrix44> m_posePalette;
	std::vector<Matrix44> m_skinPalette;
	CrossFadeController m_crossFadeController;
	float m_fadeTime;
	unsigned int m_currentClip;


	AnimationInstance m_a;
	AnimationInstance m_b;
	float m_blendTime;
	bool m_invertBlend;
	
};

#endif
