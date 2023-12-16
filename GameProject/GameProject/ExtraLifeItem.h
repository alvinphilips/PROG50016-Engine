#pragma once

#ifndef EXTRA_LIFE_ITEM_H
#define EXTRA_LIFE_ITEM_H

#include "GameCore.h"

class ExtraLifeItem final : public Item
{
    DECLARE_DYNAMIC_DERIVED_CLASS(ExtraLifeItem, Item)

public:
    void SetPlayerAttrib(Player*) override;
private:
};

#endif // EXTRA_LIFE_ITEM_H

