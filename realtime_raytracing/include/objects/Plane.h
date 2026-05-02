#pragma once
#include "Hittable.h"

class Plane: public Hittable
{
	Point3 pos;
	Vector3 normal;
	shared_ptr<Material> mat;

public:
	Plane() = default;
	Plane(const Point3& _pos, const Vector3& _normal, shared_ptr<Material> _mat): pos(_pos), normal(_normal), mat(_mat) {}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
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
		rec.mat = mat.get();

		return true;
	}
};

