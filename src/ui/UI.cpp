#include "stdafx.h"
#include "ui/UI.h"
#include "view/NumberFormat.h"

/*the ui class displays info useful to the user*/

//private functions
void UI::initHpBar(float targetX, float enemyTop)
{
	m_hpMaxBar.setSize(sf::Vector2f(130.f, 20.f));
	m_hpMaxBar.setFillColor(sf::Color(200, 200, 200, 100));
	m_hpMaxBar.setOutlineColor(sf::Color::Black);
	m_hpMaxBar.setOutlineThickness(1.f);

	m_hpCurrentBar.setSize(m_hpMaxBar.getSize());
	m_hpCurrentBar.setFillColor(sf::Color::Green);

	setHpBarPosition(targetX, enemyTop);
}

void UI::initFonts()
{
	if (!m_font.openFromFile("fonts/arial.ttf"))
	{
		std::cout << "ERROR::UI::INITFONTS::Could not load arial.ttf\n";
	}
}

void UI::initText(double coins, double damage, double upgradeCost)
{
	//displays the amount of coins the player as earned
	m_coinsText.setCharacterSize(24);
	m_coinsText.setOutlineColor(sf::Color::Black);
	m_coinsText.setOutlineThickness(1.f);
	std::stringstream ssCoins;
	ssCoins << "Coins: " << formatNumber(coins);
	m_coinsText.setString(ssCoins.str());
	m_coinsText.setPosition({20.f, 20.f});
	
	//displays the current damage the player deals
	m_damageText.setCharacterSize(24);
	m_damageText.setOutlineColor(sf::Color::Black);
	m_damageText.setOutlineThickness(1.f);
	std::stringstream ssDamage;
	ssDamage << "Damage: " << formatNumber(damage);
	m_damageText.setString(ssDamage.str());
	m_damageText.setPosition({20.f, m_coinsText.getPosition().y + m_coinsText.getGlobalBounds().size.y + 10.f});

	//displays current cost of upgrade
	m_upgradeCostText.setCharacterSize(16);
	m_upgradeCostText.setOutlineColor(sf::Color::Black);
	m_upgradeCostText.setOutlineThickness(1.f);
	std::stringstream ssUpgradeCost;
	ssUpgradeCost << "Upgrade damage cost: " << formatNumber(upgradeCost) << "Coins";
	m_upgradeCostText.setString(ssUpgradeCost.str());

	//displays timer for boss fight
	m_countdownText.setCharacterSize(24);
	m_countdownText.setOutlineColor(sf::Color::Black);
	m_countdownText.setOutlineThickness(1.f);
	m_countdownText.setString("not set");
}

void UI::initUpgradeButton()
{
	//creates an upgrade button for upgrading damage
	float posX = 20.f;
	float posY = m_damageText.getPosition().y + m_damageText.getGlobalBounds().size.y + 10.f;
	float width = 100.f;
	float height = 30.f;
	sf::Color idleColor = sf::Color(100, 100, 100, 255);
	sf::Color hoverColor = sf::Color(50, 50, 50, 255);
	std::string text = "Upgrade";
	int characterSize = 20;

	m_upgradeButton.emplace(posX, posY, width, height,
		idleColor, hoverColor, text, m_font, characterSize);

	//set upgrade cost text position once upgrade button's position is set
	m_upgradeCostText.setPosition({20.f, 
		m_upgradeButton->getPosition().y + m_upgradeButton->getGlobalBounds().size.y + 5.f});
}

void UI::initRetreatButton()
{
	//creates an upgrade button for upgrading damage
	float posX = 20.f;
	float posY = m_upgradeCostText.getPosition().y + m_upgradeCostText.getGlobalBounds().size.y + 10.f;
	float width = 100.f;
	float height = 30.f;
	sf::Color idleColor = sf::Color(100, 100, 100, 255);
	sf::Color hoverColor = sf::Color(50, 50, 50, 255);
	std::string text = "Retreat";
	int characterSize = 20;

	m_retreatButton.emplace(posX, posY, width, height,
		idleColor, hoverColor, text, m_font, characterSize);
}

void UI::initCountdown(float targetX, float bossTop)
{
	m_countdownText.setPosition({targetX / 2 - m_countdownText.getGlobalBounds().size.x / 2,
		bossTop - 60.f}); 
}

void UI::updateCountdown(float targetX, float bossTop, double timeLeft)
{
	//update countdown text
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << timeLeft;
	m_countdownText.setString(ss.str());
	m_countdownText.setPosition({targetX / 2 - m_countdownText.getGlobalBounds().size.x / 2,
		bossTop - 60.f});
	m_countdownText.setFillColor(timeLeft < 3.0 ? sf::Color::Red : sf::Color::White);
}

void UI::renderHpBar(sf::RenderTarget& target, bool escapeAnimation, bool deathAnimation)
{
	if (!escapeAnimation && !deathAnimation)
	{
		target.draw(m_hpMaxBar); 
		target.draw(m_hpCurrentBar); 
	}
}

