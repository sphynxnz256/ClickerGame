#pragma once

#include "content/EnemyType.h"
#include "core/RNG.h"
#include "view/TextureCache.h"

//draws the current enemy. holds no game state
class EnemyView
{
private:
	//private variables
	RNG& m_rng;
	TextureCache& m_textures;
	sf::Sprite m_sprite;

	//private functions
	const sf::Texture& randomTexture(const EnemyType& type);

public:
	//constructor
	EnemyView(const EnemyType& type, RNG& rng, TextureCache& textures);

	//getters
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::Sprite& getSprite() const;

	//setters
	void setPosition(const float x, const float y);

	//public functions
	void show(const EnemyType& type);
	void render(sf::RenderTarget& target);
};
