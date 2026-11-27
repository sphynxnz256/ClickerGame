#include "stdafx.h"
#include "rules/Spawning.h"
#include "content/EnemyData.h"

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

			//enough kills spawn the boss
			if (state.killCount >= content::boss.killsToSpawn)
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
