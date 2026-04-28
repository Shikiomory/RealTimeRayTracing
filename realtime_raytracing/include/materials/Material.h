#pragma once

#include "Utility.h"
#include <vector>
class PointLight;
class Ray;
struct hit_record;
class Hittable;

class Material
{

public:
	virtual ~Material() = default;

	virtual Color3 shade(const Ray& r, const hit_record& rec, const std::vector<PointLight>& lights, const Hittable& world) const = 0;
};

