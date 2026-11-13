#pragma once

//definition data shared by every enemy of one kind
struct EnemyType
{
	std::vector<std::string> texturePaths; //one is picked at random each spawn
	float hpMax;
	int coinsMin;
	int coinsMax;
	float scale;
};
