#pragma once

#include "state/PlayerState.h"

namespace rules
{
	//returns true if the upgrade was bought
	bool tryBuyDamageUpgrade(PlayerState& player);
}
