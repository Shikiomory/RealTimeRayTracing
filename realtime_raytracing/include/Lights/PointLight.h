#pragma once
#include "Utility.h"

struct PointLight
{
	Point3 position;
	Color3 color;
	float intensity;

	PointLight() = default;
	PointLight(Point3 _position, Color3 _color, float _intensity) : position(_position), color(_color), intensity(_intensity) {}
};

