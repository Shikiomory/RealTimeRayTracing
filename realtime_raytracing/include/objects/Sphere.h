#pragma once
#include "Hittable.h"

class Sphere
{
	Point3 center = Point3(0.0f, 0.0f, 0.0f);
	float radius = 0.0f;
	float radius_sqr = 0.0f;
	//shared_ptr<Material> mat;
	uint32_t mat_id;
	Aaab bbox;

	bool find_t(const Ray& r, Interval t_int, float& root) const {
		Vector3 oc = center - r.get_origin(); //луч от центра к камере

		//коэффициенты квадратного уравнени€ дл€ вычислени€ пересечени€ луча и сферы
		float a = r.get_direction().squareLength();
		float h = dot(r.get_direction(), oc); // b / 2
		float c = oc.squareLength() - radius * radius;

		//вычисление дискриминанта / 4
		float discriminant = h * h - a * c;

		if (discriminant < 0) {
			return false;
		}

		float sqrt_d = std::sqrt(discriminant);

	    root = (h - sqrt_d) / a; //поиск t

		if (!t_int.surrounds(root)) {
			root = (h + sqrt_d) / a;
			if (!t_int.surrounds(root)) {
				return false;
			}
		}
		return true;
	}

public:
	//конструкторы
	Sphere() = default;
	Sphere(const Point3& _center, float _radius, uint32_t _mat_id) : center(_center), radius(std::fmax(_radius, 0.0f)), mat_id(_mat_id) {
		radius_sqr = radius * radius;
		Vector3 rvec = Vector3(radius, radius, radius);
		bbox = Aaab(center - rvec, center + rvec);
	}

	//проверка на попадание луча (наследование от класса Object потом)
	bool hit(const Ray& r, Interval t_int, hit_record& rec) const {
		Vector3 oc = center - r.get_origin(); //луч от центра к камере

		//коэффициенты квадратного уравнени€ дл€ вычислени€ пересечени€ луча и сферы
		float a = r.get_direction().squareLength();
		float h = dot(r.get_direction(), oc); // b / 2
		float c = oc.squareLength() - radius_sqr;

		//вычисление дискриминанта / 4
		float discriminant = h * h - a * c;

		if (discriminant < 0) {
			return false;
		}

		float sqrt_d = std::sqrt(discriminant);

		float root = (h - sqrt_d) / a; //поиск t

		if (!t_int.surrounds(root)) {
			root = (h + sqrt_d) / a;
			if (!t_int.surrounds(root)) {
				return false;
			}
		}

		//запись данных о точке найденной
		rec.t = root;
		rec.p = r.at(root);
		Vector3 normal = (rec.p - center) / radius;
		rec.set_face_normal(r, normal);
		rec.mat_id = mat_id;

		return true;
	}

	bool any_hit(const Ray& r, Interval t_int) const {
		float root;
		return find_t(r, t_int, root);
	}

	Aaab bounding_box() const { return bbox; }
};

