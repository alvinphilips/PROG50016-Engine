#pragma once

#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H

#include "GameCore.h"
#include "Projectile.h"

class ProjectileSystem {

	DECLARE_SINGLETON(ProjectileSystem)
private:
	std::vector<Projectile*> projectiles;
	int capacity = 100;
	int length;
	Scene* owner_scene = nullptr;
public:
	Projectile* AddProjectile(Vec2 pos, Vec2 vel);
	void Initialize();
	void Update();
	void Destroy();
	void SetParentScene(Scene*);
};

#endif