#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "C.hpp"

constexpr int CELL_SIZE = 32;
constexpr int WALL_COLOR = 0x07ff07ff;

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
	sf::RectangleShape sprite;

	static Cell create(CellType type, sf::Vector2i coo)
	{
		Cell cell;
		cell.type = type;
		cell.coo = coo;
		cell.sprite.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
		cell.sprite.setPosition(coo.x * CELL_SIZE, coo.y * CELL_SIZE);
		if(type == CellType::Wall)
			cell.sprite.setFillColor(sf::Color(WALL_COLOR));
		return cell;
	}
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

	void addCell(Cell cell);
	void removeCell(sf::Vector2i coo);

private:
	std::vector<Cell> m_cells;
	std::vector<sf::RectangleShape> wallSprites;
};

