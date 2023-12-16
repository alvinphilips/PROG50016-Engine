#include "GameCore.h"
#include "ProjectileSystem.h"

Projectile* ProjectileSystem::AddProjectile(Vec2 pos, Vec2 vel)
{
	if (length >= capacity)
	{
		LOG("Reached capacity");
		return nullptr;
	}

	Projectile* projectile = projectiles[length++];
	Entity* entity = projectile->ownerEntity;
	entity->GetTransform().position = pos;
	entity->isActive = true;
	projectile->velocity = vel;

	return projectile;
}

void ProjectileSystem::Initialize()
{
	length = 0;
	projectiles.resize(capacity);
	for (int i = 0; i < capacity; i++)
	{
		Entity* entity = new Entity();
		entity->ownerScene = owner_scene;
		entity->isActive = false;
		projectiles[i] = (Projectile*) entity->CreateComponent("Projectile");
		projectiles[i]->Initialize();
	}
}

void ProjectileSystem::Update()
{
	for (int i = 0; i < length; i++)
	{
		Projectile* projectile = projectiles[i];
		projectile->Update();
		if (!projectile->isActive)
		{
			projectiles[i] = projectiles[--length];
			projectiles[length] = projectile;
		}
	}
}

void ProjectileSystem::Destroy()
{
	for (const auto projectile: projectiles)
	{
		projectile->ownerEntity->Destroy();
		delete projectile->ownerEntity;
	}

	projectiles.clear();
}

void ProjectileSystem::SetParentScene(Scene* scene)
{
	owner_scene = scene;
}
