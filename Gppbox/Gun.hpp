#pragma once

#include <list>

#include "SFML/Graphics.hpp"
#include "Entity.h"

class Projectile
{
public:
	Projectile(sf::Vector2f position, sf::Vector2f velocity);
	void update(double dt, GameMap &gameMap);
	void draw(sf::RenderWindow& win);

	bool getToDestroy();
private:
	sf::CircleShape m_sprite;
	sf::Vector2f m_velocity;
	bool m_toDestroy;
};

class Gun
{
public:
	Gun(Entity* entity, sf::Vector2f offset);
	void update(double dt, GameMap &gameMap);
	void shoot();
	void draw(sf::RenderWindow& win);
	void im();
	void setOrientation(uint8_t orientation);

	sf::RectangleShape& getSprite();
private:
	Entity* m_entity;
	sf::RectangleShape m_sprite;
	sf::Vector2f m_offset;
	std::list<Projectile> m_projectils;
	bool m_lookAtRight;
};

