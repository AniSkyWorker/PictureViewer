#include "Buttons.h"

const int MAX_X_ZOOM = 4;
const int RANGE_FROM_WINDOW_EDGES = 10;

Buttons::Buttons() 
	: array_size(), i(0), scale(1)
{
	texture[0].loadFromFile("img/left.png");
	sprite[0].setTexture(texture[0]);
	sprite[0].setScale(1, 0.5);

	texture[1].loadFromFile("img/right.png");
	sprite[1].setTexture(texture[1]);
	sprite[1].setScale(1, 0.5);

	texture[2].loadFromFile("img/plus-24572__180.png");
	sprite[2].setTexture(texture[2]);
	sprite[2].setTextureRect(sf::IntRect(0, 0, 90, 90));
	sprite[2].setScale(0.5, 0.5);

	sprite[3].setTexture(texture[2]);
	sprite[3].setTextureRect(sf::IntRect(0, 90, 90, 90));
	sprite[3].setScale(0.5, 0.5);
}

void Buttons::update(sf::Vector2u window)
{
	sprite[0].setPosition(window.x / 2 - sprite[0].getGlobalBounds().width, window.y - sprite[0].getGlobalBounds().height - RANGE_FROM_WINDOW_EDGES);
	sprite[1].setPosition((window.x / 2), window.y - sprite[1].getGlobalBounds().height - RANGE_FROM_WINDOW_EDGES);
	sprite[2].setPosition(window.x - sprite[0].getGlobalBounds().width - RANGE_FROM_WINDOW_EDGES, window.y / 2 - sprite[0].getGlobalBounds().height);
	sprite[3].setPosition(window.x - sprite[1].getGlobalBounds().width - RANGE_FROM_WINDOW_EDGES, window.y / 2);
}

void Buttons::left()
{
	scale = 1;
	if (i - 1 >= 0)
		i--;
	else 
		i = array_size - 1;
}
void Buttons::right()
{
	scale = 1;
	if (i + 1 < array_size)
		i++;
	else 
		i = 0;
}
void Buttons::plus()
{
	if (scale <= MAX_X_ZOOM)
		++scale;
}
void Buttons::minus()
{
	if (scale > 1)
		--scale;
}