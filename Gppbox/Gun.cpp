#include <iostream>
#include <imgui.h>

#include "Gun.hpp"
#include "Camera.h"
#include "Game.hpp"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, std::vector<Entity*>& entities, Game* game):
    m_sprite(game->createSprite("Bullet.png")),
	m_velocity(velocity),
    m_entities(entities),
    m_toDestroy(false)
{
    auto bounds = m_sprite.getGlobalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(position);
	m_sprite.setRotation(velocity.x >= 0.f ? 0.f : 180.f);
}

void Projectile::update(double dt, GameMap &gameMap, Gun& gun)
{
    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocity.x * dt;
    pos.y += m_velocity.y * dt;
    m_sprite.setPosition(pos);

    if (gameMap.collide(m_sprite.getGlobalBounds()) or abs(gun.getSprite().getPosition().x - pos.x) > 600)
        m_toDestroy = true;

    for (auto& entity : m_entities)
        if (collideWith(*entity) and !entity->isPlayer() and (m_toDestroy = true))
            entity->onHit(m_velocity.x >= 0.f ? 1.f : -1.f);
}

void Projectile::draw(sf::RenderWindow& win)
{
    win.draw(m_sprite);
}

bool Projectile::collideWith(Entity& entity)
{
    auto bounds = m_sprite.getGlobalBounds();
    auto entityBounds = entity.getSprite().getGlobalBounds();
    return bounds.intersects(entityBounds);
}

bool Projectile::getToDestroy() const
{
    return m_toDestroy;
}

Gun::Gun(Entity* entity, sf::Vector2f offset, std::vector<Entity*>& entities, Camera* camera, Game* game) :
    m_entity(entity),
	m_game(game),
    m_entities(entities),
    m_camera(camera),
    m_sprite({ 20.f, 8.f }),
    m_muzzleFireSprite(game->createSprite("muzzleFire.png")),
    m_offset(offset),
    m_lookAtRight(true),
    m_shootEnabled(false),
    m_shootDelay(0.083f),
    m_shootTimer(0.f)
{
    m_sprite.setOrigin({ m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f });
    m_sprite.setPosition(m_entity->getPos() + m_offset);
    m_muzzleFireSprite.setOrigin({0.f , m_muzzleFireSprite.getGlobalBounds().height / 2.f });
    m_muzzleFireSprite.setScale({ 0.1f,0.1f });
}

Gun::~Gun()
{
	for (Projectile* projectile : m_projectils)
		delete projectile;
}

void Gun::update(double dt, GameMap &gameMap)
{
    for (auto it = m_projectils.begin(); it != m_projectils.end();)
    {
        if ((*it)->getToDestroy())
        {
            delete *it;
            it = m_projectils.erase(it);
        }
        else ++it;
    }

    m_sprite.setPosition(m_entity->getPos() + m_offset);
    for (Projectile* projectile : m_projectils)
        projectile->update(dt, gameMap, *this);

    if (m_shootEnabled and (m_shootTimer -= dt) <= 0.f)
        shoot();
}

void Gun::shoot()
{
    sf::Vector2f gunPos = m_sprite.getPosition();
    sf::FloatRect gunBounds = m_sprite.getGlobalBounds();
    sf::Vector2f spawnPos(gunPos.x + gunBounds.width/2 * (m_lookAtRight ? 1.f : -1.f), gunPos.y);
    Projectile* newProjectile = new Projectile(spawnPos, {1200.f * (m_lookAtRight? 1.f : -1.f), 0.f}, m_entities, m_game);
    m_projectils.push_back(newProjectile);
    m_shootTimer = m_shootDelay;
    m_camera->triggerScreenShake(4, 0.2f);
    m_entity->applyRecoil(m_lookAtRight ? -1.f : 1.f);
}

void Gun::draw(sf::RenderWindow& win)
{
    win.draw(m_sprite);
    for (Projectile* projectil : m_projectils)
        projectil->draw(win);
    if (m_shootTimer <= 0.05f and m_shootTimer != 0.f)
    {
        sf::Vector2f gunPos = m_sprite.getPosition();
        sf::FloatRect gunBounds = m_sprite.getGlobalBounds();
        sf::Vector2f muzzlePos(gunPos.x + gunBounds.width / 2 * (m_lookAtRight ? 1.f : -1.f), gunPos.y);

        m_muzzleFireSprite.setPosition(muzzlePos);
        m_muzzleFireSprite.setRotation(m_lookAtRight ? 0.f : 180.f);

        win.draw(m_muzzleFireSprite);
    }
}

void Gun::setShoot(bool enable)
{
    if (m_shootEnabled = enable) m_shootTimer = 0.f;
    else m_shootTimer = m_shootDelay;
}

void Gun::launchMissile(sf::Vector2f playerPos)
{
    sf::Vector2f spawnPos(playerPos.x, playerPos.y - 60.f);
	m_projectils.push_back(new HomingMissile(spawnPos, { 200.f, 0.f }, m_entities, m_game));
}

void Gun::im()
{
    using namespace ImGui;
    if(CollapsingHeader("Gun"))
    {
        Value("PosX: ", m_sprite.getPosition().x);
        Value("PosY: ", m_sprite.getPosition().y);
        ImGui::Text("There is %i projectiles.", m_projectils.size());
        DragFloat2("Offset", &m_offset.x);
    }
}

void Gun::setOrientation(uint8_t orientation)
{
    if ((orientation & Direction::LEFT and m_lookAtRight) or
        (orientation & Direction::RIGHT and !m_lookAtRight))
    {
        m_offset.x = -m_offset.x;
        m_lookAtRight = !m_lookAtRight;
    }
}

sf::RectangleShape& Gun::getSprite()
{
    return m_sprite;
}

void HomingMissile::update(double dt, GameMap& gameMap, Gun& gun)
{
	sf::Vector2f pos = m_sprite.getPosition();
	sf::Vector2f targetPos = acquiresTargetPos();
	sf::Vector2f direction = targetPos - pos;
	float norm = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction.x /= norm;
	direction.y /= norm;
	pos.x += direction.x * m_velocity.x * dt;
	pos.y += direction.y * m_velocity.x * dt;
	m_sprite.setPosition(pos);
    m_sprite.setRotation(atan2(direction.y, direction.x) * 180.f / 3.14159265f);
	if (gameMap.collide(m_sprite.getGlobalBounds()) or abs(gun.getSprite().getPosition().x - pos.x) > 600)
		m_toDestroy = true;
	for (auto& entity : m_entities)
		if (collideWith(*entity) and !entity->isPlayer() and (m_toDestroy = true))
			entity->onHit(m_velocity.x >= 0.f ? 1.f : -1.f);
}

sf::Vector2f HomingMissile::acquiresTargetPos()
{
   sf::Vector2f currentPos = m_sprite.getPosition();
   sf::Vector2f closestPos;
   float minDistance = std::numeric_limits<float>::max();

   for (auto& entity : m_entities)
   {
       if (entity != nullptr && entity != m_entities[0])
       {
           sf::Vector2f entityPos = entity->getPos();
           float distance = std::sqrt(std::pow(entityPos.x - currentPos.x, 2) + std::pow(entityPos.y - currentPos.y, 2));

           if (distance < minDistance)
           {
               minDistance = distance;
               closestPos = entityPos;
           }
       }
   }
   if (closestPos == sf::Vector2f(0, 0))
	   m_toDestroy = true;

   return closestPos;
}
