#pragma once
class Button
{
private:
	//private variables
	sf::RectangleShape m_shape;
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
	sf::Text m_buttonText;

public:
	//constructors
	Button(float positionX, float positionY, float width, float height,
		sf::Color idleColor, sf::Color hoverColor,
		std::string text, sf::Font& font, unsigned characterSize);

	//getters
	sf::FloatRect getGlobalBounds();
	sf::Vector2f getPosition();

	//public functions
	bool isMouseOver(const sf::Vector2f& mousePos);

	void render(sf::RenderTarget& target, const sf::Vector2f& mousePos);
};

