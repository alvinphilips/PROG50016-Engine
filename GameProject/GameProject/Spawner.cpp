#include "GameCore.h"
#include "Spawner.h"

IMPLEMENT_DYNAMIC_CLASS(Spawner)

void Spawner::Initialize()
{
	Component::Initialize();

	collider = (CircleCollider*)ownerEntity->GetComponent("CircleCollider");

	if (collider == nullptr) {
		LOG("Could not get CircleCollider component");
	}
}

void Spawner::Update()
{
	Component::Update();

	spawn_timer -= Time::Instance().DeltaTime();

	if (spawn_timer > 0) {
		return;
	}

	// Do not spawn if we're currently blocked
	if (collider != nullptr && collider->OnCollisionEnter().size() > 0) {
		return;
	}

	Scene* scene = ownerEntity->GetParentScene();
	if (scene == nullptr) {
		return;
	}

	// Too many of us exist :(
	if (max_spawn_type != -1 && scene->FindEntityWithComponent(spawn_class).size() > max_spawn_type) {
		return;
	}

	// Create and position enemy
	Entity* entity = scene->CreateEntity();
	Component* component = entity->CreateComponent(spawn_class);
	component->Load(instance_data);
	entity->GetTransform().position = ownerEntity->GetTransform().position;

	spawn_timer = spawn_delay;
}

void Spawner::Load(json::JSON& node)
{
	Component::Load(node);

	if (node.hasKey("SpawnDelay")) {
		spawn_delay = (float) node.at("SpawnDelay").ToFloat();
	}

	if (node.hasKey("SpawnClass")) {
		spawn_class = node.at("SpawnClass").ToString();
	}

	if (node.hasKey("MaxSpawnsOfType")) {
		max_spawn_type = node.at("MaxSpawnsOfType").ToInt();
	}

	if (node.hasKey("DropItem")) {
		dropped_item_class = node.at("DropItem").ToString();
		drop_item_when_destroyed = true;
	}

	if (node.hasKey("InstanceData")) {
		instance_data = node["InstanceData"];
	}
}

void Spawner::Destroy()
{
	if (drop_item_when_destroyed) {
		Scene* scene = ownerEntity->GetParentScene();
		if (scene == nullptr) {
			return;
		}

		Entity* entity = scene->CreateEntity();
		entity->CreateComponent(dropped_item_class);
		entity->GetTransform().position = ownerEntity->GetTransform().position;
	}
	Component::Destroy();
}
