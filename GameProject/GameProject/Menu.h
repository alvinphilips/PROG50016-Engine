// @file: Menu.h
//
// @brief: Provides a Menu interface
//
// @author: Alvin Philips
// @date: 2023-12-12

#pragma once

#ifndef MENU_H
#define MENU_H

#include "GameCore.h"
#include "FontSprite.h"

class Menu : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Menu, Component)
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Load(json::JSON&) override;
    int SelectedIndex();
    bool SetCallback(int, std::function<void()>);

protected:
    bool use_mouse_input = true;
    bool clicked = false;
    int selected = -1;
    int item_padding = 2;
    std::string text_font = "oswald-font";
    float selection_delay = 0.2f;
    float selection_timer;
    Color default_color = { 255, 255, 255 };
    Color selection_color;
    std::vector<std::string> options;
    std::vector<FontSprite*> optionEntities;
    std::map<int, std::function<void()>> callbacks;
};

#endif // MENU_H


