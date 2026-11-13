#include "stdafx.h"
#include "entities/Enemy.h"

/*the enemy holds the enemies state*/

//private functions	
void Enemy::initVariables()
{
	m_hpMax = 10.f;
	m_hpCurrent = m_hpMax;
	m_coinsToDrop = std::make_pair(1, 2);
	m_spriteScale = std::make_pair(0.25f, 0.25f);
}

void Enemy::initTextures()
{
	//store addresses of textures in map	
	m_textureAddressMap[1] = "textures/enemies/slime.png";
	m_textureAddressMap[2] = "textures/enemies/crab.png";
	m_textureAddressMap[3] = "textures/enemies/rat.png";
	m_textureAddressMap[4] = "textures/enemies/raven.png";
	m_textureAddressMap[5] = "textures/enemies/wasp.png";

	//preload textures and store them in a map
	for (const auto& pair : m_textureAddressMap)
	{
		int uniqueNumber = pair.first;
		const std::string& textureAddress = pair.second;

		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		if (texture->loadFromFile(textureAddress))
		{
			m_textureMap[uniqueNumber] = texture;
		}
		else
		{
			std::cout << "ERROR::ENEMY::INITTEXTURE::Failed to load " << textureAddress << '\n';
		}
	}

	m_texture = *m_textureMap[m_rng.generateRandomNum(1, 5)];
}

void Enemy::initSprite()
{
	m_sprite.setTexture(m_texture, true);
	m_sprite.setScale({m_spriteScale.first, m_spriteScale.second});
}

//constructor
Enemy::Enemy(RNG& rng) : m_sprite(m_texture), m_rng(rng)
{	
	initVariables();
	initTextures();
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
	m_texture = *m_textureMap[m_rng.generateRandomNum(1, 5)];
	m_sprite.setTexture(m_texture);
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
