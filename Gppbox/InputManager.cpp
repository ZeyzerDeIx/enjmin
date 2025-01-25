#include "InputManager.hpp"
#include "SFML/Graphics.hpp"

InputManager::InputManager() : m_player(nullptr), m_camera(nullptr) {}

InputManager::InputManager(Entity* player, Camera* camera):
	m_player(player),
	m_camera(camera)
{}

void InputManager::setPlayer(Entity* player)
{
	m_player = player;
}

void InputManager::setCamera(Camera* camera)
{
	m_camera = camera;
}

void InputManager::handleInputs(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
        m_camera->getFreeCam() ? processKeyPressedFreeCam(event.key.code) : processKeyPressed(event.key.code);
	else if (event.type == sf::Event::KeyReleased)
		m_camera->getFreeCam() ? processKeyReleasedFreeCam(event.key.code) : processKeyReleased(event.key.code);
}

void InputManager::processKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Q:
            m_player->setDirection(Direction::LEFT, true);
            break;
        case sf::Keyboard::D:
            m_player->setDirection(Direction::RIGHT, true);
            break;
		case sf::Keyboard::Space:
			m_player->jump();
			break;
        default:
            break;
    }
}

void InputManager::processKeyPressedFreeCam(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Z:
		m_camera->setDirection(Direction::UP, true);
		break;
	case sf::Keyboard::S:
		m_camera->setDirection(Direction::DOWN, true);
		break;
	case sf::Keyboard::Q:
		m_camera->setDirection(Direction::LEFT, true);
		break;
	case sf::Keyboard::D:
		m_camera->setDirection(Direction::RIGHT, true);
		break;
	default:
		break;
	}
}

void InputManager::processKeyReleased(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Q:
		m_player->setDirection(Direction::LEFT, false);
		break;
	case sf::Keyboard::D:
		m_player->setDirection(Direction::RIGHT, false);
		break;
	default:
		break;
	}
}

void InputManager::processKeyReleasedFreeCam(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Z:
		m_camera->setDirection(Direction::UP, false);
		break;
	case sf::Keyboard::S:
		m_camera->setDirection(Direction::DOWN, false);
		break;
	case sf::Keyboard::Q:
		m_camera->setDirection(Direction::LEFT, false);
		break;
	case sf::Keyboard::D:
		m_camera->setDirection(Direction::RIGHT, false);
		break;
	default:
		break;
	}
}