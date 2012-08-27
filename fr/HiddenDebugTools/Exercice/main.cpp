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