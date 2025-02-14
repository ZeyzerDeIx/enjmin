#pragma once
#include "Entity.h"
#include "Camera.h"

class Game;

class InputManager
{
public:
	InputManager();
	InputManager(Entity* player, Camera* camera, GameMap* gameMap);
	void setPlayer(Entity* player);
	void setCamera(Camera* camera);
	void setGameMap(GameMap* gameMap);
	void setGame(Game* game);
	void handleInputs(sf::Event event);

	void handleJoystick();

	void processJoystick();

	void processKeyPressed(sf::Keyboard::Key key);
	void processKeyReleased(sf::Keyboard::Key key);

	void im();

private:
	Entity* m_player;
	Camera* m_camera;
	GameMap* m_gameMap;
	Game* m_game;

	bool m_ctrl;
	bool m_leftClick;
	bool m_rightClick;
	bool m_rightTrigger;
	uint8_t m_joystickDirections;
};

