#include <imgui.h>
#include "Entity.h"
#include "TextureManager.hpp"
#include "Gun.hpp"


constexpr float RATIO_THRESHOLD_X = 0.7f;
constexpr float RATIO_THRESHOLD_Y = 0.5f;


Entity::Entity(sf::Sprite sprite, GameMap* gameMap, sf::Color color) :
    m_sprite(sprite),
    m_gameMap(gameMap),
    m_coo(0, 0),
    m_ratio(0.f, 0.f),
    m_pos(0, 0),
    m_velocity(0.f, 0.f),
    m_acceleration(5000.f, 6000.f),
    m_maxSpeed(500.f),
    m_jumpForce(1400.f),
    m_brakingSpeed(10000.f),
    m_directions(0b00000000),
    m_hp(3),
    m_isJumping(false),
    m_hasAMustache(false),
    m_didCollide(true),
    m_gun(nullptr)
{
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height - CELL_SIZE * (1.f - RATIO_THRESHOLD_Y));
	updatePos();
    m_sprite.setColor(color);
}

Entity::~Entity()
{
    deleteGun();
}

void Entity::update(double dt)
{
    if (m_hasAMustache)
        doMustacheThing();

    //acceleration
    if (m_directions & Direction::LEFT)
        m_velocity.x -= m_acceleration.x * dt;
	if (m_velocity.x < -m_maxSpeed) m_velocity.x = -m_maxSpeed;

    if (m_directions & Direction::RIGHT)
        m_velocity.x += m_acceleration.x * dt;
	if (m_velocity.x > m_maxSpeed) m_velocity.x = m_maxSpeed;

	if(!isOnGround()) m_velocity.y += m_acceleration.y * dt;
	else if(!m_isJumping) m_velocity.y = 0.f;

    if (m_gun) m_gun->setOrientation(m_directions);

    //movement x
    float sign = m_velocity.x >= 0 ? 1.f : -1.f;
	for (float i = std::abs(m_velocity.x * dt); i > 0.f ; i -= CELL_SIZE)
	{
		updateCooAndRatio();
        if (m_gameMap->hasCollision(m_coo.x + sign, m_coo.y))
        {
            m_ratio.x = sign == 1.f ? RATIO_THRESHOLD_X : 1.f - RATIO_THRESHOLD_X;
            m_didCollide = true;
			updatePos();
			break;
        }
        m_pos.x += sign * (CELL_SIZE < i ? CELL_SIZE : i);
	}
	//movement y
	sign = m_velocity.y >= 0 ? 1.f : -1.f;
	if (m_isJumping and sign == 1.f) m_isJumping = false;
	for (float i = std::abs(m_velocity.y * dt); i > 0.f; i -= CELL_SIZE)
	{
		updateCooAndRatio();
		if (m_gameMap->hasCollision(m_coo.x, m_coo.y + sign))
		{
			m_ratio.y = sign == 1.f ? RATIO_THRESHOLD_Y : 1.f - RATIO_THRESHOLD_Y;
            m_didCollide = true;
			updatePos();
			break;
		}
		m_pos.y += sign * (CELL_SIZE < i ? CELL_SIZE : i);
	}
    
    //braking
    if ((!(m_directions & Direction::RIGHT) or m_directions & Direction::LEFT) and m_velocity.x > 0.f)
    {
        m_velocity.x -= m_brakingSpeed * dt;
        if (m_velocity.x < 0.f) m_velocity.x = 0.f;
    }
    if ((!(m_directions & Direction::LEFT) or m_directions & Direction::RIGHT) and m_velocity.x < 0.f)
    {
        m_velocity.x += m_brakingSpeed * dt;
        if (m_velocity.x > 0.f) m_velocity.x = 0.f;
    }

	
	updateSprite();

    if (m_gun) m_gun->update(dt, *m_gameMap);
}

void Entity::draw(sf::RenderWindow& win)
{
	win.draw(m_sprite);
    if (m_gun) m_gun->draw(win);
}

void Entity::jump()
{
	if (isOnGround()) 
    {
        m_velocity.y = -m_jumpForce;
		m_isJumping = true;
    }
}

void Entity::addGun(std::vector<Entity*>& entities, Camera* camera)
{
    if (m_gun == nullptr)
        m_gun = new Gun(this, {8.f, -8.f}, entities, camera);
}

Gun* Entity::getGun()
{
    return m_gun;
}

void Entity::deleteGun()
{
    if(m_gun) delete m_gun;
}

void Entity::shoot(bool enable)
{
    if (m_gun) m_gun->setShoot(enable);
}

void Entity::onHit(float direction)
{
    m_hp--;
    applyRecoil(direction);
}

