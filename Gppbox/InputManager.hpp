#pragma once
#include "Entity.h"

class InputManager
{
public:
	InputManager();
	InputManager(Entity* player);
	void setPlayer(Entity* player);
	void handleInputs();

private:
	Entity* m_player;
};

