#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "GameObject.h"

class PlayerManager
{
public:
	PlayerManager() 		
	{}
	

	void SetPlayer(const GameObject player);
			
private:
	GameObject m_player;
};

#endif // PLAYER_MANAGER_H_