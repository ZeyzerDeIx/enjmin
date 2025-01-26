#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"

#include "GameMap.hpp"
#include "Entity.h"
#include "InputManager.hpp"
#include "Camera.h"


class HotReloadShader;
class Game
{
public:
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	bool							closing = false;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Game(sf::RenderWindow * win);

	void processInput(sf::Event ev);

	void update(double dt);

	void draw(sf::RenderWindow& win);
	void im();

private:
	GameMap m_gameMap;
	std::vector<Entity> m_entities;
	InputManager m_inputManager;
	Camera m_camera;
	sf::Font m_font;
	sf::Text m_fpsCounter;
	double m_lastFPSUpdateElapsedTime;
	int m_lastFPSUpdateElpasedFrame;
};