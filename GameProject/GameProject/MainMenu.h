// @file: MainMenu.h
//
// @brief: The Main menu
//
// @author: Alvin Philips
// @date: 2023-12-13

#pragma once

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

class MainMenu final : public Menu
{
    DECLARE_DYNAMIC_DERIVED_CLASS(MainMenu, Menu)
public:
    void Initialize() override;
    void Load(json::JSON&) override;
private:
    STRCODE start_scene = -1;
};

#endif // MAIN_MENU_H


