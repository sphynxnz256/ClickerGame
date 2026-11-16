#include "stdafx.h"
#include "view/BossCountdownView.h"

/*the countdown shows how long until the boss escapes*/

//constructor
BossCountdownView::BossCountdownView(const sf::Font& font) : m_text(font)
{
	m_text.setCharacterSize(24);
	m_text.setOutlineColor(sf::Color::Black);
	m_text.setOutlineThickness(1.f);
}

//public functions
void BossCountdownView::render(sf::RenderTarget& target, double timeLeft, float targetX, float bossTop)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << timeLeft;
	m_text.setString(ss.str());
	m_text.setPosition({targetX / 2 - m_text.getGlobalBounds().size.x / 2,
		bossTop - 60.f});
	m_text.setFillColor(timeLeft < 3.0 ? sf::Color::Red : sf::Color::White);

	target.draw(m_text);
}
