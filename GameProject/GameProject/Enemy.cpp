#include "GameCore.h"
#include "Enemy.h"

IMPLEMENT_DYNAMIC_CLASS(Enemy)

void Enemy::Initialize()
{
	Component::Initialize();
	auto sprite = (Sprite*)ownerEntity->CreateComponent("Sprite");
	sprite->SetTextureAsset(texture);
	collider = (CircleCollider*)ownerEntity->CreateComponent("CircleCollider");
	auto entities = SceneManager::Get().FindEntityByName("Player");

	if (entities.size() == 0) {
		LOG("Player entity not found");
	}

	if (entities.size() > 1) {
		LOG("Multiple Entities with name 'Player' found, using first one.");
	}

	player = (Player*)entities.front()->GetComponent("Player");
}

void Enemy::Update()
{
	if (collider == nullptr)
	{
		LOG("no enemy collider uwu");
		return;
	}

	bool collided = false;

	for (const auto& other : collider->OnCollisionEnter())
	{
		if (other->GetOwner() != player->GetOwner())
		{
			ownerEntity->GetTransform().position = previous_position;
			collided = true;
			continue;
		}
		// We hit the Player 
		auto search_hud_entity = ownerEntity->GetParentScene()->FindEntityByName("HUD");
		if (search_hud_entity.size() > 0) {
			HUD* hud = ((HUD*)search_hud_entity.front()->GetComponent("HUD"));
			hud->kill_count++;
			hud->high_score = std::max(hud->kill_count, hud->high_score);
		}
		player->TakeDamage();

		// Remove the Enemy
		ownerEntity->GetParentScene()->RemoveEntity(ownerEntity->GetUid());
	}

	if (!collided) {
		previous_position = ownerEntity->GetTransform().position;
	}
}

void Enemy::Load(json::JSON& node)
{
	if (node.hasKey("Texture")) {
		texture = (TextureAsset*)AssetManager::Get().GetAsset(node.at("Texture").ToString());
	}

	if (node.hasKey("Speed")) {
		speed = (float)node.at("Speed").ToFloat();
	}
}

void Enemy::OnDisable()
{
	// ownerEntity->GetParentScene()->RemoveEntity(ownerEntity->GetUid());
}
