#include "stdafx.h"
#include "view/EnemyView.h"

/*the enemy view draws the current enemy*/

//private functions
const sf::Texture& EnemyView::randomTexture(const EnemyType& type)
{
	return m_textures.get(type.texturePaths[m_rng.generateRandomNum(0, static_cast<int>(type.texturePaths.size()) - 1)]);
}

//constructor
EnemyView::EnemyView(const EnemyType& type, RNG& rng, TextureCache& textures)
	: m_rng(rng), m_textures(textures), m_sprite(randomTexture(type))
{
	m_sprite.setScale({type.scale, type.scale});
}

//getters
const sf::Vector2f EnemyView::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::FloatRect EnemyView::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

const sf::Sprite& EnemyView::getSprite() const
{
	return m_sprite;
}

//setters
void EnemyView::setPosition(const float x, const float y)
{
	m_sprite.setPosition({x, y});
}

//public functions
void EnemyView::show(const EnemyType& type)
{
	m_sprite.setTexture(randomTexture(type), true);
	m_sprite.setScale({type.scale, type.scale});
}

void EnemyView::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
