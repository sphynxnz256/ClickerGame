#include "stdafx.h"
#include "rules/BossTimer.h"

namespace rules
{
	bool updateBossTimer(GameState& state, double dt)
	{
		if (state.enemy.type->isBoss && state.phase == Phase::Fighting)
		{
			state.bossTimeLeft -= dt;
			if (state.bossTimeLeft < 0.0)
			{
				state.bossTimeLeft = 0.0;
			}
			if (state.bossTimeLeft <= 0.0)
			{
				state.phase = Phase::Escaping;
				return true;
			}
		}
		return false;
	}

	void retreat(GameState& state)
	{
		state.phase = Phase::Escaping;
	}
}
