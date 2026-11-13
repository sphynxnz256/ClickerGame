#pragma once

#include "state/GameState.h"
#include "content/EnemyType.h"

namespace rules
{
	void spawnEnemy(GameState& state, const EnemyType& type);
	void finishDeath(GameState& state);
	void finishEscape(GameState& state);
}
