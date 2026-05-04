#pragma once

#include <cmath>
#include <iostream>

struct Vector3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	//конструкторы
	Vector3() = default;
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	//арифметические операции
	Vector3 operator+ (const Vector3& vec) const {
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 operator- (const Vector3& vec) const {
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	Vector3 operator* (float scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 operator/ (float scalar) const {
		if (std::abs(scalar) > 1e-8f) {
			float div = 1.0f / scalar;
			return Vector3(x * div, y * div, z * div);
		}
		return Vector3(0, 0, 0);
	}

	//обращение по иднексу
	float operator[](int i) const {
		if (i == 1) return y;
		if (i == 2) return z;
		return x;
	}
	float& operator[](int i) {
		if (i == 1) return y;
		if (i == 2) return z;
		return x;
	}

	//операторы присвоения
	Vector3& operator= (const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	Vector3& operator+= (const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& operator-= (const Vector3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator*= (float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	
	Vector3& operator/= (float scalar) {
		if (std::abs(scalar) > 1e-8f) {
			float div = 1.0f / scalar;
			x *= div;
			y *= div;
			z *= div;
		}
		else {
			x = 0; y = 0; z = 0;
		}
		return *this;
	}


	//операции с вектором

	float squareLength() const {
		return x * x + y * y + z * z;
	}

	float length() const {
		return std::sqrtf(squareLength());
	}

	Vector3 normalize() const {
		float len = length();
		if (len > 0) {
			len = 1.0f / len;
			return Vector3(x * len, y * len, z * len);
		}
		return Vector3(0, 0, 0);
	}

	float dot(const Vector3& vec) const{
		return	x * vec.x + y * vec.y + z * vec.z;
	}

	Vector3 cross(const Vector3& vec) const {
		return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
	}

	Vector3 multiply(const Vector3& vec) const {
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}
};


using Point3 = Vector3;
using Color3 = Vector3;
//внешние функции

//умножение числа на вектор
inline Vector3 operator* (float scalar, const Vector3& vec) {
	return vec * scalar;
}

//скалярное произведение
inline float dot(const Vector3& vec1, const Vector3& vec2) {
	return	vec1.dot(vec2);
}

//векторное произведение
inline Vector3 cross(const Vector3& vec1, const Vector3& vec2) {
	return vec1.cross(vec2);
}

//покомпонентное произведение
inline Vector3 multiply(const Vector3& vec1, const Vector3& vec2)  {
	return vec1.multiply(vec2);
}

//равенство
inline bool operator==(const Vector3& vec1, const Vector3& vec2) {
	return (std::abs(vec1.x - vec2.x) < 1e-8f && std::abs(vec1.y - vec2.y) < 1e-8f && std::abs(vec1.z - vec2.z) < 1e-8f);
}

inline bool operator!=(const Vector3& vec1, const Vector3& vec2) {
	return !(vec1 == vec2);
}

//вывод
inline std::ostream& operator<< (std::ostream& os, const Vector3& vec) {
	return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}