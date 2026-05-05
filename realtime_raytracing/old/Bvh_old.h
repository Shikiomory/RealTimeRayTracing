//отказ от make_shared
//#pragma once
//#include "objects/HittableList.h"
//#include <algorithm>
//
//class Bvh : public Hittable
//{
//	shared_ptr<Hittable> left;
//	shared_ptr<Hittable> right;
//	Aaab bbox;
//
//public:
//	Bvh(HittableList list) : Bvh(list.objects, 0, list.objects.size()) {}
//
//	Bvh(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
//	//TODO
//		bbox = Aaab::empty;
//		for (size_t i = start; i < end; ++i) {
//			bbox = Aaab(bbox, objects[i]->bounding_box());
//		}
//
//		int axis = bbox.longest_axis();
//
//		auto comparator = (axis == 0) ? box_x_compare
//					    : (axis == 1) ? box_y_compare
//							          : box_z_compare;
//
//		size_t object_span = end - start;
//
//		if (object_span == 1) {
//			left = right = objects[start];
//		}
//		else if (object_span == 2) {
//			left = objects[start];
//			right = objects[end];
//		}
//		else {
//			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
//
//			auto mid = start + object_span / 2;
//			left = make_shared<Bvh>(objects, start, mid);
//			right = make_shared<Bvh>(objects, mid, end);
//		}
//	}
//
//
//	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
//		if (!bbox.hit(r, t_int)) { return false; }
//
//		bool hit_left = left->hit(r, t_int, rec);
//		bool hit_right = left->hit(r, Interval(t_int.min, hit_left ? rec.t : t_int.max), rec);
//
//		return hit_left || hit_right;
//	}
//
//
//	static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index) {
//		Interval a_axis_interval = a->bounding_box().axis_interval(axis_index);
//		Interval b_axis_interval = b->bounding_box().axis_interval(axis_index);
//		return a_axis_interval.min < b_axis_interval.min;
//	}
//
//	static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 0);
//	}
//
//	static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 1);
//	}
//
//	static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 2);
//	}
//
//};
//


//Пробелмы с variant, надо костыли ставить
//#pragma once
//#include "objects/HittableList.h"
//#include <algorithm>
//
//class Bvh
//{
//	unique_ptr<HittableObject> left;
//	unique_ptr<HittableObject> right;
//	Aaab bbox;
//
//public:
//	Bvh(HittableList list) : Bvh(list.objects, 0, list.objects.size()) {}
//
//	Bvh(std::vector<HittableObject>& objects, size_t start, size_t end) {
//		bbox = Aaab::empty;
//		for (size_t i = start; i < end; ++i) {
//			Aaab object_bbox = std::visit([&](const auto& obj) { return obj.bounding_box();}, objects[i]);
//			bbox = Aaab(bbox, object_bbox);
//		}
//
//		int axis = bbox.longest_axis();
//
//		auto comparator = (axis == 0) ? box_x_compare
//			: (axis == 1) ? box_y_compare
//			: box_z_compare;
//
//		size_t object_span = end - start;
//
//		if (object_span == 1) {
//			left = std::make_unique<HittableObject>(objects[start]);
//			right = make_unique<HittableObject>(objects[start]);
//		}
//
//		else if (object_span == 2) {
//			left = std::make_unique<HittableObject>(objects[start]);
//			right = std::make_unique<HittableObject>(objects[end]);
//		}
//
//		else {
//			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
//
//			auto mid = start + object_span / 2;
//
//			left = std::make_unique<HittableObject>(Bvh(objects, start, mid));
//			right = std::make_unique<HittableObject>(Bvh(objects, mid, end));
//
//			left = make_shared<Bvh>(objects, start, mid);
//			right = make_shared<Bvh>(objects, mid, end);
//		}
//	}
//
//
//	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
//		if (!bbox.hit(r, t_int)) { return false; }
//
//		bool hit_left = left->hit(r, t_int, rec);
//		bool hit_right = left->hit(r, Interval(t_int.min, hit_left ? rec.t : t_int.max), rec);
//
//		return hit_left || hit_right;
//	}
//
//
//	static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index) {
//		Interval a_axis_interval = a->bounding_box().axis_interval(axis_index);
//		Interval b_axis_interval = b->bounding_box().axis_interval(axis_index);
//		return a_axis_interval.min < b_axis_interval.min;
//	}
//
//	static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 0);
//	}
//
//	static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 1);
//	}
//
//	static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
//		box_compare(a, b, 2);
//	}
//
//};