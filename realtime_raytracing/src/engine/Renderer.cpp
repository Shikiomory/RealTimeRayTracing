#pragma once

#pragma once

#include "Utility.h"
#include "Renderer.h"
#include "objects/Sphere.h"

void Renderer::Render(std::vector<sf::Uint8>& buffer, const Camera& cam, const Hittable& world)
{
    int height = cam.get_height();
    int width = cam.get_width();

    for (int y = 0; y < height; ++y) {
        std::clog << "\rScanlines remaing: " << (height - y) << std::flush; //индикатор готовности кадра
        for (int x = 0; x < width; ++x) {
            
            //создание луча из пикселя
            Ray r = cam.get_ray(x, y);

            //закраска пикселя
            Color3 pixelcolor = ray_color(r, world);

            //передаем цвета в sfml буффер
            int index = (x + width * y) * 4;
            buffer[index] = static_cast<sf::Uint8>(255.999f * pixelcolor.x); //красный канал
            buffer[index + 1] = static_cast<sf::Uint8>(255.999f * pixelcolor.y); //зеленый канал
            buffer[index + 2] = static_cast<sf::Uint8>(255.999f * pixelcolor.z); //синий канал
            buffer[index + 3] = 255; //альфа канал
        }
    }
    std::clog << "\rDone.";
}

Color3 Renderer::ray_color(const Ray& r, const Hittable& world) {
    hit_record rec;
    if (world.hit(r, Interval(0.0f, infty), rec)) {
        return 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));
    }

    //Point3 center(0.0f, 0.0f, -2.0f);
    //Sphere sphere(center, 1.0f);
    //hit_record rec;
    //if (sphere.hit(r, 0.0f, 20.0f, rec)) {
    //    //Vector3 N = (r.at(t) - Vector3(0.0f, 0.0f, -2.0f)).normalize();
    //    Vector3 N = rec.normal;
    //    return 0.5 * Color3(N.x + 1, N.y + 1, N.z + 1);
    //}
    //if (sphere.hit_object(r)) return Color3(1.0f, 0.0f, 0.0f);

    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
}
