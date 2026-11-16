#pragma once

#include "view/TextureCache.h"

//plays a puff of smoke from a sprite sheet. holds no game state
class SmokeAnimation
{
private:
	//private variables
	static constexpr float m_frameDuration = 0.05f;
	static constexpr int m_frameSize = 150;
	static constexpr int m_lastFrameX = 1050;

	sf::Sprite m_sprite;
	sf::IntRect m_currentFrame;
	float m_timeElapsed = 0.f;
	bool m_finished = false;

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
