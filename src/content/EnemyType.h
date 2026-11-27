#pragma once

//definition data shared by every enemy of one kind
struct EnemyType
{
	std::vector<std::string> texturePaths; //one is picked at random each spawn
	double hpMax;
	int coinsMin;
	int coinsMax;
	float scale;
	bool isBoss;
	double timeLimit; //seconds before a boss escapes, 0 for no limit
	int killsToSpawn; //regular kills before this boss appears, 0 for non-bosses
};
