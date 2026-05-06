#pragma once

#include "objects/HittableList.h"
#include "lights/PointLight.h"
#include "materials/PhongMaterial.h"
struct Scene
{
	HittableList world;
	std::vector<PointLight> lights;
	std::vector<PhongMaterial> materials;

	uint32_t add_material(const PhongMaterial& mat) {
		materials.push_back(mat);
		return static_cast<uint32_t>(materials.size() - 1);
	}
};
