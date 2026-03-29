#include <iostream>
#include <SFML/Graphics.hpp>

#include "Renderer.h"
using namespace sf;
using namespace std;


const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), L"Новый проект", Style::Default);

    Texture texture;
    texture.create(WIDTH, HEIGHT);
    Sprite sprite(texture);

    vector<Uint8> buffer(WIDTH * HEIGHT * 4);

    Renderer renderer;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        renderer.Render(buffer, WIDTH, HEIGHT);

        texture.update(buffer.data());

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

