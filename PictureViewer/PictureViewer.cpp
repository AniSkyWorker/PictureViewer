#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <iostream>
#include <vector>


const sf::Vector2u WINDOW_SIZE{ 1024 , 668 };
const int BUTTONS = 4;

std::vector<std::string> get_image_vector(std::vector<std::string> image_vector, std::string & old_path) {
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

sf::Texture texture(std::string file_name, sf::Texture texture)
{
	if (!texture.loadFromFile(file_name))
	{
		texture.loadFromFile("img/error_message.png");
	}
	return texture;
}

struct Picture
{
	sf::Sprite sprite;
	Picture(sf::Texture& texture)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	}

	void update(float scale, sf::Vector2u window)
	{
		if (scale == 1)
			sprite.setPosition(window.x / 2, window.y / 2);
		float x_diff_size = window.x / sprite.getGlobalBounds().width;
		float y_diff_size = window.y / sprite.getGlobalBounds().height;
		if (x_diff_size < 1 || y_diff_size < 1)
			scale = scale * std::min(x_diff_size, y_diff_size);
		sprite.setScale(scale, scale);
	}
};

struct Buttons
{
	sf::Texture texture[BUTTONS - 1];
	sf::Sprite sprite[BUTTONS];
	int array_size;
	int i = 0;
	float scale = 1;
	Buttons(int number)
	{
		array_size = number;

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

	void update(sf::Vector2u window)
	{
		sprite[0].setPosition(window.x / 2 - sprite[0].getGlobalBounds().width, window.y - sprite[0].getGlobalBounds().height - 10);
		sprite[1].setPosition((window.x / 2), window.y - sprite[1].getGlobalBounds().height - 10);
		sprite[2].setPosition(window.x - sprite[0].getGlobalBounds().width - 10, window.y / 2 - sprite[0].getGlobalBounds().height);
		sprite[3].setPosition(window.x - sprite[1].getGlobalBounds().width - 10, window.y / 2);
	}

	void left()
	{
		scale = 1;
		if (i - 1 >= 0) i--;
		else i = array_size - 1;
	}
	void right()
	{
		scale = 1;
		if (i + 1 < array_size) i++;
		else i = 0;
	}

	void plus()
	{
		if (scale <= 4)
		{
			++scale;
		}
	}
	void minus()
	{
		if (scale > 1)
		{
			--scale;
		}
	}
};

template<class T> bool checkClicking(T& sprite, sf::Vector2i& start_position)
{
	return sprite.getGlobalBounds().contains(start_position.x, start_position.y);
}

int main()
{
	std::string path;
	std::cout << "Input path to directory with images(Example: D:/Photos/):" << std::endl;
	std::cin >> path;
	std::vector<std::string> image_names_vector = get_image_vector(image_names_vector, path);

	while (image_names_vector.size() == 0)
	{
		std::cerr << "Incorrect path or directory is empty. Try again" << std::endl;
		std::cin >> path;
		image_names_vector = get_image_vector(image_names_vector, path);
	}

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "");
	std::vector<sf::Texture> textures;
	sf::Texture texture1;
	for (int i = 0; i < image_names_vector.size(); i++)
		textures.emplace_back(texture(image_names_vector[i], texture1));

	Buttons buttons(textures.size());

	float dx = 0;
	float dy = 0;
	bool isPressed = false;
	sf::Mouse mouse;
	sf::Vector2f start_position = {};

	while (window.isOpen())
	{

		sf::Vector2i mouse_position = mouse.getPosition(window);
		sf::Vector2u window_size = window.getSize();

		Picture picture(textures[buttons.i]);
		picture.update(buttons.scale, window_size);

		buttons.update(window_size);

		sf::View view(sf::FloatRect(0, 0, window_size.x, window_size.y));

		if (!isPressed && buttons.scale != 1)
			picture.sprite.setPosition(start_position);

		sf::FloatRect picture_bounds = picture.sprite.getGlobalBounds();

		if (isPressed && buttons.scale != 1)
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
					if (checkClicking(picture.sprite, mouse_position))
					{
						dx = mouse_position.x - start_position.x;
						dy = mouse_position.y - start_position.y;
						isPressed = true;
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
						if (start_position == sf::Vector2f{}) start_position = { float(window_size.x / 2), float(window_size.y / 2) };
						buttons.plus();
					}
					else if (checkClicking(buttons.sprite[3], mouse_position))
					{
						if (start_position == sf::Vector2f{}) start_position = { float(window_size.x / 2), float(window_size.y / 2) };
						buttons.minus();
					}
					if (isPressed)
						isPressed = false;
				}
				break;
			}
		}

		window.clear(sf::Color::White);
		window.setTitle(image_names_vector[buttons.i]);
		window.setView(view);

		window.draw(picture.sprite);
		for (int i = 0; i < BUTTONS; ++i) window.draw(buttons.sprite[i]);
		window.display();
	}
	return 0;
}