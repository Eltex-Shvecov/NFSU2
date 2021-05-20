#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>

class Engine : public sf::Drawable
{
public:
	Engine();
	~Engine();

	void set_value(DWORD value);
private:
	std::vector<sf::CircleShape> _sectors;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
