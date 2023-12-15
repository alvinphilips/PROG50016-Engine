#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "GameCore.h"

class Enemy : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Enemy, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
    void OnDisable() override;
protected:
    Vec2 previous_position;
    float speed = 5.0f;
    TextureAsset* texture = nullptr;
    CircleCollider* collider = nullptr;
    Player* player = nullptr;
};

#endif // ENEMY_H


