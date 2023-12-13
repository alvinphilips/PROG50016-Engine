// @file: Spawner.h
//
// @brief: A Spawner is responible for spawning enemies and can potentially
// drop an Item when destroyed.
//
// @author: Alvin Philips
// @date: 2023-12-13

#pragma once

#ifndef SPAWNER_H
#define SPAWNER_H

#include "GameCore.h"

class Spawner : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Spawner, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
    void Destroy() override;
private:
    CircleCollider* collider = nullptr;
    float spawn_delay = 10.0f;
    float spawn_timer;
    int max_spawn_type = -1;
    std::string spawn_class;
    bool drop_item_when_destroyed = false;
    std::string dropped_item_class;
};

#endif // SPAWNER_H


