#include "stdafx.h"
#include "graphics/Animator.h"

/*the animator class plays animations*/

//private initation functions
void Animator::initVariables()
{
	m_animationEnd = false;
	m_timeElapsed = 0.f;
	m_deathAnimationSpeed = 10.f;
	m_frameDuration = 0.05f;
}

void Animator::initSmokeSprite()
{
	m_currentFrame = sf::IntRect({0, 0}, {150, 150});
	m_smokeSprite.setTextureRect(sf::IntRect({0, 0}, {150, 150}));
	m_smokeSprite.setScale({2.f, 2.f});
}

void Animator::updateDeathAnimation(float dt)
{	
	if(m_deathSprite->getScale().x > 0.001)
	{
		//death animation. shrinks the enemy to nothing
		m_currentScale = m_deathSprite->getScale();
		m_newScale = m_currentScale - m_currentScale 
			* dt * m_deathAnimationSpeed;
		m_deathSprite->setScale(m_newScale);
	}
	else
	{
		m_deathSprite->setScale({0.f, 0.f});
		m_animationEnd = true;
	}
}

void Animator::updateEscapeAnimation(float dt)
{
	//plays a puff of smoke when enemy escapes
	m_timeElapsed += dt;

	if(m_timeElapsed >= m_frameDuration)
	{
		m_timeElapsed = 0.f;
		if (m_currentFrame.position.x < 1050)
		{
			m_currentFrame.position.x += 150;
			m_smokeSprite.setTextureRect(m_currentFrame);
		}
		else
		{
			m_animationEnd = true;
			m_timeElapsed = 0.f;
			m_currentFrame = sf::IntRect({0, 0}, {150, 150});

		}
	}
}

void Animator::renderDeathAnimation(sf::RenderTarget& target, bool playDeath)
{
	if (playDeath)
	{
		target.draw(*m_deathSprite);
	}
}

void Animator::renderEscapeAnimation(sf::RenderTarget& target, bool playEscape)
{
	if (playEscape)
	{
		target.draw(m_smokeSprite);
	}
}

//constructor
Animator::Animator(TextureCache& textures) : m_smokeSprite(textures.get("textures/smoke_sheet.png"))
{
	initVariables();
	initSmokeSprite();
}

//deconstructor
Animator::~Animator()
{
}

//getters
const sf::FloatRect& Animator::getDeathSpriteGlobalBounds() const
{
		return m_deathSprite->getGlobalBounds();
}

const sf::FloatRect& Animator::getSmokeSpriteGlobalBounds() const
{
	return m_smokeSprite.getGlobalBounds();
}

const bool Animator::getAnimationEnd() const
{
	return m_animationEnd;
}

//setters
void Animator::setDeathSpritePosition(const float x, const float y)
{
	m_deathSprite->setPosition({x, y});
}

void Animator::setSmokeSpritePosition(const float x, const float y)
{
	m_smokeSprite.setPosition({x, y});
}

void Animator::setSprite(sf::Sprite* sprite)
{
	m_deathSprite = sprite;
}

//public functions
void Animator::resetAnimator()
{
	m_animationEnd = false;
}

void Animator::update(std::string updateThis, float dt)
{
	if(updateThis == "death")
	{
		updateDeathAnimation(dt);
	}

	if(updateThis == "escape")
	{
		updateEscapeAnimation(dt);
	}
}

void Animator::render(sf::RenderTarget& target, bool playDeath, bool playEscape,
	bool playCoin)
{
	renderDeathAnimation(target, playDeath);
	renderEscapeAnimation(target, playEscape);
	//renderCoinAnimation(target, playCoin);
}
