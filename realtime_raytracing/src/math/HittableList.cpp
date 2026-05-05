#include "objects/HittableList.h"
#include "bvh/Bvh.h"

	//конструкторы
	HittableList::HittableList(HittableObject object) { add(std::move(object)); }
	HittableList::~HittableList() = default;

	void HittableList::clear() {
		objects.clear();
	}

	void HittableList::add(HittableObject object) {
		//Aaab object_box = std::visit([&](const auto& obj) { return obj.bounding_box();}, object);
		Aaab object_bbox = std::visit([&](const auto& obj) -> Aaab {
			if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->bounding_box(); }
			else { return obj.bounding_box(); }
			}, object);

		objects.push_back(std::move(object));
		bbox = Aaab(bbox, object_bbox);
	}

	bool HittableList::hit(const Ray& r, Interval t_int, hit_record& rec) const {
		hit_record temp_rec;
		bool hit_anything = false;
		float closet_so_far = t_int.max;

		for (const auto& object : objects) {
			//if (std::visit([&](const auto& obj) { return obj.hit(r, Interval(t_int.min, closet_so_far), temp_rec); }, object)) {
			bool hit = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->hit(r, Interval(t_int.min, closet_so_far), temp_rec); }
				else { return obj.hit(r, Interval(t_int.min, closet_so_far), temp_rec); }
				}, object);
			if (hit) {
				closet_so_far = temp_rec.t;
				hit_anything = true;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	bool HittableList::any_hit(const Ray& r, Interval t_int) const {
		for (const auto& object : objects) {
			bool hit = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->any_hit(r, t_int); }
				else { return obj.any_hit(r, t_int); }
				}, object);
			//if (std::visit([&](const auto& obj) { return obj.any_hit(r, t_int); }, object)) {
			if (hit) { return true; }
		}
		return false;
	}

