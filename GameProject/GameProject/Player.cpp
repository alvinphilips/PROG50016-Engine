#include "GameCore.h"
#include "Player.h"

#define NDEBUG_PLAYER

IMPLEMENT_DYNAMIC_CLASS(Player)

void LoadMainScene(STRCODE main_scene) {
    Scene* active_scene = SceneManager::Get().GetActiveScene();
    if (SceneManager::Get().SetActiveScene(main_scene)) {
        active_scene->SetEnabled(false);
    }
}


void Player::Initialize()
{
    Component::Initialize();
    start_pos = ownerEntity->GetTransform().position;
    collider = (BoxCollider*)ownerEntity->GetComponent("BoxCollider");
    auto search_hud_entity = ownerEntity->GetParentScene()->FindEntityByName("HUD");
    if (search_hud_entity.size() > 0) {
        hud = ((HUD*)search_hud_entity.front()->GetComponent("HUD"));
    }
}

void Player::OnEnable() {
    lives = max_lives;
    if (hud != nullptr) {
        hud->lives = lives;
        hud->game_countdown = 3 * 60;
        hud->kill_count = 0;
    }
}

void Player::OnDisable() {
    if (hud != nullptr) {
        hud->high_score = std::max(hud->kill_count, hud->high_score);
    }
}

void Player::Update() {
    shoot_timer -= Time::Instance().DeltaTime();
    pause_timer -= Time::Instance().DeltaTimeUnscaled();

    if (hud != nullptr && hud->game_countdown < 0) {
        LoadMainScene(main_scene);
    }

    Vec2 dir = Vec2::Zero;
    const InputSystem& input = InputSystem::Instance();

    // Handle horizontal movement
    if (input.isKeyPressed(SDLK_LEFT) || input.isKeyPressed(SDLK_a) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        dir.x -= 1;
    }
    if (input.isKeyPressed(SDLK_RIGHT) || input.isKeyPressed(SDLK_d) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        dir.x += 1;
    }

    // Handle vertical movement
    if (input.isKeyPressed(SDLK_UP) || input.isKeyPressed(SDLK_w) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        dir.y -= 1;
    }
    if (input.isKeyPressed(SDLK_DOWN) || input.isKeyPressed(SDLK_s) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        dir.y += 1;
    }

    // Handle gamepad analog stick input
    if (dir == Vec2::Zero) {
        dir.x = input.getGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTX);
        dir.y = input.getGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTY);
    }

    // Normalize the direction vector if it's not zero
    if (dir != Vec2::Zero) {
        dir.Normalize();
#ifdef DEBUG_PLAYER
        LOG("Input: " << dir.x << ", " << dir.y);
#endif
    }

    // Move the player
    ownerEntity->GetTransform().position += dir * (speed * Time::Instance().DeltaTime());

    if (input.isMouseButtonPressed(SDL_BUTTON_LEFT) && shoot_timer < 0) {
        Scene* scene = ownerEntity->GetParentScene();
        shoot_timer = shoot_delay;
    }

    if (input.isKeyPressed(SDLK_BACKQUOTE) && pause_timer < 0) {
        Scene* pause_scene = SceneManager::Get().FindScene("pause-menu");
        pause_scene->SetEnabled(!pause_scene->IsEnabled());
        pause_timer = pause_delay;
    }

    if (collider == nullptr)
    {
        LOG("no collider uwu");
        return;
    }
    for (const auto& other : collider->OnCollisionEnter())
    {
        if (other->GetOwner()->GetName() != "Enemy")
        {
            continue;
        }

        Scene* current_scene = SceneManager::Get().GetActiveScene();
        if (SceneManager::Get().SetActiveScene(main_scene))
        {
            current_scene->SetEnabled(false);
        }

        ownerEntity->GetTransform().position = start_pos;
    }
}
void Player::Load(json::JSON& node)
{
    Component::Load(node);
    if (node.hasKey("Speed"))
    {
        speed = static_cast<float>(node.at("Speed").ToFloat());
    }

    if (node.hasKey("MainScene"))
    {
        main_scene = GetHashCode(node.at("MainScene").ToString().c_str());
    }
}

void Player::TakeDamage()
{
    lives--;
    LOG(lives);
    if (lives < 0) {
        LoadMainScene(main_scene);
        return;
    }
    if (hud != nullptr) {
        hud->lives = lives;
    }
}
