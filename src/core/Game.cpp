#include "stdafx.h"
#include "core/Game.h"

//the game class runs the game

//private initiation functions
void Game::initWindow()
{
	//creates the game window
	m_window.create(sf::VideoMode({800, 600}), "Clicker Game", sf::Style::Close | sf::Style::Titlebar);
}

void Game::initPlayer()
{
	//holds player state
	m_player = new Player();
}

void Game::initEnemy()
{
	//stores enemy state
	m_enemy = new Enemy(m_rng, m_textures);
	m_enemy->setPosition(
		m_window.getSize().x /2 - m_enemy->getGlobalBounds().size.x / 2, 
		m_window.getSize().y / 2 - m_enemy->getGlobalBounds().size.y / 2);
}

void Game::initBoss()
{
	m_killCounter = 0;
	m_boss = new Boss(m_textures);
	m_boss->setPosition( 
		m_window.getSize().x / 2 - m_boss->getGlobalBounds().size.x / 2, 
		m_window.getSize().y / 2 - m_boss->getGlobalBounds().size.y / 2); 
	m_bossActive = false;
}

void Game::initAnimator()
{ 
	m_animator = new Animator(m_textures);
	m_playDeathAnimation = false;
	m_playEscapeAnimation = false;
	m_animator->setSmokeSpritePosition(
		m_window.getSize().x / 2 - m_animator->getSmokeSpriteGlobalBounds().size.x / 2,
		m_window.getSize().y / 2 - m_animator->getSmokeSpriteGlobalBounds().size.y / 2);
}

void Game::initUI()
{
	//create an hp bar and set its position above the enemy
	m_ui = new UI(static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y,
		m_player->getCoins(), m_player->getDamage(), m_player->getUpgradeCost(),
		m_boss->getGlobalBounds().position.y);
}

void Game::polledEvents()
{
	//this lets us close the window using the x button
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}
		else if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (pressed->button == sf::Mouse::Button::Left)
			{
				handleClick(sf::Vector2f(static_cast<float>(pressed->position.x),
					static_cast<float>(pressed->position.y)));
			}
		}
	}
}

void Game::handleClick(sf::Vector2f clickPos)
{
	//first match wins
	if (m_ui->upgradeButtonContains(clickPos))
	{
		upgradeDamage();
		return;
	}

	//retreat button is only visible (and clickable) during a boss fight with no animation playing
	if (m_bossActive && !m_playEscapeAnimation && !m_playDeathAnimation
		&& m_ui->retreatButtonContains(clickPos))
	{
		retreat();
		return;
	}

	//collect any coins under the click
	bool collected = false;
	std::vector<Coin> coinsToKeep;
	for (auto& coin : m_coinsVector)
	{
		if (coin.isMouseOver(clickPos) && coin.checkPosition())
		{
			m_player->addCoins(coin.getCoinValue());
			m_ui->setCoinsText(m_player->getCoins());
			collected = true;
		}
		else
		{
			coinsToKeep.push_back(coin);
		}
	}
	m_coinsVector.swap(coinsToKeep);
	if (collected)
	{
		return;
	}

	//attack the boss or enemy
	if (!m_playDeathAnimation && !m_playEscapeAnimation)
	{
		if (m_bossActive)
		{
			if (m_boss->getGlobalBounds().contains(clickPos))
			{
				bossTakeDamage();
			}
		}
		else if (m_enemy->getGlobalBounds().contains(clickPos))
		{
			enemyTakeDamage();
		}
	}
}

