#pragma once
#include "Utility.h"

class MovCamera
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

	Point3 look_from;
	Point3 look_at;
	Vector3 vup;
	Vector3 w, u, v;
	float vfov;
	int samples_per_pixel = 4;
	float pixel_samples_scale = 1;


	Vector3 sample_square() const {
		return Vector3(fast_random(-0.5f, 0.5f), fast_random(-0.5f, 0.5f), 0);
		//return Vector3(random(-0.5f, 0.5f), random(-0.5f, 0.5f), 0);
	}

public:

	MovCamera() = default;
	MovCamera(int _width, float _ratio, Point3 _look_from, Point3 _look_at, Vector3 _vup, float _vfov): look_from(_look_from), look_at(_look_at), vup(_vup), vfov(_vfov) {
		width = _width;
		height = static_cast<int>(_width / _ratio);
		height = (height < 1) ? 1 : height;

		update();

		pixel_samples_scale = 1.0f / samples_per_pixel;
	}

		void update() {
			w = (look_from - look_at).normalize();
			u = cross(vup, w).normalize();
			v = cross(w, u);
		
			float h = std::tan(pi / 360 * vfov);
			viewport_height = 2 * h * focal_length;

			float viewport_width = viewport_height * (static_cast<float>(width) / height);
			Vector3 viewport_u = viewport_width * u;
			Vector3 viewport_v = -viewport_height * v;

			upper_left_point = look_from - w * focal_length - viewport_u / 2.0f - viewport_v / 2.0f;

			//получение размеров пикселя.
			pixel_delta_u = viewport_u / static_cast<float>(width);
			pixel_delta_v = viewport_v / static_cast<float>(height);

			pixel00_loc = upper_left_point + 0.5f * (pixel_delta_u + pixel_delta_v);
		}

	void mov_cam(Vector3 dist) {
		look_from += dist;
		look_at += dist;
		update();
	}

	void rot_cam(Vector3 dir) {
		look_at = look_from + dir;
		update();
	}

	//получение луча из пикселя
	Ray get_ray(const int x, const int y) const {
		Vector3 offset = sample_square();
		Point3 pixel_sample = pixel00_loc + ((static_cast<float>(x) + offset.x) * pixel_delta_u) + ((static_cast<float>(y) + offset.y) * pixel_delta_v);
		//Point3 pixel_sample = pixel00_loc + (static_cast<float>(x) * pixel_delta_u) + (static_cast<float>(y) * pixel_delta_v);
		Vector3 ray_direction = pixel_sample - look_from;
	
		return Ray(look_from, ray_direction);
	}

	inline int get_width() const { return width; }
	inline int get_height() const { return height; }
	inline int get_samples() const { return samples_per_pixel; }
	inline float get_samples_scale() const { return pixel_samples_scale; }
	inline Vector3 get_w() { return w; }
	inline Vector3 get_u() { return u; }
	inline Vector3 get_v() { return v; }

};

