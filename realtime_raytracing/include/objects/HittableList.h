#pragma once

#include "objects/Hittable.h"
#include <vector>
#include <variant>
#include "objects/Cilinder.h"
#include "objects/Sphere.h"
#include "objects/Plane.h"

using HittableObject = std::variant<Sphere, Plane, Cilinder>;

class HittableList: public Hittable
{
public:
	std::vector<HittableObject> objects;

	//конструкторы
	HittableList() = default;
	HittableList(HittableObject object) { add(std::move(object)); }

	void clear() {
		objects.clear();
	}

	void add(HittableObject object) {
		objects.push_back(std::move(object));
	}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		float closet_so_far = t_int.max;

		for (const auto& object : objects) {
			if (std::visit([&](const auto& obj) { return obj.hit(r, Interval(t_int.min, closet_so_far), temp_rec); }, object)) {
				closet_so_far = temp_rec.t;
				hit_anything = true;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	bool any_hit(const Ray& r, Interval t_int) const override {
		bool hit_anything = false;
		for (const auto& object : objects) {
			if (std::visit([&](const auto& obj) { return obj.any_hit(r, t_int); }, object)) {
				hit_anything = true;
			}
		}
		return hit_anything;
	}
};

