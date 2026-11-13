#include "stdafx.h"
#include "entities/Enemy.h"

/*the enemy holds the enemies state*/

//private functions	
const sf::Texture& Enemy::randomTexture()
{
	return m_textures.get(m_type->texturePaths[m_rng.generateRandomNum(0, static_cast<int>(m_type->texturePaths.size()) - 1)]);
}

void Enemy::initVariables()
{
	m_hpCurrent = m_type->hpMax;
}

void Enemy::initSprite()
{
	m_sprite.setScale({m_type->scale, m_type->scale});
}

//constructor
Enemy::Enemy(const EnemyType& type, RNG& rng, TextureCache& textures)
	: m_type(&type), m_rng(rng), m_textures(textures), m_sprite(randomTexture())
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

const double Enemy::getHpMax() const
{
	return m_type->hpMax;
}

const double Enemy::getHpCurrent() const
{
	return m_hpCurrent;
}

sf::Sprite* Enemy::getSprite()
{
	return &m_sprite;
}

const std::pair<int, int> Enemy::getCoinsToDrop() const
{
	return std::make_pair(m_type->coinsMin, m_type->coinsMax);
}

const EnemyType& Enemy::getType() const
{
	return *m_type;
}

//setters
void Enemy::setPosition(const float x, const float y)
{
	m_sprite.setPosition({x, y});
}

void Enemy::takeDamage(const double damageTaken)
{
	m_hpCurrent -= damageTaken;
	if (m_hpCurrent < 0.0)
	{
		m_hpCurrent = 0;
	}
}

void Enemy::spawn(const EnemyType& type)
{
	m_type = &type;
	m_hpCurrent = m_type->hpMax;
	m_sprite.setTexture(randomTexture(), true);
	m_sprite.setScale({m_type->scale, m_type->scale});
}

bool Enemy::isDead()
{
	if (getHpCurrent() <= 0.0)
	{
		return true;
	}
	return false;
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
