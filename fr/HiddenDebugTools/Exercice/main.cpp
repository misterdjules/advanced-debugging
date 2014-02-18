/*
* The goal of this exercice is to find the cause of a crash when running the program
* in the "Release" configuration. You will need to use all the know
*/
#include <cstring>
#include <cstdio>

#include "PlayerManager.h"
#include "GameObject.h"

PlayerManager g_playerManager;

int main()
{		
	GameObject* player = new GameObject("Player");
	
	player->AddComponent(new PhysicComponent());
	g_playerManager.SetPlayer(*player);
	
	player->Update();

	delete player;
	player = 0;

	return 0;
}