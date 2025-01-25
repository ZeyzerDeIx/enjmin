#pragma once
#include "Entity.h"
#include "Camera.h"

class InputManager
{
public:
	InputManager();
	InputManager(Entity* player, Camera* camera);
	void setPlayer(Entity* player);
	void setCamera(Camera* camera);
	void handleInputs(sf::Event event);

	void processKeyPressed(sf::Keyboard::Key key);
	void processKeyPressedFreeCam(sf::Keyboard::Key key);

	void processKeyReleased(sf::Keyboard::Key key);
	void processKeyReleasedFreeCam(sf::Keyboard::Key key);

private:
	Entity* m_player;
	Camera* m_camera;
};

