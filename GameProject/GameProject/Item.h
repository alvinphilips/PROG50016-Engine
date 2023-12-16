#pragma once

#ifndef GAME_ITEM_H
#define GAME_ITEM_H

#include "GameCore.h"

class Item : public Component {
	DECLARE_DYNAMIC_DERIVED_CLASS(Item, Component);
public:
	virtual void SetPlayerAttrib(Player* player) {};
};
#endif
