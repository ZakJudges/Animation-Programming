#include "GLTFLoader.h"
#include <iostream>
#include "Transform.h"
#include "Matrix44.h"
#include "Pose.h"
#include "Track.h"
#include "Interpolation.h"

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


