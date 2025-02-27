#pragma once

#include <list>

#include "SFML/Graphics.hpp"
#include "Entity.h"

class Camera;

class Projectile
{
public:
	Projectile(sf::Vector2f position, sf::Vector2f velocity, std::vector<Entity*>& entities, Game* game, std::string spriteLocation = "Bullet.png");
	virtual void update(double dt, GameMap &gameMap, Gun& gun);
	void draw(sf::RenderWindow& win);
	bool collideWith(Entity& entity);

	bool getToDestroy() const;
protected:
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;
	std::vector<Entity*>& m_entities;
	bool m_toDestroy;

	void checkCollision(sf::Vector2f& pos, GameMap& gameMap, Gun& gun, bool isMissile = false);
};

class HomingMissile : public Projectile
{
public:
	HomingMissile(sf::Vector2f position, sf::Vector2f velocity, std::vector<Entity*>& entities, Game* game);
	void update(double dt, GameMap& gameMap, Gun& gun);
	sf::Vector2f acquiresTargetPos();

private:
	sf::Vector2f m_launchTargetPos;
};

class Gun
{
public:
	Gun(Entity* entity, sf::Vector2f offset, std::vector<Entity*>& entities, Camera* camera, Game* game);
	~Gun();
	void update(double dt, GameMap &gameMap);
	void draw(sf::RenderWindow& win);
	void setShoot(bool enable);
	void launchMissile(sf::Vector2f playerPos);
	void amFirinMaLazerBOOOOOOM();
	void im();
	void setOrientation(uint8_t orientation);
	Camera& getCamera();

	sf::RectangleShape& getSprite();
private:
	Entity* m_entity;
	Game* m_game;
	std::vector<Entity*>& m_entities;
	Camera* m_camera;
	sf::RectangleShape m_sprite;
	sf::Sprite m_muzzleFireSprite;
	sf::RectangleShape m_laser;
	sf::Vector2f m_offset;
	std::list<Projectile*> m_projectils;
	bool m_lookAtRight;
	bool m_shootEnabled;
	float m_shootDelay;
	float m_shootTimer;
	float m_missileLaunchDelay;
	float m_missileLaunchTimer;
	float m_laserDuration;
	float m_laserTimer;
	void shoot();
};

