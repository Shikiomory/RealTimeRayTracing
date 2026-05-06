#pragma once
#include "Utility.h"

class MovCamera
{
	int window_width;
	int window_height;

	float scale = 0.7f;
	int render_width;
	int render_height;

	float viewport_height = 2.0f;
	float focal_length = 1.0f;
	Point3 camera_center = Point3(0.0f, 0.0f, 0.0f);
	Vector3 pixel_delta_u;
	Vector3 pixel_delta_v;
	Point3 upper_left_point;
	Point3 pixel00_loc;

	Point3 look_from, last_look_from;
	Point3 look_at, last_look_at;
	Vector3 vup;
	Vector3 w, u, v;
	float vfov;
	int samples_per_pixel = 4;
	float pixel_samples_scale = 1;

	bool moved = false;

	Vector3 sample_square() const {
		return Vector3(fast_random(-0.5f, 0.5f), fast_random(-0.5f, 0.5f), 0);
		//return Vector3(random(-0.5f, 0.5f), random(-0.5f, 0.5f), 0);
	}

public:

	MovCamera() = default;
	MovCamera(int _width, float _ratio, Point3 _look_from, Point3 _look_at, Vector3 _vup, float _vfov): look_from(_look_from), look_at(_look_at), vup(_vup), vfov(_vfov) {
		window_width = _width;
		window_height = std::max(1, static_cast<int>(_width / _ratio));

		render_width = window_width * scale;
		render_height= window_height * scale;
		update();

		pixel_samples_scale = 1.0f / samples_per_pixel;
	}

		void update() {
			w = (look_from - look_at).normalize();
			u = cross(vup, w).normalize();
			v = cross(w, u);
		
			if (last_look_at != look_at || last_look_from != look_from) {
				last_look_at = look_at;
				last_look_from = look_from;
				moved = true;
			}
			else {
				moved = false;
			}

			float h = std::tan(pi / 360 * vfov);
			viewport_height = 2 * h * focal_length;

			float viewport_width = viewport_height * (static_cast<float>(window_width) / window_height);
			Vector3 viewport_u = viewport_width * u;
			Vector3 viewport_v = -viewport_height * v;

			upper_left_point = look_from - w * focal_length - viewport_u / 2.0f - viewport_v / 2.0f;

			//получение размеров пикселя.
			pixel_delta_u = viewport_u / static_cast<float>(render_width);
			pixel_delta_v = viewport_v / static_cast<float>(render_height);

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
	Ray get_ray(const int x, const int y, bool random) const {
		Vector3 offset = random ? sample_square() : Vector3(1.0f, 1.0f, 1.0f);
		Point3 pixel_sample = pixel00_loc + ((static_cast<float>(x) + offset.x) * pixel_delta_u) + ((static_cast<float>(y) + offset.y) * pixel_delta_v);
		//Point3 pixel_sample = pixel00_loc + (static_cast<float>(x) * pixel_delta_u) + (static_cast<float>(y) * pixel_delta_v);
		Vector3 ray_direction = pixel_sample - look_from;
	
		return Ray(look_from, ray_direction);
	}

	inline bool is_moved() const { return moved; }
	inline int get_window_width() const { return window_width; }
	inline int get_window_height() const { return window_height; }
	inline int get_render_width() const { return render_width; }
	inline int get_render_height() const { return render_height; }
	inline int get_samples() const { return samples_per_pixel; }
	inline float get_samples_scale() const { return pixel_samples_scale; }
	inline Vector3 get_w() const { return w; }
	inline Vector3 get_u() const { return u; }
	inline Vector3 get_v() const { return v; }

};

