#pragma once

#include "view/TextureCache.h"

class Boss
{
private:
	//private variables
	sf::Sprite m_sprite;

	float m_hpMax;
	float m_hpCurrent;
	std::pair<float, float> m_spriteScale;

	std::pair<int, int> m_coinsToDrop;

	//private functions
	void initVariables();
	void initSprite();

public:
	//constructor
	Boss(TextureCache& textures);
	//deconstructor
	~Boss();

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
	void resetBoss();
	bool isDead()
	{
		if (m_hpCurrent <= 0.f)
		{
			return true;
		}
		return false;
	}

	void render(sf::RenderTarget& target);
};

