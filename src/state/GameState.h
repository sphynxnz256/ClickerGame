#pragma once

#include "state/PlayerState.h"

//what the game is currently doing
enum class Phase { Fighting, Dying, Escaping };

//single source of truth for the game's state
struct GameState
{
	PlayerState player;
	Phase phase = Phase::Fighting;
	int killCount = 0;
};
