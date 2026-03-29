#pragma once
#include <vector>
#include <SFML/Config.hpp>

class Renderer
{
public:
	void Render(std::vector<sf::Uint8>& buffer, int width, int height);
};