sf::Sprite& Entity::getSprite()
{
    return m_sprite;
}

void Entity::setPos(float x, float y)
{
    m_pos = { x,y };
    updateSprite();
}

void Entity::setPos(sf::Vector2f pos)
{
    m_pos = pos;
}

void Entity::setVelocity(float x, float y)
{
    m_velocity = { x,y };
}

void Entity::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::setDirections(uint8_t directions)
{
    m_directions = directions;
}

void Entity::setDirection(uint8_t direction, bool state)
{
    if (state) m_directions |= direction;
    else m_directions ^= direction;
}

void Entity::setMustache(bool enable)
{
    m_hasAMustache = enable;
}

float Entity::getSpeed()
{
    return m_maxSpeed;
}

const sf::Vector2f& Entity::getVelocity()
{
    return m_velocity;
}

const sf::Vector2f& Entity::getPos()
{
	return m_pos;
}

const sf::Vector2i& Entity::getCoo()
{
    return m_coo;
}

bool Entity::getDirection(uint8_t direction)
{
	return m_directions & direction;
}

bool Entity::isDead()
{
    return m_hp <= 0;
}

bool Entity::isOnGround()
{
    return m_gameMap->hasCollision(m_coo.x, m_coo.y + 1);
}

bool Entity::im()
{
    using namespace ImGui;
    bool cooChanged = false, posChanged = false;

    if(CollapsingHeader("Player", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
    {
        cooChanged |= DragInt2("Coo", &m_coo.x, 1, -100, 100);
        cooChanged |= DragFloat2("Ratio", &m_ratio.x, 0.0f, 0, 1.0f);

        if (cooChanged) updatePos();

        posChanged |= DragFloat2("Pos", &m_pos.x, 1.0f, -2000.f, 2000.f);

        std::string dir = "Directions: ";
        if (m_directions & Direction::UP) dir += "UP ";
        if (m_directions & Direction::DOWN) dir += "DOWN ";
        if (m_directions & Direction::LEFT) dir += "LEFT ";
        if (m_directions & Direction::RIGHT) dir += "RIGHT ";
        Text(dir.c_str());

		std::string states = "States: ";
		if (m_isJumping) states += "JUMP ";
		if (isOnGround()) states += "OnGround ";
		Text(states.c_str());

        if (posChanged) updateCooAndRatio();

        DragFloat2("Velocity", &m_velocity.x, 5.f, -m_maxSpeed, m_maxSpeed);

        if (m_gun) m_gun->im();
    }

    return cooChanged || posChanged;
}


//---Private methods---

void Entity::updatePos()
{
	bool xN = m_pos.x < 0, yN = m_pos.y < 0; // if x or y is negative
	auto oneC = [](float x) { return 1.f - x; }; // one's complement
	// xN, yN and oneC are needed to handle negative values
    setPos(
        ((float)(m_coo.x + xN) + (xN ? -oneC(m_ratio.x) : m_ratio.x)) * CELL_SIZE,
		((float)(m_coo.y + yN) + (yN ? -oneC(m_ratio.y) : m_ratio.y)) * CELL_SIZE
    );
}

void Entity::updateCooAndRatio()
{
    sf::Vector2f absPos{std::abs(m_pos.x), std::abs(m_pos.y)};
	sf::Vector2i signs{ m_pos.x < 0 ? -1 : 1, m_pos.y < 0 ? -1 : 1 };
    m_coo = {
        ((int)m_pos.x / CELL_SIZE) - (m_pos.x < 0),
        ((int)m_pos.y / CELL_SIZE) - (m_pos.y < 0)
    };
    m_ratio = {
        ((float)((int)(m_pos.x - (m_pos.x < 0)* CELL_SIZE) % CELL_SIZE) / (float)CELL_SIZE),
        ((float)((int)(m_pos.y - (m_pos.y < 0)* CELL_SIZE) % CELL_SIZE) / (float)CELL_SIZE)
    };

    if (m_pos.x < 0) m_ratio.x = 1.f + m_ratio.x;
    if (m_pos.y < 0) m_ratio.y = 1.f + m_ratio.y;
}

void Entity::updateSprite()
{
    m_sprite.setPosition(m_pos.x, m_pos.y);
}

void Entity::doMustacheThing()
{
    if (!isOnGround()) return;
    if (m_didCollide)
    {
        if (m_directions == Direction::NONE or m_directions & Direction::RIGHT)
            setDirections(Direction::LEFT);
        else if (m_directions & Direction::LEFT)
            setDirections(Direction::RIGHT);
        m_didCollide = false;
    }
}

void Entity::applyRecoil(float direction)
{
    m_velocity.x = m_maxSpeed * direction;
}
