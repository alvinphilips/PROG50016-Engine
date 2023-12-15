#include "GameCore.h"
#include "PauseMenu.h"

#define NDEBUG_PAUSE_ENABLE

IMPLEMENT_DYNAMIC_CLASS(PauseMenu)

void PauseMenu::Initialize()
{
	Menu::Initialize();
	SetCallback(0, [this] { ownerEntity->GetParentScene()->SetEnabled(false); });

	// TODO: Implement save?
	SetCallback(1, [] {LOG("oh no, save time")});

	SetCallback(2,
		[this] {
			Scene* current_scene = ownerEntity->GetParentScene();
			Scene* active_scene = SceneManager::Get().GetActiveScene();
			if (SceneManager::Get().SetActiveScene(main_scene)) {
				current_scene->SetEnabled(false);
				active_scene->SetEnabled(false);
			}
		});
}

void PauseMenu::Load(json::JSON& node)
{
	Menu::Load(node);

	if (node.hasKey("MainScene")) {
		main_scene = GetHashCode(node.at("MainScene").ToString().c_str());
	}
}

void PauseMenu::OnEnable()
{
#ifdef DEBUG_PAUSE_ENABLE
	LOG("pause menu enabled");
#endif
	Time::Instance().time_scale = 0;
}

void PauseMenu::OnDisable()
{

#ifdef DEBUG_PAUSE_ENABLE
	LOG("pause menu disabled");
#endif
	Time::Instance().time_scale = 1;
}