#include "GameCore.h"
#include "SpeedBoostItem.h"

IMPLEMENT_DYNAMIC_CLASS(SpeedBoostItem)

void SpeedBoostItem::SetPlayerAttrib(Player* player)
{
	player->speed_multiplier = 1.5f;
}
