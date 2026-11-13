#include "stdafx.h"
#include "ui/Button.h"

//constructors
Button::Button(float posX, float posY, float width, float height,
	sf::Color idleColor, sf::Color hoverColor, 
	std::string text, sf::Font& font, unsigned characterSize)
	: m_idleColor(idleColor), m_hoverColor(hoverColor), m_buttonText(font, text, characterSize)
{
	m_shape.setPosition({posX, posY});
	m_shape.setSize(sf::Vector2f(width, height));
	m_shape.setFillColor(m_idleColor);

	m_buttonText.setFillColor(sf::Color::White);

	//center text on button
	sf::FloatRect textBounds = m_buttonText.getLocalBounds();
	m_buttonText.setOrigin(textBounds.position + textBounds.size / 2.f);
	m_buttonText.setPosition({posX + width / 2.f, posY + height / 2.f});
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

bool Button::isClicked(const sf::Vector2f& mousePos)
{
	return isMouseOver(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
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
