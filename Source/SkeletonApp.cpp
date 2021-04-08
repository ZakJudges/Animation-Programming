#include "SkeletonApp.h"
#include "glad.h"
#include "GLTFLoader.h"

SkeletonApp::SkeletonApp()
{

}

SkeletonApp::~SkeletonApp()
{
}

void SkeletonApp::Init()
{
	cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
	m_restPose = LoadRestPose(gltf);
	m_clips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	m_restPoseVis = new DebugDraw();
	m_restPoseVis->FromPose(m_restPose);
	m_restPoseVis->UpdateOpenGLBuffers();

	m_currentClip = 0;
	m_currentPose = m_restPose;

	m_currentPoseVis = new DebugDraw();
	m_currentPoseVis->FromPose(m_currentPose);
	m_currentPoseVis->UpdateOpenGLBuffers();
}

void SkeletonApp::Update(float deltaTime)
{
	
	m_playbackTime = m_clips[m_currentClip].Sample(m_currentPose, m_playbackTime + deltaTime);
	m_currentPoseVis->FromPose(m_currentPose);
}

void SkeletonApp::Render(float aspectRatio)
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Matrix44 projection = Perspective(60.0f, aspectRatio, 0.01f, 1000.0f);
	Matrix44 view = LookAt(Vector3(0, 4, 7), Vector3(0, 4, 0), Vector3(0, 1, 0));
	Matrix44 mvp = projection * view; // No model

	//m_restPoseVis->Draw(DebugDrawMode::Lines, Vector3(1, 0, 0), mvp);
	m_currentPoseVis->UpdateOpenGLBuffers();
	m_currentPoseVis->Draw(DebugDrawMode::Lines, Vector3(1, 1, 0), mvp);
}

void SkeletonApp::Shutdown()
{
	delete m_restPoseVis;
	delete m_currentPoseVis;
}
