#pragma once

#include "core/RNG.h"

class Coin
{
private:
	//private variables
	sf::Sprite m_sprite;
	double m_coinValue;

	float m_timeElapsed;
	float m_age; //seconds since spawn
	float m_pickupDelay; //seconds after spawning before the coin can be collected
	float m_frameDuration;

	sf::Vector2f m_startPoint;
	sf::Vector2f m_endPoint;
	sf::Vector2f m_controlPoint;
	float m_animationSpeed;
	float m_distanceToTravel;
	bool m_animationEnded;
	bool m_animationFirstFrame;

	//private functions
	void initVariables(RNG& rng, const sf::Vector2u& targetSize);
	void initSprite();
	void initControlPoint();
	void initAnimationSpeed();
	void initDistanceToTravel();
	void initAnimation();
	void updateLocation(float dt);

public:
	//constructor
	Coin(RNG& rng, const sf::Vector2u& targetSize, const sf::Texture& texture,
		sf::Vector2f startPoint);

	//getters
	double getCoinValue() const;
	sf::Vector2f getPosition();

	//public functions
	bool isMouseOver(const sf::Vector2f& mousePos) const;
	bool isCollectable() const;

	void update(float dt);
	void render(sf::RenderTarget& target);
};

