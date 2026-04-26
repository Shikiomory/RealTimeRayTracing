#pragma once
#include "Utility.h"

class Camera
{
	int width;
	int height;

	float viewport_height = 2.0f;
	float focal_length = 1.0f;
	Point3 camera_center = Point3(0.0f, 0.0f, 0.0f);
	Vector3 pixel_delta_u;
	Vector3 pixel_delta_v;
	Point3 upper_left_point;
	Point3 pixel00_loc;
public:

	Camera() = default;
	//Camera(int _width, int _height): width(_width), height(_height) {}
	Camera(int _width, float _ratio) {
		width = _width;
		height = static_cast<int>(_width / _ratio);
		height = (height < 1) ? 1 : height;

		//ширина вьюпорта
		float viewport_width = viewport_height * (static_cast<float>(width) / height);

		//векторы вдоль вьюпорта
		Vector3 viewport_u = Vector3(viewport_width, 0, 0);
		Vector3 viewport_v = Vector3(0, -viewport_height, 0);

		//получение размеров пикселя.
		pixel_delta_u = viewport_u / static_cast<float>(width);
		pixel_delta_v = viewport_v / static_cast<float>(height);

		//поллучение начального пикселя
		upper_left_point = camera_center - Vector3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
		pixel00_loc = upper_left_point + 0.5f * (pixel_delta_u + pixel_delta_v);
	}

	//получение луча из пикселя
	Ray get_ray(const int x, const int y) const{
		Point3 pixel_center = pixel00_loc + (static_cast<float>(x) * pixel_delta_u) + (static_cast<float>(y) * pixel_delta_v);
		Vector3 ray_direction = pixel_center - camera_center;

		return Ray(pixel_center, ray_direction);
	}

	int get_width() const { return width; }
	int get_height() const { return height; }
};

