#pragma once
#include "Hittable.h"

class Box : public Hittable {
    //shared_ptr<Material> mat;
    uint32_t mat_id;
    Aaab bbox;

    Vector3 get_normal(const Point3& p) const {
        float dx = std::min(std::abs(p.x - bbox.x.min), std::abs(p.x - bbox.x.max));
        float dy = std::min(std::abs(p.y - bbox.y.min), std::abs(p.y - bbox.y.max));
        float dz = std::min(std::abs(p.z - bbox.z.min), std::abs(p.z - bbox.z.max));

        if (dx < dy && dx < dz) {
            return (std::abs(p.x - bbox.x.min) < std::abs(p.x - bbox.x.max))
                ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
        }
        else if (dy < dz) {
            return (std::abs(p.y - bbox.y.min) < std::abs(p.y - bbox.y.max))
                ? Vector3(0, -1, 0) : Vector3(0, 1, 0);
        }
        else {
            return (std::abs(p.z - bbox.z.min) < std::abs(p.z - bbox.z.max))
                ? Vector3(0, 0, -1) : Vector3(0, 0, 1);
        }
    }
public:
    Box(const Point3& a, const Point3& b, uint32_t _mat_id): mat_id(_mat_id), bbox(a, b) {}

    bool hit(const Ray& r, Interval t_int, hit_record& rec) const override {
        const Point3& ray_orig = r.get_origin();
        const Point3& ray_dir = r.get_direction();

        for (int axis = 0; axis < 3; ++axis) {
            const Interval& ax = bbox.axis_interval(axis);
            const float adinv = 1.0f / ray_dir[axis];

            float t0 = (ax.min - ray_orig[axis]) * adinv;
            float t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > t_int.min) t_int.min = t0;
                if (t1 < t_int.max) t_int.max = t1;
            }
            else {
                if (t1 > t_int.min) t_int.min = t1;
                if (t0 < t_int.max) t_int.max = t0;
            }

            if (t_int.min >= t_int.max) return false;
        }

        // После цикла t_int.min — это и есть наше ближайшее t
        rec.t = t_int.min;
        rec.p = r.at(rec.t);
        //rec.mat = mat.get();
        rec.mat_id = mat_id;
        rec.normal = get_normal(rec.p);
        rec.set_face_normal(r, rec.normal);

        return true;
    }

    bool any_hit(const Ray& r, Interval t_int) const override {
        return bbox.hit(r, t_int);
    }

    Aaab bounding_box() const override { return bbox; }
};