#include "BlendingApp.h"
#include "glad.h"
#include "GLTFLoader.h"
#include "Uniform.h"
#include "Attribute.h"
#include "Timer.h"
#include "BlendController.h"
BlendingApp::BlendingApp()
{
	
}

BlendingApp::~BlendingApp()
{
	Shutdown();
}

void BlendingApp::Init()
{
	//	Load the mesh and animation data from the glTF file.
	cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
	m_skeleton = LoadSkeleton(gltf);
	m_clips = LoadAnimationClips(gltf);
	m_meshes = LoadSkinnedMeshes(gltf);
	FreeGLTFFile(gltf);

	//	Initialise the animation data.
	m_pose = m_skeleton.GetRestPose();
	m_pose.GetMatrixPalette(m_posePalette);
	m_skinPalette = m_skeleton.GetInvBindPose();
	m_a.m_pose = m_pose;
	m_a.m_clip = 0;
	m_b.m_clip = 1;
	m_b.m_pose = m_pose;
	m_blendTime = 0.0f;
	m_invertBlend = false;
	for (int i = 0, size = m_clips.size(); i < size; ++i)
	{
		if (m_clips[i].GetName() == "Walking")
		{
			m_a.m_clip = i;
			m_a.m_time = m_clips[i].GetStartTime();
		}

		if (m_clips[i].GetName() == "Running")
		{
			m_b.m_clip = i;
			m_b.m_time = m_clips[i].GetStartTime();
		}
	}

	//	Initialise other graphics resources.
	m_shader = new Shader("Source/Shaders/skinned.vert", "Source/Shaders/lit.frag");
	m_texture = new Texture("Assets/Woman.png");

}

void BlendingApp::Update(float deltaTime)
{
	//	Sample the animations at the new time.
	m_a.m_time = m_clips[m_a.m_clip].Sample(m_a.m_pose, m_a.m_time + deltaTime);
	m_b.m_time = m_clips[m_b.m_clip].Sample(m_b.m_pose, m_b.m_time + deltaTime);

	//	Blend the poses together.
	float bt = m_blendTime;
	if (bt < 0.0f)
	{
		bt = 0.0f;
	}
	else if (bt > 1.0f)
	{
		bt = 1.0f;
	}
	if (m_invertBlend)
	{
		bt = 1.0f - bt;
	}
	
	BlendController::Blend(m_pose, m_a.m_pose, m_b.m_pose, bt, -1);
	m_pose.GetMatrixPalette(m_posePalette);

	m_blendTime += deltaTime;
	if (m_blendTime >= 7.0f)
	{
		m_blendTime = 0.0f;
		m_invertBlend = !m_invertBlend;
		m_pose = m_skeleton.GetRestPose();
	}

}

void BlendingApp::Render(float aspectRatio)
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Matrix44 model;
	Matrix44 view = LookAt(Vector3(0, 3, 5), Vector3(0, 3, 0), Vector3(0, 1, 0));
	Matrix44 projection = Perspective(60.0f, aspectRatio, 0.01f, 1000.0f);

	m_shader->Bind();
	//	Set the shader uniforms.
	Uniform<Matrix44>::Set(m_shader->GetUniform("model"), model);
	Uniform<Matrix44>::Set(m_shader->GetUniform("view"), view);
	Uniform<Matrix44>::Set(m_shader->GetUniform("projection"), projection);
	Uniform<Matrix44>::Set(m_shader->GetUniform("pose"), m_posePalette);
	Uniform<Matrix44>::Set(m_shader->GetUniform("invBindPose"), m_skinPalette);
	Uniform<Vector3>::Set(m_shader->GetUniform("light"), Vector3(1, 1, 1));

	m_texture->Set(m_shader->GetUniform("tex0"), 0);
	//	Set the vertex attributes in the shader.
	for (int i = 0, size = m_meshes.size(); i < size; ++i)
	{
		m_meshes[i].Bind(m_shader->GetAttribute("position"), m_shader->GetAttribute("normal"), m_shader->GetAttribute("texCoord"), m_shader->GetAttribute("weights"), m_shader->GetAttribute("joints"));
		m_meshes[i].Draw();
		m_meshes[i].UnBind(m_shader->GetAttribute("position"), m_shader->GetAttribute("normal"), m_shader->GetAttribute("texCoord"), m_shader->GetAttribute("weights"), m_shader->GetAttribute("joints"));
	}

	m_texture->UnSet(0);
	m_shader->UnBind();
}

void BlendingApp::Shutdown()
{
	m_clips.clear();
	m_meshes.clear();
	delete m_shader;
	delete m_texture;
}