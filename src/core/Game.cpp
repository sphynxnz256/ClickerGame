#include "stdafx.h"
#include "core/Game.h"

//the game class runs the game

//private initiation functions
void Game::initWindow()
{
	//creates the game window
	m_window.create(sf::VideoMode({800, 600}), "Clicker Game", sf::Style::Close | sf::Style::Titlebar);
}

void Game::initEnemy()
{
	//stores enemy state
	m_enemy = new Enemy(content::regularEnemy, m_rng, m_textures);
	m_enemy->setPosition(
		m_window.getSize().x /2 - m_enemy->getGlobalBounds().size.x / 2, 
		m_window.getSize().y / 2 - m_enemy->getGlobalBounds().size.y / 2);
}

void Game::initAnimator()
{ 
	m_animator = new Animator(m_textures);
	m_animator->setSmokeSpritePosition(
		m_window.getSize().x / 2 - m_animator->getSmokeSpriteGlobalBounds().size.x / 2,
		m_window.getSize().y / 2 - m_animator->getSmokeSpriteGlobalBounds().size.y / 2);
}

void Game::initUI()
{
	//create an hp bar and set its position above the enemy
	m_ui = new UI(static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y,
		m_state.player.coins, m_state.player.damage, m_state.player.upgradeCost,
		m_enemy->getGlobalBounds().position.y);
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
	if (isBossFight() && m_state.phase == Phase::Fighting && m_ui->retreatButtonContains(clickPos))
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
			m_state.player.coins += coin.getCoinValue();
			m_ui->setCoinsText(m_state.player.coins);
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

	//attack the enemy
	if (m_state.phase == Phase::Fighting && m_enemy->getGlobalBounds().contains(clickPos))
	{
		damageEnemy();
	}
}

//private update functions
void Game::updateCombat()
{
	if (m_state.phase == Phase::Fighting)
	{
		//if fighting boss, check to see if boss escapes
		if (isBossFight() && m_ui->getTimeLeft() <= 0.f)
		{
			m_state.phase = Phase::Escaping;
			bossEscape();
		}

		//if the enemy is dead, play death animation
		if (m_enemy->isDead())
		{
			m_animator->setSprite(m_enemy->getSprite());
			m_state.phase = Phase::Dying;

			//coins drop
			dropCoins(m_enemy->getCoinsToDrop(), sf::Vector2f(
				m_enemy->getPosition().x + m_enemy->getGlobalBounds().size.x / 2,
				m_enemy->getPosition().y + m_enemy->getGlobalBounds().size.y / 2));

			if (isBossFight())
			{
				bossDeath();
			}
			else
			{
				enemyDeath();
			}
		}
	}
	else if (m_state.phase == Phase::Dying)
	{
		if (isBossFight())
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

bool Game::isBossFight() const
{
	return m_enemy->getType().isBoss;
}

void Game::damageEnemy()
{
	m_enemy->takeDamage(m_state.player.damage);
	m_ui->setHpBarColor(m_enemy->getHpCurrent(), m_enemy->getHpMax());
	m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax());
}

void Game::spawnEnemy(const EnemyType& type)
{
	m_enemy->spawn(type);
	m_enemy->setPosition(
		m_window.getSize().x / 2 - m_enemy->getGlobalBounds().size.x / 2,
		m_window.getSize().y / 2 - m_enemy->getGlobalBounds().size.y / 2);

	//set up the hp bar for the new enemy
	m_ui->setHpBarMaxLength(isBossFight());
	m_ui->setHpBarPosition(static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y);
	m_ui->setHpBarLength(m_enemy->getHpCurrent(), m_enemy->getHpMax());
	m_ui->setHpBarColor(m_enemy->getHpCurrent(), m_enemy->getHpMax());
}

void Game::bossEscape()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		m_ui->resetTimer();
		m_animator->resetAnimator();
		m_state.phase = Phase::Fighting;
		spawnEnemy(content::regularEnemy);
	}
}

void Game::enemyDeath()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		m_state.killCount++;
		m_state.phase = Phase::Fighting;
		m_animator->resetAnimator();

		//every tenth kill spawns the boss
		if (m_state.killCount >= 10)
		{
			m_state.killCount = 0;
			spawnEnemy(content::boss);
		}
		else
		{
			spawnEnemy(content::regularEnemy);
		}
	}
}

void Game::dropCoins(std::pair<int, int> coinsToDrop, sf::Vector2f coinStart)
{
	int coinCount = m_rng.generateRandomNum(coinsToDrop.first, coinsToDrop.second);
	for (int i = 0; i < coinCount; i++)
	{
		Coin tempCoin(m_rng, m_window.getSize(), m_textures.get("textures/coin.png"), coinStart);
		m_coinsVector.push_back(Coin(tempCoin));
	}	
}

void Game::bossDeath()
{
	//check if animation is finished
	if (m_animator->getAnimationEnd())
	{
		m_ui->resetTimer();
		m_animator->resetAnimator();
		m_state.phase = Phase::Fighting;
		spawnEnemy(content::regularEnemy);
	}
}

void Game::updateUI(float dt)
{
	m_ui->update(isBossFight(), static_cast<float>(m_window.getSize().x), m_enemy->getGlobalBounds().position.y, dt);
}

void Game::upgradeDamage()
{
	PlayerState& player = m_state.player;
	if (player.coins >= player.upgradeCost)
	{
		player.damage *= 2.f;
		player.coins -= player.upgradeCost;
		player.upgradeCost *= 2;
		m_ui->setDamageText(player.damage);
		m_ui->setCoinsText(player.coins);
		m_ui->setUpgradeCostText(player.upgradeCost);
	}
}

void Game::retreat()
{
	m_state.phase = Phase::Escaping;
	bossEscape();
}

void Game::updateAnimator(float dt)
{
	/*triggered when the enemy reaches 0 hp, play a death animation and make sure the animation sprite stays
	in its correct position*/
	if (m_state.phase == Phase::Dying)
	{
		m_animator->update("death", dt);
		m_animator->setDeathSpritePosition(m_window.getSize().x / 2 - m_animator->getDeathSpriteGlobalBounds().size.x / 2,
			m_window.getSize().y / 2 - m_animator->getDeathSpriteGlobalBounds().size.y / 2);
	}
	if (m_state.phase == Phase::Escaping)
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
	if (m_state.phase == Phase::Fighting)
	{
		m_enemy->render(m_window);
	}
}

void Game::renderAnimator()
{
	m_animator->render(m_window, m_state.phase == Phase::Dying, m_state.phase == Phase::Escaping, false);
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
	m_ui->render(m_window, m_mousePosition, isBossFight(), m_state.phase == Phase::Escaping, m_state.phase == Phase::Dying);
}

//constructor
Game::Game(RNG& rng) : m_rng(rng), m_background(m_textures.get("textures/backgrounds/beach.png"))
{
	initWindow();
	initEnemy();
	initAnimator();
	initUI();

	m_frameClock.restart();
}

//deconstructor
Game::~Game()
{
	delete m_enemy;
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
	renderAnimator();
	renderCoins();

	//draw ui
	renderUI();

	m_window.display();
}
