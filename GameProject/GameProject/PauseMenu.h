// @file: PauseMenu.h
//
// @brief: The Pause menu
//
// @author: Alvin Philips
// @date: 2023-12-13

#pragma once

#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Menu.h"

class PauseMenu final : public Menu
{
    DECLARE_DYNAMIC_DERIVED_CLASS(PauseMenu, Menu)
public:
    void Initialize() override;
    void Load(json::JSON&) override;
    void OnEnable() override;
    void OnDisable() override;
private:
    STRCODE main_scene = -1;
};

#endif // PAUSE_MENU_H


