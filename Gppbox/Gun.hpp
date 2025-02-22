#pragma once

#include <list>

#include "SFML/Graphics.hpp"
#include "Entity.h"

class Camera;

class Projectile
{
public:
	Projectile(sf::Vector2f position, sf::Vector2f velocity, std::vector<Entity*>& entities);
	void update(double dt, GameMap &gameMap, Gun& gun);
	void draw(sf::RenderWindow& win);
	bool collideWith(Entity& entity);

	bool getToDestroy();
private:
	sf::CircleShape m_sprite;
	sf::Vector2f m_velocity;
	std::vector<Entity*>& m_entities;
	bool m_toDestroy;
};

class Gun
{
public:
	Gun(Entity* entity, sf::Vector2f offset, std::vector<Entity*>& entities, Camera* camera, sf::Sprite muzzleFireSprite);
	void update(double dt, GameMap &gameMap);
	void draw(sf::RenderWindow& win);
	void setShoot(bool enable);
	void im();
	void setOrientation(uint8_t orientation);

	sf::RectangleShape& getSprite();
private:
	Entity* m_entity;
	std::vector<Entity*>& m_entities;
	Camera* m_camera;
	sf::RectangleShape m_sprite;
	sf::Sprite m_muzzleFireSprite;
	sf::Vector2f m_offset;
	std::list<Projectile> m_projectils;
	bool m_lookAtRight;
	bool m_shootEnabled;
	float m_shootDelay;
	float m_shootTimer;
	void shoot();
};

