#include "GameCore.h"
#include "HUD.h"

IMPLEMENT_DYNAMIC_CLASS(HUD)

FontSprite* HUD::CreateTextField(FontAsset* font) {
	Entity* entity = ownerEntity->GetParentScene()->CreateEntity();
	FontSprite* text = (FontSprite*)entity->CreateComponent("FontSprite");
	text->SetFont(font);

	return text;
}

Vec2 HUD::SetPlayerLives(int lives, Vec2 offset) {
	if (sprite_lives.size() == 0) {
		return offset;
	}

	Vec2 sprite_dimensions = sprite_lives[0]->GetSize();

	Vec2 temp_offset = offset;
	for (int i = 0; i < sprite_lives.size(); i++) {
		Sprite* sprite = sprite_lives[i];
		sprite->GetOwner()->GetTransform().position = sprite_dimensions / 2.0f + temp_offset;
		temp_offset.x += sprite_dimensions.x;
		int f = i < lives ? 255 : 100;
		sprite->SetFilterColor(f, f, f, f);
	}

	return Vec2(offset.x, offset.y + sprite_dimensions.y);
}

Vec2 HUD::SetTextField(FontSprite* text_field, const std::string& text, Vec2 offset) {
	text_field->SetText(text);
	Vec2 pos = Vec2(text_field->GetTextSize()) * text_field->GetOwner()->GetTransform().scale;
	text_field->GetOwner()->GetTransform().position = pos / 2 + offset;
	pos.x = offset.x;
	pos.y += offset.y;

	return pos;
}

void HUD::Initialize()
{
	FontAsset* font = (FontAsset*)AssetManager::Get().GetAsset(text_font);
	if (font == nullptr) {
		LOG("Could not get font");
		return;
	}

	for (int i = 0; i < 3; i++) {
		Entity* entity = ownerEntity->GetParentScene()->CreateEntity();
		entity->GetTransform().scale = Vec2(0.1f);
		Sprite* heart = (Sprite*)entity->CreateComponent("Sprite");
		heart->SetTextureAsset((TextureAsset*)AssetManager::Get().GetAsset("cheems"));
		sprite_lives.push_back(heart);
	}
	text_kill_count = CreateTextField(font);
	text_game_countdown = CreateTextField(font);
	text_high_score = CreateTextField(font);

	SetHighScore(0);
	SetKillCount(0);
}

void HUD::Update()
{
	game_countdown -= Time::Instance().DeltaTime();

	Vec2 offset = ui_offset;
	offset = SetTextField(text_kill_count, kill_count, offset);
	offset = SetPlayerLives(2, offset);

	// TODO: Switch to using a better/faster method :P
	int min = (int)game_countdown / 60;
	int sec = (int)game_countdown % 60;
	float ms = game_countdown - (float)(min * 60 + sec);
	std::string counter = "Time Left: " + std::to_string(min) + ":" + std::to_string(sec) + "." + std::to_string(int(ms * 100));

	offset = SetTextField(text_game_countdown, counter, offset);
	offset = SetTextField(text_high_score, high_score, offset);
}

void HUD::Load(json::JSON& node)
{
	if (node.hasKey("TextFont")) {
		text_font = node.at("TextFont").ToString();
	}
	if (node.hasKey("Offset")) {
		ui_offset = vec2_from_json(node.at("Offset"));
	}
}

void HUD::SetKillCount(int _kill_count)
{
	kill_count = "Kill Count: " + std::to_string(_kill_count);
}

void HUD::SetHighScore(int _high_score) {
	high_score = "High Score: " + std::to_string(_high_score);
}

void HUD::ResetGameCountdown(float _game_countdown)
{
	game_countdown = _game_countdown;
}