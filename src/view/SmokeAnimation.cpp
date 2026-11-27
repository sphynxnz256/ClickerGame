#include "stdafx.h"
#include "view/SmokeAnimation.h"

/*the smoke animation plays a puff of smoke when a boss escapes*/

//private initiation functions
void SmokeAnimation::initVariables()
{
	m_timeElapsed = 0.f;
	m_frameDuration = 0.05f;
	m_frameSize = 150;
	m_lastFrameX = 1050;
	m_finished = false;
}

void SmokeAnimation::initSprite()
{
	m_currentFrame = sf::IntRect({0, 0}, {m_frameSize, m_frameSize});
	m_sprite.setTextureRect(m_currentFrame);
	m_sprite.setScale({2.f, 2.f});
}

//constructor
SmokeAnimation::SmokeAnimation(TextureCache& textures)
	: m_sprite(textures.get("textures/smoke_sheet.png"))
{
	initVariables();
	initSprite();
}

//getters
bool SmokeAnimation::isFinished() const
{
	return m_finished;
}

//public functions
void SmokeAnimation::start(sf::Vector2f centre)
{
	m_finished = false;
	m_timeElapsed = 0.f;
	m_currentFrame = sf::IntRect({0, 0}, {m_frameSize, m_frameSize});
	m_sprite.setTextureRect(m_currentFrame);
	m_sprite.setPosition(centre - m_sprite.getGlobalBounds().size / 2.f);
}

void SmokeAnimation::update(float dt)
{
	m_timeElapsed += dt;

	if (m_timeElapsed >= m_frameDuration)
	{
		m_timeElapsed = 0.f;
		if (m_currentFrame.position.x < m_lastFrameX)
		{
			m_currentFrame.position.x += m_frameSize;
			m_sprite.setTextureRect(m_currentFrame);
		}
		else
		{
			m_finished = true;
		}
	}
}

void SmokeAnimation::draw(sf::RenderTarget& target) const
{
	target.draw(m_sprite);
}
