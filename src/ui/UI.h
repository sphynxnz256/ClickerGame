#pragma once

#include "entities/Enemy.h"
#include "ui/Button.h"

class UI
{
private:
	sf::RectangleShape m_hpMaxBar;
	sf::RectangleShape m_hpCurrentBar;	

	sf::Font m_font;

	sf::Text m_coinsText;
	sf::Text m_damageText;
	sf::Text m_upgradeCostText;

	std::optional<Button> m_upgradeButton;
	std::optional<Button> m_retreatButton;

	sf::Text m_countdownText;
	float m_timeLeft;

	//private initiation functions
	void initHpBar(float targetX, float enemyTop);
	void initFonts();
	void initText(double coins, double damage, double upgradeCost);
	void initUpgradeButton();
	void initRetreatButton();
	void initCountdown(float targetX, float bossTop);

	//private update functions
	void updateCountdown(float targetX, float bossTop, float dt);

	//private render functions
	void renderHpBar(sf::RenderTarget& target, bool escapeAnimation, bool deathAnimation);
	void renderPlayerStats(sf::RenderTarget& target);
	void renderUpradeButton(sf::RenderTarget& target, const sf::Vector2f& mousePos);
	void renderEscapeButton(sf::RenderTarget& target, const sf::Vector2f& mousePos,
		bool bossActive, bool bossEscape, bool deathAnimation);
	void renderCountdown(sf::RenderTarget& target, bool bossActive, bool escapeAnimation, bool deathAnimation);

public:
	//constructor
	UI(float targetX, float enemyTop, double coins, double damage, double upgradeCost, float bossTop);
	//deconstructor
	~UI();

	//getters
	const float getTimeLeft();

	//setters
	void setHpBarPosition(const float targetX, const float enemyTop);
	void setHpBarColor(const double currentHp, const double maxHp);
	void setHpBarLength(const double currentHp, const double maxHp);
	void setHpBarMaxLength(const bool bossActive);
	void setCoinsText(const double coins);
	void setDamageText(const double damage);
	void setUpgradeCostText(const double upgradeCost);
	void setCountdownPosition(float targetX, float bossTop);

	//public functions
	bool upgradeButtonContains(const sf::Vector2f& pos);
	bool retreatButtonContains(const sf::Vector2f& pos);
	void resetTimer();
	void update(const bool bossActive, float targetX, float bossTop, float dt);
	void render(sf::RenderTarget& target, const sf::Vector2f& mousePos,
		bool bossActive, bool bossEscape, bool deathAnimation);
};

