#pragma once
#include <vector>
#include <SFML/Config.hpp>
#include "Camera.h"
#include "objects/Hittable.h"

class Renderer
{
	Color3 ray_color(const Ray& r, const Hittable&);

public:
	void Render(std::vector<sf::Uint8>& buffer, const Camera& cam, const Hittable& world);
};

