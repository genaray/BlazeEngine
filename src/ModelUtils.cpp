//
// Created by Lars on 15.03.2021.
//

#include <vector>
#include "ModelUtils.h"

Model ModelUtils::merge(const Model &first, const Model &second) {

	auto meshes = vector<Mesh>();
	auto materials = vector<Material>();
	meshes.reserve(first.meshCount+second.meshCount);
	materials.reserve(first.materialCount+second.materialCount);

	auto firstMeshSize = sizeof(first.meshes)/sizeof(first.meshes[0]);
	for (auto index = 0; index < firstMeshSize; index++)
		meshes.push_back(first.meshes[index]);

	auto secondMeshSize = sizeof(second.meshes)/sizeof(second.meshes[0]);
	for (auto index = 0; index < secondMeshSize; index++)
		meshes.push_back(second.meshes[index]);

	auto model = Model();
	model.meshCount = first.meshCount + second.meshCount;
	model.materialCount = first.materialCount + second.materialCount;

	model.meshMaterial = first.meshMaterial;
	model.meshes = meshes.data();
	model.materials = materials.data();

	return model;
}
