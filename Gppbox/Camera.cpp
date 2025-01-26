#include "Camera.h"

#include "Entity.h"

Camera::Camera(sf::RenderWindow* window) :
	m_window(window),
	m_entity(nullptr),
	m_zoomFactor(0.6f),
	m_freeCam(false),
	m_directions(Direction::NONE),
	m_freeCamSpeed(500.f)
{
	auto size = m_window->getSize();
	m_view = sf::View(sf::Vector2f(size.x * 0.5f, size.y * 0.5f),
		sf::Vector2f(((float)size.x) * m_zoomFactor, ((float)size.y) * m_zoomFactor));
}

void Camera::update(double dt)
{
	if(m_freeCam)
	{
		if (m_directions & Direction::UP) move(0, -m_freeCamSpeed * dt);
		if (m_directions & Direction::DOWN) move(0, m_freeCamSpeed * dt);
		if (m_directions & Direction::LEFT) move(-m_freeCamSpeed * dt, 0);
		if (m_directions & Direction::RIGHT) move(m_freeCamSpeed * dt, 0);
	}
	else if (m_entity)
	{
		m_view.setCenter(m_entity->getPos());
		m_window->setView(m_view);
	}
}

void Camera::attachEntity(Entity* entity)
{
	m_entity = entity;
	m_view.setCenter(m_entity->getPos());
	m_window->setView(m_view);
}

void Camera::enableFreeCam(bool enable) { m_freeCam = enable; }
bool Camera::getFreeCam() const { return m_freeCam; }

sf::Vector2i Camera::getMouseMapCoo() const
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
	sf::Vector2f worldPos = m_window->mapPixelToCoords(mousePos, m_view);
	return sf::Vector2i((int)worldPos.x / CELL_SIZE, (int)worldPos.y / CELL_SIZE);
}

void Camera::move(float offsetX, float offsetY)
{
	m_view.move(offsetX, offsetY);
	m_window->setView(m_view);
}

void Camera::enableAbsolute(bool enable)
{
	m_window->setView(enable ? m_window->getDefaultView() : m_view);
}

void Camera::setDirection(uint8_t direction, bool state)
{
	if (state) m_directions |= direction;
	else m_directions ^= direction;
}
