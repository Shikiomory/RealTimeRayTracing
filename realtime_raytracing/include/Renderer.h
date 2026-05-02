#pragma once
#include <vector>
#include <SFML/Config.hpp>
#include "Camera\Camera.h"
#include "Camera\MovCamera.h"
#include "objects/Hittable.h"
#include <omp.h>
#include "scene/Scene.h"
class Renderer
{
	Color3 ray_color(const Ray& r, const Scene& scene);
	void set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color);

public:
	void Render(std::vector<sf::Uint8>& buffer, const MovCamera& cam, const Scene& scene);
};

inline float linear_to_gamma(float linear_component) {
	if (linear_component > 0.0f) {
		return std::sqrt(linear_component);
	}
	return 0.0f;
}
