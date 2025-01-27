#include <imgui.h>
#include "InputManager.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

InputManager::InputManager() :
	m_player(nullptr),
	m_camera(nullptr),
	m_gameMap(nullptr),
	m_ctrl(false),
	m_leftClick(false),
	m_rightClick(false)
{}

InputManager::InputManager(Entity* player, Camera* camera, GameMap* gameMap):
	m_player(player),
	m_camera(camera),
	m_gameMap(gameMap),
	m_ctrl(false),
	m_leftClick(false),
	m_rightClick(false)
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

	if (ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		return;
	if (m_leftClick and m_camera->getFreeCam())
		m_gameMap->addCell(Cell::create(CellType::Wall, m_camera->getMouseMapCoo()));
	else if (m_rightClick and m_camera->getFreeCam())
		m_gameMap->removeCell(m_camera->getMouseMapCoo());
}

void InputManager::processKeyPressed(sf::Keyboard::Key key)
{
	bool freeCam = m_camera->getFreeCam();
    switch (key)
    {
		case sf::Keyboard::Z:
			if (freeCam) m_camera->setDirection(Direction::UP, true);
			break;
		case sf::Keyboard::S:
			if (m_ctrl) m_gameMap->saveMap();
			else if (freeCam) m_camera->setDirection(Direction::DOWN, true);
			break;
        case sf::Keyboard::Q:
			if (freeCam) m_camera->setDirection(Direction::LEFT, true);
			else m_player->setDirection(Direction::LEFT, true);
            break;
        case sf::Keyboard::D:
            if (freeCam) m_camera->setDirection(Direction::RIGHT, true);
			else m_player->setDirection(Direction::RIGHT, true);
            break;
		case sf::Keyboard::E:
			if (m_ctrl) m_camera->enableFreeCam(!freeCam);
			break;
		case sf::Keyboard::Space:
			if (!freeCam) m_player->jump();
			break;
		case sf::Keyboard::LControl:
			m_ctrl = true;
			break;
        default:
            break;
    }
}

void InputManager::processKeyReleased(sf::Keyboard::Key key)
{
	bool freeCam = m_camera->getFreeCam();
	switch (key)
	{
		case sf::Keyboard::Z:
			if(freeCam) m_camera->setDirection(Direction::UP, false);
			break;
		case sf::Keyboard::S:
			if(!m_ctrl and freeCam) m_camera->setDirection(Direction::DOWN, false);
			break;
		case sf::Keyboard::Q:
			if(freeCam) m_camera->setDirection(Direction::LEFT, false);
			else m_player->setDirection(Direction::LEFT, false);
			break;
		case sf::Keyboard::D:
			if(freeCam) m_camera->setDirection(Direction::RIGHT, false);
			else m_player->setDirection(Direction::RIGHT, false);
			break;
		case sf::Keyboard::LControl:
			m_ctrl = false;
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
