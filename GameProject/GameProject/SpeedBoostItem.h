#pragma once
#ifndef SPEED_BOOST_ITEM_H
#define SPEED_BOOST_ITEM_H

#include "GameCore.h"

class SpeedBoostItem final : public Item
{
    DECLARE_DYNAMIC_DERIVED_CLASS(SpeedBoostItem, Item)

public:
    void SetPlayerAttrib(Player*) override;
private:
};

#endif // SPEED_BOOST_ITEM_H

