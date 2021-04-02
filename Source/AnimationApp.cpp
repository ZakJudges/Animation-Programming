#include "AnimationApp.h"
#include "Uniform.h"
#include "Draw.h"

AnimationApp::AnimationApp()
{
}

AnimationApp::~AnimationApp()
{
}

void AnimationApp::Init()
{
	m_shader = new Shader("Source/Shaders/static.vert", "Source/Shaders/lit.frag");
}

void AnimationApp::Update(float deltaTime)
{
}

void AnimationApp::Render(float aspectRatio)
{
}

void AnimationApp::Shutdown()
{
	if (m_shader)
	{
		delete m_shader;
		m_shader = nullptr;
	}
	if (m_vertexPositions)
	{
		delete m_vertexPositions;
		m_vertexPositions = nullptr;
	}
	if (m_vertexNormals)
	{
		delete m_vertexNormals;
		m_vertexNormals = nullptr;
	}
	if (m_vertexTexCoords)
	{
		delete m_vertexTexCoords;
		m_vertexTexCoords = nullptr;
	}
	if (m_indexBuffer)
	{
		delete m_indexBuffer;
		m_indexBuffer = nullptr;
	}
	if (m_texture)
	{
		delete m_texture;
		m_texture = nullptr;
	}
}
