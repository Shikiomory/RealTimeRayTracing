#pragma once

#include "objects/Hittable.h"
#include <vector>
#include <variant>
#include "objects/Cilinder.h"
#include "objects/Sphere.h"
#include "objects/Plane.h"
#include "objects/Box.h"

//проверка на этапе компиляции на тип: указатель или простая переменная
template<typename T> struct is_unique_ptr : std::false_type {};
template<typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
template<typename T> inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

class Bvh;

using HittableObject = std::variant<Sphere, Box, Plane, Cilinder, std::unique_ptr<Bvh>>;

class HittableList: public Hittable
{
	Aaab bbox;
public:
	std::vector<HittableObject> objects;

	//конструкторы
	HittableList() = default;
	HittableList(HittableObject object);
	~HittableList();
	
	void clear();
	void add(HittableObject object);

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override;
	bool any_hit(const Ray& r, Interval t_int) const override;

	Aaab bounding_box() const override { return bbox; }
};



//#pragma once
//
//#include "objects/Hittable.h"
//#include <vector>
//#include <variant>
//#include "objects/Cilinder.h"
//#include "objects/Sphere.h"
//#include "objects/Plane.h"
//
////проверка на этапе компиляции на тип: указатель или простая переменная
//template<typename T> struct is_unique_ptr : std::false_type {};
//template<typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
//template<typename T> inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;
//
//class Bvh;
//
//using HittableObject = std::variant<Sphere, Plane, Cilinder, std::unique_ptr<Bvh>>;
//
//class HittableList : public Hittable
//{
//	Aaab bbox;
//public:
//	std::vector<HittableObject> objects;
//
//	//конструкторы
//	HittableList() = default;
//	HittableList(HittableObject object) { add(std::move(object)); }
//
//	void clear() {
//		objects.clear();
//	}
//
//	void add(HittableObject object) {
//		//Aaab object_box = std::visit([&](const auto& obj) { return obj.bounding_box();}, object);
//		Aaab object_bbox = std::visit([&](const auto& obj) -> Aaab {
//			if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->bounding_box(); }
//			else { return obj.bounding_box(); }
//			}, object);
//
//		objects.push_back(std::move(object));
//		bbox = Aaab(bbox, object_bbox);
//	}
//
//	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
//		hit_record temp_rec;
//		bool hit_anything = false;
//		float closet_so_far = t_int.max;
//
//		for (const auto& object : objects) {
//			//if (std::visit([&](const auto& obj) { return obj.hit(r, Interval(t_int.min, closet_so_far), temp_rec); }, object)) {
//			bool hit = std::visit([&](const auto& obj) -> bool {
//				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->hit(r, Interval(t_int.min, closet_so_far), temp_rec); }
//				else { return obj.hit(r, Interval(t_int.min, closet_so_far), temp_rec); }
//				}, object);
//			if (hit) {
//				closet_so_far = temp_rec.t;
//				hit_anything = true;
//				rec = temp_rec;
//			}
//		}
//		return hit_anything;
//	}
//
//	bool any_hit(const Ray& r, Interval t_int) const override {
//		for (const auto& object : objects) {
//			bool hit = std::visit([&](const auto& obj) -> bool {
//				if constexpr (is_unique_ptr_v<std::decay_t<decltype(obj)>>) { return obj->any_hit(r, t_int); }
//				else { return obj.any_hit(r, t_int); }
//				}, object);
//			//if (std::visit([&](const auto& obj) { return obj.any_hit(r, t_int); }, object)) {
//			if (hit) { return true; }
//		}
//		return false;
//	}
//
//	Aaab bounding_box() const override { return bbox; }
//};
//
