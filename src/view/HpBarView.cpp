#include "stdafx.h"
#include "view/HpBarView.h"
#include "content/EnemyType.h"

/*the hp bar shows how much health the current enemy has left*/

//constructor
HpBarView::HpBarView()
{
	m_maxBar.setSize(sf::Vector2f(130.f, 20.f));
	m_maxBar.setFillColor(sf::Color(200, 200, 200, 100));
	m_maxBar.setOutlineColor(sf::Color::Black);
	m_maxBar.setOutlineThickness(1.f);

	m_currentBar.setSize(m_maxBar.getSize());
	m_currentBar.setFillColor(sf::Color::Green);
}

//public functions
void HpBarView::render(sf::RenderTarget& target, const EnemyState& enemy, float targetX, float enemyTop)
{
	const double currentHp = enemy.hp;
	const double maxHp = enemy.type->hpMax;

	//bosses get a wider bar
	m_maxBar.setSize(sf::Vector2f(enemy.type->isBoss ? 260.f : 130.f, m_maxBar.getSize().y));

	//position above the enemy
	float xPosition = targetX / 2 - m_maxBar.getGlobalBounds().size.x / 2;
	float yPosition = enemyTop - 20.f;
	m_maxBar.setPosition({xPosition, yPosition});
	m_currentBar.setPosition({xPosition, yPosition});

	//length and colour follow the remaining hp
	m_currentBar.setSize(sf::Vector2f(static_cast<float>(m_maxBar.getSize().x * currentHp / maxHp), m_maxBar.getSize().y));
	if (currentHp > 0.5f * maxHp)
	{
		m_currentBar.setFillColor(sf::Color(static_cast <int>(255 - (255 * currentHp / maxHp)) * 2, 255, 0, 255));
	}
	else
	{
		m_currentBar.setFillColor(sf::Color(255, static_cast <int>(255 * (currentHp * 2) / maxHp), 0, 255));
	}

	target.draw(m_maxBar);
	target.draw(m_currentBar);
}
