#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameCore.h"

class Projectile: public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Projectile, Component)
	void Initialize() override;
	void Update() override;
	void SetLifetime(float);
	void SetVelocity(Vec2);
	void SetColor(Color) const;
	bool player_projectile = true;
private:
	CircleCollider* collider = nullptr;
	TextureAsset* texture = nullptr;
	float lifetime = 10;
	Vec2 velocity;
};

#endif