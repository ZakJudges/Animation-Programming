#include "GLTFLoader.h"
#include <iostream>

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
