#include "SkinnedMesh.h"
#include "Draw.h"

SkinnedMesh::SkinnedMesh()
{
    m_posAttrib = new Attribute<Vector3>();
    m_normAttrib = new Attribute<Vector3>();
    m_texAttrib = new Attribute<Vector2F>();
    m_weightAttrib = new Attribute<Vector4F>();
    m_influenceAttrib = new Attribute<Vector4I>();
    m_indexBuffer = new IndexBuffer();
}

SkinnedMesh::SkinnedMesh(const SkinnedMesh& other)
{
    m_posAttrib = new Attribute<Vector3>();
    m_normAttrib = new Attribute<Vector3>();
    m_texAttrib = new Attribute<Vector2F>();
    m_weightAttrib = new Attribute<Vector4F>();
    m_influenceAttrib = new Attribute<Vector4I>();
    m_indexBuffer = new IndexBuffer();
    *this = other;
}

SkinnedMesh& SkinnedMesh::operator=(const SkinnedMesh& other)
{
    //  Check for self-assigment.
    if (this == &other)
    {
        return *this;
    }

    m_positions = other.m_positions;
    m_normals = other.m_normals;
    m_texCoords = other.m_texCoords;
    m_weights = other.m_weights;
    m_influences = other.m_influences;
    m_indices = other.m_indices;

    UpdateOpenGLBuffers();

    return *this;

}

SkinnedMesh::~SkinnedMesh()
{
    delete m_posAttrib;
    delete m_normAttrib;
    delete m_texAttrib;
    delete m_weightAttrib;
    delete m_influenceAttrib;
    delete m_indexBuffer;
}

std::vector<Vector3>& SkinnedMesh::GetPositions()
{
    return m_positions;
}

std::vector<Vector3>& SkinnedMesh::GetNormals()
{
    return m_normals;
}

std::vector<Vector2F>& SkinnedMesh::GetTexCoords()
{
    return m_texCoords;
}

std::vector<Vector4F>& SkinnedMesh::GetWeights()
{
    return m_weights;
}

std::vector<Vector4I>& SkinnedMesh::GetInfluences()
{
    return m_influences;
}

std::vector<unsigned int>& SkinnedMesh::GetIndices()
{
    return m_indices;
}

void SkinnedMesh::CPUSkin(Skeleton& skeleton, Pose& pose)
{
}

void SkinnedMesh::UpdateOpenGLBuffers()
{
    if (m_positions.size() > 0)
    {
        m_posAttrib->Set(m_positions);
    }
    if (m_normals.size() > 0)
    {
        m_normAttrib->Set(m_normals);
    }
    if (m_texCoords.size() > 0)
    {
        m_texAttrib->Set(m_texCoords);
    }
    if (m_weights.size() > 0)
    {
        m_weightAttrib->Set(m_weights);
    }
    if (m_influences.size() > 0)
    {
        m_influenceAttrib->Set(m_influences);
    }
    if (m_indices.size() > 0)
    {
        m_indexBuffer->Set(m_indices);
    }
}

void SkinnedMesh::Bind(int position, int normal, int texCoord, int weight, int influence)
{
    if (position >= 0)
    {
        m_posAttrib->BindTo(position);
    }
    if (normal >= 0)
    {
        m_normAttrib->BindTo(normal);
    }
    if (texCoord >= 0)
    {
        m_texAttrib->BindTo(texCoord);
    }
    if (weight >= 0)
    {
        m_weightAttrib->BindTo(weight);
    }
    if (influence >= 0)
    {
        m_influenceAttrib->BindTo(influence);
    }

}

void SkinnedMesh::Draw()
{
    if (m_indices.size() > 0)
    {
        ::Draw(*m_indexBuffer, DrawMode::Triangles);
    }
    else
    {
        ::Draw(m_positions.size(), DrawMode::Triangles);
    }
}

void SkinnedMesh::DrawInstanced(unsigned int instanceCount)
{
    if (m_indices.size() > 0)
    {
        ::DrawInstanced(*m_indexBuffer, DrawMode::Triangles, instanceCount);
    }
    else
    {
        ::DrawInstanced(m_positions.size(), DrawMode::Triangles, instanceCount);
    }
}

void SkinnedMesh::UnBind(int position, int normal, int texCoord, int weight, int influence)
{
    if (position >= 0)
    {
        m_posAttrib->UnbindFrom(position);
    }
    if (normal >= 0)
    {
        m_normAttrib->UnbindFrom(normal);
    }
    if (texCoord >= 0)
    {
        m_texAttrib->UnbindFrom(texCoord);
    }
    if (weight >= 0)
    {
        m_weightAttrib->UnbindFrom(weight);
    }
    if (influence >= 0)
    {
        m_influenceAttrib->UnbindFrom(influence);
    }
}
