#include "DebugDraw.h"
#include "Uniform.h"
#include "Draw.h"

DebugDraw::DebugDraw() {
	mAttribs = new Attribute<Vector3>();

	mShader = new Shader(
		"#version 330 core\n"
		"uniform mat4 mvp;\n"
		"in vec3 position;\n"
		"void main() {\n"
		"	gl_Position = mvp * vec4(position, 1.0);\n"
		"}"
		,
		"#version 330 core\n"
		"uniform vec3 color;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(color, 1);\n"
		"}"
	);
}

DebugDraw::DebugDraw(unsigned int size) {
	mAttribs = new Attribute<Vector3>();

	mShader = new Shader(
		"#version 330 core\n"
		"uniform mat4 mvp;\n"
		"in vec3 position;\n"
		"void main() {\n"
		"	gl_Position = mvp * vec4(position, 1.0);\n"
		"}"
		,
		"#version 330 core\n"
		"uniform vec3 color;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(color, 1);\n"
		"}"
	);

	Resize(size);
}

DebugDraw::~DebugDraw() {
	delete mAttribs;
	delete mShader;
}

unsigned int DebugDraw::Size() {
	return (unsigned int)mPoints.size();
}

void DebugDraw::Resize(unsigned int newSize) {
	mPoints.resize(newSize);
}

Vector3& DebugDraw::operator[](unsigned int index) {
	return mPoints[index];
}

void DebugDraw::Push(const Vector3& v) {
	mPoints.push_back(v);
}


void DebugDraw::UpdateOpenGLBuffers() {
	mAttribs->Set(mPoints);
}

void DebugDraw::Draw(DebugDrawMode mode, const Vector3& color, const Matrix44& mvp) {
	mShader->Bind();
	Uniform<Matrix44>::Set(mShader->GetUniform("mvp"), mvp);
	Uniform<Vector3>::Set(mShader->GetUniform("color"), color);
	mAttribs->BindTo(mShader->GetAttribute("position"));
	if (mode == DebugDrawMode::Lines) {
		::Draw(Size(), DrawMode::Lines);
	}
	else if (mode == DebugDrawMode::Loop) {
		::Draw(Size(), DrawMode::LineLoop);
	}
	else if (mode == DebugDrawMode::Strip) {
		::Draw(Size(), DrawMode::LineStrip);
	}
	else {
		::Draw(Size(), DrawMode::Points);
	}
	mAttribs->UnbindFrom(mShader->GetAttribute("position"));
	mShader->Unbind();
}