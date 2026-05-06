#pragma once
#include "Hittable.h"

class Plane
{
	Point3 pos;
	Vector3 normal;
	//shared_ptr<Material> mat;
	uint32_t mat_id;
	Aaab bbox;

public:
	Plane() = default;
	Plane(const Point3& _pos, const Vector3& _normal, uint32_t _mat_id): pos(_pos), normal(_normal), mat_id(_mat_id) {}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const {
		float denom = dot(normal, r.get_direction());
		if (std::abs(denom) < 1e-6f) {
			return false;
		}

		float root = dot(normal, pos - r.get_origin()) / denom;

		if (!t_int.contains(root)) {
			return false;
		}

		rec.t = root;
		rec.p = r.at(root);
		rec.set_face_normal(r, normal);
		//rec.mat = mat.get();
		rec.mat_id = mat_id;

		return true;
	}

	bool any_hit(const Ray& r, Interval t_int) const {
		float denom = dot(normal, r.get_direction());
		if (std::abs(denom) < 1e-6f) {
			return false;
		}

		float root = dot(normal, pos - r.get_origin()) / denom;

		if (!t_int.contains(root)) {
			return false;
		}
		return true;
	}

	Aaab bounding_box() const { return bbox; }
};

