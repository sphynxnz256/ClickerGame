#pragma once

#include "view/TextureCache.h"

class Animator
{
private:
	//private variables
	sf::Sprite* m_deathSprite;

	sf::Vector2f m_currentScale;
	sf::Vector2f m_targetScale;
	sf::Vector2f m_newScale;

	float m_timeElapsed;
	float m_frameDuration;

	bool m_animationEnd;

	float m_deathAnimationSpeed;		

	sf::Sprite m_smokeSprite;
	sf::IntRect m_currentFrame;

	//sf::Sprite m_coinSprite;
	//sf::Vector2f m_coinStart;
	//sf::Vector2f m_coinEnd;
	//sf::Vector2f m_coinControlPoint;
	//float m_coinAnimationSpeed;
	//float m_coinDistance;
	//bool m_coinAnimationEnded;
	//bool m_coinFirstFrame;

	//private initation functions
	void initVariables();
	void initSmokeSprite();
	//void initCoinAnimation(sf::Texture& coinTexture); 

	//private update functions
	void updateDeathAnimation(float dt);
	void updateEscapeAnimation(float dt);
	//void updateCoinAnimation();

	//private render functions
	void renderDeathAnimation(sf::RenderTarget& target, bool playDeath);
	void renderEscapeAnimation(sf::RenderTarget& target, bool playEscape);
	//void renderCoinAnimation(sf::RenderTarget& target, bool playCoin);

public:
	//constructor
	Animator(TextureCache& textures);
	//deconstructor
	~Animator();

	//getters
	const sf::FloatRect& getDeathSpriteGlobalBounds() const;
	const sf::FloatRect& getSmokeSpriteGlobalBounds() const;
	const bool getAnimationEnd() const;

	//setters
	void setDeathSpritePosition(const float x, const float y);
	void setSmokeSpritePosition(const float x, const float y);
	void setSprite(sf::Sprite* sprite);

	//public functions
	//void startDeathAnimation();
	//void startEscapeAnimation();
	//void startCoinAnimation(sf::Vector2f coinStart, sf::Vector2f coinEnd);
	void resetAnimator();

	void update(std::string updateThis, float dt); 
	void render(sf::RenderTarget& target, bool playDeath, bool playEscape,
		bool playCoin);
};

