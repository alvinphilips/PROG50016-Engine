#pragma once
#ifndef CURSOR_H
#define CURSOR_H

#include "GameCore.h"

class Cursor : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Cursor, Component)

public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
private:
};

#endif // CURSOR_H


