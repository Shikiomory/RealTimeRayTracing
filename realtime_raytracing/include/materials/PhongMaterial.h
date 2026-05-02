#pragma once
#include "Material.h"
#include "lights/PointLight.h"

class PhongMaterial: public Material
{
	float ka, kd, ks;
	float shininess;
	Color3 color;
public:

	PhongMaterial() = default;
	PhongMaterial(Color3 _color, float _ka, float _kd, float _ks, float _shininess): color(_color), ka(_ka), kd(_kd), ks(_ks), shininess(_shininess) {}

	Color3 shade(const Ray& r, const hit_record& rec, const std::vector<PointLight>& lights, const Hittable& world) const override {
		
		float ambient = getAmbientK();
		float diff = 0.0f;
		float spec = 0.0f;
		for (const auto& light : lights) {
			Point3 light_pos = light.position;
			Vector3 light_dir = (light_pos - rec.p).normalize();
			Ray shadow_ray(rec.p + rec.normal * 0.001f, light_dir);
			float light_distance = (light_pos - rec.p).length();


			hit_record temp_rec;
			if (!world.hit(shadow_ray, Interval(0.001f, light_distance), temp_rec)) {
				Vector3 camera_dir = -r.get_direction().normalize();
				diff += getDiffusion(rec.normal, light_dir);
				spec += getSpecular(rec.normal, light_dir, camera_dir);
			}

		}
		Color3 totalLight = ambient * color + diff * color + Point3(1.0f, 1.0f, 1.0f) * spec;
		return totalLight;
		//Vector3 light_dir = (light_pos - rec.p).normalize();
		//Ray shadow_ray(rec.p, light_dir);
		//float light_distance = (light_pos - rec.p).length();

		//float diff = 0.0f;
		//float spec = 0.0f;
		//hit_record temp_rec;
		//if (!world.hit(shadow_ray, Interval(0.001f, light_distance), temp_rec)) {
		//	Vector3 camera_dir = -r.get_direction().normalize();
		//	diff = getDiffusion(rec.normal, light_dir);
		//	spec = getSpecular(rec.normal, light_dir, camera_dir);
		//	//diff = mat->getDiffusion(rec.normal, light_dir);
		//	//spec = mat->getSpecular(rec.normal, light_dir, camera_dir);
		//}

		//float ambient = getAmbientK();

		//Color3 totalLight = ambient * color + diff * color + Point3(1.0f, 1.0f, 1.0f) * spec;

		//return totalLight;
	}


	float getDiffusion(const Vector3& normal, const Vector3& light_dir) const {
		float norm_o_light = dot(normal, light_dir);
		float diffusion_light = std::max(0.0f, kd * norm_o_light);
		return diffusion_light;
	}

	float getSpecular(const Vector3& normal, const Vector3& light_dir, const Vector3& camera_dir) const {
		Vector3 reflect_dir = 2.0f * dot(light_dir, normal) * normal - light_dir;
		float ref_o_cam = dot(reflect_dir, camera_dir);
		ref_o_cam = ref_o_cam > 0 ? ref_o_cam : 0;
		float specular_light = std::max(0.0f, ks * std::pow(ref_o_cam, shininess));
		return specular_light;
	}

	float getAmbientK() const { return ka; }
	float getdiffusionK() const { return kd; }
	float getSpecularK() const { return ks; }
	Color3 getColor() const { return color; }
};

