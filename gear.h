#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>

class Gear : public sf::Drawable
{
public:
	Gear();
	~Gear();

	void set_value(DWORD value);
private:
	sf::Font _font;
	sf::Text _gearstr;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
