#pragma once

#include "objects/HittableList.h"
#include "lights/PointLight.h"
struct Scene
{
	HittableList world;
	std::vector<PointLight> lights;
};
