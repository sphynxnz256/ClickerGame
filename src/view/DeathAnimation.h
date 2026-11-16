#pragma once

//shrinks a copy of the enemy sprite to nothing. holds no game state
class DeathAnimation
{
private:
	//private variables
	static constexpr float m_shrinkSpeed = 10.f;

	std::optional<sf::Sprite> m_sprite;
	bool m_finished = false;

public:
	//getters
	bool isFinished() const;

	//public functions
	void start(const sf::Sprite& enemySprite);
	void update(float dt, sf::Vector2f centre);
	void draw(sf::RenderTarget& target) const;
};
