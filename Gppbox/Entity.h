#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Directions.hpp"
#include "GameMap.hpp"

class Gun;
class Camera;


/**
 * @class Entity
 * @brief Represents a game entity with graphical representation and movement functionality.
 */
class Entity
{
public:
    //Constructs a new Entity with default values.
    Entity(sf::Sprite sprite, GameMap* gameMap = nullptr, sf::Color color = sf::Color::White);
    ~Entity();

    //Updates the entity's position based on its movement vector.
    void update(double dt);
	void draw(sf::RenderWindow& win);
    void jump();

    void addGun(std::vector<Entity*>& entities, Camera* camera);
    Gun* getGun();
    void deleteGun();
    void shoot(bool enable = true);

    void onHit(float direction);

    void setPos(float x, float y);
    void setPos(sf::Vector2f pos);
    void setVelocity(float x, float y);
    void setVelocity(sf::Vector2f velocity);
    //Set all directions in one go.
    void setDirections(uint8_t directions);
    //Set one direction on true or false.
    void setDirection(uint8_t direction, bool state);
    void setMustache(bool enable);

    float getSpeed();
    const sf::Vector2f& getVelocity();
	const sf::Vector2f& getPos();
	bool getDirection(uint8_t direction);
    bool isDead();
    sf::Sprite& getSprite();

	/**
	 * @brief Checks if the entity is on the ground.
	 * @return True if the entity is on the ground, false otherwise.
	 */
	bool isOnGround();

    bool im();

private:
    //The graphical representation of the entity.
    sf::Sprite m_sprite;

	GameMap* m_gameMap;

    //The grid cell coordinates of the entity.
    sf::Vector2i m_coo;
    //The offset ratio of the entity inside it's current cell.
    sf::Vector2f m_ratio;
    //The real pixel position of the entity.
    sf::Vector2f m_pos;
    //The velocity of the entity.
    sf::Vector2f m_velocity;
    //Acceleration of the entity.
    sf::Vector2f m_acceleration;

    //The maximum entity speed in pixel per second.
    float m_maxSpeed;
	//The force applied to the entity when jumping.
    float m_jumpForce;
    //Braking entity speed in pixel per second.
    float m_brakingSpeed;

    //Directions of the entity.
    uint8_t m_directions;

	//Is the entity jumping.
	bool m_isJumping;
    bool m_hasAMustache;
    bool m_didCollide;

    //Heal points
    int m_hp;

    Gun* m_gun;

    // Private methods

    //Update the real position based on m_coo and m_ratio.
    void updatePos();
    //Update the coordinates and the ratio based on m_position.
    void updateCooAndRatio();
    //Update m_sprite position based on m_position.
    void updateSprite();
    //Process mustache IA behaviour.
    void doMustacheThing();

    void applyRecoil(float direction);

    friend Gun;
};