//private update functions
void Game::updateCombat()
{
	//check no animations are playing
	if(!m_playDeathAnimation && !m_playEscapeAnimation)
	{
		//if fighting boss, check to see if boss escapes
		if (m_bossActive && m_ui->getTimeLeft() <= 0.f)
		{
			m_playEscapeAnimation = true;
			bossEscape();			
		}

		//if fighting boss and its dead, play death animation
		if (m_bossActive && m_boss->isDead())
		{
			m_animator->setSprite(m_boss->getSprite());
			m_playDeathAnimation = true;

			//coins drop
			dropCoins(m_boss->getCoinsToDrop(), sf::Vector2f(
				m_boss->getPosition().x + m_boss->getGlobalBounds().size.x / 2,
				m_boss->getPosition().y + m_boss->getGlobalBounds().size.y / 2));

			bossDeath();
		}
		//if fighting regular enemy and its dead, play death animation
		else if (m_enemy->isDead())
		{
			m_animator->setSprite(m_enemy->getSprite());
			m_playDeathAnimation = true;

			//coins drop
			dropCoins(m_enemy->getCoinsToDrop(), sf::Vector2f(
				m_enemy->getPosition().x + m_enemy->getGlobalBounds().size.x / 2,
				m_enemy->getPosition().y + m_enemy->getGlobalBounds().size.y / 2));

			enemyDeath();
		}
	}
	else if (m_playDeathAnimation)
	{
		if(m_bossActive)
		{
			bossDeath();

		}
		else
		{
			enemyDeath();
		}
	}
	else
	{
		bossEscape();
	}
}

void Game::bossTakeDamage()
{
	//boss takes damage
	m_boss->takeDamage(m_player->getDamage());
	m_ui->setHpBarColor(m_boss->getHpCurrent(), m_boss->getHpMax());
	m_ui->setHpBarLength(m_boss->getHpCurrent(), m_boss->getHpMax());
}

void Game::enemyTakeDamage()
{
	m_enemy->takeDamage(m_player->getDamage());
	m_ui->setHpBarColor(m_enemy->getHpCurrent(), m_enemy->getHpMax());
	m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax());
}

void Game::bossEscape()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		//reset timer
		m_ui->resetTimer();

		//reset animator
		m_animator->resetAnimator();
		m_playEscapeAnimation = false;

		//reset boss
		m_boss->resetBoss();
		m_bossActive = false;

		//set hp bar for next enemy
		m_ui->setHpBarMaxLength(m_bossActive); 
		m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax()); 
		m_ui->setHpBarColor(m_enemy->getHpCurrent(), m_enemy->getHpMax()); 
		m_ui->setHpBarPosition(static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y);
	}
}

void Game::enemyDeath()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		//update kill count
		m_killCounter++;

		//reset the animator
		m_playDeathAnimation = false;
		m_animator->resetAnimator();

		//set up new random enemy
		m_enemy->resetEnemy();
		m_enemy->setPosition(
			m_window.getSize().x / 2 - m_enemy->getGlobalBounds().size.x / 2,
			m_window.getSize().y / 2 - m_enemy->getGlobalBounds().size.y / 2);
		
		//reset health bars for new enemy
		m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax()); 
		m_ui->setHpBarColor(m_enemy->getHpCurrent(), m_enemy->getHpMax());

		//check if enough enemies are killed for boss to spawn
		if (m_killCounter >= 10)
		{
			setUpBoss();
			m_killCounter = 0;
		}
	}		
}

void Game::dropCoins(std::pair<int, int> coinsToDrop, sf::Vector2f coinStart)
{
	m_coinsToDrop = m_rng.generateRandomNum(coinsToDrop.first, coinsToDrop.second);
	m_playCoinAnimation = true;
	for (size_t i = 0; i < m_coinsToDrop; i++)
	{
		Coin tempCoin(m_rng, m_window.getSize(), m_textures.get("textures/coin.png"), coinStart);
		m_coinsVector.push_back(Coin(tempCoin));
	}	
}

void Game::setUpBoss()
{
	//active boss
	m_bossActive = true;	

	//set hp bars for boss
	m_ui->setHpBarMaxLength(m_bossActive);
	m_ui->setHpBarPosition(static_cast<float>(m_window.getSize().x), m_boss->getGlobalBounds().position.y);
	m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax()); 
}

