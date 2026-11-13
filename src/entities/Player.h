#pragma once
class Player
{
private:
	//private variables
	float m_damage;
	float m_attackCDMax;
	float m_attackCD;
	int m_coins;
	int m_upgradeCost;

	//private functions
	void initVariables();

public:
	//constructor
	Player();
	//deconstructor
	~Player();

	//getters
	const float getDamage() const;
	const int getCoins() const;
	const int getUpgradeCost() const;

	//setters
	void upgradeDamage();
	void addCoins(int coinsToAdd);

	//public functions
	bool canUpgrade();
};

