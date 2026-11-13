#pragma once

#include "content/EnemyType.h"

namespace content
{
	//regular enemy: one type, several textures purely for variety
	inline const EnemyType regularEnemy{
		{ "textures/enemies/slime.png", "textures/enemies/crab.png", "textures/enemies/rat.png",
		  "textures/enemies/raven.png", "textures/enemies/wasp.png" },
		10.f, 1, 2, 0.25f, false };

	inline const EnemyType boss{
		{ "textures/enemies/jellyfish.png" },
		250.f, 10, 20, 0.5f, true };
}
