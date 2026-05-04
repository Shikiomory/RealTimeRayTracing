#pragma once
#include "Hittable.h"

class Cilinder
{
	Point3 pos1, pos2;
	float a, b, c, radius, radius_sqr;
	float y_max = infty;
	float y_min = 0;
	std::shared_ptr<Material> mat;

public:

	Cilinder() = default;
	//Cilinder(Point3 _pos1, float _radius, shared_ptr<Material> _mat) : pos1(_pos1), radius(_radius), mat(_mat), a(1.0f), b(1.0f), c(1.0f) {}
	Cilinder(Point3 _pos1, Point3 _pos2, float _radius, float _a, float _b, float _c, shared_ptr<Material> _mat) : pos1(_pos1), pos2(_pos2), radius(_radius), a(_a), b(_b), c(_c), mat(_mat), radius_sqr(_radius * _radius) {
		y_max = std::max(pos1.y, pos2.y);
		y_min = std::min(pos1.y, pos2.y);
	}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const {
		Vector3 local_origin = r.get_origin() - pos1;
		local_origin.y = 0.0f;
		Vector3 local_dir = r.get_direction();
		local_dir.y = 0.0f;

		float ak = local_dir.squareLength();
		float h = dot(local_dir, local_origin);
		float ck = local_origin.squareLength() - radius_sqr;

		float discriminant = h * h - ak * ck;

		float closet_root = infty;
		Vector3 normal;
		Point3 p;

		if (discriminant >= 0) {
			float sqrt_d = std::sqrt(discriminant);
			float root = (-h - sqrt_d) / ak; //поиск t
			float y;
			if (t_int.surrounds(root)) {
				y = root * r.get_direction().y + r.get_origin().y;
				if (y >= y_min && y <= y_max) {
					closet_root = root;
					p = r.at(root);
					normal = (p - Vector3(pos1.x, p.y, pos1.z)) / radius;
				}
			}

			root = (-h + sqrt_d) / ak;
			if (root < closet_root && t_int.surrounds(root)) {
				y = root * r.get_direction().y + r.get_origin().y;
				if (y >= y_min && y <= y_max) {
					closet_root = root;
					p = r.at(root);
					normal = (p - Vector3(pos1.x, p.y, pos1.z)) / radius;
				}
			}

		}

		float dir_y = r.get_direction().y;
		
		if (std::abs(dir_y) > 1e-6f) {
			float orig_y = r.get_origin().y;

			float root = (y_max - orig_y) / dir_y;
			if (root < closet_root && t_int.surrounds(root)) {
				p = r.at(root);
				if ((p.x - pos1.x) * (p.x - pos1.x) + (p.z - pos1.z) * (p.z - pos1.z) <= radius_sqr) {
					normal = Vector3(0.0f, 1.0f, 0.0f);
					closet_root = root;
				}
			}

			root = (y_min - orig_y) / dir_y;
			if (root < closet_root && t_int.surrounds(root)) {
				p = r.at(root);
				if ((p.x - pos1.x) * (p.x - pos1.x) + (p.z - pos1.z) * (p.z - pos1.z) <= radius_sqr) {
					normal = Vector3(0.0f, -1.0f, 0.0f);
					closet_root = root;
				}
			}
		}


		if (closet_root == infty) { return false; }

		rec.t = closet_root;
		rec.p = r.at(closet_root);
		rec.set_face_normal(r, normal);
		rec.mat = mat.get();

		return true;
	}

	bool any_hit(const Ray& r, Interval t_int) const {
		Vector3 local_origin = r.get_origin() - pos1;
		local_origin.y = 0.0f;
		Vector3 local_dir = r.get_direction();
		local_dir.y = 0.0f;

		float ak = local_dir.squareLength();
		float h = dot(local_dir, local_origin);
		float ck = local_origin.squareLength() - radius_sqr;

		float discriminant = h * h - ak * ck;

		if (discriminant >= 0) {
			float sqrt_d = std::sqrt(discriminant);
			float root = (-h - sqrt_d) / ak; //поиск t
			float y;
			if (t_int.surrounds(root)) {
				y = root * r.get_direction().y + r.get_origin().y;
				if (y >= y_min && y <= y_max) {
					return true;
				}
			}

			root = (-h + sqrt_d) / ak;
			if (t_int.surrounds(root)) {
				y = root * r.get_direction().y + r.get_origin().y;
				if (y >= y_min && y <= y_max) {
					return true;
				}
			}

		}

		float dir_y =	r.get_direction().y;

		Point3 p;
		if (std::abs(dir_y) > 1e-6f) {
			float orig_y = r.get_origin().y;

			float root = (y_max - orig_y) / dir_y;
			if (t_int.surrounds(root)) {
				p = r.at(root);
				if ((p.x - pos1.x) * (p.x - pos1.x) + (p.z - pos1.z) * (p.z - pos1.z) <= radius_sqr) {
					return true;
				}
			}

			root = (y_min - orig_y) / dir_y;
			if (t_int.surrounds(root)) {
				p = r.at(root);
				if ((p.x - pos1.x) * (p.x - pos1.x) + (p.z - pos1.z) * (p.z - pos1.z) <= radius_sqr) {
					return true;
				}
			}
		}

		return false;
	}
};

