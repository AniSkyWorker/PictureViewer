#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Buttons.h"
#include "Picture.h"


struct PictureViewer
{
	PictureViewer();
	void run();
	void processEvents(sf::Sprite& sprite);
	void render(sf::View& view, sf::Sprite& sprite);

	std::vector<std::string> getImageVector(std::vector<std::string> image_vector, std::string & old_path);
	sf::Texture texture(std::string file_name, sf::Texture texture);

	sf::RenderWindow window;
	std::vector<sf::Texture> textures;
	std::vector<std::string> image_names_vector;
	sf::Texture current_texture;

	float dx;
	float dy;
	bool is_pressed;
	sf::Mouse mouse;
	sf::Vector2f start_position;
	sf::Vector2i mouse_position;
	sf::Vector2u window_size;

	Buttons buttons;

	template<class T> bool checkClicking(T& sprite, sf::Vector2i& start_position)
	{
		return sprite.getGlobalBounds().contains(start_position.x, start_position.y);
	}
};