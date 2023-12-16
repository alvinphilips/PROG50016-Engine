#pragma once

#ifndef FIRE_RATE_ITEM
#define FIRE_RATE_ITEM

#include "GameCore.h"
#include "Item.h"

class FireRateItem final : public Item
{
    DECLARE_DYNAMIC_DERIVED_CLASS(FireRateItem, Item)

public:
    void SetPlayerAttrib(Player*) override;
private:
};

#endif