#include "InputManager.hpp"
#include "SFML/Graphics.hpp"

InputManager::InputManager() : m_player(nullptr) {}

InputManager::InputManager(Entity* player): m_player(player) {}

void InputManager::setPlayer(Entity* player)
{
	m_player = player;
}

void InputManager::handleInputs()
{
	m_player->setDirections(Direction::NONE);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_player->setDirection(Direction::LEFT, true);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_player->setDirection(Direction::RIGHT, true);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		m_player->jump();
}
