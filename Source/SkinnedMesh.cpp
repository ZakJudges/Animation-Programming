#include "SkinnedMesh.h"
#include "Draw.h"
#include "Transform.h"

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

void SkinnedMesh::CPUSkinTransform(Skeleton& skeleton, Pose& pose)
{
    unsigned int vertexCount = m_positions.size();
    if (vertexCount == 0)
    {
        return;
    }

    m_skinnedPositions.resize(vertexCount);
    m_skinnedNormals.resize(vertexCount);
    Pose& bindPose = skeleton.GetBindPose();

    for (int i = 0; i < vertexCount; ++i)
    {
        //  Each vertex is influenced by 4 joints.
        Vector4I& joint = m_influences[i];
        Vector4F& weight = m_weights[i];

        //  Skin space transform for the first joint influencing the current vertex.
        Transform skin0 = Combine(pose[joint.x], Inverse(bindPose[joint.x]));
        //  Now need to transform the vertex to get the new position and normal.
        Vector3 p0 = TransformPoint(skin0, m_positions[i]);
        Vector3 n0 = TransformVector(skin0, m_normals[i]);

        //  Repeat for the other joints that might influence the vertex:
        Transform skin1 = Combine(pose[joint.y], Inverse(bindPose[joint.y]));
        Vector3 p1 = TransformPoint(skin1, m_positions[i]);
        Vector3 n1 = TransformVector(skin1, m_normals[i]);

        Transform skin2 = Combine(pose[joint.z], Inverse(bindPose[joint.z]));
        Vector3 p2 = TransformPoint(skin2, m_positions[i]);
        Vector3 n2 = TransformVector(skin2, m_normals[i]);

        Transform skin3 = Combine(pose[joint.w], Inverse(bindPose[joint.w]));
        Vector3 p3 = TransformPoint(skin3, m_positions[i]);
        Vector3 n3 = TransformVector(skin3, m_normals[i]);

        //  Combine the positions and normals into one vertex based on the weights.
        m_skinnedPositions[i] = p0 * weight.x + p1 * weight.y + p2 * weight.z + p3 * weight.w;
        m_skinnedNormals[i] = n0 * weight.x + n1 * weight.y + n2 * weight.x + n3 * weight.w;
    }

    m_posAttrib->Set(m_skinnedPositions);
    m_normAttrib->Set(m_skinnedNormals);
}

void SkinnedMesh::CPUSkinMatrix(Skeleton& skeleton, Pose& pose)
{
    unsigned int vertexCount = m_positions.size();
    if (vertexCount == 0)
    {
        return;
    }

    m_skinnedPositions.resize(vertexCount);
    m_skinnedNormals.resize(vertexCount);

    //  The global pose.
    pose.GetMatrixPalette(m_posePalette);
    std::vector<Matrix44> invPosePalette = skeleton.GetInvBindPose();
    for (int i = 0; i < vertexCount; ++i)
    {
        Vector4I& joint = m_influences[i];
        Vector4F& weight = m_weights[i];

        //  Skin space matrix for each joint influencing the vertex.
        Matrix44 skin0 = (m_posePalette[joint.x] * invPosePalette[joint.x]) * weight.x;
        Matrix44 skin1 = (m_posePalette[joint.y] * invPosePalette[joint.y]) * weight.y;
        Matrix44 skin2 = (m_posePalette[joint.z] * invPosePalette[joint.z]) * weight.z;
        Matrix44 skin3 = (m_posePalette[joint.w] * invPosePalette[joint.w]) * weight.w;

        //  Combine the skin matrices into one then transform the position and normal to get the final vertex.
        Matrix44 skinMatrix = skin0 + skin1 + skin2 + skin3;
        m_skinnedPositions[i] = TransformPoint(skinMatrix, m_positions[i]);
        m_skinnedNormals[i] = TransformVector(skinMatrix, m_normals[i]);
    }

    m_posAttrib->Set(m_skinnedPositions);
    m_normAttrib->Set(m_skinnedNormals);
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
