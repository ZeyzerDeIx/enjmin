#include <iostream>
#include <imgui.h>

#include "Gun.hpp"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity):
    m_sprite(10.f),
	m_velocity(velocity),
    m_toDestroy(false)
{
    m_sprite.setPosition(position);
    auto bounds = m_sprite.getGlobalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Projectile::update(double dt, GameMap &gameMap)
{
    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocity.x * dt;
    pos.y += m_velocity.y * dt;
    m_sprite.setPosition(pos);

    if (gameMap.hasCollision(pos.x / CELL_SIZE, pos.y / CELL_SIZE))
        m_toDestroy = true;
}

void Projectile::draw(sf::RenderWindow& win)
{
    win.draw(m_sprite);
}

bool Projectile::getToDestroy()
{
    return m_toDestroy;
}

Gun::Gun(Entity* entity, sf::Vector2f offset):
	m_entity(entity),
    m_sprite({20.f, 8.f}),
    m_offset(offset),
    m_lookAtRight(true)
{
    m_sprite.setOrigin({ m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f });
    m_sprite.setPosition(m_entity->getPos() + m_offset);
}

void Gun::update(double dt, GameMap &gameMap)
{
    m_sprite.setPosition(m_entity->getPos() + m_offset);
    for (auto& projectile : m_projectils)
        projectile.update(dt, gameMap);

    for (auto it = m_projectils.begin(); it != m_projectils.end();)
    {
        if (it->getToDestroy()) it = m_projectils.erase(it);
        else ++it;
    }
}

void Gun::shoot()
{
    sf::Vector2f gunPos = m_sprite.getPosition();
    sf::FloatRect gunBounds = m_sprite.getGlobalBounds();
    sf::Vector2f spawnPos(gunPos.x + gunBounds.width * m_lookAtRight, gunPos.y + gunBounds.height / 2);
    Projectile newProjectile(spawnPos, {600.f * (m_lookAtRight? 1.f : -1.f), 0.f});
    m_projectils.push_back(newProjectile);
}

void Gun::draw(sf::RenderWindow& win)
{
    win.draw(m_sprite);
    for (Projectile& projectil : m_projectils)
        projectil.draw(win);
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
