#include "Renderer.h"

void Renderer::Render(std::vector<sf::Uint8>& buffer, int width, int height)
{
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int index = (x + width * y) * 4;

            buffer[index] = 255;
            buffer[index + 1] = (x * 255 / width);
            buffer[index + 2] = (y * 255 / height);
            buffer[index + 3] = 255;
        }
    }

}
