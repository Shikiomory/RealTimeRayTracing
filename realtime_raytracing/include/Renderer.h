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
	std::vector<Color3> colorBuffer;
	std::vector<Color3> temporalBuffer;
	int frames = 1;

	Color3 ray_color(const Ray& r, const Scene& scene);
	void set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color);
	bool is_different(const std::vector<Color3>& buffer, int idx_1, int idx_2);
	//bool need_sampling(const std::vector<sf::Uint8>& buffer, int index, int width, int height);
public:
	Renderer(const MovCamera& cam) {
		int size = cam.get_render_height() * cam.get_render_width();
		colorBuffer.resize(size);
		temporalBuffer.resize(size);
	}

	void Render(std::vector<sf::Uint8>& buffer, const MovCamera& cam, const Scene& scene);
};

inline float linear_to_gamma(float linear_component) {
	if (linear_component > 0.0f) {
		return std::sqrt(linear_component);
	}
	return 0.0f;
}
