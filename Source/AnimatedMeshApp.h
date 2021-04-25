//#ifndef H_ANIMATED_MESH_APP
//#define H_ANIMATED_MESH_APP
//
//#include "Application.h"
//#include "Maths.h"
//#include "Pose.h"
//#include "Clip.h"
//#include "Skeleton.h"
//#include "Shader.h"
//#include "Texture.h"
//#include "SkinnedMesh.h"
//#include <vector>
//
//struct AnimationInstance
//{
//	Pose m_animatedPose;
//	std::vector<Matrix44> m_posePalette;
//	unsigned int m_clip;
//	float m_playback;
//	Transform m_model;
//
//	inline AnimationInstance() : m_clip(0), m_playback(0.0f) {}
//};
//
//
//class AnimatedMeshApp : public Application
//{
//public:
//	AnimatedMeshApp();
//	virtual ~AnimatedMeshApp();
//	virtual void Init() override;
//	virtual void Update(float deltaTime) override;
//	virtual void Render(float aspectRatio) override;
//	virtual void Shutdown() override;
//
//private:
//	Texture* m_diffuseTexture;
//	Shader* m_staticShader;
//	Shader* m_skinnedShader;
//	std::vector<SkinnedMesh> m_cpuMeshes;
//	std::vector<SkinnedMesh> m_gpuMeshes;
//	Skeleton m_skeleton;
//	std::vector<Clip> m_clips;
//	AnimationInstance m_cpuAnimInfo;
//	AnimationInstance m_gpuAnimInfo;
//};
//#endif
