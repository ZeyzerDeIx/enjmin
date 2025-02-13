#include "Camera.h"

#include "Entity.h"

#include <imgui.h>

Camera::Camera(sf::RenderWindow* window) :
	m_window(window),
	m_entity(nullptr),
	m_zoomFactor(0.6f),
	m_shakeDuration(0.f),
	m_shakeIntensity(0),
	m_editorMode(false),
	m_directions(Direction::NONE),
	m_freeCamSpeed(500.f)
{
	auto size = m_window->getSize();
	m_view = sf::View(sf::Vector2f(size.x * 0.5f, size.y * 0.5f),
		sf::Vector2f(((float)size.x) * m_zoomFactor, ((float)size.y) * m_zoomFactor));
}

void Camera::update(double dt)
{
	if(m_editorMode)
	{
		if (m_directions & Direction::UP) move(0, -m_freeCamSpeed * dt);
		if (m_directions & Direction::DOWN) move(0, m_freeCamSpeed * dt);
		if (m_directions & Direction::LEFT) move(-m_freeCamSpeed * dt, 0);
		if (m_directions & Direction::RIGHT) move(m_freeCamSpeed * dt, 0);
	}
	else if (m_entity)
	{
		m_view.setCenter(m_entity->getPos());
		if(m_shakeDuration > 0.f) move(rand() % m_shakeIntensity, rand() % m_shakeIntensity);
		m_window->setView(m_view);
	}
	m_shakeDuration -= dt;
}

void Camera::attachEntity(Entity* entity)
{
	m_entity = entity;
	m_view.setCenter(m_entity->getPos());
	m_window->setView(m_view);
}

void Camera::im()
{
	using namespace ImGui;

	if(CollapsingHeader("Camera"))
	{
		Checkbox("Editor mode", &m_editorMode);
		if (m_editorMode)
		{
			DragFloat("Free cam speed", &m_freeCamSpeed, 10.f, 10.f, 1000.f);

			std::string dir = "Directions: ";
			if (m_directions & Direction::UP) dir += "UP ";
			if (m_directions & Direction::DOWN) dir += "DOWN ";
			if (m_directions & Direction::LEFT) dir += "LEFT ";
			if (m_directions & Direction::RIGHT) dir += "RIGHT ";
			Text(dir.c_str());
		}
	}

	if (CollapsingHeader("Mouse"))
	{
		sf::Vector2i mouseCoo = getMouseMapCoo();
		Text("Mouse Map Coordinates: (%d, %d)", mouseCoo.x, mouseCoo.y);
	}
}

void Camera::setEditorMode(bool enable) { m_editorMode = enable; }
bool Camera::getEditorMode() const { return m_editorMode; }

sf::Vector2i Camera::getMouseMapCoo() const
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
	sf::Vector2f worldPos = m_window->mapPixelToCoords(mousePos, m_view);
	//here the minus one is required because the division is floored
	return {
		((int)worldPos.x / CELL_SIZE) - (worldPos.x < 0),
		((int)worldPos.y / CELL_SIZE) - (worldPos.y < 0)
	};
}

sf::Vector2f Camera::getMouseMapPos() const
{
	return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);
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

void Camera::triggerScreenShake(int intensity, float duration)
{
	m_shakeIntensity = intensity;
	m_shakeDuration = duration;
}
