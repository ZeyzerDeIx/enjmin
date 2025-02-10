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
		if (((bounds.left + bounds.width >= cellBounds.left and
			  bounds.left + bounds.width <= cellBounds.left + cellBounds.width) or
			 (bounds.left >= cellBounds.left and
			  bounds.left <= cellBounds.left + cellBounds.width)) and
			((bounds.top + bounds.height >= cellBounds.top and
			  bounds.top + bounds.height <= cellBounds.top + cellBounds.height) or
			 (bounds.top >= cellBounds.top and
			  bounds.top <= cellBounds.top + cellBounds.height)))
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
		addCell(Cell::create((CellType)type, { x, y }));
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
	file.close();
	std::cout << "Map saved" << std::endl;
}
