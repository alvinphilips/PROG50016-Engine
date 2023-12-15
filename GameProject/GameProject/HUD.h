// @file: HUD.h
//
// @brief: The HUD is responsible for displaying game-related data, including current and high scores, play lives and time left.
//
// @author: Alvin Philips
// @date: 2023-12-15

#pragma once

#ifndef HUD_H
#define HUD_H

#include "GameCore.h"

class HUD : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(HUD, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
    void SetKillCount(int);
    void SetHighScore(int);
    void ResetGameCountdown(float);

private:
    FontSprite* text_kill_count = nullptr;
    std::vector<Sprite*> sprite_lives;
    FontSprite* text_game_countdown = nullptr;
    FontSprite* text_high_score = nullptr;

protected:
    FontSprite* CreateTextField(FontAsset*);
    Vec2 SetTextField(FontSprite* text_field, const std::string& text, Vec2 offset=Vec2::Zero);
    Vec2 SetPlayerLives(int lives, Vec2 offset = Vec2::Zero);

    std::string kill_count;
    float game_countdown = 3 * 60;
    std::string high_score;
    std::string text_font = "oswald-font";
    Vec2 ui_offset = { 10, 10 };
};

#endif // HUD_H


