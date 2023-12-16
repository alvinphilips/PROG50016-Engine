#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameCore.h"

class HUD;

class Player : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

public:
    void Initialize() override;
    void OnEnable() override;
    void OnDisable() override;
    void Update() override;
    void Load(json::JSON&) override;
    void TakeDamage();
    float speed_multiplier = 1;
    float fire_rate_multiplier = 1;
    int lives;
    int max_lives = 3;
private:
    float speed = 5.0f;
    float shoot_delay = 0.2f;
    float shoot_timer;
    float pause_delay = 0.2f;
    float pause_timer;
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    HUD* hud = nullptr;
    STRCODE main_scene = -1;
};

#endif // PLAYER_H


