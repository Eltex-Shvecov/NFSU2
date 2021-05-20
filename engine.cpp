#include "engine.h"

sf::Color const def_red = sf::Color(100, 0, 0, 255);
sf::Color const def_green = sf::Color(0, 100, 0, 255);
sf::Color const active_red = sf::Color(220, 0, 0, 255);
sf::Color const active_green = sf::Color(0, 220, 0, 255);

Engine::Engine()
{
	int x = 10;
	int y = 30;

	for (int i = 0; i < 7; i++)
	{
		sf::CircleShape temp(10.f);
		_sectors.push_back(temp);
		_sectors[i].setPosition(sf::Vector2f(x, y));
		_sectors[i].setFillColor(def_red);

		if (i >= 5)
		{
			_sectors[i].setFillColor(def_green);
		}

		x += 25;
	}
}

Engine::~Engine() {}

void Engine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 7; i++)
	{
		target.draw(_sectors[i]);
	}
}

void Engine::set_value(DWORD value)
{
	value /= 1000;
	if (value <= 7)
	{
		for (int i = 0; i < 7; i++)
		{
			if (i < value)
			{
				if (i > 4)
				{
					_sectors[i].setFillColor(active_green);
				}
				else
				{
					_sectors[i].setFillColor(active_red);
				}
			}
			else
			{
				if (i > 4)
				{
					_sectors[i].setFillColor(def_green);
				}
				else
				{
					_sectors[i].setFillColor(def_red);
				}
			}
		}
	}
}
