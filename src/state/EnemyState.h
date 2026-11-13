#pragma once

#include "content/EnemyType.h"

//the current enemy's state
struct EnemyState
{
	const EnemyType* type = nullptr;
	double hp = 0.0;
};
