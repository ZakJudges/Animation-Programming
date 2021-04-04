#ifndef _H_GLTFLOADER
#define _H_GLTFLOADER

#include "Source/cgltf.h"

cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* data);

#endif