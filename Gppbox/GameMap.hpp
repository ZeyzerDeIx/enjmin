#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "C.hpp"

constexpr int CELL_SIZE = 32;

/**
 * @class CellType
 * @brief An enum representing all cell types.
 */
enum class CellType
{
	Void,
	Wall
};

/**
 * @struct Cell
 * @brief A struct representing a cell.
 */
struct Cell
{
	CellType type;
	sf::Vector2i coo;
};

/**
 * @class GameMap
 * @brief A class representing the game map.
 */
class GameMap
{
public:
	GameMap();
	const std::vector<Cell>& getCells();
	/**
	 * @brief Checks if a cell has a collision.
	 * @param coo The cell coordinates.
	 * @return True if the cell has a collision, false otherwise.
	 */
	bool hasCollision(sf::Vector2i coo);
	/**
	 * @brief Checks if a cell has a collision.
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return True if the cell has a collision, false otherwise.
	 */
	bool hasCollision(int x, int y);

	void draw(sf::RenderWindow& win);

private:
	std::vector<Cell> m_cells;

	std::vector<sf::RectangleShape> wallSprites;
};

