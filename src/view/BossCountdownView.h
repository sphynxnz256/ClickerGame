#pragma once

class BossCountdownView
{
private:
	sf::Text m_text;

public:
	BossCountdownView(const sf::Font& font);

	void render(sf::RenderTarget& target, double timeLeft, float targetX, float bossTop);
};
