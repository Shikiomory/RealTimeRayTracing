#include <SFML/Graphics.hpp>

#include "Renderer.h"

#include "objects/Hittable.h"
#include "objects/HittableList.h"
#include "objects/Sphere.h"
#include "materials/Material.h"
#include "materials/PhongMaterial.h"
#include <string>
using namespace std;


const int WIDTH = 800;
//const int HEIGHT = 600;
const float RATIO = 16.0f / 9.0f;
int main()
{
    int height = static_cast<int>(WIDTH / RATIO);
    height = (height < 1) ? 1 : height;
    
    sf::RenderWindow window(sf::VideoMode(WIDTH, height), L"Новый проект", sf::Style::Default);

    sf::Texture texture;
    texture.create(WIDTH, height);
    sf::Sprite sprite(texture);

    vector<sf::Uint8> buffer(WIDTH * height * 4);

    Renderer renderer;
    Camera cam(WIDTH, RATIO);

    HittableList world;
    auto material_1 = std::make_shared<PhongMaterial>(Color3(1.0f, 0.0f, 0.0f), 0.1f, 1.0f, 0.5f, 32.0f);
    auto material_2 = std::make_shared<PhongMaterial>(Color3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f, 0.5f, 32.0f);
    auto material_3 = std::make_shared<PhongMaterial>(Color3(0.2f, 0.7f, 0.8f), 0.1f, 1.0f, 0.5f, 32.0f);

    world.add(make_shared<Sphere>(Point3(0.0f, 0.0f, -2.0f), 1.0f, material_1));
    world.add(make_shared<Sphere>(Point3(0.0f, -105.0f, -1.0f), 100.0f, material_2));
    world.add(make_shared<Sphere>(Point3(2.0f, 0.0f, -3.0f), 1.0f, material_3));
    //world.add(make_shared<Sphere>(Point3(0.0f, 0.0f, -2.0f), 1.0f));
    //world.add(make_shared<Sphere>(Point3(0.0f, -105.0f, -1.0f), 100.0f));
    //world.add(make_shared<Sphere>(Point3(2.0f, 0.0f, -3.0f), 1.0f));

    int frames = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        renderer.Render(buffer, cam, world);
        texture.update(buffer.data());

        ++frames;
        float time = clock.getElapsedTime().asSeconds();
        if (time > 1.0f) {
            int fps = frames / static_cast<int>(time);
            window.setTitle("FPS: " + std::to_string(fps));

            frames = 0;
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

