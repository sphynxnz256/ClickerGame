#pragma once

#include "state/EnemyState.h"

class HpBarView
{
private:
	sf::RectangleShape m_maxBar;
	sf::RectangleShape m_currentBar;

public:
	HpBarView();

	void render(sf::RenderTarget& target, const EnemyState& enemy, float targetX, float enemyTop);
};
