#pragma once

#include "content/EnemyTypes.h"
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
	sf::RenderWindow m_window;
	sf::Clock m_frameClock;
	TextureCache m_textures;

	GameState m_state;
	EnemyView* m_enemyView;
	sf::Font m_font;
	HudView* m_hud;
	HpBarView* m_hpBar;
	BossCountdownView* m_bossCountdown;

	DeathAnimation m_deathAnimation;
	SmokeAnimation m_smokeAnimation;

	RNG& m_rng;

	sf::Vector2f m_mousePosition;

	sf::Sprite m_background;

	std::vector<Coin> m_coinsVector;

	//private initiation functions
	void initWindow();
	void initEnemy();
	void initFont();
	void initUI();

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
	void onEnemyKilled();
	void onBossEscaped();
	void finishDeathIfDone();
	void finishEscapeIfDone();

public:
	//constructor
	Game(RNG& rng);
	//deconstuctor
	~Game();

	//getters
	const sf::RenderWindow& getWindow() const;

	//public functions	
	void update();
	void render();
};

