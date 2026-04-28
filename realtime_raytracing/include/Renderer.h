#pragma once
#include <vector>
#include <SFML/Config.hpp>
#include "Camera.h"
#include "objects/Hittable.h"
#include <omp.h>

class Renderer
{
	Color3 ray_color(const Ray& r, const Hittable& world);
	void set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color);

public:
	void Render(std::vector<sf::Uint8>& buffer, const Camera& cam, const Hittable& world);
};

