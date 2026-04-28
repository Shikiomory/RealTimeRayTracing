#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
	Point3 center = Point3(0.0f, 0.0f, 0.0f);
	float radius = 0.0f;
	shared_ptr<Material> mat;

public:
	//конструкторы
	Sphere() = default;
	Sphere(const Point3& _center, float _radius, shared_ptr<Material> _mat) : center(_center), radius(std::fmax(_radius, 0.0f)), mat(_mat) {}

	//проверка на попадание луча (наследование от класса Object потом)
	bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
		Vector3 oc = center - r.get_origin(); //луч от центра к камере

		//коэффициенты квадратного уравнения для вычисления пересечения луча и сферы
		float a = r.get_direction().squareLength();
		float h = dot(r.get_direction(), oc); // b / 2
		float c = oc.squareLength() - radius * radius;

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
		Vector3 normal = (r.at(root) - center) / radius;
		rec.set_face_normal(r, normal);
		rec.mat = mat;

		return true;
	}
};

