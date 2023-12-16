#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameCore.h"

class Projectile: public Component
{
	friend class ProjectileSystem;
	DECLARE_DYNAMIC_DERIVED_CLASS(Projectile, Component)
	void Initialize() override;
	void Update() override;
	void SetLifetime(float);
	void SetVelocity(Vec2);
	void SetColor(Color) const;
private:
	TextureAsset* texture = nullptr;
	float lifetime = 10;
	Vec2 velocity;
};

#endif