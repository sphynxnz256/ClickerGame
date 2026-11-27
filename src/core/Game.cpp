#include "stdafx.h"
#include "core/Game.h"

//the game class runs the game

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
	if (m_hud.upgradeButtonContains(clickPos))
	{
		rules::tryBuyDamageUpgrade(m_state.player);
		return;
	}

	//retreat button is only visible (and clickable) during a boss fight with no animation playing
	if (isBossFight() && m_state.phase == Phase::Fighting && m_hud.retreatButtonContains(clickPos))
	{
		rules::retreat(m_state);
		onBossEscaped();
		finishEscapeIfDone();
		return;
	}

	//attack the enemy
	if (m_state.phase == Phase::Fighting && m_enemyView.getGlobalBounds().contains(clickPos))
	{
		const bool killed = rules::hitEnemy(m_state);
		if (killed)
		{
			onEnemyKilled();
		}
	}
}

//private update functions
void Game::updateCombat(float dt)
{
	if (m_state.phase == Phase::Fighting)
	{
		//boss escapes when its time runs out
		if (rules::updateBossTimer(m_state, dt))
		{
			onBossEscaped();
			finishEscapeIfDone();
		}
	}
	else if (m_state.phase == Phase::Dying)
	{
		finishDeathIfDone();
	}
	else
	{
		finishEscapeIfDone();
	}
}

bool Game::isBossFight() const
{
	return m_state.enemy.type->isBoss;
}

void Game::dropCoins(int coinCount, sf::Vector2f coinStart)
{
	for (int i = 0; i < coinCount; i++)
	{
		//built in place inside the vector, so there is only one Coin
		m_coinsVector.emplace_back(m_rng, m_window.getSize(), m_textures.get("textures/coin.png"), coinStart);
	}
}

void Game::onEnemySpawned()
{
	m_enemyView.show(*m_state.enemy.type);
	centreEnemy();
}

void Game::centreEnemy()
{
	m_enemyView.setPosition(
		m_window.getSize().x / 2 - m_enemyView.getGlobalBounds().size.x / 2,
		m_window.getSize().y / 2 - m_enemyView.getGlobalBounds().size.y / 2);
}

void Game::onEnemyKilled()
{
	m_deathAnimation.start(m_enemyView.getSprite());

	//coins drop
	dropCoins(rules::rollCoinDrop(*m_state.enemy.type, m_rng), sf::Vector2f(
		m_enemyView.getPosition().x + m_enemyView.getGlobalBounds().size.x / 2,
		m_enemyView.getPosition().y + m_enemyView.getGlobalBounds().size.y / 2));
}

void Game::onBossEscaped()
{
	m_smokeAnimation.start(sf::Vector2f(m_window.getSize()) / 2.f);
}

void Game::finishDeathIfDone()
{
	if (m_deathAnimation.isFinished())
	{
		rules::finishDeath(m_state);
		onEnemySpawned();
	}
}

void Game::finishEscapeIfDone()
{
	if (m_smokeAnimation.isFinished())
	{
		rules::finishEscape(m_state);
		onEnemySpawned();
	}
}

void Game::updateAnimator(float dt)
{
	//death animation stays centred in the window
	if (m_state.phase == Phase::Dying)
	{
		m_deathAnimation.update(dt, sf::Vector2f(m_window.getSize()) / 2.f);
	}
	if (m_state.phase == Phase::Escaping)
	{
		m_smokeAnimation.update(dt);
	}
}

void Game::updateCoins(float dt)
{
	for (Coin& coin : m_coinsVector)
	{
		coin.update(dt);
	}

	//coins are collected by hovering, only while the window has focus
	const bool hasFocus = m_window.hasFocus();
	const auto newEnd = std::remove_if(m_coinsVector.begin(), m_coinsVector.end(),
		[&](const Coin& coin)
		{
			if (hasFocus && coin.isCollectable() && coin.isMouseOver(m_mousePosition))
			{
				rules::collectCoin(m_state.player, coin.getCoinValue());
				return true;
			}
			return false;
		});
	m_coinsVector.erase(newEnd, m_coinsVector.end());
}

//private rendering functions
void Game::renderEnemy()
{
	if (m_state.phase == Phase::Fighting)
	{
		m_enemyView.render(m_window);
	}
}

void Game::renderAnimator()
{
	if (m_state.phase == Phase::Dying)
	{
		m_deathAnimation.draw(m_window);
	}
	if (m_state.phase == Phase::Escaping)
	{
		m_smokeAnimation.draw(m_window);
	}
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
	const float windowWidth = static_cast<float>(m_window.getSize().x);
	const float enemyTop = m_enemyView.getGlobalBounds().position.y;

	if (m_state.phase == Phase::Fighting)
	{
		m_hpBar.render(m_window, m_state.enemy, windowWidth, enemyTop);
	}

	m_hud.render(m_window, m_state, m_mousePosition);

	if (isBossFight() && m_state.phase == Phase::Fighting)
	{
		m_bossCountdown.render(m_window, m_state.bossTimeLeft, windowWidth, enemyTop);
	}
}

//constructor
Game::Game(RNG& rng) :
	m_rng(rng),
	m_window(sf::VideoMode({800, 600}), "Clicker Game", sf::Style::Close | sf::Style::Titlebar),
	m_state(rules::startingState()),
	m_font("fonts/arial.ttf"),
	m_enemyView(*m_state.enemy.type, m_rng, m_textures),
	m_hud(m_font, m_state.player),
	m_bossCountdown(m_font),
	m_smokeAnimation(m_textures),
	m_background(m_textures.get("textures/backgrounds/beach.png"))
{
	centreEnemy();

	m_frameClock.restart();
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

	//mouse position is polled once per frame
	m_mousePosition = sf::Vector2f(sf::Mouse::getPosition(m_window));

	polledEvents();
	updateCombat(dt);
	updateAnimator(dt);
	updateCoins(dt);
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
