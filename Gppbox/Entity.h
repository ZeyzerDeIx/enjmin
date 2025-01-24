#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Directions.hpp"
#include "GameMap.hpp"


/**
 * @class Entity
 * @brief Represents a game entity with graphical representation and movement functionality.
 */
class Entity
{
public:
    //@brief Constructs a new Entity with default values.
    Entity(std::string spritePath = "", GameMap* gameMap = nullptr);

    //@brief Updates the entity's position based on its movement vector.
    void update(double dt);
	void draw(sf::RenderWindow& win);
    void jump();

    sf::Sprite& getSprite();

    void setPos(float x, float y);
    void setPos(sf::Vector2f pos);
    void setVelocity(float x, float y);
    void setVelocity(sf::Vector2f velocity);
    //Set all directions in one go.
    void setDirections(uint8_t directions);
    //Set one direction on true or false.
    void setDirection(uint8_t direction, bool state);

    float getSpeed();
    sf::Vector2f getVelocity();

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

    // Private methods

    //Update the real position based on m_coo and m_ratio.
    void updatePos();
    //Update the coordinates and the ratio based on m_position.
    void updateCooAndRatio();
    //Update m_sprite position based on m_position.
    void updateSprite();
};