
#include <imgui.h>
#include <array>
#include <vector>
#include <fstream>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"



Game::Game(sf::RenderWindow * win):
	m_camera(win)
{
	this->win = win;
	win->setKeyRepeatEnabled(false);
	win->setVerticalSyncEnabled(true);
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	sf::Texture& texture = m_textureManager.getTexture("bg_stars.png");
	bg.setTexture(&texture);
	bg.setSize(sf::Vector2f(1280, 720));

	//bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	m_entities.push_back(new Entity(createSprite("Player.png"), &m_gameMap, sf::Color{ 0x29a329ff }));
	m_entities[0]->addGun(m_entities, &m_camera);
	m_inputManager.setPlayer(m_entities[0]);
	m_inputManager.setCamera(&m_camera);
	m_inputManager.setGameMap(&m_gameMap);
	m_inputManager.setGame(this);
	m_camera.attachEntity(m_entities[0]);

	if (!m_font.loadFromFile("res/MAIAN.TTF"))
	{
		cout << "ERROR NO FONT" << endl;
		exit(1);
	}
	m_fpsCounter.setFont(m_font);
	m_fpsCounter.setString("FPS:");

	m_lastFPSUpdateElapsedTime = 0.0;
	m_lastFPSUpdateElpasedFrame = 0;

	loadEnemies();
}

Game::~Game()
{
	for (Entity* entityPtr : m_entities)
		delete entityPtr;
}

void Game::processInput(sf::Event ev)
{
	if (ev.type == sf::Event::Closed)
	{
		win->close();
		closing = true;
		return;
	}
	m_inputManager.handleInputs(ev);
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
	g_time += dt;
	m_lastFPSUpdateElpasedFrame++;
	//if (bgShader) bgShader->update(dt);

	//beforeParts.update(dt);
	for (auto& entity : m_entities) entity->update(dt);
	for (auto it = m_entities.begin(); it != m_entities.end();)
	{
		if ((*it)->isDead())
		{
			delete (*it);
			it = m_entities.erase(it);
		}
		else it++;
	}
	m_camera.update(dt);

	m_inputManager.handleJoystick(); //must be in update to not depend on events
	//afterParts.update(dt);

	if ((m_lastFPSUpdateElapsedTime += dt) > 0.25)
	{
		m_fpsCounter.setString("FPS: " + std::to_string((int)(m_lastFPSUpdateElpasedFrame / m_lastFPSUpdateElapsedTime)));
		m_lastFPSUpdateElapsedTime = 0.0;
		m_lastFPSUpdateElpasedFrame = 0;
	}
}

 void Game::draw(sf::RenderWindow & win)
 {
	if (closing) return;

	/*sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	auto& tex = TextureManager::getTexture("bg_stars.png");
	states.texture = &tex;
	sh->setUniform("texture", tex);*/
	//sh->setUniform("time", g_time);
	m_camera.enableAbsolute(true);
	win.draw(bg/*, states*/);
	win.draw(m_fpsCounter);
	m_camera.enableAbsolute(false);

	//beforeParts.draw(win);

	m_gameMap.draw(win);

	for (auto& entity : m_entities) entity->draw(win);

	//afterParts.draw(win);
}

void Game::im()
{
	m_inputManager.im();
	m_entities[0]->im();
	m_camera.im();
}

sf::Sprite Game::createSprite(std::string spritePath)
{
	return sf::Sprite(m_textureManager.getTexture(spritePath));
}

void Game::spawnEnemy(sf::Vector2f pos)
{
	m_entities.push_back(new Entity(createSprite("Player.png"), &m_gameMap, sf::Color{0xcc3300ff}));
	m_entities.back()->setPos(pos);
	m_entities.back()->setMustache(true);
}

void Game::loadEnemies()
{
	std::ifstream file("enemies.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening enemies file to load" << std::endl;
		return;
	}
	float x, y;
	while (file >> x >> y)
		spawnEnemy({ x,y });
	file.close();
	std::cout << "Enemies loaded" << std::endl;
}

void Game::saveEnemies()
{
	std::ofstream file("enemies.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening enemies file to save" << std::endl;
		return;
	}
	for (int i=1 ; i<m_entities.size() ; i++)
		file << m_entities[i]->getPos().x << " " << m_entities[i]->getPos().y << " " << std::endl;
	file.close();
	std::cout << "Enemies saved" << std::endl;
}
