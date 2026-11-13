#include "stdafx.h"
#include "ui/Button.h"

//constructors
Button::Button(float positionX, float positionY, float width, float height,
	sf::Color idleColor, sf::Color hoverColor, 
	std::string text, sf::Font& font, unsigned characterSize)
	: m_idleColor(idleColor), m_hoverColor(hoverColor), m_buttonText(font, text, characterSize)
{
	m_shape.setPosition({positionX, positionY});
	m_shape.setSize(sf::Vector2f(width, height));
	m_shape.setFillColor(m_idleColor);

	m_buttonText.setFillColor(sf::Color::White);

	//center text on button
	sf::FloatRect textBounds = m_buttonText.getLocalBounds();
	m_buttonText.setOrigin(textBounds.position + textBounds.size / 2.f);
	m_buttonText.setPosition({positionX + width / 2.f, positionY + height / 2.f});
}

//getters
sf::FloatRect Button::getGlobalBounds()
{
	return m_shape.getGlobalBounds();
}

sf::Vector2f Button::getPosition()
{
	return m_shape.getPosition();
}

//public functions
bool Button::isMouseOver(const sf::Vector2f& mousePos)
{
	return m_shape.getGlobalBounds().contains(mousePos);
}

void Button::render(sf::RenderTarget& target, const sf::Vector2f& mousePos)
{

	if (isMouseOver(mousePos))
	{
		m_shape.setFillColor(m_hoverColor);
	}
	else
	{
		m_shape.setFillColor(m_idleColor);
	}

	target.draw(m_shape);
	target.draw(m_buttonText);
}
