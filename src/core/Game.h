#pragma once

#include "content/EnemyData.h"
#include "view/EnemyView.h"
#include "rules/Combat.h"
#include "rules/Upgrades.h"
#include "rules/BossTimer.h"
#include "rules/Spawning.h"
#include "state/GameState.h"
#include "view/HudView.h"
#include "view/HpBarView.h"
#include "view/BossCountdownView.h"
#include "view/DeathAnimation.h"
#include "view/SmokeAnimation.h"
#include "core/RNG.h"
#include "entities/Coin.h"
#include "view/TextureCache.h"

class Game
{
private:
	//private variables
	//members construct in this order, so dependencies come first
	RNG& m_rng;
	sf::RenderWindow m_window;
	sf::Clock m_frameClock;
	TextureCache m_textures;

	GameState m_state;
	sf::Font m_font;
	EnemyView m_enemyView;
	HudView m_hud;
	HpBarView m_hpBar;
	BossCountdownView m_bossCountdown;

	DeathAnimation m_deathAnimation;
	SmokeAnimation m_smokeAnimation;

	sf::Vector2f m_mousePosition;

	sf::Sprite m_background;

	std::vector<Coin> m_coinsVector;

	//private update functions
	void polledEvents();
	void handleClick(sf::Vector2f clickPos);
	void updateCombat(float dt);
	void updateAnimator(float dt);
	void updateCoins(float dt);

	//private rendering functions
	void renderEnemy();
	void renderAnimator();
	void renderBackground();
	void renderCoins();
	void renderUI();

	//other private functions
	bool isBossFight() const;
	void dropCoins(int coinCount, sf::Vector2f coinStart);
	void onEnemySpawned();
	void centreEnemy();
	void onEnemyKilled();
	void onBossEscaped();
	void finishDeathIfDone();
	void finishEscapeIfDone();

public:
	//constructor
	Game(RNG& rng);

	//getters
	const sf::RenderWindow& getWindow() const;

	//public functions	
	void update();
	void render();
};

