#pragma once
#include "objects/HittableList.h"

class Bvh: public Hittable
{
	std::unique_ptr<HittableObject> left;
	std::unique_ptr<HittableObject> right;
	Aaab bbox;

public:
	Bvh(HittableList& list);
	Bvh(std::vector<HittableObject>& objects, size_t start, size_t end);	
	~Bvh();

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override;
	bool any_hit(const Ray& r, Interval t_int) const override;

	Aaab bounding_box() const override { return bbox; }

	static bool box_compare(const HittableObject& a, const HittableObject& b, int axis_index);
	static bool box_x_compare(const HittableObject& a, const HittableObject& b);	
	static bool box_y_compare(const HittableObject& a, const HittableObject& b);
	static bool box_z_compare(const HittableObject& a, const HittableObject& b);

};
