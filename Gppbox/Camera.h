#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Camera
{
public:
	Camera(sf::RenderWindow * window);
	void update(double dt);
	void attachEntity(Entity* entity);
	void enableFreeCam(bool enable);

	bool getFreeCam() const;
    void move(float offsetX, float offsetY);

	void setDirection(uint8_t direction, bool state);
private:
	sf::RenderWindow* m_window;
	Entity* m_entity;
	float m_zoomFactor;
	bool m_freeCam = false;
	uint8_t m_directions;
	sf::View m_view;
};

