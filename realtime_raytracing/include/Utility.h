#pragma once

#include <cmath>
#include <memory>
#include <iostream>
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

