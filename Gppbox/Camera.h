#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Camera
{
public:
	Camera(sf::RenderWindow * window);
	void update(double dt);
	void attachEntity(Entity* entity);
	void im();


	void enableFreeCam(bool enable);

	bool getFreeCam() const;
	sf::Vector2i getMouseMapCoo() const;

    void move(float offsetX, float offsetY);

	/**
	 * @brief Enables or disables the absolute view in order to draw in absolute coordinates.
	 * @param enable True to enable, false to disable.
	 */
	void enableAbsolute(bool enable);

	void setDirection(uint8_t direction, bool state);
private:
	sf::RenderWindow* m_window;
	Entity* m_entity;
	float m_zoomFactor;
	bool m_freeCam = false;
	uint8_t m_directions;
	float m_freeCamSpeed;
	sf::View m_view;
};

