#pragma once
#include "utility.h"

class Aaab
{
public:
	Interval x, y, z;

	Aaab() = default;

	Aaab(const Interval& _x, const Interval& _y, const Interval& _z): x(_x), y(_y), z(_z) {}

	Aaab(const Point3& a, const Point3& b) {
		x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
		y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
		z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);
	} 

	Aaab(const Aaab& bbox_1, const Aaab& bbox_2) {
		x = Interval(bbox_1.x, bbox_2.x);
		y = Interval(bbox_1.y, bbox_2.y);
		z = Interval(bbox_1.z, bbox_2.z);
	}

	const Interval& axis_interval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const Ray& r, Interval t_int) const {
		const Point3& ray_orig = r.get_origin();
		const Point3& ray_dir = r.get_direction();

		for (int axis = 0; axis < 3; ++axis) {
			const Interval& ax = axis_interval(axis);
			const float adinv = 1.0f / ray_dir[axis];

			float t_0 = (ax.min - ray_orig[axis]) * adinv;
			float t_1 = (ax.max - ray_orig[axis]) * adinv;

			if (t_0 < t_1) {
				if (t_0 > t_int.min) t_int.min = t_0;
				if (t_1 < t_int.max) t_int.max = t_1;
			}
			else {
				if (t_1 > t_int.min) t_int.min = t_1;
				if (t_0 < t_int.max) t_int.max = t_0;
			}

			if (t_int.min >= t_int.max) { return false; }
		}
		return true;
	}

	int longest_axis() const {
		if (x.size() > y.size()) {
			return z.size() > x.size() ? 2 : 0;
		}
		else
			return y.size() > z.size() ? 1 : 2;
	}

	static const Aaab empty, universe;
};

inline const Aaab Aaab::empty = Aaab(Interval::empty, Interval::empty, Interval::empty);
inline const Aaab Aaab::universe = Aaab(Interval::universe, Interval::universe, Interval::universe);

	