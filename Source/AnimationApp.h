#ifndef H_ANIMATION_APP
#define H_ANIMATION_APP

#include "Application.h"
#include "Maths.h"
#include "Shader.h"
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Texture.h" 

class AnimationApp : public Application
{
public:
	AnimationApp();
	virtual ~AnimationApp();
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(float aspectRatio) override;
	virtual void Shutdown() override;

private:
	Shader* m_shader;
	Attribute<Vector3>* m_vertexPositions;
	Attribute<Vector3>* m_vertexNormals;
	Attribute<Vector2F>* m_vertexTexCoords;
	IndexBuffer* m_indexBuffer;
	Texture* m_texture;
};
#endif
