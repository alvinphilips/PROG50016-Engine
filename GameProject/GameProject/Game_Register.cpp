#include "GameCore.h"

void GameRegisterClasses()
{
	REGISTER_TYPECLASS(Player);
	REGISTER_TYPECLASS(GameOver);
	REGISTER_TYPECLASS(Menu);
	REGISTER_TYPECLASS(MainMenu);
	REGISTER_TYPECLASS(PauseMenu);
}