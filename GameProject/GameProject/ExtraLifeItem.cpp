#include "GameCore.h"
#include "ExtraLifeItem.h"

IMPLEMENT_DYNAMIC_CLASS(ExtraLifeItem);

void ExtraLifeItem::SetPlayerAttrib(Player* player)
{
	player->lives = std::min(player->lives + 1, player->max_lives);
}
