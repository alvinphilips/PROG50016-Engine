#pragma once

#ifndef BOMB_ENEMY_H
#define BOMB_ENEMY_H

#include "GameCore.h"
#include "Enemy.h"

class BombEnemy : public Enemy
{
    DECLARE_DYNAMIC_DERIVED_CLASS(BombEnemy, Enemy)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:
    float min_padding_from_player_squared = 0.1f;
};

#endif // BOMB_ENEMY_H


