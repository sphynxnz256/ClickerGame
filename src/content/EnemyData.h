#pragma once

#include "content/EnemyType.h"

namespace content
{
	//regular enemy: one type, several textures purely for variety
	inline const EnemyType regularEnemy{
		{ "textures/enemies/slime.png", "textures/enemies/crab.png", "textures/enemies/rat.png",
		  "textures/enemies/raven.png", "textures/enemies/wasp.png" }, //texturePaths
		10.0,   //hpMax
		1,      //coinsMin
		2,      //coinsMax
		0.25f,  //scale
		false,  //isBoss
		0.0,    //timeLimit
		0 };    //killsToSpawn

	inline const EnemyType boss{
		{ "textures/enemies/jellyfish.png" }, //texturePaths
		250.0,  //hpMax
		10,     //coinsMin
		20,     //coinsMax
		0.5f,   //scale
		true,   //isBoss
		15.0,   //timeLimit
		10 };   //killsToSpawn
}
