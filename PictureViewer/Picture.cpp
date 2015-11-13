#include "Picture.h"
Picture::Picture(sf::Texture& texture) : sprite(texture)
{

}
void Picture::update(float scale, sf::Vector2u window)
{
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);

	if (scale == 1)
		sprite.setPosition(window.x / 2, window.y / 2);
	scale = checkCurrentScale(scale, window);
	sprite.setScale(scale, scale);
}

float Picture::checkCurrentScale(float scale, sf::Vector2u window)
{
	float x_diff_size = window.x / sprite.getGlobalBounds().width;
	float y_diff_size = window.y / sprite.getGlobalBounds().height;
	if (x_diff_size < 1 || y_diff_size < 1)
		scale = scale * std::min(x_diff_size, y_diff_size);
	return scale;
}