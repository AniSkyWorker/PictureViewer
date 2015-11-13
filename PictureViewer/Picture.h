#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct Picture
{
	Picture(sf::Texture& texture);

	void update(float scale, sf::Vector2u window);
	float checkCurrentScale(float scale, sf::Vector2u window);

	sf::Sprite sprite;
};


