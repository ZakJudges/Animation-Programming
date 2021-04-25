//#include "AnimatedMeshApp.h"
//#include "glad.h"
//#include "GLTFLoader.h"
//#include "Uniform.h"
//#include "Attribute.h"
//#include "Timer.h"
//AnimatedMeshApp::AnimatedMeshApp()
//{
//
//}
//
//AnimatedMeshApp::~AnimatedMeshApp()
//{
//	Shutdown();
//}
//
//void AnimatedMeshApp::Init()
//{
//	Instrumentor::Get().BeginSession("Rendering", "AnimationUpdate.json");
//	
//
//	cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
//	m_cpuMeshes = LoadSkinnedMeshes(gltf);
//	
//	m_skeleton = LoadSkeleton(gltf);
//	m_clips = LoadAnimationClips(gltf);
//	FreeGLTFFile(gltf);
//
//	m_gpuMeshes = m_cpuMeshes;
//	for (int i = 0; i < m_gpuMeshes.size(); i++)
//	{
//		m_gpuMeshes[i].UpdateOpenGLBuffers();
//	}
//	
//	m_staticShader = new Shader("Source/Shaders/static.vert", "Source/Shaders/lit.frag");
//	m_skinnedShader = new Shader("Source/Shaders/skinned.vert", "Source/Shaders/lit.frag");
//	m_diffuseTexture = new Texture("Assets/Woman.png");
//	
//	
//
//	m_gpuAnimInfo.m_animatedPose = m_skeleton.GetRestPose();
//	m_gpuAnimInfo.m_posePalette.resize(m_skeleton.GetRestPose().GetSize());
//	m_cpuAnimInfo.m_animatedPose = m_skeleton.GetRestPose();
//	m_cpuAnimInfo.m_posePalette.resize(m_skeleton.GetRestPose().GetSize());
//
//	m_gpuAnimInfo.m_model.position = Vector3(-2, 0, 0);
//	m_cpuAnimInfo.m_model.position = Vector3(2, 0, 0);
//
//	unsigned int numUIClips = (unsigned int)m_clips.size();
//	for (unsigned int i = 0; i < numUIClips; ++i) {
//		if (m_clips[i].GetName() == "Walking") {
//			m_cpuAnimInfo.m_clip = i;
//		}
//		else if (m_clips[i].GetName() == "Running") {
//			m_gpuAnimInfo.m_clip = i;
//		}
//	}
//}
//
//void AnimatedMeshApp::Update(float deltaTime)
//{
//	PROFILE_FUNCTION();
//	//m_cpuAnimInfo.m_playback = m_clips[m_cpuAnimInfo.m_clip].Sample(m_cpuAnimInfo.m_animatedPose, m_cpuAnimInfo.m_playback + deltaTime);
//	{
//		Timer timer("Sampling");
//		m_gpuAnimInfo.m_playback = m_clips[m_gpuAnimInfo.m_clip].Sample(m_gpuAnimInfo.m_animatedPose, m_gpuAnimInfo.m_playback + deltaTime);
//	}
//
//	//for (unsigned int i = 0, size = (unsigned int)m_cpuMeshes.size(); i < size; ++i) {
//	//	m_cpuMeshes[i].CPUSkinMatrix(m_skeleton, m_cpuAnimInfo.m_animatedPose);
//	//}
//
//	m_gpuAnimInfo.m_animatedPose.GetMatrixPalette(m_gpuAnimInfo.m_posePalette);
//
//
//}
//
//void AnimatedMeshApp::Render(float aspectRatio)
//{
//	//PROFILE_FUNCTION();
//
//	
//	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//	Matrix44 projection = Perspective(60.0f, aspectRatio, 0.01f, 1000.0f);
//	Matrix44 view = LookAt(Vector3(0, 5, 7), Vector3(0, 3, 0), Vector3(0, 1, 0));
//	//Matrix44 mvp = projection * view; // No model
//	Matrix44 model;
//	
//	
//
//	// CPU Skinned Mesh
//	model = TransformToMatrix44(m_cpuAnimInfo.m_model);
//	m_staticShader->Bind();
//	Uniform<Matrix44>::Set(m_staticShader->GetUniform("model"), model);
//	Uniform<Matrix44>::Set(m_staticShader->GetUniform("view"), view);
//	Uniform<Matrix44>::Set(m_staticShader->GetUniform("projection"), projection);
//	Uniform<Vector3>::Set(m_staticShader->GetUniform("light"), Vector3(1, 1, 1));
//	
//	m_diffuseTexture->Set(m_staticShader->GetUniform("tex0"), 0);
//	for (unsigned int i = 0, size = (unsigned int)m_cpuMeshes.size(); i < size; ++i) 
//	{
//		m_cpuMeshes[i].Bind(m_staticShader->GetAttribute("position"), m_staticShader->GetAttribute("normal"), m_staticShader->GetAttribute("texCoord"), -1, -1);
//		m_cpuMeshes[i].Draw();
//		m_cpuMeshes[i].UnBind(m_staticShader->GetAttribute("position"), m_staticShader->GetAttribute("normal"), m_staticShader->GetAttribute("texCoord"), -1, -1);
//	}
//	m_diffuseTexture->UnSet(0);
//	m_staticShader->UnBind();
//	
//	// GPU Skinned Mesh
//	model = TransformToMatrix44(m_gpuAnimInfo.m_model);
//	m_skinnedShader->Bind();
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("model"), model);
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("view"), view);
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("projection"), projection);
//	Uniform<Vector3>::Set(m_skinnedShader->GetUniform("light"), Vector3(1, 1, 1));
//	
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("pose"), m_gpuAnimInfo.m_posePalette);
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("invBindPose"), m_skeleton.GetInvBindPose());
//	
//	m_diffuseTexture->Set(m_skinnedShader->GetUniform("tex0"), 0);
//	for (unsigned int i = 0, size = (unsigned int)m_gpuMeshes.size(); i < size; ++i) {
//		m_gpuMeshes[i].Bind(m_skinnedShader->GetAttribute("position"), m_skinnedShader->GetAttribute("normal"), m_skinnedShader->GetAttribute("texCoord"), m_skinnedShader->GetAttribute("weights"), m_skinnedShader->GetAttribute("joints"));
//		m_gpuMeshes[i].Draw();
//		m_gpuMeshes[i].UnBind(m_skinnedShader->GetAttribute("position"), m_skinnedShader->GetAttribute("normal"), m_skinnedShader->GetAttribute("texCoord"), m_skinnedShader->GetAttribute("weights"), m_skinnedShader->GetAttribute("joints"));
//	}
//	m_diffuseTexture->UnSet(0);
//	m_skinnedShader->UnBind();
//}
//
//void AnimatedMeshApp::Shutdown()
//{
//	Instrumentor::Get().EndSession();
//}
