#include <algorithm>
#include <bvh/Bvh.h>

	Bvh::Bvh(HittableList& list) : Bvh(list.objects, 0, list.objects.size()) {}

	Bvh::Bvh(std::vector<HittableObject>& objects, size_t start, size_t end) {
		bbox = Aaab::empty;
		for (size_t i = start; i < end; ++i) {
			//Aaab object_bbox = std::visit([&](const auto& obj) { return obj.bounding_box();}, objects[i]);
			Aaab object_bbox = std::visit([&](const auto& obj) -> Aaab {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->bounding_box(); }
				else { return obj.bounding_box(); }
				}, objects[i]);
			bbox = Aaab(bbox, object_bbox);
		}

		int axis = bbox.longest_axis();

		auto comparator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
			: box_z_compare;

		size_t object_span = end - start;

		if (object_span == 1) {
			left = std::make_unique<HittableObject>(std::move(objects[start]));
			//right = std::make_unique<HittableObject>(*left);
			right = nullptr;
		}

		else if (object_span == 2) {
			left = std::make_unique<HittableObject>(std::move(objects[start]));
			right = std::make_unique<HittableObject>(std::move(objects[start + 1]));
		}

		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + object_span / 2;

			left = std::make_unique<HittableObject>(std::make_unique<Bvh>(objects, start, mid));
			right = std::make_unique<HittableObject>(std::make_unique<Bvh>(objects, mid, end));
		}
	}

	Bvh::~Bvh() = default;
	bool Bvh::hit(const Ray& r, Interval t_int, hit_record& rec) const {
		if (!bbox.hit(r, t_int)) { return false; }

		//bool hit_left = left->hit(r, t_int, rec);
		bool hit_left = false;
		if (left != nullptr) {
			hit_left = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->hit(r, t_int, rec); }
				else { return obj.hit(r, t_int, rec); }
				}, *left);
		}
		//bool hit_right = right->hit(r, Interval(t_int.min, hit_left ? rec.t : t_int.max), rec);
		bool hit_right = false;
		if (right != nullptr) {
			hit_right = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->hit(r, Interval(t_int.min, hit_left ? rec.t : t_int.max), rec); }
				else { return obj.hit(r, Interval(t_int.min, hit_left ? rec.t : t_int.max), rec); }
				}, *right);
		}

		return hit_left || hit_right;
	}

	bool Bvh::any_hit(const Ray& r, Interval t_int) const  {
		if (!bbox.hit(r, t_int)) { return false; }

		//bool hit_left = left->any_hit(r, t_int);
		bool hit_left = false;
		if (left != nullptr) {
			hit_left = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->any_hit(r, t_int); }
				else { return obj.any_hit(r, t_int); }
				}, *left);
		}
		bool hit_right = false;
		if (right != nullptr) {
			hit_right = std::visit([&](const auto& obj) -> bool {
				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->any_hit(r, t_int); }
				else { return obj.any_hit(r, t_int); }
				}, *right);
		}

		return hit_left || hit_right;

	}

	bool Bvh::box_compare(const HittableObject& a, const HittableObject& b, int axis_index) {
		Aaab bbox_a = std::visit([&](const auto& obj) -> Aaab {
			if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->bounding_box(); }
			else { return obj.bounding_box(); }
			}, a);
		Aaab bbox_b = std::visit([&](const auto& obj) -> Aaab {
			if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->bounding_box(); }
			else { return obj.bounding_box(); }
			}, b);
		Interval a_axis_interval = bbox_a.axis_interval(axis_index);
		Interval b_axis_interval = bbox_b.axis_interval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}

	bool Bvh::box_x_compare(const HittableObject& a, const HittableObject& b) {
		return box_compare(a, b, 0);
	}

	bool Bvh::box_y_compare(const HittableObject& a, const HittableObject& b) {
		return box_compare(a, b, 1);
	}

	bool Bvh::box_z_compare(const HittableObject& a, const HittableObject& b) {
		return box_compare(a, b, 2);
	}
