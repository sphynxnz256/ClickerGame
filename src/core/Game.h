#pragma once

#include "entities/Enemy.h"
#include "entities/Player.h"
#include "entities/Boss.h"
#include "ui/UI.h"
#include "graphics/Animator.h"
#include "core/RNG.h"
#include "entities/Coin.h"

class Game
{
private:
	//private variables
	sf::RenderWindow m_window;
	sf::Clock m_frameClock;

	Player* m_player;
	Enemy* m_enemy;
	Boss* m_boss;
	UI* m_ui;

	Animator* m_animator;
	bool m_playDeathAnimation;
	bool m_playEscapeAnimation;
	bool m_playCoinAnimation;

	RNG& m_rng;

	bool m_mouseHeld;
	sf::Vector2f m_mousePosition;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;

	int m_killCounter;
	bool m_bossActive;
	int m_coinsToDrop;
	std::vector<Coin> m_coinsVector;
	std::shared_ptr<sf::Texture> m_sharedCoinTexture;
	sf::Vector2f m_coinStart;
	sf::Vector2f m_coinEnd;

	//private initiation functions
	void initWindow();
	void initPlayer();
	void initEnemy();
	void initBoss();
	void initAnimator();
	void initBackground();
	void initCoins();
	void initUI();

	//private update functions
	void polledEvents();
	void updateCombat();
	void updateUI(float dt);
	void updateAnimator(float dt);
	void updateCoins(float dt);

	//private rendering functions
	void renderEnemy();
	void renderBoss();
	void renderAnimator();
	void renderBackground();
	void renderCoins();
	void renderUI();

	//other private functions
	void bossTakeDamage();
	void enemyTakeDamage();
	void bossEscape();
	void enemyDeath();
	void dropCoins(std::pair<int, int> coinsToDrop, sf::Vector2f coinStart);
	void setUpBoss();
	void bossDeath();
	void upgradeDamage();
	void retreat();

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

