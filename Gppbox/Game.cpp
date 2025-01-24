
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include "TextureManager.hpp"



Game::Game(sf::RenderWindow * win)
{
	this->win = win;
	win->setVerticalSyncEnabled(true);
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	sf::Texture& texture = TextureManager::getTexture("bg_stars.png");
	bg.setTexture(&texture);
	bg.setSize(sf::Vector2f(1280, 720));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	m_entities.push_back(Entity("Player.png", &m_gameMap));
	m_inputManager.setPlayer(&m_entities[0]);
}

void Game::processInput(sf::Event ev)
{
	if (ev.type == sf::Event::Closed)
	{
		win->close();
		closing = true;
		return;
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;

static sf::VertexArray va;
static RenderStates vaRs;

int blendModeIndex(sf::BlendMode bm)
{
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt)
{

	m_inputManager.handleInputs();
	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	for (auto& entity : m_entities) entity.update(dt);
	afterParts.update(dt);
}

 void Game::draw(sf::RenderWindow & win)
 {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	auto& tex = TextureManager::getTexture("bg_stars.png");
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	m_gameMap.draw(win);


	for (auto& entity : m_entities) entity.draw(win);
	m_entities[0].im();

	afterParts.draw(win);
}

void Game::im()
{
	
}

