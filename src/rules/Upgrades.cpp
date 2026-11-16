#include "stdafx.h"
#include "rules/Upgrades.h"

namespace rules
{
	bool tryBuyDamageUpgrade(PlayerState& player)
	{
		if (player.coins >= player.upgradeCost)
		{
			player.damage *= 2.0;
			player.coins -= player.upgradeCost;
			player.upgradeCost *= 2.0;
			return true;
		}
		return false;
	}
}
