#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
const int BUTTONS = 4;

struct Buttons
{
	Buttons();

	void update(sf::Vector2u window);

	void left();
	void right();
	void plus();
	void minus();

	sf::Texture texture[BUTTONS - 1];
	sf::Sprite sprite[BUTTONS];
	int array_size;
	int i;
	float scale;
};
