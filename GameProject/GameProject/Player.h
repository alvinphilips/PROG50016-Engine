#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameCore.h"

class Player : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:
    float speed = 5.0f;
    int max_lives = 3;
    int lives;
    float shoot_delay = 0.2f;
    float shoot_timer;
    float pause_delay = 0.2f;
    float pause_timer;
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;
};

#endif // PLAYER_H


