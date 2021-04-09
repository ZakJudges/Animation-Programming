#include "GLTFLoader.h"
#include <iostream>
#include "Transform.h"
#include "Matrix44.h"
#include "Pose.h"
#include "Track.h"
#include "Interpolation.h"
#include <algorithm>

namespace GLTFHelper
{
    //  Get the local transform of the cgltf node.
    Transform GetLocalTransform(cgltf_node& n)
    {
        Transform result;
        if (n.has_matrix)
        {
            Matrix44 matrix(&n.matrix[0]);
            result = Matrix44ToTransform(matrix);
        }

        if (n.has_translation)
        {
            result.position = Vector3(n.translation[0], n.translation[1], n.translation[2]);
        }
        if (n.has_rotation)
        {
            result.rotation = Quaternion(n.rotation[0], n.rotation[1], n.rotation[2], n.rotation[3]);
        }
        if (n.has_scale)
        {
            result.scale = Vector3(n.scale[0], n.scale[1], n.scale[2]);
        }

        return result;
    }

    //  Loop through all the nodes in the glTF file and return the index of the desired node.
    int GetNodeIndex(cgltf_node* target, cgltf_node* allNodes, unsigned int numNodes)
    {
        if (target == 0)
        {
            return -1;
        }

        for (unsigned int i = 0; i < numNodes; ++i)
        {
            if (target == &allNodes[i])
            {
                return (int)i;
            }
        }

        return -1;
    }

    void GetScalarValues(std::vector<float>& output, unsigned int compCount, const cgltf_accessor& accessor)
    {
        output.resize(accessor.count * compCount);

        for (cgltf_size i = 0; i < accessor.count; ++i)
        {
            cgltf_accessor_read_float(&accessor, i, &output[i * compCount], compCount);
        }
    }

    template<typename T, int N>
    void TrackFromChannel(Track<T, N>& result, const cgltf_animation_channel& channel)
    {
        cgltf_animation_sampler& sampler = *channel.sampler;
        Interpolation interpolationType = Interpolation::CONSTANT;
        bool isSamplerCubic = false;

        if (sampler.interpolation == cgltf_interpolation_type_linear)
        {
            interpolationType = Interpolation::LINEAR;
        }
        else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline)
        {
            interpolationType = Interpolation::CUBIC;
            isSamplerCubic = true;
        }


        result.SetInterpolationType(interpolationType);

        //  Resize the track to have enough room to store all of the frames in the animation.
        std::vector<float> time;
        GetScalarValues(time, 1, *sampler.input);
        std::vector<float> values;
        GetScalarValues(values, N, *sampler.output);
        unsigned int frameCount = sampler.input->count;
        //  Number of components per frame.
        unsigned int compCount = values.size() / time.size();
        result.Resize(frameCount);

