#pragma once

#include "state/GameState.h"
#include "content/EnemyType.h"
#include "core/RNG.h"

namespace rules
{
	//damage dealt by one click. upgrades will plug in here later
	double clickDamage(const PlayerState& player);
	//returns true if the hit killed the enemy
	bool hitEnemy(GameState& state);
	int rollCoinDrop(const EnemyType& type, RNG& rng);
	void collectCoin(PlayerState& player, double value);
}
