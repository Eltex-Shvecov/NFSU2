#include "nitro.h"

Nitro::Nitro()
{
	color = sf::Color(52, 229, 235, 255);
	rectNitro.setSize(sf::Vector2f(380.f, 15.f));
	rectNitro.setPosition(sf::Vector2f(10.f, 10.f));
	rectNitro.setFillColor(color);
}

Nitro::~Nitro(){}

void Nitro::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectNitro);
}

void Nitro::setValue(DWORD nitro)
{
	rectNitro.setSize(sf::Vector2f(nitro / 42.1, 10.f));
}
