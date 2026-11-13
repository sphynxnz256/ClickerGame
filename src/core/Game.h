#pragma once

#include "content/EnemyTypes.h"
#include "entities/Enemy.h"
#include "state/GameState.h"
#include "ui/UI.h"
#include "graphics/Animator.h"
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
	Enemy* m_enemy;
	UI* m_ui;

	Animator* m_animator;

	RNG& m_rng;

	sf::Vector2f m_mousePosition;

	sf::Sprite m_background;

	std::vector<Coin> m_coinsVector;

	//private initiation functions
	void initWindow();
	void initEnemy();
	void initAnimator();
	void initUI();

	//private update functions
	void polledEvents();
	void handleClick(sf::Vector2f clickPos);
	void updateCombat();
	void updateUI(float dt);
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
	void damageEnemy();
	void spawnEnemy(const EnemyType& type);
	void bossEscape();
	void enemyDeath();
	void dropCoins(std::pair<int, int> coinsToDrop, sf::Vector2f coinStart);
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

