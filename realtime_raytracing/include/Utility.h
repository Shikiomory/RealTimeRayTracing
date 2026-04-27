#pragma once

#include <cmath>
#include <memory>
#include <iostream>
#include <random>

#include "Ray.h"
#include "Vector3.h"
#include "Interval.h"
#include "Constants.h"
using std::make_shared;
using std::shared_ptr; //для автоматического удаления объектов после прекращения использования

//константы



//функции

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0f;
}

inline float random() {
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random(float min, float max) {
	return min + (max - min) * random();
}

inline float fast_random() {
	static thread_local uint32_t x = 123456789, y = 362436069, z = 521288629;
	uint32_t t;
	x ^= x << 16; x ^= x >> 5; x ^= x << 1;
	t = x; x = y; y = z; z = t ^ x ^ y;
	return static_cast<float>(z) * (1.0f / 4294967296.0f);
}

inline float fast_random(float min, float max) {
	return min + (max - min) * fast_random();
}

