#include "stdafx.h"
#include "entities/Boss.h"

//private functions
void Boss::initVariables()
{
	m_hpMax = 250;
	m_hpCurrent = m_hpMax;
	m_coinsToDrop = std::make_pair(10, 20);
	m_spriteScale = std::make_pair(0.5f, 0.5f);
}

void Boss::initSprite()
{
	m_sprite.setScale({m_spriteScale.first, m_spriteScale.second});
}
//constructor
Boss::Boss(TextureCache& textures) : m_sprite(textures.get("textures/enemies/jellyfish.png"))
{
	initVariables();
	initSprite();
}

//deconstructor
Boss::~Boss()
{
}

//getters
const sf::Vector2f Boss::getPosition() const
{
	return m_sprite.getPosition(); 
}

const sf::FloatRect Boss::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds(); 
}

const float Boss::getHpMax() const
{
	return m_hpMax; 
}

const float Boss::getHpCurrent() const
{
	return m_hpCurrent;
}

sf::Sprite* Boss::getSprite()
{
	return &m_sprite;
}

const std::pair<int, int> Boss::getCoinsToDrop() const
{
	return m_coinsToDrop;
}

//setters
void Boss::setPosition(const float x, const float y)
{
	m_sprite.setPosition({x, y});
}

void Boss::takeDamage(const float damageTaken)
{
	m_hpCurrent -= damageTaken; 
	if (m_hpCurrent < 0)
	{
		m_hpCurrent = 0;
	}
}

//public functions
void Boss::resetBoss()
{
	m_hpCurrent = m_hpMax;
	m_sprite.setScale({m_spriteScale.first, m_spriteScale.second});
}

void Boss::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}


