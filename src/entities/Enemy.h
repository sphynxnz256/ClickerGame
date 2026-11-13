#pragma once

#include "core/RNG.h"
#include "view/TextureCache.h"

class Enemy
{
private:
	//private variables
	RNG& m_rng;
	TextureCache& m_textures;
	sf::Sprite m_sprite;

	float m_hpMax;
	float m_hpCurrent;
	std::pair<float, float> m_spriteScale;

	std::pair<int, int> m_coinsToDrop;

	//private functions
	void initVariables();
	const sf::Texture& randomTexture();
	void initSprite();

public:
	//constructor
	Enemy(RNG& rng, TextureCache& textures);
	//deconstructor
	~Enemy();

	//getters
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const float getHpMax() const;
	const float getHpCurrent() const;
	sf::Sprite* getSprite();
	const std::pair<int, int> getCoinsToDrop() const;

	//setters
	void setPosition(const float x, const float y);
	void takeDamage(const float damageTaken);
	
	//public functions
	void resetEnemy();
	bool isDead();

	void render(sf::RenderTarget& target);
};

