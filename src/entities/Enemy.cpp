#include "stdafx.h"
#include "entities/Enemy.h"

namespace
{
	const std::string texturePaths[] = {
		"textures/enemies/slime.png",
		"textures/enemies/crab.png",
		"textures/enemies/rat.png",
		"textures/enemies/raven.png",
		"textures/enemies/wasp.png"
	};
}

/*the enemy holds the enemies state*/

//private functions	
const sf::Texture& Enemy::randomTexture()
{
	return m_textures.get(texturePaths[m_rng.generateRandomNum(1, 5) - 1]);
}

void Enemy::initVariables()
{
	m_hpMax = 10.f;
	m_hpCurrent = m_hpMax;
	m_coinsToDrop = std::make_pair(1, 2);
	m_spriteScale = std::make_pair(0.25f, 0.25f);
}

void Enemy::initSprite()
{
	m_sprite.setScale({m_spriteScale.first, m_spriteScale.second});
}

//constructor
Enemy::Enemy(RNG& rng, TextureCache& textures)
	: m_rng(rng), m_textures(textures), m_sprite(randomTexture())
{	
	initVariables();
	initSprite();
}

//deconstructor
Enemy::~Enemy()
{
}

//getters
const sf::Vector2f Enemy::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::FloatRect Enemy::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

const float Enemy::getHpMax() const
{
	return m_hpMax;
}

const float Enemy::getHpCurrent() const
{
	return m_hpCurrent;
}

sf::Sprite* Enemy::getSprite()
{
	return &m_sprite;
}

const std::pair<int, int> Enemy::getCoinsToDrop() const
{
	return m_coinsToDrop;
}

//setters
void Enemy::setPosition(const float x, const float y)
{
	m_sprite.setPosition({x, y});
}

void Enemy::takeDamage(const float damageTaken)
{
	m_hpCurrent -= damageTaken;
	if (m_hpCurrent < 0)
	{
		m_hpCurrent = 0;
	}
}

void Enemy::resetEnemy()
{
	m_hpCurrent = m_hpMax;
	m_sprite.setTexture(randomTexture());
	m_sprite.setScale({m_spriteScale.first, m_spriteScale.second});
}

bool Enemy::isDead()
{
	if (getHpCurrent() <= 0.f)
	{
		return true;
	}
	return false;
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
