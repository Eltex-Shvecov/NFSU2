#include "gear.h"

Gear::Gear()
{
	_font.loadFromFile("C:/Users/Admin/source/repos/NFSU2SFML/Font/arial.ttf");
	_gearstr.setFont(_font);
	_gearstr.setFillColor(sf::Color::Black);
	_gearstr.setPosition(sf::Vector2f(10.f, 50.f));
}

Gear::~Gear()
{

}

void Gear::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_gearstr);
}

void Gear::set_value(DWORD value)
{
	char gr = value;
	gr += '0' - 1;
	if (value == 0)
	{
		_gearstr.setString("R");
	}
	else if (value == 1)
	{
		_gearstr.setString("N");
	}
	else
	{
		_gearstr.setString(gr);
	}
}
