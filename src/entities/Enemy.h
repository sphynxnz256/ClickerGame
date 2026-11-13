#pragma once

#include "content/EnemyType.h"
#include "core/RNG.h"
#include "view/TextureCache.h"

class Enemy
{
private:
	//private variables
	const EnemyType* m_type;
	RNG& m_rng;
	TextureCache& m_textures;
	sf::Sprite m_sprite;
	float m_hpCurrent;

	//private functions
	void initVariables();
	const sf::Texture& randomTexture();
	void initSprite();

public:
	//constructor
	Enemy(const EnemyType& type, RNG& rng, TextureCache& textures);
	//deconstructor
	~Enemy();

	//getters
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const float getHpMax() const;
	const float getHpCurrent() const;
	sf::Sprite* getSprite();
	const std::pair<int, int> getCoinsToDrop() const;
	const EnemyType& getType() const;

	//setters
	void setPosition(const float x, const float y);
	void takeDamage(const float damageTaken);
	
	//public functions
	void spawn(const EnemyType& type);
	bool isDead();

	void render(sf::RenderTarget& target);
};

