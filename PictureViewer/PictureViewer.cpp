#include "PictureViewer.h"
#include <string>
#include <windows.h>
#include <iostream>

const sf::Vector2u WINDOW_SIZE{ 1024 , 668 };

PictureViewer::PictureViewer() 
	:dx(0)
	,dy(0)
	,is_pressed(false)
	,mouse()
	,start_position()
	,mouse_position()
	,window_size()
{

}

void PictureViewer::run()
{
	std::string path;
	std::cout << "Input path to directory with images(Example: D:/Photos/):" << std::endl;
	std::cin >> path;
	image_names_vector = getImageVector(image_names_vector, path);

	while (image_names_vector.size() == 0)
	{
		std::cerr << "Incorrect path or directory is empty. Try again" << std::endl;
		std::cin >> path;
		image_names_vector = getImageVector(image_names_vector, path);
	}
	window.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "");
	for (int i = 0; i < image_names_vector.size(); i++)
		textures.emplace_back(texture(image_names_vector[i], current_texture));

	buttons.array_size = (textures.size());

	while (window.isOpen())
	{

		mouse_position = mouse.getPosition(window);
		window_size = window.getSize();

		Picture picture(textures[buttons.i]);

		picture.update(buttons.scale, window_size);
		buttons.update(window_size);

		sf::View view(sf::FloatRect(0, 0, window_size.x, window_size.y));

		if (!is_pressed && buttons.scale != 1)
			picture.sprite.setPosition(start_position);

		sf::FloatRect picture_bounds = picture.sprite.getGlobalBounds();

		if (is_pressed && buttons.scale != 1)
		{
			if (picture_bounds.intersects(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)))
			{
				if (picture_bounds.width < window_size.x)
				{
					picture.sprite.setPosition(start_position.x, mouse_position.y - dy);
					start_position = { picture.sprite.getPosition().x, mouse_position.y - dy };
				}
				else if (picture_bounds.height < window_size.y)
				{
					picture.sprite.setPosition(mouse_position.x - dx, start_position.y);
					start_position = { mouse_position.x - dx, picture.sprite.getPosition().y };
				}
				else
				{
					start_position = { mouse_position.x - dx,mouse_position.y - dy };
					picture.sprite.setPosition(mouse_position.x - dx, mouse_position.y - dy);
				}
			}
		}
		processEvents(picture.sprite);
		render(view, picture.sprite);
	}
}

void PictureViewer::processEvents(sf::Sprite& sprite)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Left:
				buttons.left();
				break;
			case sf::Keyboard::Right:
				buttons.right();
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left)
			{
				if (checkClicking(sprite, mouse_position))
				{
					dx = mouse_position.x - start_position.x;
					dy = mouse_position.y - start_position.y;
					is_pressed = true;
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.key.code == sf::Mouse::Left)
			{
				if (checkClicking(buttons.sprite[0], mouse_position))
				{
					start_position = {};
					buttons.left();
				}
				else if (checkClicking(buttons.sprite[1], mouse_position))
				{
					start_position = {};
					buttons.right();
				}
				else if (checkClicking(buttons.sprite[2], mouse_position))
				{
					if (start_position == sf::Vector2f{})
						start_position = { float(window_size.x / 2), float(window_size.y / 2) };
					buttons.plus();
				}
				else if (checkClicking(buttons.sprite[3], mouse_position))
				{
					if (start_position == sf::Vector2f{}) 
						start_position = { float(window_size.x / 2), float(window_size.y / 2) };
					buttons.minus();
				}
				if (is_pressed)
					is_pressed = false;
			}
			break;
		}
	}
}

void PictureViewer::render(sf::View& view, sf::Sprite& sprite)
{
	window.clear(sf::Color::White);
	window.setTitle(image_names_vector[buttons.i]);
	window.setView(view);

	window.draw(sprite);
	for (int i = 0; i < BUTTONS; ++i) window.draw(buttons.sprite[i]);
	window.display();
}

std::vector<std::string> PictureViewer::getImageVector(std::vector<std::string> image_vector, std::string & old_path) {
	std::string ext_array[] = { ".bmp", ".png", ".tga", ".jpg", ".gif", ".psd", ".hdr", ".pic" };
	WIN32_FIND_DATA list_files;
	HANDLE image_file = FindFirstFile((old_path + "*").c_str(), &list_files);
	if (image_file != INVALID_HANDLE_VALUE) {
		do {
			if (!(list_files.dwFileAttributes& FILE_ATTRIBUTE_DIRECTORY))
			{
				bool is_image = false;
				std::string file_name = std::string(list_files.cFileName);
				for (std::string str : ext_array)
				{
					if (file_name.rfind(str) != -1)
					{
						is_image = true;
						break;
					}
				}
				if (is_image)
					image_vector.emplace_back(old_path + file_name);
			}
		} while (FindNextFile(image_file, &list_files));
	}
	return image_vector;
}

sf::Texture PictureViewer::texture(std::string file_name, sf::Texture texture)
{
	if (!texture.loadFromFile(file_name))
	{
		texture.loadFromFile("img/error_message.png");
	}
	return texture;
}