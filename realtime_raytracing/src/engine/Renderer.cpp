#pragma once

#include "Utility.h"
#include "Renderer.h"
#include "objects/Sphere.h"
#include "materials/PhongMaterial.h"


void Renderer::Render(std::vector<sf::Uint8>& buffer, const Camera& cam, const Hittable& world)
{
    int height = cam.get_height();
    int width = cam.get_width();

    float sample_scale = cam.get_samples_scale();
    float samples_per_pixel = cam.get_samples();

#pragma omp parallel for num_threads(16) schedule(dynamic, 16)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            
            Color3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < samples_per_pixel; ++i) {
                //создание луча из пикселя
                Ray r = cam.get_ray(x, y);
                //закраска пикселя
                pixel_color += ray_color(r, world);
            }

            //передаем цвета в sfml буффер
            int index = (x + width * y) * 4;
            set_pixel_color(buffer, index, sample_scale * pixel_color);
        }
    }
}

Color3 Renderer::ray_color(const Ray& r, const Hittable& world) {
    hit_record rec;

    if (world.hit(r, Interval(0.001f, infty), rec)) {
        //Color3 color = 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));

        Point3 light_pos(5.0f, 7.0f, 5.0f);
        Vector3 light_dir = (light_pos - rec.p).normalize();

        Ray shadow_ray(rec.p, light_dir);
        float light_distance = (light_pos - rec.p).length();
        auto mat = std::dynamic_pointer_cast<PhongMaterial>(rec.mat);

        float diff = 0.0f;
        float spec = 0.0f;
        hit_record temp_rec;
        if (!world.hit(shadow_ray, Interval(0.001f, light_distance), temp_rec)) {
            Vector3 camera_dir = -r.get_direction().normalize();
            diff = mat->getDiffusion(rec.normal, light_dir);
            spec = mat->getSpecular(rec.normal, light_dir, camera_dir);
        }


       
        float ambient = mat->getAmbientK();
        Color3 color = mat->getColor();

        Color3 totalLight = ambient * color + diff * color + Point3(1.0f, 1.0f, 1.0f) * spec;
        return totalLight;
    }

    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
}

//Color3 Renderer::ray_color(const Ray& r, const Hittable& world) {
//
//    hit_record rec;
//
//
//
//    if (world.hit(r, Interval(0.001f, infty), rec)) {
//
//        Color3 color = 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));
//
//
//
//        Point3 light_pos(3.0f, 2.0f, 0.0f);
//
//        Vector3 light_dir = (light_pos - rec.p).normalize();
//
//        Vector3 camera_dir = -r.get_direction().normalize();
//
//
//
//
//
//        PhongMaterial mat(color, 0.1f, 1.0f, 0.2f, 32.0f);
//
//        float diff = mat.getDiffusion(rec.normal, light_dir);
//
//        float spec = mat.getSpecular(rec.normal, light_dir, camera_dir);
//
//        float ambient = mat.getAmbientK();
//
//
//
//        Color3 totalLight = ambient * color * 2.0f + diff * color + Point3(1.0f, 1.0f, 1.0f) * spec;
//
//        return totalLight;
//
//    }
//
//
//
//    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
//
//    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
//
//    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
//
//}

void Renderer::set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color) {
    static const Interval intensity(0.0f, 0.999f);
    buffer[index] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.x)); //красный канал
    buffer[index + 1] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.y)); //зеленый канал
    buffer[index + 2] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.z)); //синий канал
    buffer[index + 3] = 255; //альфа канал
}
