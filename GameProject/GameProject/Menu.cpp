#include "GameCore.h"
#include "Menu.h"

#include "FontAsset.h"

IMPLEMENT_DYNAMIC_CLASS(Menu)

void Menu::Initialize()
{
	Component::Initialize();

	// Get the parent scene
	Scene* scene = ownerEntity->GetParentScene();
	if (scene == nullptr) {
		LOG("Could not get scene");
		return;
	}

	FontAsset* font = (FontAsset*)AssetManager::Get().GetAsset(text_font);
	if (font == nullptr) {
		LOG("Could not get font");
		return;
	}

	// Early exit if we do not have any menu items
	if (options.size() == 0) {
		return;
	}

	// Figure out the offset for the menu options
	int item_offset = item_padding + font->GetTextSize(options[0]).y;

	// Create the items
	for (const auto& option : options) {
		Entity* optionEntity = scene->CreateEntity();
		FontSprite* fs = (FontSprite*)optionEntity->CreateComponent("FontSprite");
		fs->SetFont(font);
		fs->SetText(option);
		fs->SetFontColor(default_color.r, default_color.g, default_color.b, default_color.a);
		optionEntity->GetTransform().position.y = (float)item_offset;
		optionEntity->GetTransform().position.x = (float)RenderSystem::Instance().GetWindowSize().x / 2.0f;
		item_offset += item_padding + font->GetTextSize(option).y;
		optionEntities.push_back(fs);
	}

	item_offset /= 2;
	item_offset = RenderSystem::Instance().GetWindowSize().y / 2 - item_offset;

	for (auto& entity : optionEntities) {
		entity->GetOwner()->GetTransform().position.y += item_offset;
	}

	selected = 0;
	selection_timer = 0;
}

void Menu::Update()
{
	if (optionEntities.size() == 0) {
		return;
	}

	const InputSystem& input = InputSystem::Instance();

	selection_timer -= Time::Instance().DeltaTimeUnscaled();

	if (clicked) {
		selection_timer = selection_delay;
		auto callback = callbacks.find(selected);
		if (callback != callbacks.end()) {
			callback->second();
		}
		clicked = false;
	}

	if (selection_timer > 0) {
		return;
	}

	const int old_selected = selected;

	// Handle vertical movement
	if (input.isKeyPressed(SDLK_UP) || input.isKeyPressed(SDLK_w) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
		selected--;
	}
	if (input.isKeyPressed(SDLK_DOWN) || input.isKeyPressed(SDLK_s) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
		selected++;
	}

	if (input.isKeyPressed(SDLK_RETURN)) {
		clicked = true;
	}

	if (use_mouse_input) {
		IVec2 mouse_pos = input.MousePosition();
		for (int i = 0; i < optionEntities.size(); i++) {
			const auto& text = optionEntities[i];
			IVec2 size = text->GetTextSize();
			Vec2 pos = text->GetOwner()->GetTransform().position;
			Vec2 top_left = pos - size / 2;
			// Silly little bounds check
			if (mouse_pos.x > top_left.x && mouse_pos.x < top_left.x + size.x
				&& mouse_pos.y > top_left.y && mouse_pos.y < top_left.y + size.y) {
				selected = i;

				if (input.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
					clicked = true;
				}

				break;
			}
		}
	}

	// Tiny hack to let us go from the first to last option
	selected = int(selected + optionEntities.size()) % (int) optionEntities.size();

	if (selected != old_selected) {
		optionEntities.at(old_selected)->SetFontColor(default_color.r, default_color.g, default_color.b, default_color.a);
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
	Component::Load(node);
	if (node.hasKey("Options")) {
		for (const auto& option : node.at("Options").ArrayRange()) {
			options.push_back(option.ToString());
		}
	}

	if (node.hasKey("TextFont")) {
		text_font = node.at("TextFont").ToString();
	}

	if (node.hasKey("DefaultColor")) {
		default_color = Color(node.at("DefaultColor"));
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
