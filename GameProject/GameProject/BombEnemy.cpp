#include "GameCore.h"
#include "BombEnemy.h"

IMPLEMENT_DYNAMIC_CLASS(BombEnemy)

void BombEnemy::Initialize()
{
	Enemy::Initialize();
}

void BombEnemy::Update()
{
	auto dir = player->GetOwner()->GetTransform().position - ownerEntity->GetTransform().position;

	// Let's not get TOO close to the player, either :P
	if (dir.MagnitudeSquared() > min_padding_from_player_squared) {
		dir.Normalize();
		ownerEntity->GetTransform().position += dir * speed * Time::Instance().DeltaTime();
	}

	// Enemy update takes care of enemy-enemy collisions
	Enemy::Update();
}

void BombEnemy::Load(json::JSON& node)
{
	Enemy::Load(node);
}
