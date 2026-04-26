#pragma once

#include "objects/Hittable.h"
#include <vector>


class HittableList: public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> objects;

	//конструкторы
	HittableList() = default;
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() {
		objects.clear();
	}

	void add(shared_ptr<Hittable> object) {
		objects.push_back(object);
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

