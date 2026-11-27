#pragma once

//shrinks a copy of the enemy sprite to nothing. holds no game state
class DeathAnimation
{
private:
	//private variables
	std::optional<sf::Sprite> m_sprite;
	float m_shrinkSpeed;
	bool m_finished;

	//private initiation functions
	void initVariables();

public:
	//constructor
	DeathAnimation();

	//getters
	bool isFinished() const;

	//public functions
	void start(const sf::Sprite& enemySprite);
	void update(float dt, sf::Vector2f centre);
	void draw(sf::RenderTarget& target) const;
};
