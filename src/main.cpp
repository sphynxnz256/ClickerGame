#include "stdafx.h"
#include "core/Game.h"

int main()
{
	try
	{
		//create a reusable random number generator
		RNG rng;

		Game game(rng);

		//main game loop
		while (game.getWindow().isOpen())
		{
			game.update();
			game.render();
		}
	}
	catch (const std::exception& e)
	{
		//e.g. a missing font or texture file
		std::cerr << "ERROR: " << e.what() << "\n";
		return 1;
	}

	return 0;
}

/*to do

-levels
	-map for level backgrounds
	-vector of file names of textures for enemes and boss for each level
	-multipliers for boss/enemy max hp
	-multiplier for coin drops

-menu screen
	-load game
	-new game
	-options
	-exit etc

-saving games


*/