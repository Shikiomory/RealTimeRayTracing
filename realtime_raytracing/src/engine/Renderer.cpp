#pragma once

#include "Utility.h"
#include "Renderer.h"
#include "objects/Sphere.h"
#include "materials/PhongMaterial.h"

void Renderer::Render(std::vector<sf::Uint8>& buffer, const MovCamera& cam, const Scene& scene)
{
    int window_height = cam.get_window_height();
    int window_width = cam.get_window_width();
    
    int render_height = cam.get_render_height();
    int render_width = cam.get_render_width();
    float sample_scale = cam.get_samples_scale();
    float samples_per_pixel = cam.get_samples();

    //кадры и знаменатель для накопительного буфера
    if (cam.is_moved()) {
        frames = 1;
    }
    else {
        frames++;
    }

    float denom = 1.0f / static_cast<float>(frames);

    //1 проход для заполнения буффера
#pragma omp parallel for num_threads(16) schedule(dynamic, 16)
    for (int y = 0; y < render_height; ++y) {
        for (int x = 0; x < render_width; ++x) {
            Ray r = cam.get_ray(x, y, true);
            int index = (x + render_width * y);
            colorBuffer[index] = ray_color(r, scene);
        }
    }

    //2 проход для сэмплинга и закрашивания
#pragma omp parallel for num_threads(16) schedule(dynamic, 16)
    for (int y = 0; y < render_height; ++y) {
        for (int x = 0; x < render_width; ++x) {

            int index = (x + render_width * y);
            int byte_index = index * 4;

            bool sampling = false;
            if (x > 0 && x < render_width - 1 && y > 0 && y < render_height - 1) {
                int left = index - 1;
                int right = index + 1;
                int up = index - render_width;
                int down = index + render_width;

                if (is_different(colorBuffer, index, left) || is_different(colorBuffer, index, right) || is_different(colorBuffer, index, up) || is_different(colorBuffer, index, down)) {
                    sampling = true;
                }
            }


            Color3 pixel_color = colorBuffer[index]; //цвет центра пикселя
            //усредняем цвет, если на границе
            if (sampling) {
                for (int i = 0; i < samples_per_pixel - 1; ++i) {
                    //создание луча из пикселя
                    Ray r = cam.get_ray(x, y, true);
                    //закраска пикселя
                    pixel_color += ray_color(r, scene);
                }
                pixel_color *= sample_scale;
                //pixel_color = Color3(1.0f,1.0f,0.0f);
            }

            //накопительный буфер (TAA)
            pixel_color = temporalBuffer[index] * (1 - denom) + pixel_color * denom;
            temporalBuffer[index] = pixel_color;

            //передаем цвета в sfml буффер
            //set_pixel_color(buffer, byte_index, pixel_color);   
        }
    }

    //3 проход (upscaling)
    #pragma omp parallel for num_threads(16) schedule(dynamic, 16)
    for (int y = 0; y < window_height; ++y) {
        for (int x = 0; x < window_width; ++x) {
            //перевод экранных координат в дробные рендерные координаты
            float rx = static_cast<float>(x) * (render_width - 1) / (window_width - 1);
            float ry = static_cast<float>(y) * (render_height - 1) / (window_height - 1);

            //координаты соседних пикселей
            int x0 = static_cast<int>(rx);
            int y0 = static_cast<int>(ry);
            int x1 = std::min(x0+1, render_width - 1);
            int y1 = std::min(y0+1, render_height - 1);

            //получение цветов соседних пикселей
            Color3 c00 = temporalBuffer[y0 * render_width + x0];
            Color3 c01 = temporalBuffer[y0 * render_width + x1];
            Color3 c10 = temporalBuffer[y1 * render_width + x0];
            Color3 c11 = temporalBuffer[y1 * render_width + x1];

            //веса интерполяции
            float tx = rx - x0;
            float ty = ry - y0;

            Color3 interpolated_color = c00 * (1.0f - tx) * (1.0f - ty) + c01 * tx * (1.0f - ty) + c10 * (1.0f - tx) * ty + c11 * tx * ty;
            int index = (x + window_width * y) * 4;
            set_pixel_color(buffer, index, interpolated_color);
        }
    }
}

Color3 Renderer::ray_color(const Ray& r, const Scene& scene) {
    hit_record rec;

    const Hittable& world = scene.world;
    if (world.hit(r, Interval(0.001f, infty), rec)) {
        //Color3 color = 0.5f * Color3(rec.normal + Color3(1.0f, 1.0f, 1.0f));

        const PhongMaterial& mat = scene.materials[rec.mat_id];
        Color3 totalLight = mat.shade(r, rec, scene.lights, world);
        return totalLight;
    }

    Vector3 unit_direction = r.get_direction().normalize(); //нормализуем вектор направления
    float a = 0.5f * (unit_direction.y + 1.0f); //переводим [-1, 1] в [0,1]
    return (1.0f - a) * Color3(1.0f, 1.0f, 1.0f) + a * Color3(0.5f, 0.7f, 1.0f); //возвращаем градиент от синего к белому через lerp blendedValue=(1-a) * startValue + a * endValue
}

bool Renderer::is_different(const std::vector<Color3>& buffer, int idx_1, int idx_2)
{
    float dr = std::abs(buffer[idx_1].x - buffer[idx_2].x);
    float dg = std::abs(buffer[idx_1].y - buffer[idx_2].y);
    float db = std::abs(buffer[idx_1].z - buffer[idx_2].z);

    float threshold = 0.05f;
    return (0.2126 * dr + 0.7152 * dg + 0.0722 * db) > threshold;
}


void Renderer::set_pixel_color(std::vector<sf::Uint8>& buffer, int index, const Color3& pixel_color) {
    static const Interval intensity(0.0f, 0.999f);
    float r = pixel_color.x;
    float g = pixel_color.y;
    float b = pixel_color.z;

    //применение гамма коррекции
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);
    buffer[index] = static_cast<sf::Uint8>(256.0f * intensity.clamp(r)); //красный канал
    buffer[index + 1] = static_cast<sf::Uint8>(256.0f * intensity.clamp(g)); //зеленый канал
    buffer[index + 2] = static_cast<sf::Uint8>(256.0f * intensity.clamp(b)); //синий канал
    buffer[index + 3] = 255; //альфа канал
}


