#include "stdafx.h"
#include "view/HudView.h"
#include "view/NumberFormat.h"

/*the hud shows coins, damage, the upgrade cost tooltip and the two buttons*/

//private functions
void HudView::updateText(const PlayerState& player)
{
	m_coinsText.setString("Coins: " + formatNumber(player.coins));
	m_damageText.setString("Damage: " + formatNumber(player.damage));
	m_upgradeCostText.setString("Upgrade damage cost: " + formatNumber(player.upgradeCost) + " Coins");
}

//constructor
HudView::HudView(const sf::Font& font, const PlayerState& player)
	: m_coinsText(font), m_damageText(font), m_upgradeCostText(font)
{
	updateText(player);

	//displays the amount of coins the player as earned
	m_coinsText.setCharacterSize(24);
	m_coinsText.setOutlineColor(sf::Color::Black);
	m_coinsText.setOutlineThickness(1.f);
	m_coinsText.setPosition({20.f, 20.f});

	//displays the current damage the player deals
	m_damageText.setCharacterSize(24);
	m_damageText.setOutlineColor(sf::Color::Black);
	m_damageText.setOutlineThickness(1.f);
	m_damageText.setPosition({20.f, m_coinsText.getPosition().y + m_coinsText.getGlobalBounds().size.y + 10.f});

	//displays current cost of upgrade
	m_upgradeCostText.setCharacterSize(16);
	m_upgradeCostText.setOutlineColor(sf::Color::Black);
	m_upgradeCostText.setOutlineThickness(1.f);

	//creates an upgrade button for upgrading damage
	float upgradeX = 20.f;
	float upgradeY = m_damageText.getPosition().y + m_damageText.getGlobalBounds().size.y + 10.f;
	m_upgradeButton.emplace(upgradeX, upgradeY, 100.f, 30.f,
		sf::Color(100, 100, 100, 255), sf::Color(50, 50, 50, 255), "Upgrade", font, 20);

	//set upgrade cost text position once upgrade button's position is set
	m_upgradeCostText.setPosition({20.f,
		m_upgradeButton->getPosition().y + m_upgradeButton->getGlobalBounds().size.y + 5.f});

	//creates a retreat button for escaping a boss fight
	float retreatX = 20.f;
	float retreatY = m_upgradeCostText.getPosition().y + m_upgradeCostText.getGlobalBounds().size.y + 10.f;
	m_retreatButton.emplace(retreatX, retreatY, 100.f, 30.f,
		sf::Color(100, 100, 100, 255), sf::Color(50, 50, 50, 255), "Retreat", font, 20);
}

//public functions
bool HudView::upgradeButtonContains(const sf::Vector2f& pos)
{
	return m_upgradeButton->isMouseOver(pos);
}

bool HudView::retreatButtonContains(const sf::Vector2f& pos)
{
	return m_retreatButton->isMouseOver(pos);
}

void HudView::render(sf::RenderTarget& target, const GameState& state, const sf::Vector2f& mousePos)
{
	updateText(state.player);

	target.draw(m_coinsText);
	target.draw(m_damageText);

	m_upgradeButton->render(target, mousePos);
	if (m_upgradeButton->isMouseOver(mousePos))
	{
		target.draw(m_upgradeCostText);
	}

	if (state.enemy.type->isBoss && state.phase == Phase::Fighting)
	{
		m_retreatButton->render(target, mousePos);
	}
}
