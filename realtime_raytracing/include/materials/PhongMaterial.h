#pragma once
#include "Material.h"

class PhongMaterial: public Material
{
	float ka, kd, ks;
	float shininess;
	Color3 color;
public:

	PhongMaterial() = default;
	PhongMaterial(Color3 _color, float _ka, float _kd, float _ks, float _shininess): color(_color), ka(_ka), kd(_kd), ks(_ks), shininess(_shininess) {}

	
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

