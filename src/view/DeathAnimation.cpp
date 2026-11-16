#include "stdafx.h"
#include "view/DeathAnimation.h"

/*the death animation shrinks the killed enemy to nothing*/

//getters
bool DeathAnimation::isFinished() const
{
	return m_finished;
}

//public functions
void DeathAnimation::start(const sf::Sprite& enemySprite)
{
	m_sprite = enemySprite;
	m_finished = false;
}

void DeathAnimation::update(float dt, sf::Vector2f centre)
{
	if (!m_sprite)
	{
		return;
	}

	if (m_sprite->getScale().x > 0.001)
	{
		//shrinks the enemy to nothing
		const sf::Vector2f currentScale = m_sprite->getScale();
		m_sprite->setScale(currentScale - currentScale * dt * m_shrinkSpeed);
	}
	else
	{
		m_sprite->setScale({0.f, 0.f});
		m_finished = true;
	}

	//keep the sprite centred while it shrinks
	m_sprite->setPosition(centre - m_sprite->getGlobalBounds().size / 2.f);
}

void DeathAnimation::draw(sf::RenderTarget& target) const
{
	if (m_sprite)
	{
		target.draw(*m_sprite);
	}
}
