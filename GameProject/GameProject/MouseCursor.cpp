#include "GameCore.h"
#include "MouseCursor.h"

IMPLEMENT_DYNAMIC_CLASS(Cursor)

void Cursor::Initialize()
{
	Component::Initialize();
}

void Cursor::Update() {
	const InputSystem& input = InputSystem::Instance();
	ownerEntity->GetTransform().position = input.MousePosition();
}

void Cursor::Destroy()
{
	Component::Destroy();
}

void Cursor::OnEnable() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Cursor::OnDisable() {
	SDL_ShowCursor(SDL_ENABLE);
}