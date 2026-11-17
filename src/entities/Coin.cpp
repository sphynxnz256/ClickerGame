#include "stdafx.h"
#include "entities/Coin.h"

//private functions
void Coin::initVariables(RNG& rng, const sf::Vector2u& targetSize)
{
	m_coinValue = 1.0;
	m_pickupDelay = 0.3f;
	m_endPoint = sf::Vector2f(static_cast<float>(rng.generateRandomNum(20,
		static_cast<int>(targetSize.x - m_sprite.getGlobalBounds().size.x - 20))),
		static_cast<float>(targetSize.y) - 150.f);
}

void Coin::initSprite()
{
	m_sprite.setScale({0.1f, 0.1f});
	m_sprite.setPosition(sf::Vector2f(m_startPoint.x - m_sprite.getGlobalBounds().size.x / 2, 
		m_startPoint.y - m_sprite.getGlobalBounds().size.y / 2));
}

void Coin::initControlPoint()
{
	//set the control point so its on the same side as the coin ends up on
	if (m_endPoint.x > 400.f)
	{
		m_controlPoint = sf::Vector2f(600.f, 200.f);
	}
	else
	{
		m_controlPoint = sf::Vector2f(200.f, 200.f);
	}
}

void Coin::initAnimationSpeed()
{
	//smooth the speed out relative to the center
	if (m_endPoint.x < 100.f || m_endPoint.x > 700.f)
	{
		m_animationSpeed = 600.f;
	}
	else if (m_endPoint.x < 200.f || m_endPoint.x > 600.f)
	{
		m_animationSpeed = 500.f;
	}
	else if (m_endPoint.x < 300.f || m_endPoint.x > 500.f)
	{
		m_animationSpeed = 400.f;
	}
	else
	{
		m_animationSpeed = 300.f;
	}
}

void Coin::initDistanceToTravel()
{
	//calculate distance
	m_distanceToTravel = std::sqrt((m_endPoint.x - m_startPoint.x) *
		(m_endPoint.x - m_startPoint.x) +
		(m_endPoint.y - m_startPoint.y) *
		(m_endPoint.y - m_startPoint.y));
}

void Coin::initAnimation()
{
	m_animationEnded = false;
	m_timeElapsed = 0.f;
	m_age = 0.f;
}

void Coin::updateLocation(float dt)
{
	if (!m_animationEnded)
	{
		//plays animation of coin dropping from killed enemy
		m_timeElapsed += dt;

		//calculate progress based on speed
		float progress = (m_timeElapsed * m_animationSpeed) /
			m_distanceToTravel;


		//check if animation is finished
		if (progress >= 1.f)
		{
			progress = 1.f;
			m_animationEnded = true;
		}

		//calc bezier interpilation
		sf::Vector2f position = (1 - progress) * (1 - progress) * m_startPoint +
			2 * (1 - progress) * progress * m_controlPoint +
			progress * progress * m_endPoint;
		m_sprite.setPosition(position);
	}
}

//constructor
Coin::Coin(RNG& rng, const sf::Vector2u& targetSize, const sf::Texture& texture,
	sf::Vector2f startPoint)
	: m_sprite(texture), m_startPoint(startPoint)
{
	initSprite();
	initVariables(rng, targetSize);
	initControlPoint();
	initAnimationSpeed();
	initDistanceToTravel();
	initAnimation();
	
}

//getters
double Coin::getCoinValue() const
{
	return m_coinValue;
}

sf::Vector2f Coin::getPosition()
{
	return m_sprite.getPosition();
}

//public functions
bool Coin::isMouseOver(const sf::Vector2f& mousePos) const
{
	return m_sprite.getGlobalBounds().contains(mousePos);
}

bool Coin::isCollectable() const
{
	return m_age >= m_pickupDelay;
}

void Coin::update(float dt)
{
	m_age += dt;
	updateLocation(dt);
}

void Coin::render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
