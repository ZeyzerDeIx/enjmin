#include <iostream>
#include <imgui.h>

#include "Gun.hpp"
#include "Camera.h"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, std::vector<Entity*>& entities):
    m_sprite(7.f),
	m_velocity(velocity),
    m_entities(entities),
    m_toDestroy(false)
{
    auto bounds = m_sprite.getGlobalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    m_sprite.setPosition(position);
}

void Projectile::update(double dt, GameMap &gameMap, Gun& gun)
{
    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocity.x * dt;
    pos.y += m_velocity.y * dt;
    m_sprite.setPosition(pos);

    float sign = m_velocity.x > 0 ? 1.f : -1.f;
    sf::Vector2i coo{
        (int)pos.x / CELL_SIZE - (pos.x < 0),
        (int)pos.y / CELL_SIZE - (pos.x < 0)
    };
    sf::Vector2i caseToCehck{ coo.x + (int)sign, coo.y };
    if (gameMap.collide(m_sprite.getGlobalBounds()) or abs(gun.getSprite().getPosition().x - pos.x) > 600)
        m_toDestroy = true;

    for (auto& entity : m_entities)
        if (collideWith(*entity) and (m_toDestroy = true))
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

bool Projectile::getToDestroy()
{
    return m_toDestroy;
}

Gun::Gun(Entity* entity, sf::Vector2f offset, std::vector<Entity*>& entities, Camera* camera) :
    m_entity(entity),
    m_entities(entities),
    m_camera(camera),
    m_sprite({ 20.f, 8.f }),
    m_offset(offset),
    m_lookAtRight(true),
    m_shootEnabled(false),
    m_shootDelay(0.083f),
    m_shootTimer(0.f)
{
    m_sprite.setOrigin({ m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f });
    m_sprite.setPosition(m_entity->getPos() + m_offset);
}

void Gun::update(double dt, GameMap &gameMap)
{
    for (auto it = m_projectils.begin(); it != m_projectils.end();)
    {
        if (it->getToDestroy()) it = m_projectils.erase(it);
        else ++it;
    }

    m_sprite.setPosition(m_entity->getPos() + m_offset);
    for (auto& projectile : m_projectils)
        projectile.update(dt, gameMap, *this);

    if (m_shootEnabled and (m_shootTimer -= dt) <= 0.f)
        shoot();
}

void Gun::shoot()
{
    sf::Vector2f gunPos = m_sprite.getPosition();
    sf::FloatRect gunBounds = m_sprite.getGlobalBounds();
    sf::Vector2f spawnPos(gunPos.x + gunBounds.width/2 * (m_lookAtRight ? 1.f : -1.f), gunPos.y);
    Projectile newProjectile(spawnPos, {600.f * (m_lookAtRight? 1.f : -1.f), 0.f}, m_entities);
    m_projectils.push_back(newProjectile);
    m_shootTimer = m_shootDelay;
    m_camera->triggerScreenShake(4, 0.2f);
    m_entity->applyRecoil(m_lookAtRight ? -1.f : 1.f);
}

void Gun::draw(sf::RenderWindow& win)
{
    win.draw(m_sprite);
    for (Projectile& projectil : m_projectils)
        projectil.draw(win);
}

void Gun::setShoot(bool enable)
{
    if (m_shootEnabled = enable) m_shootTimer = 0.f;
}

void Gun::im()
{
    using namespace ImGui;
    if(CollapsingHeader("Gun"))
    {
        Value("PosX: ", m_sprite.getPosition().x);
        Value("PosY: ", m_sprite.getPosition().y);
        Text("There is %i projectiles.", m_projectils.size());
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