        //  Loop through the frame data to parse the time and values into frame structures usable for the track.
        for (unsigned int i = 0; i < frameCount; ++i)
        {
            int baseIndex = i * compCount;
            Frame<N>& frame = result[i];
            int offset = 0;
            frame.m_time = time[i];

            for (int comp = 0; comp < N; ++comp)
            {
                frame.m_in[comp] = isSamplerCubic ?
                    values[baseIndex + offset++] : 0.0f;
            }

            for (int comp = 0; comp < N; ++comp)
            {
                frame.m_value[comp] = values[baseIndex + offset++];
            }

            for (int comp = 0; comp < N; ++comp)
            {
                frame.m_out[comp] = isSamplerCubic ?
                    values[baseIndex + offset++] : 0.0f;
            }
        }
    }

    void MeshFromAttribute(SkinnedMesh& output, cgltf_attribute& attribute, cgltf_skin* skin, cgltf_node* nodes, unsigned int nodeCount)
    {
        cgltf_attribute_type attribType = attribute.type;
        cgltf_accessor& accessor = *attribute.data;
        unsigned int componentCount = 0;

        //  Check if we will be parsing 2,3 or 4 floating point values per vertex.
        if (accessor.type == cgltf_type_vec2)
        {
            componentCount = 2;
        }
        if (accessor.type == cgltf_type_vec3)
        {
            componentCount = 3;
        }
        if (accessor.type == cgltf_type_vec4)
        {
            componentCount = 4;
        }

        std::vector<float> values;
        GetScalarValues(values, componentCount, accessor);
        unsigned int accessorCount = accessor.count;
        std::vector<Vector3>& positions = output.GetPositions();
        std::vector<Vector3>& normals = output.GetNormals();
        std::vector<Vector2F>& texCoords = output.GetTexCoords();
        std::vector<Vector4I>& influences = output.GetInfluences();
        std::vector<Vector4F>& weights = output.GetWeights();

        //  Loop through all the values in the current accessor and assign them to the appropriate vector based on the accessor type.
        for (unsigned int i = 0; i < accessorCount; ++i)
        {
            int index = i * componentCount;
            switch (attribType)
            {
                case cgltf_attribute_type_position:
                
                    positions.push_back(Vector3(values[index + 0], values[index + 1], values[index + 2]));
                
                break;

                case cgltf_attribute_type_texcoord:
                
                    texCoords.push_back(Vector2F(values[index + 0], values[index + 1]));
                
                break;

                case cgltf_attribute_type_weights:
                
                    weights.push_back(Vector4F(values[index + 0], values[index + 1], values[index + 2], values[index + 3]));
                
                break;

                case cgltf_attribute_type_normal:
                {
                    Vector3 normal = Vector3(values[index + 0], values[index + 1], values[index + 2]);
                    if (LengthSq(normal) < 0.000001f)
                    {
                        std::cout << "Invalid mesh normal from cgltf_attribute\n";
                        normal = Vector3(0, 1, 0);
                    }
                    normals.push_back(Normalised(normal));
                }
                break;

                case cgltf_attribute_type_joints:
                {
                    //  Values are stored as floating point numbers, so add 0.5 to remove precision issues.
                    Vector4I joints((int)values[index + 0] + 0.5f, (int)values[index + 1] + 0.5f, (int)values[index + 2] + 0.5f
                        , (int)values[index + 3] + 0.5f);
                    //  Joints are stored relative to the joints array. 
                    //      But we need them relative to the skeleton hierarchy.
                    joints.x = GetNodeIndex(skin->joints[joints.x], nodes, nodeCount);
                    joints.y = GetNodeIndex(skin->joints[joints.y], nodes, nodeCount);
                    joints.z = GetNodeIndex(skin->joints[joints.z], nodes, nodeCount);
                    joints.w = GetNodeIndex(skin->joints[joints.w], nodes, nodeCount);

                    //  Ensure that invalid node indices have a value of 0;
                    joints.x = std::max(0, joints.x);
                    joints.y = std::max(0, joints.y);
                    joints.z = std::max(0, joints.z);
                    joints.w = std::max(0, joints.w);

                    influences.push_back(joints);
                }
                break;   
            }
        }
    }
}

cgltf_data* LoadGLTFFile(const char* path)
{
    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data* data = NULL;

    //  Parse GLTF file.
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if (result != cgltf_result_success)
    {
        std::cout << "Could not load: " << path << "\n";
        return 0;
    }

    //  Load buffers.
    result = cgltf_load_buffers(&options, data, path);
    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Could not load: " << path << "\n";
        return 0;
    }

    //  Validate the data.
    result = cgltf_validate(data);
    if (result != cgltf_result_success)
    {
        cgltf_free(data);
        std::cout << "Invalid file: " << path << "\n";
        return 0;
    }

    return data;

}

void FreeGLTFFile(cgltf_data* data)
{
    if (data == 0)
    {
        std::cout << "Trying to free null data\n";
        return;
    }

    cgltf_free(data);
}

Pose LoadRestPose(cgltf_data* data)
{
    //  Assuming one animated character for each glTF file.
    unsigned int boneCount = data->nodes_count;
    Pose result(boneCount);
    for (unsigned int i = 0; i < boneCount; ++i)
    {
        cgltf_node* node = &(data->nodes[i]);
        Transform transform = GLTFHelper::GetLocalTransform(data->nodes[i]);
        result.SetLocalTransform(i, transform);
        int parent = GLTFHelper::GetNodeIndex(node->parent, data->nodes, boneCount);
        result.SetParent(i, parent);
    }

    return result;
}

//  glTF files do not store the bind pose, only the inverse bind pose, so we must reconstruct it.
Pose LoadBindPose(cgltf_data* data)
{
    Pose restPose = LoadRestPose(data);
    unsigned int numBones = restPose.GetSize();
    std::vector<Transform> worldBindPose(numBones);

    for (int i = 0; i < numBones; ++i)
    {
        worldBindPose[i] = restPose.GetGlobalTransform(i);
    }

    //  Loop through all of the skinned meshes and calculate the bind pose for each.
    unsigned int numSkins = data->skins_count;
    for (int i = 0; i < numSkins; ++i)
    {
        //  Read the inverse bind matrices into a vector of float values.
        //      The vector will contain numBones * 16 float values.
        cgltf_skin* skin = &(data->skins[i]);
        std::vector<float> invBindAccessor;
        GLTFHelper::GetScalarValues(invBindAccessor, 16, *skin->inverse_bind_matrices);
        
        unsigned int numJoints = skin->joints_count;
        for (int j = 0; j < numJoints; ++j)
        {
            //  Read the inverse bind matrix of the joint.
            float* matrix = &(invBindAccessor[j * 16]);
            Matrix44 invBindMatrix = Matrix44(matrix);

            //  Invert to get the bind matrix.
            Matrix44 bindMatrix = Inverse(invBindMatrix);
            Transform bindTransform = Matrix44ToTransform(bindMatrix);

            cgltf_node* jointNode = skin->joints[j];
            int jointIndex = GLTFHelper::GetNodeIndex(jointNode, data->nodes, numBones);
            worldBindPose[jointIndex] = bindTransform;
        }
    }

    //  Convert each joint so that it is relative to its parent.
    Pose bindPose = restPose;
    for (int i = 0; i < numBones; ++i)
    {
        Transform current = worldBindPose[i];
        int p = bindPose.GetParent(i);
        if (p >= 0)
        {
            //  Make joint transform relative to its parent, unless it is a root.
            Transform parent = worldBindPose[p];
            current = Combine(Inverse(parent), current);
        }
        bindPose.SetLocalTransform(i, current);
    }

    return bindPose;

}

