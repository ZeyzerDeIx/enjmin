#include <iostream>
#include <imgui.h>
#include "InputManager.hpp"
#include "SFML/Graphics.hpp"
#include "Gun.hpp"
#include "Game.hpp"

InputManager::InputManager() :
	m_player(nullptr),
	m_camera(nullptr),
	m_gameMap(nullptr),
	m_game(nullptr),
	m_ctrl(false),
	m_leftClick(false),
	m_rightClick(false),
	m_rightTrigger(false),
	m_joystickDirections(0)
{}

InputManager::InputManager(Entity* player, Camera* camera, GameMap* gameMap):
	m_player(player),
	m_camera(camera),
	m_gameMap(gameMap),
	m_game(nullptr),
	m_ctrl(false),
	m_leftClick(false),
	m_rightClick(false),
	m_rightTrigger(false),
	m_joystickDirections(0)
{}

void InputManager::setPlayer(Entity* player)
{
	m_player = player;
}

void InputManager::setCamera(Camera* camera)
{
	m_camera = camera;
}

void InputManager::setGameMap(GameMap* gameMap)
{
	m_gameMap = gameMap;
}

void InputManager::setGame(Game* game)
{
	m_game = game;
}

void InputManager::handleInputs(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
		processKeyPressed(event.key.code);
	else if (event.type == sf::Event::KeyReleased)
		processKeyReleased(event.key.code);
	else if (event.type == sf::Event::MouseButtonPressed or
			event.type == sf::Event::MouseButtonReleased)
	{
		m_leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		m_rightClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	}

	if (ImGui::GetIO().WantCaptureMouse) return;
	if (m_leftClick and m_camera->getEditorMode())
	{
		if (m_ctrl) m_game->spawnEnemy(m_camera->getMouseMapPos());
		else m_gameMap->addCell(Cell::create(CellType::Wall, m_camera->getMouseMapCoo()));
	}
	else if (m_rightClick and m_camera->getEditorMode())
	{
		if (m_ctrl) m_game->deleteEnemy(m_camera->getMouseMapPos());
		else m_gameMap->removeCell(m_camera->getMouseMapCoo());
	}
}

void InputManager::handleJoystick()
{
	if (sf::Joystick::isConnected(0))
		processJoystick();
}

void InputManager::processJoystick()
{
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 10 and
		!(m_player->getDirection(Direction::RIGHT)))
		m_player->setDirection(Direction::RIGHT, true);
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -10 and
		!(m_player->getDirection(Direction::LEFT)))
		m_player->setDirection(Direction::LEFT, true);
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < 10 and
			sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > -10)
	{
		if (m_player->getDirection(Direction::RIGHT) and
			!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			m_player->setDirection(Direction::RIGHT, false);
		else if (m_player->getDirection(Direction::LEFT) and
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			m_player->setDirection(Direction::LEFT, false);
	}

	if (sf::Joystick::isButtonPressed(0, 0))
		m_player->jump();

	if (m_rightTrigger and sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) >= -10)
	{
		m_rightTrigger = false;
		m_player->shoot(false);
	}
	if (!m_rightTrigger and sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) < -10)
	{
		m_rightTrigger = true;
		m_player->shoot();
	}
}

void InputManager::processKeyPressed(sf::Keyboard::Key key)
{
	bool editorMode = m_camera->getEditorMode();
    switch (key)
    {
		case sf::Keyboard::Z:
			if (editorMode) m_camera->setDirection(Direction::UP, true);
			break;
		case sf::Keyboard::S:
			if (m_ctrl)
			{
				m_game->saveEnemies();
				m_gameMap->saveMap();
			}
			else if (editorMode) m_camera->setDirection(Direction::DOWN, true);
			break;
        case sf::Keyboard::Q:
			if (editorMode) m_camera->setDirection(Direction::LEFT, true);
			else m_player->setDirection(Direction::LEFT, true);
            break;
        case sf::Keyboard::D:
            if (editorMode) m_camera->setDirection(Direction::RIGHT, true);
			else m_player->setDirection(Direction::RIGHT, true);
            break;
		case sf::Keyboard::E:
			if (m_ctrl) m_camera->setEditorMode(!editorMode);
			break;
		case sf::Keyboard::Space:
			if (!editorMode) m_player->jump();
			break;
		case sf::Keyboard::LControl:
			m_ctrl = true;
			break;
		case sf::Keyboard::LShift:
			m_player->shoot();
			break;
        default:
            break;
    }
}

void InputManager::processKeyReleased(sf::Keyboard::Key key)
{
	bool editorMode = m_camera->getEditorMode();
	switch (key)
	{
		case sf::Keyboard::Z:
			if(editorMode) m_camera->setDirection(Direction::UP, false);
			break;
		case sf::Keyboard::S:
			if(!m_ctrl and editorMode) m_camera->setDirection(Direction::DOWN, false);
			break;
		case sf::Keyboard::Q:
			if(editorMode) m_camera->setDirection(Direction::LEFT, false);
			else m_player->setDirection(Direction::LEFT, false);
			break;
		case sf::Keyboard::D:
			if(editorMode) m_camera->setDirection(Direction::RIGHT, false);
			else m_player->setDirection(Direction::RIGHT, false);
			break;
		case sf::Keyboard::LControl:
			m_ctrl = false;
			break;
		case sf::Keyboard::LShift:
			m_player->shoot(false);
			break;
		default:
			break;
	}
}

void InputManager::im()
{
	using namespace ImGui;
	Value("Ctrl", m_ctrl);
	Value("Left Click", m_leftClick);
	Value("Right Click", m_rightClick);
}
