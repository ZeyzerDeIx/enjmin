#pragma once
#include "Entity.h"
#include "Camera.h"

class InputManager
{
public:
	InputManager();
	InputManager(Entity* player, Camera* camera, GameMap* gameMap);
	void setPlayer(Entity* player);
	void setCamera(Camera* camera);
	void setGameMap(GameMap* gameMap);
	void handleInputs(sf::Event event);

	void processKeyPressed(sf::Keyboard::Key key);
	void processKeyReleased(sf::Keyboard::Key key);

	void im();

private:
	Entity* m_player;
	Camera* m_camera;
	GameMap* m_gameMap;

	bool m_ctrl;
	bool m_leftClick;
	bool m_rightClick;
};

