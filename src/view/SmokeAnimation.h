#pragma once

#include "view/TextureCache.h"

//plays a puff of smoke from a sprite sheet. holds no game state
class SmokeAnimation
{
private:
	//private variables
	sf::Sprite m_sprite;
	sf::IntRect m_currentFrame;
	float m_timeElapsed;
	float m_frameDuration;
	int m_frameSize;
	int m_lastFrameX;
	bool m_finished;

	//private initiation functions
	void initVariables();
	void initSprite();

public:
	//constructor
	SmokeAnimation(TextureCache& textures);

	//getters
	bool isFinished() const;

	//public functions
	void start(sf::Vector2f centre);
	void update(float dt);
	void draw(sf::RenderTarget& target) const;
};
