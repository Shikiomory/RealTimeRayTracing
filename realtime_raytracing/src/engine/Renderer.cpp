#pragma once

#include "Utility.h"
#include "Renderer.h"
#include "objects/Sphere.h"


void Renderer::Render(std::vector<sf::Uint8>& buffer, const Camera& cam, const Hittable& world)
{
    int height = cam.get_height();
    int width = cam.get_width();

    float sample_scale = cam.get_samples_scale();
    float samples_per_pixel = cam.get_samples();
    for (int y = 0; y < height; ++y) {
        //std::clog << "\rScanlines remaing: " << (height - y) << std::flush; //индикатор готовности кадра
        for (int x = 0; x < width; ++x) {
            
            Color3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < samples_per_pixel; ++i) {
                //создание луча из пикселя
                Ray r = cam.get_ray(x, y);
                //закраска пикселя
                pixel_color += ray_color(r, world);
            }

            //r = cam.get_ray(x, y);
            //Color3 pixel_color = ray_color(r, world);

            //передаем цвета в sfml буффер
            int index = (x + width * y) * 4;
            set_pixel_color(buffer, index, sample_scale * pixel_color);
        }
    }
    //std::clog << "\rDone.";
}

Color3 Renderer::ray_color(const Ray& r, const Hittable& world) {
    hit_record rec;

    if (world.hit(r, Interval(0.0f, infty), rec)) {
        return 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));
    }

    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
}

void Renderer::set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color) {
    static const Interval intensity(0.0f, 0.999f);
    buffer[index] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.x)); //красный канал
    buffer[index + 1] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.y)); //зеленый канал
    buffer[index + 2] = static_cast<sf::Uint8>(256.0f * intensity.clamp(pixel_color.z)); //синий канал
    buffer[index + 3] = 255; //альфа канал
}
