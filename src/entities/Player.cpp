#include "stdafx.h"
#include "entities/Player.h"

/*the player class holds the players state*/

//private functions
void Player::initVariables()
{
	m_damage = 1.f;
	m_attackCDMax = 10.f;
	m_attackCD = m_attackCDMax;
	m_coins = 0;
	m_upgradeCost = 10;
}

//constructor
Player::Player()
{
	initVariables();
}

//deconstructor
Player::~Player()
{
}

//getters
const float Player::getDamage() const
{
	return m_damage;
}

const int Player::getCoins() const
{
	return m_coins;
}

const int Player::getUpgradeCost() const
{
	return m_upgradeCost;
}

//public functions
void Player::upgradeDamage()
{
	m_damage *= 2.f;
	m_coins -= m_upgradeCost;
	m_upgradeCost *= 2;
}

void Player::addCoins(int coinsToAdd)
{
	m_coins += coinsToAdd;
}

bool Player::canUpgrade()
{
	if (m_coins >= m_upgradeCost)
	{
		return true;
	}
	return false;
}
