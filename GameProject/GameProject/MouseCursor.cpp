#include "GameCore.h"
#include "MouseCursor.h"

IMPLEMENT_DYNAMIC_CLASS(Cursor)

void Cursor::Initialize()
{
	Component::Initialize();
	SDL_ShowCursor(SDL_DISABLE);
}

void Cursor::Update() {
	const InputSystem& input = InputSystem::Instance();
	ownerEntity->GetTransform().position = input.MousePosition();
}

void Cursor::Destroy()
{
	SDL_ShowCursor(SDL_ENABLE);
	Component::Destroy();
}