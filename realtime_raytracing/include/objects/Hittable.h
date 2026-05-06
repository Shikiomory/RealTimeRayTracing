#pragma once
#include "Utility.h"	
#include  "materials\Material.h"
#include "bvh/Aaab.h"
struct hit_record {
	Point3 p;
	Vector3 normal;
	float t;
	bool front_face;
	//const Material* mat;
	uint32_t mat_id;

	//разворот нормали всегда против направления луча
	void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		front_face = dot(r.get_direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{

public:

	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval t_int, hit_record& rec) const = 0;

	virtual bool any_hit(const Ray& r, Interval t_int) const = 0;

	virtual Aaab bounding_box() const = 0;

};

