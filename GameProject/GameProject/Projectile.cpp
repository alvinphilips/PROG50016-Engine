#include "GameCore.h"
#include "Projectile.h"

IMPLEMENT_DYNAMIC_CLASS(Projectile)

void Projectile::Initialize()
{
	texture = (TextureAsset*) AssetManager::Get().GetAsset("cheems");
	Component::Initialize();
	Sprite* sprite = (Sprite*)ownerEntity->CreateComponent("Sprite");
	sprite->SetTextureAsset(texture);
}

void Projectile::Update()
{
	Component::Update();

	lifetime -= Time::Instance().DeltaTime();

	if (lifetime < 0)
	{
		ownerEntity->isActive = false;
	}

	ownerEntity->GetTransform().position += velocity * Time::Instance().DeltaTime();
}

void Projectile::SetLifetime(const float lifetime)
{
	this->lifetime = lifetime;
}

void Projectile::SetVelocity(const Vec2 velocity)
{
	this->velocity = velocity;
}

void Projectile::SetColor(const Color color) const
{
	Sprite* sprite = (Sprite*)ownerEntity->GetComponent("Sprite");
	if (sprite == nullptr)
	{
		return;
	}

	sprite->SetFilterColor(color.r, color.g, color.b, color.a);
}
