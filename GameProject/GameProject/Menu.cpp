#include "GameCore.h"
#include "Menu.h"

#include "FontAsset.h"

IMPLEMENT_DYNAMIC_CLASS(Menu)

void Menu::Initialize()
{
	Scene* scene = ownerEntity->GetParentScene();
	if (scene == nullptr) {
		LOG("Could not get scene");
		return;
	}

	FontAsset* font = (FontAsset*)AssetManager::Get().GetAsset("oswald-font");
	if (font == nullptr) {
		LOG("Could not get font");
		return;
	}

	int item_offset = 0;
	if (options.size() > 0) {
		item_offset = item_padding + font->GetTextSize(options[0]).y;
	}

	for (const auto& option : options) {
		Entity* optionEntity = scene->CreateEntity();
		FontSprite* fs = (FontSprite*)optionEntity->CreateComponent("FontSprite");
		fs->SetFont(font);
		fs->SetText(option);
		optionEntity->GetTransform().position.y = (float) item_offset;
		optionEntity->GetTransform().position.x = (float) RenderSystem::Instance().GetWindowSize().x / 2.0f;
		item_offset += item_padding + font->GetTextSize(option).y;
		optionEntities.push_back(fs);
	}

	item_offset /= 2;
	item_offset = RenderSystem::Instance().GetWindowSize().y / 2 - item_offset;

	for (auto& entity : optionEntities) {
		entity->GetOwner()->GetTransform().position.y += item_offset;
	}

	selected = optionEntities.size() > 0 ? 0 : -1;
	selection_timer = 0;
}

void Menu::Update()
{
	if (optionEntities.size() == 0) {
		return;
	}

	const InputSystem& input = InputSystem::Instance();

	selection_timer -= Time::Instance().DeltaTime();

	if (selection_timer > 0) {
		return;
	}

	if (input.isKeyPressed(SDLK_RETURN)) {
		selection_timer = selection_delay;
		auto callback = callbacks.find(selected);
		if (callback != callbacks.end()) {
			callback->second();
		}
		clicked = true;
	}

	const int old_selected = selected;

	// Handle vertical movement
	if (input.isKeyPressed(SDLK_UP) || input.isKeyPressed(SDLK_w) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
		selected++;
	}
	if (input.isKeyPressed(SDLK_DOWN) || input.isKeyPressed(SDLK_s) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
		selected--;
	}

	selected %= optionEntities.size();

	if (selected != old_selected) {
		optionEntities.at(old_selected)->SetFontColor(255, 255, 255, 255);
		optionEntities.at(old_selected)->GetOwner()->GetTransform().scale = Vec2(1.0f);
		selection_timer = selection_delay;
	}
	optionEntities.at(selected)->SetFontColor(selection_color.r, selection_color.g, selection_color.b, selection_color.a);
	optionEntities.at(selected)->GetOwner()->GetTransform().scale = Vec2(1.2f);
}

void Menu::Destroy()
{
}

void Menu::Load(json::JSON& node)
{
	if (node.hasKey("Options")) {
		for (const auto& option : node.at("Options").ArrayRange()) {
			options.push_back(option.ToString());
		}
	}

	if (node.hasKey("SelectionColor")) {
		selection_color = Color(node.at("SelectionColor"));
	}
}

int Menu::SelectedIndex()
{
	return selected;
}

bool Menu::SetCallback(int index, std::function<void()> callback)
{
	// Cannot assign callback since we're out of bounds
	if (index < 0 || index >= options.size()) {
		return false;
	}

	callbacks.emplace(index, callback);
	return true;
}
