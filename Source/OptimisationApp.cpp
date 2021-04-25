//#include "OptimisationApp.h"
//#include "glad.h"
//#include "GLTFLoader.h"
//#include "Uniform.h"
//#include "Attribute.h"
//#include "Timer.h"
//OptimisationApp::OptimisationApp()
//{
//
//}
//
//OptimisationApp::~OptimisationApp()
//{
//	Shutdown();
//}
//
//void OptimisationApp::Init()
//{
//	Instrumentor::Get().BeginSession("Rendering", "GetMatrixPalette.json");
//
//
//	cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
//	m_cpuMeshes = LoadSkinnedMeshes(gltf);
//
//	m_skeleton = LoadSkeleton(gltf);
//	m_clips = LoadAnimationClips(gltf);
//	for (int i = 0; i < m_clips.size(); ++i)
//	{
//		m_clips[i].SampleUsingLookupTable(true);
//	}
//
//	FreeGLTFFile(gltf);
//
//	m_gpuMeshes = m_cpuMeshes;
//	for (int i = 0; i < m_gpuMeshes.size(); i++)
//	{
//		m_gpuMeshes[i].UpdateOpenGLBuffers();
//	}
//
//	m_staticShader = new Shader("Source/Shaders/static.vert", "Source/Shaders/lit.frag");
//	m_skinnedShader = new Shader("Source/Shaders/preskinned.vert", "Source/Shaders/lit.frag");
//	m_diffuseTexture = new Texture("Assets/Woman.png");
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
//void OptimisationApp::Update(float deltaTime)
//{
//	//PROFILE_FUNCTION();
//
//	////	CPU SkinnedMesh:
//	////	Sample the clip to set the pose from the specified time.
//	//m_cpuAnimInfo.m_playback = m_clips[m_cpuAnimInfo.m_clip].Sample(m_cpuAnimInfo.m_animatedPose, m_cpuAnimInfo.m_playback + deltaTime);
//	////	Get the pose in the form of a matrix palette.
//	//m_cpuAnimInfo.m_animatedPose.GetMatrixPalette(m_cpuAnimInfo.m_posePalette);
//	////	Get the inverse bind pose.
//	std::vector<Matrix44>& invBindPose = m_skeleton.GetInvBindPose();
//	////	Combine the pose with the inverse bind pose to get the animated pose.
//	//for (int i = 0; i < m_cpuAnimInfo.m_posePalette.size(); ++i)
//	//{
//	//	m_cpuAnimInfo.m_posePalette[i] = m_cpuAnimInfo.m_posePalette[i] * invBindPose[i];
//	//}
//	////	Skin the mesh before sending to the static vertex shader.
//	//for (unsigned int i = 0, size = (unsigned int)m_cpuMeshes.size(); i < size; ++i) 
//	//{
//	//	//m_cpuMeshes[i].CPUSkinMatrix(m_skeleton, m_cpuAnimInfo.m_animatedPose);
//	//
//	//	//	Skin the mesh by sending it the combined pose and inverse bind pose.
//	//	m_cpuMeshes[i].CPUSkin(m_cpuAnimInfo.m_posePalette);
//	//}
//
//	//	GPU SkinnedMesh:
//	//	Sample the clip to set the pose at the specified time.
//	m_gpuAnimInfo.m_playback = m_clips[m_gpuAnimInfo.m_clip].Sample(m_gpuAnimInfo.m_animatedPose, m_gpuAnimInfo.m_playback + deltaTime);
//	//	Convert the pose into a palette of matrices, each matrix represents the global transform of a bone.
//	m_gpuAnimInfo.m_animatedPose.GetMatrixPalette(m_gpuAnimInfo.m_posePalette);
//	//	Multiply the animation pose with the inverse bind pose.
//	for (int i = 0; i < m_gpuAnimInfo.m_posePalette.size(); ++i)
//	{
//		m_gpuAnimInfo.m_posePalette[i] = m_gpuAnimInfo.m_posePalette[i] * invBindPose[i];
//	}
//
//}
//
//void OptimisationApp::Render(float aspectRatio)
//{
//	//PROFILE_FUNCTION();
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
//	Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("animated"), m_gpuAnimInfo.m_posePalette);
//	//Uniform<Matrix44>::Set(m_skinnedShader->GetUniform("invBindPose"), m_skeleton.GetInvBindPose());
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
//void OptimisationApp::Shutdown()
//{
//	Instrumentor::Get().EndSession();
//}
