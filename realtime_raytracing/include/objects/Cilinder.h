#pragma once
#include "Hittable.h"

class Cilinder
{
	Point3 pos1, pos2;
	float radius, radius_sqr;
	float y_max = infty;
	float y_min = 0;
	float height;
	Vector3 u, v, w;
	//std::shared_ptr<Material> mat;
	uint32_t mat_id;
	Aaab bbox;

public:

	Cilinder() = default;
	//Cilinder(Point3 _pos1, float _radius, shared_ptr<Material> _mat) : pos1(_pos1), radius(_radius), mat(_mat), a(1.0f), b(1.0f), c(1.0f) {}
	Cilinder(Point3 _pos1, Point3 _pos2, float _radius, uint32_t _mat_id) : pos1(_pos1), pos2(_pos2), radius(_radius), mat_id(_mat_id), radius_sqr(_radius * _radius) {
		y_max = std::max(pos1.y, pos2.y);
		y_min = std::min(pos1.y, pos2.y);

		Vector3 axis = pos2 - pos1;
		height = std::sqrt(axis.squareLength());
		v = axis / height;

		Vector3 temp_vec = (std::abs(v.y) > 0.9f) ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(0.0f, 1.0f, 0.0f);
		u = cross(v, temp_vec).normalize();
		w = cross(u, v).normalize();

		Point3 min_p(std::min(pos1.x, pos2.x), std::min(pos1.y, pos2.y), std::min(pos1.z, pos2.z));
		Point3 max_p(std::max(pos1.x, pos2.x), std::max(pos1.y, pos2.y), std::max(pos1.z, pos2.z));
		Vector3 rvec = Vector3(radius, radius, radius);
		bbox = Aaab(min_p - rvec, max_p + rvec);
	}

	bool hit(const Ray& r, Interval t_int, hit_record& rec) const {
		//смещение
		Vector3 oc = r.get_origin() - pos1;
		Vector3 dir = r.get_direction();

		//поворот
		Point3 local_origin(dot(oc, u), dot(oc, v), dot(oc, w));
		Vector3 local_dir(dot(dir, u), dot(dir, v), dot(dir, w));

		float ak = local_dir.x * local_dir.x + local_dir.z * local_dir.z;
		float h = local_dir.x * local_origin.x + local_dir.z * local_origin.z;
		float ck = local_origin.x * local_origin.x + local_origin.z * local_origin.z - radius_sqr;

		float discriminant = h * h - ak * ck;

		float closet_root = infty;

		int hitN = -1;
		if (discriminant >= 0) {
			float sqrt_d = std::sqrt(discriminant);
			float root = (-h - sqrt_d) / ak; //поиск t
			float y;
			if (t_int.surrounds(root)) {
				y = local_origin.y + root * local_dir.y;
				if (y >= 0 && y <= height) {
					closet_root = root;
					hitN = 0;
				}
			}

			root = (-h + sqrt_d) / ak;
			if (root < closet_root && t_int.surrounds(root)) {
				y = local_origin.y + root * local_dir.y;
				if (y >= 0 && y <= height) {
					closet_root = root;
					hitN = 1;
				}
			}

		}

		Point3 p;

		
		if (std::abs(local_dir.y) > 1e-6f) {

			float root = (height - local_origin.y) / local_dir.y;
			if (root < closet_root && t_int.surrounds(root)) {
				float x = local_origin.x + root * local_dir.x;
				float z = local_origin.z + root * local_dir.z;
				if ((x * x + z * z) <= radius_sqr) {
					hitN = 2;
					closet_root = root;
				}
			}

			root = (0.0f - local_origin.y) / local_dir.y;
			if (root < closet_root && t_int.surrounds(root)) {
				//p = r.at(root);
				float x = local_origin.x + root * local_dir.x;
				float z = local_origin.z + root * local_dir.z;
				if ((x * x + z * z) <= radius_sqr) {
					hitN = 3;
					closet_root = root;
				}
			}
		}


		if (closet_root == infty) { return false; }

		rec.t = closet_root;
		rec.p = r.at(closet_root);
		Vector3 local_normal;

		Point3 loc_p = local_origin + local_dir * closet_root;
		switch (hitN) {
		case 0:
			p = r.at(closet_root);
			local_normal = Vector3(loc_p.x / radius, 0.0f, loc_p.z / radius);
			break;
		case 1:
			p = r.at(closet_root);
			local_normal = Vector3(loc_p.x / radius, 0.0f, loc_p.z / radius);
			break;
		case 2:
			local_normal = Vector3(0.0f, 1.0f, 0.0f);
			break;
		case 3:
			local_normal = Vector3(0.0f, -1.0f, 0.0f);
			break;
		}

		Vector3 normal = local_normal.x * u + local_normal.y * v + local_normal.z * w;
		rec.set_face_normal(r, normal);
		//rec.mat = mat.get();
		rec.mat_id = mat_id;

		return true;
	}

	bool any_hit(const Ray& r, Interval t_int) const {
		Vector3 oc = r.get_origin() - pos1;
		Vector3 dir = r.get_direction();

		Point3 local_origin(dot(oc, u), dot(oc, v), dot(oc, w));
		Vector3 local_dir(dot(dir, u), dot(dir, v), dot(dir, w));

		float ak = local_dir.x * local_dir.x + local_dir.z * local_dir.z;
		float h = local_dir.x * local_origin.x + local_dir.z * local_origin.z;
		float ck = local_origin.x * local_origin.x + local_origin.z * local_origin.z - radius_sqr;

		float discriminant = h * h - ak * ck;

		if (discriminant >= 0) {
			float sqrt_d = std::sqrt(discriminant);

			float root = (-h - sqrt_d) / ak;
			if (t_int.surrounds(root)) {
				float y = local_origin.y + root * local_dir.y;
				if (y >= 0 && y <= height) return true;
			}

			root = (-h + sqrt_d) / ak;
			if (t_int.surrounds(root)) {
				float y = local_origin.y + root * local_dir.y;
				if (y >= 0 && y <= height) return true;
			}
		}

		if (std::abs(local_dir.y) > 1e-6f) {
			float root = (height - local_origin.y) / local_dir.y;
			if (t_int.surrounds(root)) {
				float x = local_origin.x + root * local_dir.x;
				float z = local_origin.z + root * local_dir.z;
				if ((x * x + z * z) <= radius_sqr) return true;
			}

			root = (0.0f - local_origin.y) / local_dir.y;
			if (t_int.surrounds(root)) {
				float x = local_origin.x + root * local_dir.x;
				float z = local_origin.z + root * local_dir.z;
				if ((x * x + z * z) <= radius_sqr) return true;
			}
		}

		return false;
	}

	Aaab bounding_box() const { return bbox; }
};