void Game::bossDeath()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		//reset timer
		m_ui->resetTimer();

		//reset animator
		m_animator->resetAnimator();
		m_playDeathAnimation = false;

		//reset boss for next boss fight
		m_bossActive = false;
		m_boss->resetBoss();		
		m_boss->setPosition(
			m_window.getSize().x / 2 - m_boss->getGlobalBounds().size.x / 2,
			m_window.getSize().y / 2 - m_boss->getGlobalBounds().size.y / 2);

		//set the hp bar for next enemy
		m_ui->setHpBarMaxLength(m_bossActive);
		m_ui->setHpBarLength(m_boss->getHpCurrent(), m_boss->getHpMax());
		m_ui->setHpBarColor(m_boss->getHpCurrent(), m_boss->getHpMax());
		m_ui->setHpBarPosition(static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y);
	}
}

void Game::updateUI(float dt)
{
	m_ui->update(m_bossActive, static_cast<float>(m_window.getSize().x), m_boss->getGlobalBounds().position.y, dt);
}

void Game::upgradeDamage()
{
	if (m_player->canUpgrade())
	{
		m_player->upgradeDamage();
		m_ui->setDamageText(m_player->getDamage());
		m_ui->setCoinsText(m_player->getCoins());
		m_ui->setUpgradeCostText(m_player->getUpgradeCost());
	}
}

void Game::retreat()
{
	m_playEscapeAnimation = true;
	bossEscape();
}

void Game::updateAnimator(float dt)
{
	/*triggered when the enemy reaches 0 hp, play a death animation and make sure the animation sprite stays
	in its correct position*/
	if (m_playDeathAnimation)
	{
		m_animator->update("death", dt);
		m_animator->setDeathSpritePosition(m_window.getSize().x / 2 - m_animator->getDeathSpriteGlobalBounds().size.x / 2,
			m_window.getSize().y / 2 - m_animator->getDeathSpriteGlobalBounds().size.y / 2);
	}
	if (m_playEscapeAnimation)
	{
		m_animator->update("escape", dt);
	}
}

void Game::updateCoins(float dt)
{
	for (Coin& coin : m_coinsVector)
	{
		coin.update(dt);
	}
}

//private rendering functions
void Game::renderEnemy()
{
	if(!m_playDeathAnimation && !m_bossActive)
	{
		m_enemy->render(m_window);
	}
}

void Game::renderBoss()
{
	if (m_bossActive && !m_playEscapeAnimation && !m_playDeathAnimation)
	{
		m_boss->render(m_window);
	}
}

void Game::renderAnimator()
{
	m_animator->render(m_window, m_playDeathAnimation, m_playEscapeAnimation,
		m_playCoinAnimation);
}

void Game::renderBackground()
{
	m_window.draw(m_background);
}

void Game::renderCoins()
{
	for (Coin& coin : m_coinsVector)
	{
		coin.render(m_window);
	}
}

void Game::renderUI()
{
	m_mousePosition = sf::Vector2f(static_cast<float>(sf::Mouse::getPosition(m_window).x),
		static_cast<float>(sf::Mouse::getPosition(m_window).y));
	m_ui->render(m_window, m_mousePosition, m_bossActive, m_playEscapeAnimation, 
		m_playDeathAnimation);
}

//constructor
Game::Game(RNG& rng) : m_rng(rng), m_background(m_textures.get("textures/backgrounds/beach.png"))
{
	initWindow();
	initEnemy();
	initBoss();
	initPlayer();
	initAnimator();
	initUI();

	m_frameClock.restart();
}

//deconstructor
Game::~Game()
{
	delete m_player;
	delete m_enemy;
	delete m_boss;
	delete m_ui;
}

//getters
const sf::RenderWindow& Game::getWindow() const
{
	return m_window;
}

//public functions
void Game::update()
{
	const float dt = m_frameClock.restart().asSeconds();

	polledEvents();
	updateCombat();
	updateAnimator(dt);
	updateCoins(dt);
	updateUI(dt);
}

void Game::render()
{
	m_window.clear();

	//draw game
	renderBackground();
	renderEnemy();
	renderBoss();
	renderAnimator();
	renderCoins();

	//draw ui
	renderUI();

	m_window.display();
}
