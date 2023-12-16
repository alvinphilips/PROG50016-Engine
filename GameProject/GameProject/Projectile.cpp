#include "GameCore.h"
#include "Projectile.h"

IMPLEMENT_DYNAMIC_CLASS(Projectile)

void Projectile::Initialize()
{
	texture = (TextureAsset*) AssetManager::Get().GetAsset("cheems");
	Component::Initialize();
	Sprite* sprite = (Sprite*)ownerEntity->CreateComponent("Sprite");
	collider = (CircleCollider*)ownerEntity->CreateComponent("CircleCollider");
	sprite->SetTextureAsset(texture);
}

void Projectile::Update()
{
	Component::Update();

	lifetime -= Time::Instance().DeltaTime();

	ownerEntity->GetTransform().position += velocity * Time::Instance().DeltaTime();

	bool is_dead = lifetime < 0;

	for (const auto other: collider->OnCollisionEnter())
	{
		std::string& other_name = other->GetOwner()->GetName();
		if (other_name == "Projectile")
		{
			continue;
		}
		is_dead = is_dead || ((other_name == "Player") != player_projectile);
		break;
	}

	if (is_dead)
	{
		ownerEntity->GetParentScene()->RemoveEntity(ownerEntity->GetUid());
	}
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
