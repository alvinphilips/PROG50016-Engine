#include "GameCore.h"
#include "FireRateItem.h"

IMPLEMENT_DYNAMIC_CLASS(FireRateItem);

void FireRateItem::SetPlayerAttrib(Player* player)
{
	float multiplier = 2.0f;
	player->fire_rate_multiplier = multiplier;
}
