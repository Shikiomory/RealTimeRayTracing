#pragma once

#include "objects/Hittable.h"
#include <vector>
//#include <variant>

//using objects = std::variant<Sphere, Plane, Cilinder>;

class HittableList: public Hittable
{
public:
	std::vector<std::unique_ptr<Hittable>> objects;

	//конструкторы
	HittableList() = default;
	HittableList(std::unique_ptr<Hittable> object) { add(std::move(object)); }

	void clear() {
		objects.clear();
	}

	void add(std::unique_ptr<Hittable> object) {
		objects.push_back(std::move(object));
	}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		float closet_so_far = t_int.max;

		for (const auto& object : objects) {
			if (object->hit(r, Interval(t_int.min, closet_so_far), temp_rec)) {	
				closet_so_far = temp_rec.t;
				hit_anything = true;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};

