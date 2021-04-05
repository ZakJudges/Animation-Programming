#ifndef _H_DEBUGLINE_
#define _H_DEBUGLINE_

#include "Shader.h"
#include "Attribute.h"
#include "Maths.h"
#include <vector>
//	to remove.
enum class DebugDrawMode {
	Lines, Loop, Strip, Points
};

class DebugDraw {
protected:
	std::vector<Vector3> mPoints;
	Attribute<Vector3>* mAttribs;
	Shader* mShader;
private:
	DebugDraw(const DebugDraw&);
	DebugDraw& operator=(const DebugDraw&);
public:
	DebugDraw();
	DebugDraw(unsigned int size);
	~DebugDraw();

	unsigned int Size();
	void Resize(unsigned int newSize);
	Vector3& operator[](unsigned int index);
	void Push(const Vector3& v);

	void UpdateOpenGLBuffers();
	void Draw(DebugDrawMode mode, const Vector3& color, const Matrix44& mvp);
};

#endif