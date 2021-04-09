#ifndef _H_GLTFLOADER
#define _H_GLTFLOADER

#include "cgltf.h"
#include <vector>
#include <string>
#include "Clip.h"
#include "Pose.h"
#include "Skeleton.h"
#include "SkinnedMesh.h"

//	Assuming that each glTF file contains only one animated character.

cgltf_data* LoadGLTFFile(const char* path);
Pose LoadRestPose(cgltf_data* data);
Pose LoadBindPose(cgltf_data* data);
Skeleton LoadSkeleton(cgltf_data* data);
void FreeGLTFFile(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);
std::vector<SkinnedMesh> LoadSkinnedMeshes(cgltf_data* data);



#endif