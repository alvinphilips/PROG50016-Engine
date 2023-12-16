#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "GameCore.h"

class Enemy : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Enemy, Component)

    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
protected:
    Vec2 previous_position;
    float speed = 5.0f;
    float scale_sprite = 1;
    TextureAsset* texture = nullptr;
    CircleCollider* collider = nullptr;
    Player* player = nullptr;
};

#endif // ENEMY_H


