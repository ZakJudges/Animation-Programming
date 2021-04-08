#include "AnimationApp.h"
#include "Uniform.h"
#include "Draw.h"
#include "glad.h"

AnimationApp::AnimationApp()
{
	m_planeRotation = 0.0f;
	m_rotationIncrement = 30.0f;
}

AnimationApp::~AnimationApp()
{
}

void AnimationApp::Init()
{
	m_shader = new Shader("Source/Shaders/static.vert", "Source/Shaders/lit.frag");
	m_texture = new Texture("Assets/test.jpg");
	m_vertexPositions = new Attribute<Vector3>();
	m_vertexNormals = new Attribute<Vector3>();
	m_vertexTexCoords = new Attribute<Vector2F>();
	m_indexBuffer = new IndexBuffer();

	//	Plane.
	std::vector<Vector3> positions;
	positions.push_back(Vector3(-1, -1, 0));
	positions.push_back(Vector3(-1, 1, 0));
	positions.push_back(Vector3(1, -1, 0));
	positions.push_back(Vector3(1, 1, 0));
	m_vertexPositions->Set(positions);

	std::vector<Vector3> normals;
	normals.resize(4, Vector3(0, 0, 1));
	m_vertexNormals->Set(normals);

	std::vector<Vector2F> uvs;
	uvs.push_back(Vector2F(0, 0));
	uvs.push_back(Vector2F(0, 1));
	uvs.push_back(Vector2F(1, 0));
	uvs.push_back(Vector2F(1, 1));
	m_vertexTexCoords->Set(uvs);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	m_indexBuffer->Set(indices);
}

void AnimationApp::Update(float deltaTime)
{
	
	if (m_planeRotation > 60.0f || m_planeRotation < -60.0f)
	{
		m_rotationIncrement *= -1.0f;
	}

	m_planeRotation += m_rotationIncrement * deltaTime;
}

void AnimationApp::Render(float aspectRatio)
{
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Matrix44 projection = Perspective(60.0f, aspectRatio, 0.01f, 1000.0f);
	//Matrix44 projection = Ortho(-aspectRatio * 11.0f, aspectRatio * 11.0f, -11, 11, 0.001f, 10);
	Matrix44 view = LookAt(Vector3(0, 0, -5), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Matrix44 model = QuaternionToMatrix44(AngleAxis(DEG2RAD * m_planeRotation, Vector3(0, 1, 0)));

	m_shader->Bind();

	m_vertexPositions->BindTo(m_shader->GetAttribute("position"));
	m_vertexNormals->BindTo(m_shader->GetAttribute("normal"));
	m_vertexTexCoords->BindTo(m_shader->GetAttribute("texCoord"));
	Uniform<Matrix44>::Set(m_shader->GetUniform("model"), model);
	Uniform<Matrix44>::Set(m_shader->GetUniform("view"), view);
	Uniform<Matrix44>::Set(m_shader->GetUniform("projection"), projection);

	Uniform<Vector3>::Set(m_shader->GetUniform("light"), Vector3(0, 0, 1));

	m_texture->Set(m_shader->GetUniform("tex0"), 0);
	Draw(*m_indexBuffer, DrawMode::Triangles);
	m_texture->UnSet(0);

	m_vertexPositions->UnbindFrom(m_shader->GetAttribute("position"));
	m_vertexNormals->UnbindFrom(m_shader->GetAttribute("normal"));
	m_vertexTexCoords->UnbindFrom(m_shader->GetAttribute("texCoord"));

	m_shader->Unbind();

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
