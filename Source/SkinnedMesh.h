#ifndef _H_SKINNED_MESH
#define _H_SKINNED_MESH

#include <vector>
#include "Maths.h"
#include "Attribute.h"
#include "IndexBuffer.h"
#include "Skeleton.h"

//	Temporary skinned mesh class.
class SkinnedMesh
{
public:
	SkinnedMesh();
	SkinnedMesh(const SkinnedMesh&);
	SkinnedMesh& operator=(const SkinnedMesh&);
	~SkinnedMesh();
	std::vector<Vector3>& GetPositions();
	std::vector<Vector3>& GetNormals();
	std::vector<Vector2F>& GetTexCoords();
	std::vector<Vector4F>& GetWeights();
	std::vector<Vector4I>& GetInfluences();
	std::vector<unsigned int>& GetIndices();
	void CPUSkinTransform(Skeleton& skeleton, Pose& pose);
	void CPUSkinMatrix(Skeleton& skeleton, Pose& pose);
	void UpdateOpenGLBuffers();
	void Bind(int position, int normal, int texCoord, int weight, int influence);
	void Draw();
	void DrawInstanced(unsigned int instanceCount);
	void UnBind(int position, int normal, int texCoord, int weight, int influence);
protected:
	std::vector<Vector3> m_positions;
	std::vector<Vector3> m_normals;
	std::vector<Vector2F> m_texCoords;
	std::vector<Vector4F> m_weights;
	std::vector<Vector4I> m_influences;
	std::vector<unsigned int> m_indices;
	Attribute<Vector3>* m_posAttrib;
	Attribute<Vector3>* m_normAttrib;
	Attribute<Vector2F>* m_texAttrib;
	Attribute<Vector4F>* m_weightAttrib;
	Attribute<Vector4I>* m_influenceAttrib;
	IndexBuffer* m_indexBuffer;
	std::vector<Vector3> m_skinnedPositions;
	std::vector<Vector3> m_skinnedNormals;
	std::vector<Matrix44> m_posePalette;
};

#endif