#include "GameCore.h"
#include "PauseMenu.h"

IMPLEMENT_DYNAMIC_CLASS(PauseMenu)

void PauseMenu::Initialize()
{
	Menu::Initialize();
	SetCallback(0, [this] { ownerEntity->GetParentScene()->isEnabled = false; });

	// TODO: Implement save?
	SetCallback(1, [] {LOG("oh no, save time")});

	SetCallback(2,
		[this] {
			Scene* current_scene = ownerEntity->GetParentScene();
			if (SceneManager::Get().SetActiveScene(main_scene)) {
				current_scene->isEnabled = false;
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
