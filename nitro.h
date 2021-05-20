#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>

class Nitro : public sf::Drawable
{
public:
	Nitro();
	~Nitro();

	void setValue(DWORD nitro);

private:
	sf::Color color;
	sf::RectangleShape rectNitro;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
