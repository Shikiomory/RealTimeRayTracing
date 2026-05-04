//#pragma once
//
//#include "Utility.h"
//#include "Renderer.h"
//#include "objects/Sphere.h"
//#include "materials/PhongMaterial.h"
//
//void Renderer::Render(std::vector<sf::Uint8>& buffer, const MovCamera& cam, const Scene& scene)
//{
//    int height = cam.get_height();
//    int width = cam.get_width();
//
//    float sample_scale = cam.get_samples_scale();
//    float samples_per_pixel = cam.get_samples();
//
//
//#pragma omp parallel for num_threads(16) schedule(dynamic, 16)
//    for (int y = 0; y < height; ++y) {
//        for (int x = 0; x < width; ++x) {
//
//            Color3 pixel_color(0.0f, 0.0f, 0.0f);
//            for (int i = 0; i < samples_per_pixel; ++i) {
//                //создание луча из пикселя
//                Ray r = cam.get_ray(x, y);
//                //закраска пикселя
//                pixel_color += ray_color(r, scene);
//            }
//
//            //передаем цвета в sfml буффер
//            int index = (x + width * y) * 4;
//            set_pixel_color(buffer, index, sample_scale * pixel_color);
//        }
//    }
//}
//
//Color3 Renderer::ray_color(const Ray& r, const Scene& scene) {
//    hit_record rec;
//
//    const Hittable& world = scene.world;
//    if (world.hit(r, Interval(0.001f, infty), rec)) {
//        //Color3 color = 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));
//
//        Color3 totalLight = rec.mat->shade(r, rec, scene.lights, world);
//        return totalLight;
//    }
//
//    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
//    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
//    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
//}
//
//void Renderer::set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color) {
//    static const Interval intensity(0.0f, 0.999f);
//    float r = pixel_color.x;
//    float g = pixel_color.y;
//    float b = pixel_color.z;
//
//    //применение гамма коррекции
//    r = linear_to_gamma(r);
//    g = linear_to_gamma(g);
//    b = linear_to_gamma(b);
//    buffer[index] = static_cast<sf::Uint8>(256.0f * intensity.clamp(r)); //красный канал
//    buffer[index + 1] = static_cast<sf::Uint8>(256.0f * intensity.clamp(g)); //зеленый канал
//    buffer[index + 2] = static_cast<sf::Uint8>(256.0f * intensity.clamp(b)); //синий канал
//    buffer[index + 3] = 255; //альфа канал
//}