void UI::renderPlayerStats(sf::RenderTarget& target)
{
	target.draw(m_coinsText); 
	target.draw(m_damageText); 
}

void UI::renderUpradeButton(sf::RenderTarget& target, const sf::Vector2f& mousePos)
{
	m_upgradeButton->render(target, mousePos); 
	if (m_upgradeButton->isMouseOver(mousePos)) 
	{
		target.draw(m_upgradeCostText); 
	}
}

void UI::renderEscapeButton(sf::RenderTarget& target, const sf::Vector2f& mousePos,
	bool bossActive, bool escapeAnimation, bool deathAnimation)
{
	if (bossActive && !escapeAnimation && !deathAnimation)
	{
		m_retreatButton->render(target, mousePos);
	}
}

void UI::renderCountdown(sf::RenderTarget& target, bool bossActive, bool escapeAnimation, bool deathAnimation)
{
	if (bossActive && !escapeAnimation && !deathAnimation)
	{
		target.draw(m_countdownText); 
	}
}

//constructors
UI::UI(float targetX, float enemyTop, double coins, double damage, double upgradeCost, float bossTop)
	: m_coinsText(m_font), m_damageText(m_font), m_upgradeCostText(m_font), m_countdownText(m_font)
{
	initHpBar(targetX, enemyTop); 
	initFonts();
	initText(coins, damage, upgradeCost);
	initUpgradeButton();
	initRetreatButton();
	initCountdown(targetX, bossTop);
}

//deconstructor
UI::~UI()
{
}

//setters
void UI::setHpBarPosition(const float targetX, const float enemyTop)
{
	float xPosition = targetX / 2 - m_hpMaxBar.getGlobalBounds().size.x / 2;
	float yPosition = enemyTop - 20.f;
	m_hpMaxBar.setPosition({xPosition, yPosition});
	m_hpCurrentBar.setPosition({xPosition, yPosition});
}

void UI::setHpBarColor(const double currentHp, const double maxHp)
{
	if (currentHp > 0.5f * maxHp)
	{
		m_hpCurrentBar.setFillColor(sf::Color(static_cast <int>(255 - (255 * currentHp / maxHp)) * 2, 255, 0, 255));
	}
	else
	{
		m_hpCurrentBar.setFillColor(sf::Color(255, static_cast <int>(255 * (currentHp * 2) / maxHp), 0, 255));
	}
}

void UI::setHpBarLength(const double currentHp, const double maxHp)
{
	m_hpCurrentBar.setSize(sf::Vector2f(static_cast<float>(m_hpMaxBar.getSize().x * currentHp / maxHp), m_hpMaxBar.getSize().y));
}

void UI::setHpBarMaxLength(const bool bossActive)
{
	if (bossActive)
	{
		m_hpMaxBar.setSize(sf::Vector2f(260.f, m_hpMaxBar.getSize().y));
	}
	else
	{
		m_hpMaxBar.setSize(sf::Vector2f(130.f, m_hpMaxBar.getSize().y));
	}
}

void UI::setCoinsText(const double coins)
{
	std::stringstream ss;
	ss << "Coins: " << formatNumber(coins) << '\n';
	m_coinsText.setString(ss.str());
}

void UI::setDamageText(const double damage)
{
	std::stringstream ss;
	ss << "Damage: " << formatNumber(damage) << '\n';
	m_damageText.setString(ss.str());
}

void UI::setUpgradeCostText(const double upgradeCost)
{
	std::stringstream ss;
	ss << "Upgrade damage cost: " << formatNumber(upgradeCost) << " Coins\n";
	m_upgradeCostText.setString(ss.str());
}

void UI::setCountdownPosition(float targetX, float bossTop)
{
	m_countdownText.setPosition({targetX / 2 - m_countdownText.getGlobalBounds().size.x / 2,
		bossTop - 60.f});
}

//public functions
bool UI::upgradeButtonContains(const sf::Vector2f& pos)
{
	return m_upgradeButton->isMouseOver(pos);
}

bool UI::retreatButtonContains(const sf::Vector2f& pos)
{
	return m_retreatButton->isMouseOver(pos);
}

void UI::update(const bool bossActive, float targetX, float bossTop, double timeLeft)
{
	if(bossActive)	{
		
		updateCountdown(targetX, bossTop, timeLeft);
	}
}

void UI::render(sf::RenderTarget& target, const sf::Vector2f& mousePos,
	bool bossActive, bool escapeAnimation, bool deathAnimation)
{
	renderHpBar(target, escapeAnimation, deathAnimation);
	renderPlayerStats(target);
	renderUpradeButton(target, mousePos);
	renderEscapeButton(target, mousePos, bossActive, escapeAnimation, deathAnimation);
	renderCountdown(target, bossActive, escapeAnimation, deathAnimation);
}