Skeleton LoadSkeleton(cgltf_data* data)
{
    return Skeleton(LoadRestPose(data), LoadBindPose(data), LoadJointNames(data));
}

std::vector<std::string> LoadJointNames(cgltf_data* data)
{
    //  Assuming one animated character per glTF file.
    unsigned int boneCount = (unsigned int)data->nodes_count;

    std::vector<std::string> result(boneCount, "not_set");

    for (unsigned int i = 0; i < boneCount; ++i)
    {
        cgltf_node* node = &(data->nodes[i]);
        if (node->name == 0)
        {
            result[i] = "empty_node";
        }
        else
        {
            result[i] = node->name;
        }
    }

    return result;
}

std::vector<Clip> LoadAnimationClips(cgltf_data* data)
{
    unsigned int numClips = data->animations_count;
    unsigned int numNodes = data->nodes_count;
    std::vector<Clip> result;
    result.resize(numClips);

    for (unsigned int i = 0; i < numClips; ++i)
    {
        result[i].SetName(data->animations[i].name);

        //  Each channel is an animation track.
        unsigned int numChannels = data->animations[i].channels_count;
        for (int j = 0; j < numChannels; ++j)
        {
            //  Populate the tracks (j) from the clips (i).
            cgltf_animation_channel& channel = data->animations[i].channels[j];

            cgltf_node* target = channel.target_node;
            int nodeId = GLTFHelper::GetNodeIndex(target, data->nodes, numNodes);

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                VectorTrack& track = result[i][nodeId].GetPositionTrack();
                GLTFHelper::TrackFromChannel<Vector3, 3>(track, channel);
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                VectorTrack& track = result[i][nodeId].GetScaleTrack();
                GLTFHelper::TrackFromChannel<Vector3, 3>(track, channel);
            }
            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                QuaternionTrack& track = result[i][nodeId].GetRotationTrack();
                GLTFHelper::TrackFromChannel<Quaternion, 4>(track, channel);
            }
        }

        result[i].RecalculateDuration();
    }

    return result;
}

//  Assuming one glTF file contains one model only.
std::vector<SkinnedMesh> LoadSkinnedMeshes(cgltf_data* data)
{
    std::vector<SkinnedMesh> result;
    cgltf_node* nodes = data->nodes;
    unsigned int nodeCount = data->nodes_count;

    //  Loop through all the nodes in the glTF file. Only process nodes that have both a mesh and a skin.
    for (int i = 0; i < nodeCount; ++i)
    {
        cgltf_node* node = &nodes[i]; 
        if (node->mesh == 0 || node->skin == 0)
        {
            continue;
        }

        //  In glTF, a mesh is made up of primitives. 
        unsigned int primitiveCount = node->mesh->primitives_count;
        for (unsigned int j = 0; j < primitiveCount; ++j)
        {
            result.push_back(SkinnedMesh());
            SkinnedMesh& mesh = result[result.size() - 1];
            cgltf_primitive* primitive = &node->mesh->primitives[j];

            //  Each primitive contains attributes.
            unsigned int attribCount = primitive->attributes_count;
            for (unsigned int k = 0; k < attribCount; ++k)
            {
                cgltf_attribute* attribute = &primitive->attributes[k];
                GLTFHelper::MeshFromAttribute(mesh, *attribute, node->skin, nodes, nodeCount);
            }

            //  Check if the primitive contains indices.
            if (primitive->indices != 0)
            {
                int indexCount = primitive->indices->count;
                std::vector<unsigned int>& indices = mesh.GetIndices();
                indices.resize(indexCount);

                for (unsigned int k = 0; k < indexCount; ++k)
                {
                    indices[k] = cgltf_accessor_read_index(primitive->indices, k);
                }
            }

            mesh.UpdateOpenGLBuffers();
        }
    }

    return result;
}





