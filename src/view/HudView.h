#pragma once

#include "view/Button.h"
#include "state/GameState.h"

class HudView
{
private:
	sf::Text m_coinsText;
	sf::Text m_damageText;
	sf::Text m_upgradeCostText;

	std::optional<Button> m_upgradeButton;
	std::optional<Button> m_retreatButton;

	void updateText(const PlayerState& player);

public:
	HudView(const sf::Font& font, const PlayerState& player);

	bool upgradeButtonContains(const sf::Vector2f& pos);
	bool retreatButtonContains(const sf::Vector2f& pos);
	void render(sf::RenderTarget& target, const GameState& state, const sf::Vector2f& mousePos);
};
