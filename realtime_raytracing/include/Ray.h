#pragma once

#include "Vector3.h"

class Ray
{
	Point3 origin;
	Vector3 direction;

public:

	//конструкторы
	Ray() = default;
	Ray(const Point3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}

	//получение точки на луче
	Point3 at(float t) const {
		return origin + t * direction;
	}
	
	//геттеры
	const Point3& get_origin() const { return origin; }
	const Vector3& get_direction() const { return direction; }

};

