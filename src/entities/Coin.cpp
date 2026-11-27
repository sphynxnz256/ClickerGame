#include "stdafx.h"
#include "entities/Coin.h"

//private functions
void Coin::initVariables()
{
	m_coinValue = 1.0;
	m_pickupDelay = 0.3f;

	//these ranges were checked so coins never leave the screen, assuming an
	//800x600 window, the enemy centred and a 56 px coin; recheck if any change
	m_gravity = 2200.f;
	m_minLaunchSpeed = 450;
	m_maxLaunchSpeed = 650;
	m_minLaunchAngle = 45;
	m_maxLaunchAngle = 135;
}

void Coin::initSprite()
{
	m_sprite.setScale({0.1f, 0.1f});
	m_sprite.setPosition(sf::Vector2f(m_startPoint.x - m_sprite.getGlobalBounds().size.x / 2, 
		m_startPoint.y - m_sprite.getGlobalBounds().size.y / 2));
}

void Coin::initLaunch(RNG& rng, const sf::Vector2u& targetSize)
{
	m_launchPosition = m_sprite.getPosition();

	//random launch speed and angle
	int speed = rng.generateRandomNum(m_minLaunchSpeed, m_maxLaunchSpeed);
	int angleDegrees = rng.generateRandomNum(m_minLaunchAngle, m_maxLaunchAngle);
	float angle = sf::degrees(static_cast<float>(angleDegrees)).asRadians();

	//negative y is up
	m_velocity = sf::Vector2f(static_cast<float>(speed) * std::cos(angle),
		-static_cast<float>(speed) * std::sin(angle));

	//time for y(t) = launchY + vy * t + 0.5 * g * t^2 to reach the floor
	float floorY = static_cast<float>(targetSize.y) - 150.f;
	float drop = floorY - m_launchPosition.y;
	m_flightTime = (-m_velocity.y + std::sqrt(m_velocity.y * m_velocity.y +
		2.f * m_gravity * drop)) / m_gravity;
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
		//plays animation of coin flying out of killed enemy
		m_timeElapsed += dt;

		//stop on the floor
		if (m_timeElapsed >= m_flightTime)
		{
			m_timeElapsed = m_flightTime;
			m_animationEnded = true;
		}

		//projectile motion
		float t = m_timeElapsed;
		m_sprite.setPosition(sf::Vector2f(m_launchPosition.x + m_velocity.x * t,
			m_launchPosition.y + m_velocity.y * t + 0.5f * m_gravity * t * t));
	}
}

//constructor
Coin::Coin(RNG& rng, const sf::Vector2u& targetSize, const sf::Texture& texture,
	sf::Vector2f startPoint)
	: m_sprite(texture), m_startPoint(startPoint)
{
	initSprite();
	initVariables();
	initLaunch(rng, targetSize);
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
