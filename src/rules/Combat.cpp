#include "stdafx.h"
#include "rules/Combat.h"

namespace rules
{
	double clickDamage(const PlayerState& player)
	{
		return player.damage;
	}

	bool hitEnemy(GameState& state)
	{
		state.enemy.hp -= clickDamage(state.player);
		if (state.enemy.hp < 0.0)
		{
			state.enemy.hp = 0.0;
		}
		if (state.enemy.hp <= 0.0)
		{
			state.phase = Phase::Dying;
			return true;
		}
		return false;
	}

	int rollCoinDrop(const EnemyType& type, RNG& rng)
	{
		return rng.generateRandomNum(type.coinsMin, type.coinsMax);
	}

	void collectCoin(PlayerState& player, double value)
	{
		player.coins += value;
	}
}
