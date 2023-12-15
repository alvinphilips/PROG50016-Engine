#include "GameCore.h"
#include "MainMenu.h"

IMPLEMENT_DYNAMIC_CLASS(MainMenu)

void MainMenu::Initialize()
{
	Menu::Initialize();
	SetCallback(0, [this] {
		Scene* current_scene = SceneManager::Get().GetActiveScene();
		if (SceneManager::Get().SetActiveScene(start_scene)) {
			current_scene->SetEnabled(false);
		}
		else {
			LOG("Could not change scene");
		}
	});

	// TODO: Implement load?
	SetCallback(1, [] {LOG("oh no, load time")});
}

void MainMenu::Load(json::JSON& node)
{
	Menu::Load(node);

	if (node.hasKey("StartScene")) {
		start_scene = GetHashCode(node.at("StartScene").ToString().c_str());
	}
}
