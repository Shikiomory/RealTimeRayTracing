#pragma once

#include "Constants.h"
struct Interval
{
	float min, max;

	
	//конструкторы
	Interval() : min(+infty), max(-infty) {}
	Interval(float _min, float _max) : min(_min), max(_max) {}

	float size() const {
		return max - min;
	}

	bool contains(float t) {
		return min <= t && max >= t;
	}

	bool surrounds(float t) {
		return min < t && max > t;
	}

	float clamp(float x) const {
		if (x > max) return max;
		if (x < min) return min;
		return x;
	}

	Interval expand(float delta) {
		float padding = delta / 2.0f;
		return Interval(min - padding, max + padding);
	}
	static const Interval empty, universe;
};


inline const Interval Interval::empty = Interval(+infty, -infty);
inline const Interval Interval::universe = Interval(-infty, +infty);



