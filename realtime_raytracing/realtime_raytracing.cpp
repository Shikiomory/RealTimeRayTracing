#include <SFML/Graphics.hpp>

#include "Renderer.h"

#include "objects/Hittable.h"
#include "objects/HittableList.h"
#include "objects/Sphere.h"
#include "objects/Plane.h"
#include "objects/Cilinder.h"
#include "materials/Material.h"
#include "materials/PhongMaterial.h"
#include "scene/Scene.h"
#include "Camera/CameraController.h"
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
    //Camera cam(WIDTH, RATIO);
    MovCamera cam(WIDTH, RATIO, Point3(0.0f, 0.0f, 0.0f), Point3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), 90);
    CameraController controller(cam);
    

    Scene scene;
    HittableList world;

    auto material_1 = std::make_shared<PhongMaterial>(Color3(1.0f, 0.0f, 0.0f), 0.1f, 1.0f, 0.5f, 32.0f);
    auto material_2 = std::make_shared<PhongMaterial>(Color3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f, 0.1f, 2.0f);
    auto material_3 = std::make_shared<PhongMaterial>(Color3(0.2f, 0.7f, 0.8f), 0.1f, 1.0f, 0.5f, 32.0f);
    auto material_4 = std::make_shared<PhongMaterial>(Color3(0.7f, 0.3f, 0.8f), 0.1f, 1.0f, 0.5f, 32.0f);
        
    world.add(make_shared<Sphere>(Point3(0.0f, 0.0f, -2.0f), 1.0f, material_1));
    //world.add(make_shared<Sphere>(Point3(0.0f, -105.0f, -1.0f), 100.0f, material_2));
    //world.add(make_shared<Sphere>(Point3(2.0f, 0.0f, -3.0f), 1.0f, material_3));
    world.add(make_shared<Sphere>(Point3(2.0f, 0.0f, -3.0f), 1.0f, material_3));
    //world.add(make_shared<Sphere>(Point3(2.0f, 0.0f, -3.0f), 1.0f, material_3));

    world.add(make_shared<Plane>(Point3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), material_2));
    //Cilinder(Point3 _pos1, Point3 _pos2, float _radius, float _a, float _b, float _c, shared_ptr<Material> _mat)
    world.add(make_shared<Cilinder>(Point3(0.0f, -4.0f, -3.0f), Point3(0.0f, 1.0f, -3.0f), 1.0f, 1.0f, 1.0f, 1.0f, material_4));

    scene.world = world;
    //scene.lights.push_back(PointLight(Point3(5.0f, 7.0f, 5.0f), Color3(1.0f, 1.0f, 1.0f), 1.0f));
    scene.lights.push_back(PointLight(Point3(-5.0f, 7.0f, 5.0f), Color3(1.0f, 1.0f, 1.0f), 1.0f));

    int frames = 0;
    sf::Clock fps_clock;
    sf::Clock dt_clock;
    sf::Vector2i mousePos;
    sf::Vector2i center(cam.get_width() / 2, cam.get_height() / 2);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(center, window);
    while (window.isOpen()) {
        sf::Event event;

        float dt = dt_clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        mousePos = sf::Mouse::getPosition(window);
        float offset_x = static_cast<float>(mousePos.x - center.x);
        float offset_y = static_cast<float>(mousePos.y - center.y);
        controller.rotate_cam(offset_x, offset_y, dt);
        sf::Mouse::setPosition(center, window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { controller.move_forward(dt); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { controller.move_left(dt); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { controller.move_backward(dt); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { controller.move_right(dt); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { controller.move_up(dt); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { controller.move_down(dt); }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { controller.speed_up(); }
        else { controller.normal_speed(); }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
        renderer.Render(buffer, cam, scene);
        texture.update(buffer.data());

        ++frames;
        float time = fps_clock.getElapsedTime().asSeconds();
        if (time > 1.0f) {
            int fps = frames / static_cast<int>(time);
            window.setTitle("FPS: " + std::to_string(fps));

            frames = 0;
            fps_clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

