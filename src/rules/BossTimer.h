#pragma once

#include "state/GameState.h"

namespace rules
{
	//returns true if the boss just started escaping
	bool updateBossTimer(GameState& state, double dt);
	void retreat(GameState& state);
}
