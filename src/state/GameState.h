#pragma once

#include "state/PlayerState.h"
#include "state/EnemyState.h"

//what the game is currently doing
enum class Phase { Fighting, Dying, Escaping };

//single source of truth for the game's state
struct GameState
{
	PlayerState player;
	EnemyState enemy;
	Phase phase = Phase::Fighting;
	int killCount = 0;
	double bossTimeLeft = 0.0;
};
