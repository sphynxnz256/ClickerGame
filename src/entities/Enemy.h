#pragma once

#include "core/RNG.h"

class Enemy
{
private:
	//private variables
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	RNG& m_rng;
	std::unordered_map<int, std::string> m_textureAddressMap;
	std::unordered_map<int, std::shared_ptr<sf::Texture>> m_textureMap;
	int m_textureNumber;

	float m_hpMax;
	float m_hpCurrent;
	std::pair<float, float> m_spriteScale;

	std::pair<int, int> m_coinsToDrop;

	//private functions
	void initVariables();
	void initTextures();
	void initSprite();

public:
	//constructor
	Enemy(RNG& rng);
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

