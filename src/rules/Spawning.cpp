#include "stdafx.h"
#include "rules/Spawning.h"
#include "content/EnemyTypes.h"

namespace
{
	const int killsPerBoss = 10;
}

namespace rules
{
	GameState startingState()
	{
		GameState state;
		spawnEnemy(state, content::regularEnemy);
		return state;
	}

	void spawnEnemy(GameState& state, const EnemyType& type)
	{
		state.enemy.type = &type;
		state.enemy.hp = type.hpMax;
		state.bossTimeLeft = type.timeLimit;
	}

	void finishDeath(GameState& state)
	{
		if (state.enemy.type->isBoss)
		{
			spawnEnemy(state, content::regularEnemy);
		}
		else
		{
			state.killCount++;

			//every tenth kill spawns the boss
			if (state.killCount >= killsPerBoss)
			{
				state.killCount = 0;
				spawnEnemy(state, content::boss);
			}
			else
			{
				spawnEnemy(state, content::regularEnemy);
			}
		}
		state.phase = Phase::Fighting;
	}

	void finishEscape(GameState& state)
	{
		spawnEnemy(state, content::regularEnemy);
		state.phase = Phase::Fighting;
	}
}
