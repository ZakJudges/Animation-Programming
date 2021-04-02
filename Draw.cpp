#include "Draw.h"
#include "glad.h"
#include <iostream>

static GLenum DrawModeToGLEnum(DrawMode input)
{
	if (input == DrawMode::Points) 
	{
		return  GL_POINTS;
	}
	else if (input == DrawMode::LineStrip) 
	{
		return GL_LINE_STRIP;
	}
	else if (input == DrawMode::LineLoop) 
	{
		return  GL_LINE_LOOP;
	}
	else if (input == DrawMode::Lines) 
	{
		return  GL_LINES;
	}
	else if (input == DrawMode::Triangles) 
	{
		return  GL_TRIANGLES;
	}
	else if (input == DrawMode::TriangleStrip) 
	{
		return  GL_TRIANGLE_STRIP;
	}
	else if (input == DrawMode::TriangleFan) 
	{
		return   GL_TRIANGLE_FAN;
	}

	std::cout << "Invalid DrawModeToGLEnum\n";
	return 0;
}

void Draw(IndexBuffer& indexBuffer, DrawMode mode)
{
	unsigned int handle = indexBuffer.GetHandle();
	unsigned int numIndices = indexBuffer.Count();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glDrawElements(DrawModeToGLEnum(mode), numIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Draw(unsigned int vertexCount, DrawMode mode)
{
	glDrawArrays(DrawModeToGLEnum(mode), 0, vertexCount);
}

void DrawInstanced(IndexBuffer& indexBuffer, DrawMode mode, unsigned int instanceCount)
{
	unsigned int handle = indexBuffer.GetHandle();
	unsigned int numIndices = indexBuffer.Count();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glDrawElementsInstanced(DrawModeToGLEnum(mode), numIndices, GL_UNSIGNED_INT, 0, instanceCount);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawInstanced(unsigned int vertexCount, DrawMode mode, unsigned int instanceCount)
{
	glDrawArraysInstanced(DrawModeToGLEnum(mode), 0, vertexCount, instanceCount);
}
