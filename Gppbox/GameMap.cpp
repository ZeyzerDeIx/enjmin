#include "GameMap.hpp"
#include <iostream>
#include <fstream>

GameMap::GameMap()
{
	loadMap();
}

const std::vector<Cell>& GameMap::getCells()
{
	return m_cells;
}

const Cell* GameMap::getCell(sf::Vector2i coo)
{
	for(auto& cell : m_cells)
		if (cell.coo == coo and cell.type == CellType::Wall)
			return &cell;
	return nullptr;
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

bool GameMap::collide(sf::FloatRect bounds)
{
	for (auto& cell : m_cells)
	{
		sf::FloatRect cellBounds = cell.sprite.getGlobalBounds();
		if (std::abs(bounds.top - cellBounds.top) > CELL_SIZE * 2 or
			std::abs(bounds.left - cellBounds.left) > CELL_SIZE * 2)
			continue;
		if (bounds.intersects(cellBounds))
			return true;
	}
	return false;
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

	//std::cout << "Cell added at " << cell.coo.x << " " << cell.coo.y << std::endl;
}

void GameMap::removeCell(sf::Vector2i coo)
{
	/*bool found = false;*/
	for (auto it = m_cells.begin(); it != m_cells.end(); ++it)
		if (it->coo == coo) { m_cells.erase(it); /*found = true;*/ break; }

	/*if (!found) return;
	std::cout << "Cell removed at " << coo.x << " " << coo.y << std::endl;*/
}

void GameMap::loadMap()
{
	std::ifstream file("map.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening map file to load" << std::endl;
		return;
	}
	int x, y, type;
	while (file >> x >> y >> type)
	{
		if ((CellType)type == CellType::Wall)
			addCell(Cell::create((CellType)type, { x, y }));
		else if ((CellType)type == CellType::Enemy)
			m_enemiesCoo.push_back({ x,y });
	}
		
	file.close();
	std::cout << "Map loaded" << std::endl;
}

void GameMap::saveMap()
{
	std::ofstream file("map.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening map file to save" << std::endl;
		return;
	}
	for (auto& cell : m_cells)
		file << cell.coo.x << " " << cell.coo.y << " " << (int)cell.type << std::endl;
	for (auto& enemyCoo : m_enemiesCoo)
		file << enemyCoo.x << " " << enemyCoo.y << " " << (int)CellType::Enemy << std::endl;
	file.close();
	std::cout << "Map saved" << std::endl;
}
