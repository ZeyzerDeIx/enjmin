#include "GameMap.hpp"
#include <iostream>

static int cols = 1280 / CELL_SIZE;
static int lastLine = 720 / CELL_SIZE - 1;

GameMap::GameMap()
{
	for (int i = 0; i < 1280 / C::GRID_SIZE; ++i)
		addCell(Cell::create(CellType::Wall, sf::Vector2i(i, lastLine)));

	addCell(Cell::create(CellType::Wall, sf::Vector2i(0, lastLine - 1)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i(0, lastLine - 2)));
	addCell(Cell::create(CellType::Wall,
		sf::Vector2i(0, lastLine - 3)));

	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols - 1, lastLine - 1)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols - 1, lastLine - 2)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols - 1, lastLine - 3)));

	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 2)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 3)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i(cols >> 2, lastLine - 4)));
	addCell(Cell::create(CellType::Wall, sf::Vector2i((cols >> 2) + 1, lastLine - 4) ));
}

const std::vector<Cell>& GameMap::getCells()
{
	return m_cells;
}

bool GameMap::hasCollision(sf::Vector2i coo)
{
	for(auto& cell : m_cells)
		if (cell.coo == coo and cell.type == CellType::Wall)
			return true;
	return false;
}

bool GameMap::hasCollision(int x, int y)
{
	return hasCollision({ x, y });
}

void GameMap::draw(sf::RenderWindow& win)
{
	for (Cell& cell : m_cells)
		win.draw(cell.sprite);
}

void GameMap::addCell(Cell cell)
{
	for (Cell& c : m_cells) if (c.coo == cell.coo) return;

	m_cells.push_back(cell);
}

void GameMap::removeCell(sf::Vector2i coo)
{
	for (auto it = m_cells.begin(); it != m_cells.end(); ++it)
		if (it->coo == coo) { m_cells.erase(it); break; }
}
