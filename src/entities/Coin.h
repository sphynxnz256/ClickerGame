#pragma once

#include "core/RNG.h"

class Coin
{
private:
	//private variables
	std::shared_ptr<sf::Texture> m_texture;
	sf::Sprite m_sprite;
	RNG& m_rng;
	int m_coinValue;

	sf::Clock m_dtClock;
	float m_timeElapsed;
	float m_frameDuration;
	float m_deltaTime;

	sf::Vector2f m_startPoint;
	sf::Vector2f m_endPoint;
	sf::Vector2f m_controlPoint;
	float m_animationSpeed;
	float m_distanceToTravel;
	bool m_animationEnded;
	bool m_animationFirstFrame;

	//private functions
	void initVariables(const sf::Vector2u& targetSize);
	void initSprite(std::shared_ptr<sf::Texture> sharedTexture);
	void initControlPoint();
	void initAnimationSpeed();
	void initDistanceToTravel();
	void initAnimation();
	void updateLocation();

public:
	//constructor
	Coin(RNG& rng, const sf::Vector2u& targetSize, std::shared_ptr<sf::Texture> sharedTexture,
		sf::Vector2f startPoint);

	//getters
	int getCoinValue();
	sf::Vector2f getPosition();

	//public functions
	bool isMouseOver(const sf::Vector2f& mousePos);
	bool checkPosition();

	void update();
	void render(sf::RenderTarget& target);
};

