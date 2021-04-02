#ifndef _H_DRAW
#define _H_DRAW

#include "IndexBuffer.h"

enum class DrawMode
{
	Points,
	LineStrip,
	LineLoop,
	Lines,
	Triangles,
	TriangleStrip,
	TriangleFan
};

void Draw(IndexBuffer& indexBuffer, DrawMode mode);
void Draw(unsigned int vertexCount, DrawMode mode);
void DrawInstanced(IndexBuffer& indexBuffer, DrawMode mode, unsigned int instanceCount);
void DrawInstanced(unsigned int vertexCount, DrawMode mode, unsigned int instanceCount);

#endif
